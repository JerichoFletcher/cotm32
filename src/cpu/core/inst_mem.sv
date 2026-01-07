import cotm32_pkg::XLEN;
import cotm32_pkg::INST_WIDTH;

// Instruction memory
module inst_mem (
  input logic [XLEN-1:0] i_addr,
  output logic [INST_WIDTH-1:0] o_inst
);

  import cotm32_pkg::*;

  localparam INST_BYTE_COUNT = INST_WIDTH/BYTE_WIDTH;
  localparam INST_COUNT = INST_MEM_SIZE/INST_BYTE_COUNT;
  localparam ADDR_WIDTH = $clog2(INST_MEM_SIZE);

  logic [BYTE_WIDTH-1:0] mem_bytes [INST_MEM_START:INST_MEM_END];
  wire [ADDR_WIDTH-1:0] mem_addr = i_addr[ADDR_WIDTH+1:2];

  always_comb begin
    for (int i = 0; i < INST_BYTE_COUNT; i++) begin
      o_inst[i*BYTE_WIDTH+:BYTE_WIDTH] = mem_bytes[INST_MEM_START + mem_addr*INST_BYTE_COUNT + i];
    end
  end

  initial begin
    for (int i = 0; i < INST_MEM_SIZE; i++) begin
      mem_bytes[i] = '0;
    end
    $readmemh(INST_MEM_SRC_PATH, mem_bytes, INST_MEM_START, INST_MEM_END);
  end

endmodule
