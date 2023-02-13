package chipyard

import freechips.rocketchip.config.{Config}
import freechips.rocketchip.diplomacy.{AsynchronousCrossing}

// DOC include start: BISMOTLRocketConfig
class BISMOTLRocketConfig extends Config(
  new bismo.WithBISMO() ++
  new freechips.rocketchip.subsystem.WithNBigCores(1) ++
  new chipyard.config.AbstractConfig)
// DOC include end: BISMOTLRocketConfig