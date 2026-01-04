`include "defs.svh"

// Register
module register (
  input logic i_clk,
  input logic i_we,
  input logic [`XLEN-1:0] i_wdata,
  output logic [`XLEN-1:0] o_qbus
);

  initial begin
    o_qbus = '0;
  end

  always_ff @(posedge i_clk) begin
    if (i_we) begin
      o_qbus <= i_wdata;
    end
  end

endmodule

// Register read port
module register_rport (
  input logic [$clog2(`NUM_REGS)-1:0] i_raddr,
  input logic [`XLEN-1:0] i_qbus [0:`NUM_REGS-1],
  output logic [`XLEN-1:0] o_rdata
);

  mux #(
    .NUM_OPTIONS(`NUM_REGS),
    .DATA_WIDTH(`XLEN)
  ) m(
    .i_sel(i_raddr),
    .i_val(i_qbus),
    .o_val(o_rdata)
  );

endmodule

// Register write port
module register_wport (
  input logic i_we,
  input logic [$clog2(`NUM_REGS)-1:0] i_waddr,
  output logic [`NUM_REGS-1:0] o_we
);

  logic [`NUM_REGS-1:0] we;

  dec #(
    .N_SEL_BITS($clog2(`NUM_REGS))
  ) d(
    .i_sel(i_waddr),
    .o(we)
  );

  assign o_we = (i_we && (i_waddr != '0)) ? we : '0;

endmodule

// Register file
module register_file #(
  parameter NUM_RPORTS = 2
) (
  input logic i_clk,
  input logic i_we,
  input logic [`XLEN-1:0] i_wdata,
  input logic [$clog2(`NUM_REGS)-1:0] i_waddr,
  input logic [$clog2(`NUM_REGS)-1:0] i_raddr [0:NUM_RPORTS-1],
  output logic [`XLEN-1:0] o_rdata [0:NUM_RPORTS-1]
);
  
  logic [`XLEN-1:0] qbus [0:`NUM_REGS-1];
  logic [`NUM_REGS-1:0] we;

  assign qbus[0] = '0;

  register_wport wport(
    .i_we(i_we),
    .i_waddr(i_waddr),
    .o_we(we)
  );

  genvar i;
  generate;
    for (i = 1; i < `NUM_REGS; i++) begin : gen_regs
      register r(
        .i_clk(i_clk),
        .i_we(we[i]),
        .i_wdata(i_wdata),
        .o_qbus(qbus[i])
      );
    end

    for (i = 0; i < NUM_RPORTS; i++) begin : gen_rports
      register_rport rport(
        .i_raddr(i_raddr[i]),
        .i_qbus(qbus),
        .o_rdata(o_rdata[i])
      );
    end
  endgenerate

endmodule