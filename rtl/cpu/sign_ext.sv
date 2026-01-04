`include "defs.svh"
`include "defs_rv32i.svh"

typedef enum logic [2:0] {
  IMM_I, IMM_S, IMM_B, IMM_U, IMM_J
} imm_t;

module sign_ext (
  input logic [`INST_WIDTH-1:0] i_inst,
  input imm_t i_sel,
  output logic [`XLEN-1:0] o_imm
);

  always_comb begin
    case (i_sel)
      IMM_I: o_imm = {{20{i_inst[31]}}, i_inst[31:20]};
      IMM_S: o_imm = {{20{i_inst[31]}}, i_inst[31:25], i_inst[11:7]};
      IMM_B: o_imm = {{19{i_inst[31]}}, i_inst[31], i_inst[7], i_inst[30:25], i_inst[11:8], 1'b0};
      IMM_U: o_imm = {i_inst[31:12], 12'b0};
      IMM_J: o_imm = {{11{i_inst[31]}}, i_inst[31], i_inst[19:12], i_inst[20], i_inst[30:21], 1'b0};
    endcase
  end

endmodule