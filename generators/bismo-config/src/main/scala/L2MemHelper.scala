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

class L2MemHelper()(implicit p: Parameters) extends LazyModule {
  lazy val module = new BISMOL2MemHelperModule(this)
  val masterNode = TLIdentityNode()
  val dbb_axi_node = AXI4MasterNode(
    Seq(
      AXI4MasterPortParameters(
        masters =
          Seq(AXI4MasterParameters(name = "BISMO DBB", id = IdRange(0, 256)))
      )
    )
  )
  // TL <-> AXI
  (masterNode
    := TLBuffer()
    := TLWidthWidget(dataBits / 8)
    := AXI4ToTL(numTlTxns = numTlTxns)
    := AXI4UserYanker(capMaxFlight = Some(numTlTxns))
    := AXI4Fragmenter()
    := AXI4IdIndexer(idBits = idBits)
    := AXI4Buffer()
    := dbb_axi_node)
}

class BISMOL2MemHelperModule(outer: L2MemHelper)(implicit p: Parameters)
    extends LazyModuleImp(outer)
    with HasCoreParameters
    with MemoryOpConstants {
  // TODO make these numbers come from somewhere
  val io = IO(new Bundle {
    val axi = Flipped(new AXI4Bundle(new AXI4BundleParameters(addrBits, dataBits, idBits)))
  })

  val (axi_val, _) = outer.dbb_axi_node.out(0)
  io.axi <> axi_val

}