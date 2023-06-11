package bismoconfig

import bismo._
import fpgatidbits._
import fpgatidbits.dma.GenericMemoryMasterPort
import fpgatidbits.ocm.OCMRequest

object BISMOConfigs {
  // BISMO variables
  val dpaDimLHS = 8;
  val dpaDimRHS = 8
  val dpaDimCommon = 256;
  val lhsEntriesPerMem = 64 * 32 * 1024 / (8 * 256)
  val rhsEntriesPerMem = 64 * 32 * 1024 / (8 * 256)
  val cmdQueueEntries = 16
  val bismoInitParams = new BitSerialMatMulParams(
    dpaDimLHS = dpaDimLHS,
    dpaDimRHS = dpaDimRHS,
    dpaDimCommon = dpaDimCommon,
    lhsEntriesPerMem = lhsEntriesPerMem,
    rhsEntriesPerMem = rhsEntriesPerMem,
    // TODO make seperate for FPGA
    mrp = fpgatidbits.PlatformWrapper.PYNQZ1Params.toMemReqParams(),
    cmdQueueEntries = cmdQueueEntries
  )

  // AXI
  val beatBytes = 8
  val numTlTxns = 256

  val addrBits = 32
  val dataBits = 64
  val idBits = 6
}
