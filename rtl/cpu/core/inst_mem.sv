import cotm32_pkg::XLEN;
import cotm32_pkg::INST_WIDTH;

// Instruction memory
module inst_mem (
  input logic [XLEN-1:0] i_addr,
  output logic [INST_WIDTH-1:0] o_inst
);

  import cotm32_pkg::*;

  localparam INST_BYTE_COUNT = INST_WIDTH/BYTE_WIDTH;
  localparam ADDR_WIDTH = $clog2(INST_MEM_SIZE);

  logic [BYTE_WIDTH-1:0] mem_bytes [0:INST_MEM_SIZE-1] /* verilator public_flat_rw */;
  wire [ADDR_WIDTH-1:0] mem_addr = i_addr[ADDR_WIDTH+1:2];

  always_comb begin
    for (int i = 0; i < INST_BYTE_COUNT; i++) begin
      o_inst[i*BYTE_WIDTH+:BYTE_WIDTH] = mem_bytes[mem_addr*INST_BYTE_COUNT + i];
    end
  end

endmodule
