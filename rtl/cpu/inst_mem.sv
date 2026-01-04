`include "defs.svh"

// Instruction memory
module inst_mem #(
  parameter MEM_SIZE = 1024
) (
  input logic [`XLEN-1:0] i_addr,
  output logic [`INST_WIDTH-1:0] o_inst
);

  localparam INST_BYTE_COUNT = `INST_WIDTH/`BYTE_WIDTH;
  localparam ADDR_WIDTH = $clog2(MEM_SIZE);

  logic [`BYTE_WIDTH-1:0] mem_bytes [0:MEM_SIZE*INST_BYTE_COUNT-1];
  wire [ADDR_WIDTH-1:0] mem_addr = i_addr[ADDR_WIDTH+1:2];

  always @* begin
    for (int i = 0; i < INST_BYTE_COUNT; i++) begin
      o_inst[i*`BYTE_WIDTH+:`BYTE_WIDTH] = mem_bytes[mem_addr*INST_BYTE_COUNT+i];
    end
  end

  initial begin
    $readmemh("./out/as/prog.verilog", mem_bytes);
  end

endmodule;