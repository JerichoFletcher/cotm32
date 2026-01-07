SRC_FILES = \
	./rtl/cotm32_pkg.sv \
	./rtl/cotm32_priv_pkg.sv \
	$(shell find ./rtl/comp/ -name "*.sv") \
	$(shell find ./rtl/cpu/ -name "*.sv")
SRC_TEST_FILES = \
	./rtl/cotm32_pkg.sv \
	./rtl/cotm32_priv_pkg.sv \
	./test/cotm32_test_pkg.sv \
	$(shell find ./rtl/comp/ -name "*.sv") \
	$(shell find ./rtl/cpu/ -name "*.sv")
IVERILOG_OUT_DIR = ./out/iverilog
IVERILOG_OUT = sim
VERILATOR_MAIN = ./sim/src/main.cpp

ELF_OUT_DIR = ./out/elf

IVERILOG = iverilog
IVERILOG_FLAGS = -g2012

VVP = vvp
VVP_FLAGS = 

GTKWAVE = gtkwave
GTKWAVE_FLAGS = 

RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
$(eval $(RUN_ARGS):;@:)
FILENAME := $(notdir $(basename $(RUN_ARGS)))

# HDL
compile:
	@$(IVERILOG) $(IVERILOG_FLAGS) -o $(IVERILOG_OUT_DIR)/$(IVERILOG_OUT) $(SRC_TEST_FILES) $(RUN_ARGS)
vvp: compile
	@$(VVP) $(VVP_FLAGS) $(IVERILOG_OUT_DIR)/$(IVERILOG_OUT)
sim: vvp
	@$(GTKWAVE) $(GTKWAVE_FLAGS) ./*.vcd

clean:
	@rm -f $(IVERILOG_OUT_DIR)/* $(ELF_OUT_DIR)/*