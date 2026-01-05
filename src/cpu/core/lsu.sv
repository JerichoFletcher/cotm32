import cotm32_pkg::*;

// Load-store unit
module lsu (
  input lsu_ls_t i_op,
  input logic [XLEN-1:0] i_addr,
  input logic [XLEN-1:0] i_wdata,
  input logic [XLEN-1:0] i_rdata,

  input logic i_trap_req,

  output logic [XLEN-1:0] o_wdata,
  output logic [XLEN-1:0] o_rdata,
  output logic [XLEN/BYTE_WIDTH-1:0] o_wstrb,
  output logic o_we,

  output logic o_t_load_addr_misaligned,
  output logic o_t_store_addr_misaligned
);

  always_comb begin
    o_t_load_addr_misaligned = '0;
    o_t_store_addr_misaligned = '0;

    case (i_op)
      LSU_LOAD_H, LSU_LOAD_HU: begin
        if (i_addr[0] != '0) begin
          o_t_load_addr_misaligned = '1;
        end
      end
      LSU_LOAD_W: begin
        if (i_addr[1:0] != '0) begin
          o_t_load_addr_misaligned = '1;
        end
      end
      LSU_STORE_H: begin
        if (i_addr[0] != '0) begin
          o_t_store_addr_misaligned = '1;
        end
      end
      LSU_STORE_W: begin
        if (i_addr[1:0] != '0) begin
          o_t_store_addr_misaligned = '1;
        end
      end
    endcase
  end

  always_comb begin
    unique case (i_op)
      LSU_LOAD_B  : o_rdata = $signed(i_rdata[XLEN/4-1:0]);
      LSU_LOAD_H  : o_rdata = $signed(i_rdata[XLEN/2-1:0]);
      LSU_LOAD_W  : o_rdata = i_rdata[XLEN-1:0];
      LSU_LOAD_BU : o_rdata = {{(3*XLEN/4){1'b0}}, i_rdata[XLEN/4-1:0]};
      LSU_LOAD_HU : o_rdata = {{(XLEN/2){1'b0}}, i_rdata[XLEN/2-1:0]};
      default     : o_rdata = i_rdata[XLEN-1:0];
    endcase
  end

  always_comb begin
    unique case (i_op)
      LSU_STORE_B : begin
        o_wdata = {{(3*XLEN/4){1'b0}}, i_wdata[XLEN/4-1:0]};
        o_wstrb = 4'b0001;
        o_we = ~i_trap_req;
      end
      LSU_STORE_H : begin
        o_wdata = {{(XLEN/2){1'b0}}, i_wdata[XLEN/2-1:0]};
        o_wstrb = 4'b0011;
        o_we = ~i_trap_req;
      end
      LSU_STORE_W : begin
        o_wdata = i_wdata[XLEN-1:0];
        o_wstrb = 4'b1111;
        o_we = ~i_trap_req;
      end
      default     : begin
        o_wdata = i_wdata[XLEN-1:0];
        o_wstrb = 4'b0000;
        o_we = '0;
      end
    endcase
  end

endmodule