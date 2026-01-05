import cotm32_pkg::*;

// Data memory
module data_mem #(
  parameter MEM_SIZE = DATA_MEM_SIZE,
  parameter DATA_WIDTH = XLEN
) (
  input logic i_clk,
  input logic i_we,
  input logic [XLEN-1:0] i_addr,
  input logic [DATA_WIDTH-1:0] i_wdata,
  input logic [DATA_WIDTH/BYTE_WIDTH-1:0] i_wstrb,
  output logic [DATA_WIDTH-1:0] o_rdata
);
  
  localparam DATA_BYTE_COUNT = DATA_WIDTH/BYTE_WIDTH;
  localparam MEM_BYTE_COUNT = MEM_SIZE * DATA_BYTE_COUNT;
  logic [BYTE_WIDTH-1:0] mem_bytes [0:MEM_BYTE_COUNT-1];

  initial begin
    for (int i = 0; i < MEM_BYTE_COUNT; i++) begin
      mem_bytes[i] = '0;
    end
  end

  always_comb begin
    for (int i = 0; i < DATA_BYTE_COUNT; i++) begin
      o_rdata[i*BYTE_WIDTH+:BYTE_WIDTH] = mem_bytes[i_addr + i];
    end
  end

  always_ff @(posedge i_clk) begin
    if (i_we) begin
      for (int i = 0; i < DATA_BYTE_COUNT; i++) begin
        if (i_wstrb[i]) begin
          mem_bytes[i_addr + i] <= i_wdata[i*BYTE_WIDTH+:BYTE_WIDTH];
        end
      end
    end
  end

endmodule