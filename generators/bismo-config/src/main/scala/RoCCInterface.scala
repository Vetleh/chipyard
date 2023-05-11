package bismoconfig

import bismo._
import chisel3._
import chisel3.util._
import BISMOConfigs._
import fpgatidbits.PlatformWrapper._
import freechips.rocketchip.tile.HasCoreParameters
import freechips.rocketchip.rocket.constants.MemoryOpConstants
import freechips.rocketchip.tile._
import freechips.rocketchip.config._
import freechips.rocketchip.diplomacy._
import freechips.rocketchip.rocket.{TLBConfig, HellaCacheReq}
import freechips.rocketchip.diplomacy._
import freechips.rocketchip.tilelink._
import freechips.rocketchip.system.DefaultConfig
import freechips.rocketchip.amba.axi4._

class BISMOAccel(opcodes: OpcodeSet, val n: Int = 64)(implicit p: Parameters)
    extends LazyRoCC(opcodes) {
  override lazy val module = new BISMOAccelImp(this)
  val l2mem = (0 until 1).map { x =>
    val y = LazyModule(new L2MemHelper())
    tlNode := y.masterNode
    y
  }
}

class BISMOAccelImp(outer: BISMOAccel)(implicit p: Parameters)
    extends LazyRoCCModuleImp(outer)
    with HasCoreParameters {
  val regfile = Reg(Vec(outer.n, UInt(xLen.W)))
  val busy = RegInit(VecInit(Seq.fill(outer.n) { false.B }))
  val cmd = Queue(io.cmd)

  val funct = cmd.bits.inst.funct
  val addr = cmd.bits.rs2(log2Up(outer.n) - 1, 0)
  val doWrite = funct === 0.U
  val doRead = funct === 1.U
  val doLoad = funct === 2.U
  val memRespTag = io.mem.resp.bits.tag(log2Up(outer.n) - 1, 0)

  // datapath
  val wdata = cmd.bits.rs1

  // Return 0 if nothing read related happens
  io.resp.bits.data := 0.U

  // Write and read data to the register
  when(cmd.fire && doWrite) {
    regfile(addr) := wdata
  }.elsewhen(cmd.fire && doRead) {
    io.resp.bits.data := regfile(addr)
  }

  val doResp = cmd.bits.inst.xd
  val stallReg = busy(addr)
  val stallLoad = doLoad && !io.mem.req.ready
  // val stallLoad = doLoad && !io.mem.req.ready
  val stallResp = doResp && !io.resp.ready

  // command resolved if no stalls AND not issuing a load that will need a request
  cmd.ready := !stallReg && !stallLoad && !stallResp

  // PROC RESPONSE INTERFACE
  // valid response if valid command, need a response, and no stalls
  io.resp.valid := cmd.valid && doResp && !stallReg && !stallLoad

  // Must respond with the appropriate tag or undefined behavior
  io.resp.bits.rd := cmd.bits.inst.rd

  // Semantics is to always send out prior accumulator register value
  // io.resp.bits.data := outputVal

  // Be busy when have pending memory requests or committed possibility of pending requests
  io.busy := cmd.valid || busy.reduce(_ || _)

  // Set this true to trigger an interrupt on the processor (please refer to supervisor documentation)
  io.interrupt := false.B
  
//   val dpaDimLHS = 8;
//   val dpaDimRHS = 8
//   val dpaDimCommon = 256;
//   val lhsEntriesPerMem = 64 * 32 * 1024 / (dpaDimLHS * dpaDimCommon)
//   val rhsEntriesPerMem = 64 * 32 * 1024 / (dpaDimRHS * dpaDimCommon)

//   val bismoInitParams = new BitSerialMatMulParams(
//     dpaDimLHS = dpaDimLHS,
//     dpaDimRHS = dpaDimRHS,
//     dpaDimCommon = dpaDimCommon,
//     lhsEntriesPerMem = lhsEntriesPerMem,
//     rhsEntriesPerMem = rhsEntriesPerMem,
//     // TODO make seperate for FPGA
//     mrp = PYNQZ1Params.toMemReqParams()
//   )

  // Instansiate BISMO
  val emuP = TesterWrapperParams
  val bismo = Module(new BitSerialMatMulAccel(bismoInitParams, emuP))
  val impl = bismo.io
  // wire up

  // Fetch
  impl.fetch_enable := regfile(0)

  // Fetch op
  impl.fetch_op.bits.opcode := regfile(1)
  impl.fetch_op.bits.token_channel := regfile(2)
  impl.fetch_op.valid := regfile(3)
  regfile(4) := impl.fetch_op.ready

  // Fetch runcfg
  impl.fetch_runcfg.valid := regfile(5)
  impl.fetch_runcfg.bits.dram_base := regfile(6)
  impl.fetch_runcfg.bits.dram_block_size_bytes := regfile(7)
  impl.fetch_runcfg.bits.dram_block_offset_bytes := regfile(8)
  impl.fetch_runcfg.bits.dram_block_count := regfile(9)
  impl.fetch_runcfg.bits.tiles_per_row := regfile(10)
  impl.fetch_runcfg.bits.bram_addr_base := regfile(11)
  impl.fetch_runcfg.bits.bram_id_start := regfile(12)
  impl.fetch_runcfg.bits.bram_id_range := regfile(13)

  // Fetch op count
  regfile(14) := impl.fetch_op_count

  // Exec
  impl.exec_enable := regfile(15)
  // Exec op
  impl.exec_op.bits.opcode := regfile(16)
  impl.exec_op.bits.token_channel := regfile(17)
  impl.exec_op.valid := regfile(18)
  regfile(19) := impl.exec_op.ready

  // Exec runcfg
  impl.exec_runcfg.valid := regfile(20)
  impl.exec_runcfg.bits.clear_before_first_accumulation := regfile(21)
  impl.exec_runcfg.bits.rhsOffset := regfile(22)
  impl.exec_runcfg.bits.lhsOffset := regfile(23)
  impl.exec_runcfg.bits.negate := regfile(24)
  impl.exec_runcfg.bits.numTiles := regfile(25)
  impl.exec_runcfg.bits.writeEn := regfile(26)
  impl.exec_runcfg.bits.writeAddr := regfile(27)
  impl.exec_runcfg.bits.shiftAmount := regfile(28)

  // Exec op count
  regfile(29) := impl.exec_op_count

  // Result
  impl.result_enable := regfile(30)

  // Result op
  impl.result_op.bits.opcode := regfile(31)
  impl.result_op.bits.token_channel := regfile(32)
  impl.result_op.valid := regfile(33)
  regfile(34) := impl.result_op.ready

  // Result runcfg
  impl.result_runcfg.valid := regfile(35)
  impl.result_runcfg.bits.dram_base := regfile(36)
  impl.result_runcfg.bits.dram_skip := regfile(37)
  impl.result_runcfg.bits.resmem_addr := regfile(38)
  impl.result_runcfg.bits.waitComplete := regfile(39)
  impl.result_runcfg.bits.waitCompleteBytes := regfile(40)

  // Result op count
  regfile(41) := impl.result_op_count

  // Hardware info
  regfile(42) := impl.hw.readChanWidth
  regfile(43) := impl.hw.writeChanWidth
  regfile(44) := impl.hw.dpaDimLHS
  regfile(45) := impl.hw.dpaDimRHS
  regfile(46) := impl.hw.dpaDimCommon
  regfile(47) := impl.hw.lhsEntriesPerMem
  regfile(48) := impl.hw.rhsEntriesPerMem
  regfile(49) := impl.hw.accWidth
  regfile(50) := impl.hw.maxShiftSteps
  regfile(51) := impl.hw.cmdQueueEntries

  // Runcfgs ready values
  regfile(52) := impl.fetch_runcfg.ready
  regfile(53) := impl.exec_runcfg.ready
  regfile(54) := impl.result_runcfg.ready

  // CC
  regfile(55) := impl.perf.cc
  impl.perf.cc_enable := regfile(56)

  // perf fetch
  regfile(57) := impl.perf.prf_fetch.count
  impl.perf.prf_fetch.sel := regfile(58)

  // perf exec
  regfile(59) := impl.perf.prf_exec.count
  impl.perf.prf_exec.sel := regfile(60)

  // perf result
  regfile(61) := impl.perf.prf_res.count
  impl.perf.prf_res.sel := regfile(62)

  bismo.reset.asBool
  when(regfile(63) === 1.U){
    bismo.reset := true.B
  }.otherwise {
    bismo.reset := false.B
  }
  

  val axi = outer.l2mem(0).module.io.axi
  // Memory requests
  // Read address axi
  impl.memPort(0).memRdReq.ready := axi.ar.ready
  axi.ar.valid := impl.memPort(0).memRdReq.valid
  axi.ar.bits.addr := impl.memPort(0).memRdReq.bits.addr
  axi.ar.bits.id := impl.memPort(0).memRdReq.bits.channelID
  // Burst is not supported so only 1 read at a time
  // AXI specifies amount of bursts as (len + 1)
  axi.ar.bits.len := 0.U
  axi.ar.bits.size := log2Ceil(bismoInitParams.mrp.dataWidth / 8).U

  // Read response axi
  axi.r.ready := impl.memPort(0).memRdRsp.ready
  impl.memPort(0).memRdRsp.valid := axi.r.valid
  impl.memPort(0).memRdRsp.bits.readData := axi.r.bits.data
  impl.memPort(0).memRdRsp.bits.isLast := axi.r.bits.last
  impl.memPort(0).memRdRsp.bits.channelID := axi.r.bits.id
  impl.memPort(0).memRdRsp.bits.isWrite := false.B
  impl.memPort(0).memRdRsp.bits.metaData := 0.U

  // MemWrReq
  impl.memPort(0).memWrReq.ready := axi.aw.ready
  axi.aw.valid := impl.memPort(0).memWrReq.valid
  axi.aw.bits.addr := impl.memPort(0).memWrReq.bits.addr
  axi.aw.bits.id := impl.memPort(0).memWrReq.bits.channelID
  // Burst is not supported so only 1 read at a time
  // AXI specifies amount of bursts as (len + 1)
  axi.aw.bits.len := 0.U
  axi.aw.bits.size := log2Ceil(bismoInitParams.mrp.dataWidth / 8).U

  // MemWrDat
  impl.memPort(0).memWrDat.ready := axi.w.ready
  axi.w.valid := impl.memPort(0).memWrDat.valid
  axi.w.bits.data := impl.memPort(0).memWrDat.bits
  axi.w.bits.strb := (Math.pow(2,(bismoInitParams.mrp.dataWidth / 8)).toInt - 1).U

  // MemWrRsp
  axi.b.ready := impl.memPort(0).memWrRsp.ready
  impl.memPort(0).memWrRsp.valid := axi.b.valid
  impl.memPort(0).memWrRsp.bits.isWrite := true.B
  impl.memPort(0).memWrRsp.bits.isLast := false.B
  impl.memPort(0).memWrRsp.bits.channelID := axi.b.bits.id
  impl.memPort(0).memWrRsp.bits.metaData := 0.U
  impl.memPort(0).memWrRsp.bits.readData := axi.b.bits.resp

  // MEMORY REQUEST INTERFACE
  // io.mem.req.valid := cmd.valid && doLoad && !stallReg && !stallResp
  // io.mem.req.bits.addr := addend
  // io.mem.req.bits.tag := addr
  // io.mem.req.bits.cmd := M_XRD // perform a load (M_XWR for stores)
  // io.mem.req.bits.size := log2Ceil(8).U
  // io.mem.req.bits.signed := false.B
  // io.mem.req.bits.data := 0.U // we're not performing any stores...
  // io.mem.req.bits.phys := false.B
  // io.mem.req.bits.dprv := cmd.bits.status.dprv
  // io.mem.req.bits.dv := cmd.bits.status.dv
}

class WithBISMORoCC
    extends Config((site, here, up) => { case BuildRoCC =>
      up(BuildRoCC) ++ Seq((p: Parameters) => {
        val bismo = LazyModule.apply(new BISMOAccel(OpcodeSet.custom0)(p))
        bismo
      })
    })