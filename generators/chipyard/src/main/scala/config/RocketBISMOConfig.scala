package chipyard

import freechips.rocketchip.config.{Config}
import freechips.rocketchip.diplomacy.{AsynchronousCrossing}

// DOC include start: BISMOTLRocketConfig
class BISMOTLRocketConfig extends Config(
  new bismoconfig.WithBISMOTL() ++
  new freechips.rocketchip.subsystem.WithNBigCores(1) ++
  new chipyard.config.AbstractConfig)
// DOC include end: BISMOTLRocketConfig

// DOC include start: BISMORoCCRocketConfig
class BISMORoCCRocketConfig extends Config(
  new bismoconfig.WithBISMORoCC() ++
  new freechips.rocketchip.subsystem.WithNBigCores(1) ++
  new chipyard.config.AbstractConfig)
// DOC include end: BISMOTLRocketConfig