import cotm32_pkg::XLEN;
import cotm32_pkg::INST_WIDTH;
import cotm32_pkg::imm_t;

// Immediate sign extender
module sign_ext (
  input logic [INST_WIDTH-1:0] i_inst,
  input imm_t i_sel,
  output logic [XLEN-1:0] o_imm
);

  import cotm32_pkg::*;

  always_comb begin
    unique case (i_sel)
      IMM_I   : o_imm = {{20{i_inst[31]}}, i_inst[31:20]};
      IMM_S   : o_imm = {{20{i_inst[31]}}, i_inst[31:25], i_inst[11:7]};
      IMM_B   : o_imm = {{19{i_inst[31]}}, i_inst[31], i_inst[7], i_inst[30:25], i_inst[11:8], 1'b0};
      IMM_U   : o_imm = {i_inst[31:12], 12'b0};
      IMM_J   : o_imm = {{11{i_inst[31]}}, i_inst[31], i_inst[19:12], i_inst[20], i_inst[30:21], 1'b0};
      default : o_imm = '0;
    endcase
  end

endmodule
