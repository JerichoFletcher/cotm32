import cotm32_pkg::*;

// Read-only memory
module rodata_mem #(
  parameter DATA_WIDTH = XLEN
) (
  input logic [XLEN-1:0] i_addr,
  output logic [DATA_WIDTH-1:0] o_rdata
);

  localparam ROM_BYTE_COUNT = DATA_WIDTH/BYTE_WIDTH;
  localparam ROM_COUNT = ROM_MEM_SIZE/ROM_BYTE_COUNT;
  localparam ADDR_WIDTH = $clog2(ROM_MEM_SIZE);

  logic [BYTE_WIDTH-1:0] mem_bytes [ROM_MEM_START:ROM_MEM_END];
  wire [ADDR_WIDTH-1:0] mem_addr = i_addr[ADDR_WIDTH+1:2];

  always_comb begin
    for (int i = 0; i < ROM_BYTE_COUNT; i++) begin
      o_rdata[i*BYTE_WIDTH+:BYTE_WIDTH] = mem_bytes[ROM_MEM_START + mem_addr*ROM_BYTE_COUNT + i];
    end
  end

  initial begin
    for (int i = 0; i < ROM_MEM_SIZE; i++) begin
      mem_bytes[i] = '0;
    end
    $readmemh("./out/boot-rodata.verilog", mem_bytes, ROM_MEM_START, ROM_MEM_END);
  end

endmodule;