package chipyard

import bismo._
import freechips.rocketchip.config.{Config}
import freechips.rocketchip.diplomacy.{AsynchronousCrossing}

// DOC include start: BISMOTLRocketConfig
class BISMOTLRocketConfig extends Config(
  new bismo.WithBISMO(useAXI4=false, useBlackBox=false) ++
  new freechips.rocketchip.subsystem.WithNBigCores(1) ++
  new chipyard.config.AbstractConfig)
// DOC include end: BISMOTLRocketConfig