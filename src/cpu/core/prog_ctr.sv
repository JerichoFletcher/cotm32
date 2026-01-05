import cotm32_pkg::*;

// Program counter register
module prog_ctr #(
  parameter RESET_VECTOR = '0
) (
  input logic i_clk,
  input logic i_rst,
  input logic [XLEN-1:0] i_addr,
  output logic [XLEN-1:0] o_addr,
  output logic o_t_inst_addr_misaligned
);

  initial begin
    o_addr = RESET_VECTOR;
  end

  always_ff @(posedge i_clk) begin
    if (i_rst) begin
      o_addr <= RESET_VECTOR;
    end else begin
      o_addr <= i_addr;
    end
  end

  always_comb begin
    o_t_inst_addr_misaligned = '0;
    if (i_addr[1:0] != '0) begin
      o_t_inst_addr_misaligned = '1;
    end
  end

endmodule