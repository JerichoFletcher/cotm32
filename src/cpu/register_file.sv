import cotm32_pkg::*;

// Register
module register #(
  parameter DATA_WIDTH = XLEN
) (
  input logic i_clk,
  input logic i_we,
  input logic [DATA_WIDTH-1:0] i_wdata,
  output logic [DATA_WIDTH-1:0] o_qbus
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
module register_rport #(
  parameter N_REGS = NUM_REGS,
  parameter DATA_WIDTH = XLEN
) (
  input logic [$clog2(N_REGS)-1:0] i_raddr,
  input logic [DATA_WIDTH-1:0] i_qbus [0:N_REGS-1],
  output logic [DATA_WIDTH-1:0] o_rdata
);

  mux #(
    .N_OPTIONS(N_REGS),
    .DATA_WIDTH(DATA_WIDTH)
  ) m(
    .i_sel(i_raddr),
    .i_val(i_qbus),
    .o_val(o_rdata)
  );

endmodule

// Register write port
module register_wport #(
  parameter N_REGS = NUM_REGS
) (
  input logic i_we,
  input logic [$clog2(N_REGS)-1:0] i_waddr,
  output logic [N_REGS-1:0] o_we
);

  logic [N_REGS-1:0] we;

  dec #(
    .N_SEL_BITS($clog2(N_REGS))
  ) d(
    .i_sel(i_waddr),
    .o(we)
  );

  assign o_we = (i_we && (i_waddr != '0)) ? we : '0;

endmodule

// Register file
module register_file #(
  parameter N_RPORTS = 2,
  parameter N_REGS = NUM_REGS,
  parameter DATA_WIDTH = XLEN
) (
  input logic i_clk,
  input logic i_we,
  input logic [DATA_WIDTH-1:0] i_wdata,
  input logic [$clog2(N_REGS)-1:0] i_waddr,
  input logic [$clog2(N_REGS)-1:0] i_raddr [0:N_RPORTS-1],
  output logic [DATA_WIDTH-1:0] o_rdata [0:N_RPORTS-1]
);
  
  logic [DATA_WIDTH-1:0] qbus [0:N_REGS-1];
  logic [N_REGS-1:0] we;

  assign qbus[0] = '0;

  register_wport #(
    .N_REGS(N_REGS)
  ) wport(
    .i_we(i_we),
    .i_waddr(i_waddr),
    .o_we(we)
  );

  genvar i;
  generate;
    for (i = 1; i < N_REGS; i++) begin : gen_regs
      register #(
        .DATA_WIDTH(DATA_WIDTH)
      ) r(
        .i_clk(i_clk),
        .i_we(we[i]),
        .i_wdata(i_wdata),
        .o_qbus(qbus[i])
      );
    end

    for (i = 0; i < N_RPORTS; i++) begin : gen_rports
      register_rport #(
        .N_REGS(N_REGS),
        .DATA_WIDTH(DATA_WIDTH)
      ) rport(
        .i_raddr(i_raddr[i]),
        .i_qbus(qbus),
        .o_rdata(o_rdata[i])
      );
    end
  endgenerate

endmodule