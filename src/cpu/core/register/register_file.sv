import cotm32_pkg::NUM_REGS;
import cotm32_pkg::XLEN;

// Register file
module register_file #(
  parameter N_RPORTS = 2,
  parameter N_REGS = NUM_REGS
) (
  input logic i_clk,
  input logic i_rst,
  input logic i_we,
  input logic [XLEN-1:0] i_wdata,
  input logic [$clog2(N_REGS)-1:0] i_waddr,
  input logic [$clog2(N_REGS)-1:0] i_raddr [0:N_RPORTS-1],

  input logic i_trap_req,

  output logic [XLEN-1:0] o_rdata [0:N_RPORTS-1]
);
  
  import cotm32_pkg::*;

  logic [XLEN-1:0] qbus [0:N_REGS-1];
  logic [N_REGS-1:0] we_vec;

  wire we = i_we & ~i_trap_req;
  wire [XLEN-1:0] rdata_a = o_rdata[0];
  wire [XLEN-1:0] rdata_b = o_rdata[1];

  assign qbus[0] = '0;

  register_wport #(
    .N_REGS(N_REGS)
  ) wport(
    .i_we(we),
    .i_waddr(i_waddr),
    .o_we(we_vec)
  );

  genvar i;
  generate;
    for (i = 1; i < N_REGS; i++) begin : gen_regs
      register r(
        .i_clk(i_clk),
        .i_rst(i_rst),
        .i_we(we_vec[i]),
        .i_wdata(i_wdata),
        .o_qbus(qbus[i])
      );
    end

    for (i = 0; i < N_RPORTS; i++) begin : gen_rports
      register_rport #(
        .N_REGS(N_REGS)
      ) rport(
        .i_raddr(i_raddr[i]),
        .i_qbus(qbus),
        .o_rdata(o_rdata[i])
      );
    end
  endgenerate

endmodule
