.DEFAULT_GOAL := spike
VERILATOR_FOLDER_PATH = ../../../../../../sims/verilator/
SIM_PATH = $(VERILATOR_FOLDER_PATH)/simulator-chipyard-BISMOTLRocketConfig
SIM_DEBUG_PATH = $(VERILATOR_FOLDER_PATH)/simulator-chipyard-BISMOTLRocketConfig-debug
BUILD_TOOLS_PATH = ../../../../../../scripts/build-toolchain-extra.sh
PATH_FROM_SIM = ../../generators/BISMO_Chisel_3/src/main/cpp/app
sim_compile:
	$(BUILD_TOOLS_PATH)
	riscv64-unknown-linux-gnu-g++ -static cpptest.cpp -o cpptest
	$(SIM_PATH) pk ./cpptest

spike-debug-2:
	riscv64-unknown-linux-gnu-g++ -static cpptest.cpp -o cpptest
	spike --dump-dts a pk cpptest

spike:
	riscv64-unknown-linux-gnu-g++ -static main.cpp -o main
	test -d out || mkdir out
	spike pk main 2>&1 | tee out/bismo-spike-log.txt

spike-debug:
	riscv64-unknown-linux-gnu-g++ -static main.cpp -o main
	spike -d pk main

sim:
	riscv64-unknown-linux-gnu-g++ -static main.cpp -o main
	test -d out || mkdir out
	time $(SIM_PATH) --cycle-count pk ./main 2>&1 | tee out/bismo-log.txt

sim-debug:
	riscv64-unknown-linux-gnu-g++ -static main.cpp -o main
	test -d out || mkdir out
	time $(SIM_DEBUG_PATH) --vcd=out/bismo-debug.vcd --cycle-count pk ./main 2>&1 | tee out/bismo-debug-log.txt
