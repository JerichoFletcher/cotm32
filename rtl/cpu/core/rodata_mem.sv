import cotm32_pkg::XLEN;

// Read-only memory
module rodata_mem #(
  parameter DATA_WIDTH = XLEN
) (
  input logic [XLEN-1:0] i_addr,
  output logic [DATA_WIDTH-1:0] o_rdata
);

  import cotm32_pkg::*;

  localparam ROM_BYTE_COUNT = DATA_WIDTH/BYTE_WIDTH;
  localparam ADDR_WIDTH = $clog2(ROM_MEM_SIZE);

  logic [BYTE_WIDTH-1:0] mem_bytes [0:ROM_MEM_SIZE-1] /* verilator public_flat_rw */;
  wire [ADDR_WIDTH-1:0] mem_addr = i_addr[ADDR_WIDTH+1:2];

  always_comb begin
    for (int i = 0; i < ROM_BYTE_COUNT; i++) begin
      o_rdata[i*BYTE_WIDTH+:BYTE_WIDTH] = mem_bytes[mem_addr*ROM_BYTE_COUNT + i];
    end
  end

endmodule
