SRC_FILES = \
	./src/cotm32_pkg.sv \
	./src/cotm32_priv_pkg.sv \
	./test/cotm32_test_pkg.sv \
	$(shell find ./src/comp/ -name "*.sv") \
	$(shell find ./src/cpu/ -name "*.sv")
IVERILOG_OUT_DIR = ./out/iverilog
IVERILOG_OUT = sim

AS_DIR = ./as
AS_OUT_DIR = ./out/as
LD_OUT_DIR = ./out/as
OBJCOPY_OUT_DIR = ./out/as
AS_OUT = prog.o
LD_OUT = prog.elf
OBJCOPY_OUT_BIN = prog.bin
OBJCOPY_OUT_VERILOG = prog.verilog

IVERILOG = iverilog
IVERILOG_FLAGS = -g2012

VVP = vvp
VVP_FLAGS = 

GTKWAVE = gtkwave
GTKWAVE_FLAGS = 

XPACKS_BIN_DIR = ./xpacks/.bin
AS = "$(XPACKS_BIN_DIR)/riscv-none-elf-as"
AS_FLAGS = -march=rv32i_zicsr -mabi=ilp32
LD = "$(XPACKS_BIN_DIR)/riscv-none-elf-ld"
LD_FLAGS = 
OBJCOPY = "$(XPACKS_BIN_DIR)/riscv-none-elf-objcopy"
OBJCOPY_FLAGS_BIN = -O binary
OBJCOPY_FLAGS_VERILOG = -O verilog
OBJDUMP = "$(XPACKS_BIN_DIR)/riscv-none-elf-objdump"

RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
$(eval $(RUN_ARGS):;@:)

# HDL
compile:
	$(IVERILOG) $(IVERILOG_FLAGS) -o $(IVERILOG_OUT_DIR)/$(IVERILOG_OUT) $(SRC_FILES) $(RUN_ARGS)
sim: compile
	@$(VVP) $(VVP_FLAGS) $(IVERILOG_OUT_DIR)/$(IVERILOG_OUT)
	@$(GTKWAVE) $(GTKWAVE_FLAGS) ./*.vcd

# Assembly
obj:
	@$(AS) $(AS_FLAGS) -o $(AS_OUT_DIR)/$(AS_OUT) $(RUN_ARGS)
link: obj
	@$(LD) $(LD_FLAGS) -T $(RUN_ARGS:.s=.ld) -o $(LD_OUT_DIR)/$(LD_OUT) $(AS_OUT_DIR)/$(AS_OUT)
asbin: link
	@$(OBJCOPY) $(OBJCOPY_FLAGS_BIN) $(LD_OUT_DIR)/$(LD_OUT) $(OBJCOPY_OUT_DIR)/$(OBJCOPY_OUT_BIN)
	@$(OBJCOPY) $(OBJCOPY_FLAGS_VERILOG) $(LD_OUT_DIR)/$(LD_OUT) $(OBJCOPY_OUT_DIR)/$(OBJCOPY_OUT_VERILOG)
asdump: link
	@$(OBJDUMP) -d $(LD_OUT_DIR)/$(LD_OUT)

clean:
	@rm -f $(IVERILOG_OUT_DIR)/* $(AS_OUT_DIR)/* $(LD_OUT_DIR)/* $(OBJCOPY_OUT_DIR)/*