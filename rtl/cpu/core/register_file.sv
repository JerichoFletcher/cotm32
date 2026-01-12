import cotm32_pkg::NUM_REGS;
import cotm32_pkg::XLEN;

// Register file
module register_file (
  input logic i_clk,
  input logic i_rst,
  
  input logic i_we,
  input logic [XLEN-1:0] i_wdata,
  input logic [$clog2(NUM_REGS)-1:0] i_waddr,
  
  input logic [$clog2(NUM_REGS)-1:0] i_rs1_addr,
  input logic [$clog2(NUM_REGS)-1:0] i_rs2_addr,

  output logic [XLEN-1:0] o_rs1,
  output logic [XLEN-1:0] o_rs2
);

  import cotm32_pkg::*;

  logic [XLEN-1:0] mem [1:NUM_REGS-1] /* verilator public */;

  always_comb begin
    // RS1
    if (i_rs1_addr == '0) begin
      o_rs1 = '0;
    end else if (i_we && i_waddr != '0 && i_waddr == i_rs1_addr) begin
      o_rs1 = i_wdata;
    end else begin
      o_rs1 = mem[i_rs1_addr];
    end

    // RS2
    if (i_rs2_addr == '0) begin
      o_rs2 = '0;
    end else if (i_we && i_waddr != '0 && i_waddr == i_rs2_addr) begin
      o_rs2 = i_wdata;
    end else begin
      o_rs2 = mem[i_rs2_addr];
    end
  end

  always_ff @(posedge i_clk) begin
    if (i_rst) begin
      for (int i = 0; i < NUM_REGS; i++) begin
        mem[i] <= '0;
      end
    end else if (i_we && i_waddr != '0) begin
      mem[i_waddr] <= i_wdata;
    end
  end

endmodule
