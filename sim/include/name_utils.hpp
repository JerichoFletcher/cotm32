#pragma once

#include <string>

#include "cotm32_defs.hpp"

namespace cotm32::name_utils {
const std::string& reg_name(uint8_t reg_index);
const std::string& alu_op_name(AluOp alu_op);
const std::string& bu_op_name(BuOp bu_op);
const std::string& alu_a_name(AluPortA alu_a_sel);
const std::string& alu_b_name(AluPortB alu_b_sel);
const std::string& lsu_ls_op_name(LsuLoadStoreOp lsu_ls_op);
const std::string& reg_wb_name(RegWritebackSrc reg_wb_sel);

const std::string& csr_name(CsrId csr_id);
}  // namespace cotm32::name_utils
