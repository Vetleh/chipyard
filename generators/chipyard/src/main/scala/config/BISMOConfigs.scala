package chipyard

import freechips.rocketchip.system.DefaultConfig
import freechips.rocketchip.config.{Config}
import freechips.rocketchip.subsystem.WithNBigCores
// import freechips.rocketchip.diplomacy.{AsynchronousCrossing}

class BISMORoCCRocketConfig
    extends Config(
      new bismo.WithBISMOAccel() ++
        new WithNBigCores(1) ++
        new chipyard.config.AbstractConfig
    )
