.DEFAULT_GOAL := spike
MMIO_SIM_PATH = ../../../../../sims/verilator/simulator-chipyard-BISMOTLRocketConfig
MMIO_SIM_DEBUG_PATH = ../../../../../sims/verilator/simulator-chipyard-BISMOTLRocketConfig-debug
ROCC_SIM_PATH = ../../../../../sims/verilator/simulator-chipyard-BISMOTLRocketConfig
ROCC_SIM_DEBUG_PATH = ../../../../../sims/verilator/simulator-chipyard-BISMOTLRocketConfig-debug

mmio-sw:
	cd mmio
	riscv64-unknown-linux-gnu-g++ -static main.cpp -o main
	test -d mmio-out || mkdir mmio-out
	time $(MMIO_SIM_PATH) --cycle-count --vcd=mmio-out/bismo-debug-clock.vcd pk ./main 2>&1 | tee mmio-out/bismo-log-clock.txt
mmio-sw-debug:
	cd mmio
	riscv64-unknown-linux-gnu-g++ -static main.cpp -o main
	test -d mmio-debug-out || mkdir mmio-debug-out
	time $(MMIO_SIM_DEBUG_PATH) --cycle-count --vcd=mmio-debug-out/bismo-debug-clock.vcd pk ./main 2>&1 | tee mmio-debug-out/bismo-log-clock.txt

rocc-sw:
	cd rocc
	riscv64-unknown-linux-gnu-g++ -static main.cpp -o main
	test -d rocc-out || mkdir rocc-out
	time $(ROCC_SIM_PATH) --cycle-count --vcd=rocc-out/bismo-debug-clock.vcd pk ./main 2>&1 | tee rocc-out/bismo-log-clock.txt

rocc-sw-debug:chipyard/generators/bismo-config/src/main/tests/Makefile
	cd rocc
	riscv64-unknown-linux-gnu-g++ -static main.cpp -o main
	test -d rocc-debug-out || mkdir rocc-debug-out
	time $(ROCC_SIM_DEBUG_PATH) --cycle-count --vcd=rocc-debug-out/out/bismo-debug-clock.vcd pk ./main 2>&1 | tee rocc-debug-out/bismo-log-clock.txt