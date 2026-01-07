SRC_FILES = \
	./src/cotm32_pkg.sv \
	./src/cotm32_priv_pkg.sv \
	./test/cotm32_test_pkg.sv \
	$(shell find ./src/comp/ -name "*.sv") \
	$(shell find ./src/cpu/ -name "*.sv")
IVERILOG_OUT_DIR = ./out/iverilog
IVERILOG_OUT = sim

ELF_OUT_DIR = ./out/elf

IVERILOG = iverilog
IVERILOG_FLAGS = -g2012

VVP = vvp
VVP_FLAGS = 

GTKWAVE = gtkwave
GTKWAVE_FLAGS = 

GCC = riscv64-unknown-elf-gcc
GCC_FLAGS = -march=rv32i_zicsr -mabi=ilp32 -nostdlib
OBJCOPY = riscv64-unknown-elf-objcopy
OBJCOPY_FLAGS_VERILOG = -O verilog
OBJDUMP = riscv64-unknown-elf-objdump

RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
$(eval $(RUN_ARGS):;@:)
FILENAME := $(notdir $(basename $(RUN_ARGS)))

# HDL
compile:
	$(IVERILOG) $(IVERILOG_FLAGS) -o $(IVERILOG_OUT_DIR)/$(IVERILOG_OUT) $(SRC_FILES) $(RUN_ARGS)
vvp: compile
	@$(VVP) $(VVP_FLAGS) $(IVERILOG_OUT_DIR)/$(IVERILOG_OUT)
sim: vvp
	@$(GTKWAVE) $(GTKWAVE_FLAGS) ./*.vcd

# Assembly
elf:
	$(GCC) $(GCC_FLAGS) -T $(RUN_ARGS:.s=.ld) -o $(ELF_OUT_DIR)/$(FILENAME).elf $(RUN_ARGS)
asbin: elf
	@$(OBJCOPY) $(OBJCOPY_FLAGS_VERILOG) -j .text $(ELF_OUT_DIR)/$(FILENAME).elf $(ELF_OUT_DIR)/$(FILENAME)-text.verilog
	@$(OBJCOPY) $(OBJCOPY_FLAGS_VERILOG) -j .rodata $(ELF_OUT_DIR)/$(FILENAME).elf $(ELF_OUT_DIR)/$(FILENAME)-rodata.verilog
asdump: elf
	@$(OBJDUMP) -d $(ELF_OUT_DIR)/$(FILENAME).elf

clean:
	@rm -f $(IVERILOG_OUT_DIR)/* $(ELF_OUT_DIR)/*