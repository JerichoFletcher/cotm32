import cotm32_pkg::*;

// Load-store unit
module lsu #(
  parameter DATA_WIDTH = XLEN
) (
  input lsu_ls_t i_op,
  input logic [DATA_WIDTH-1:0] i_wdata,
  input logic [DATA_WIDTH-1:0] i_rdata,
  output logic [DATA_WIDTH-1:0] o_wdata,
  output logic [DATA_WIDTH-1:0] o_rdata,
  output logic [DATA_WIDTH/BYTE_WIDTH-1:0] o_wstrb,
  output logic o_we
);

  always_comb begin
    unique case (i_op)
      LSU_LOAD_B  : o_rdata = $signed(i_rdata[DATA_WIDTH/4-1:0]);
      LSU_LOAD_H  : o_rdata = $signed(i_rdata[DATA_WIDTH/2-1:0]);
      LSU_LOAD_W  : o_rdata = i_rdata[DATA_WIDTH-1:0];
      LSU_LOAD_BU : o_rdata = {{(3*DATA_WIDTH/4){1'b0}}, i_rdata[DATA_WIDTH/4-1:0]};
      LSU_LOAD_HU : o_rdata = {{(DATA_WIDTH/2){1'b0}}, i_rdata[DATA_WIDTH/2-1:0]};
      default     : o_rdata = i_rdata[DATA_WIDTH-1:0];
    endcase
  end

  always_comb begin
    unique case (i_op)
      LSU_STORE_B : begin
        o_wdata = {{(3*DATA_WIDTH/4){1'b0}}, i_wdata[DATA_WIDTH/4-1:0]};
        o_wstrb = 4'b0001;
        o_we = '1;
      end
      LSU_STORE_H : begin
        o_wdata = {{(DATA_WIDTH/2){1'b0}}, i_wdata[DATA_WIDTH/2-1:0]};
        o_wstrb = 4'b0011;
        o_we = '1;
      end
      LSU_STORE_W : begin
        o_wdata = i_wdata[DATA_WIDTH-1:0];
        o_wstrb = 4'b1111;
        o_we = '1;
      end
      default     : begin
        o_wdata = i_wdata[DATA_WIDTH-1:0];
        o_wstrb = 4'b0000;
        o_we = '0;
      end
    endcase
  end

endmodule