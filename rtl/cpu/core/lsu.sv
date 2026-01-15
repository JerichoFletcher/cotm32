import cotm32_pkg::BYTE_WIDTH;
import cotm32_pkg::XLEN;
import cotm32_pkg::lsu_ls_t;

// Load-store unit
module lsu (
  input lsu_ls_t i_op,
  input logic [XLEN-1:0] i_addr,
  input logic [XLEN-1:0] i_wdata,
  input logic [XLEN-1:0] i_rdata_bootrom,
  input logic [XLEN-1:0] i_rdata_dmem,
  input logic [XLEN-1:0] i_rdata_clint,

  input logic i_trap_req,

  output logic [XLEN-1:0] o_addr,
  output logic [XLEN-1:0] o_wdata,
  output logic [XLEN-1:0] o_rdata,
  output logic [XLEN/BYTE_WIDTH-1:0] o_wstrb,
  output logic o_we_dmem,
  output logic o_we_clint,

  output logic o_t_load_addr_misaligned,
  output logic o_t_load_access_fault,
  output logic o_t_store_addr_misaligned,
  output logic o_t_store_access_fault
);

  import cotm32_pkg::*;

  lsu_mem_src_t mem_src;
  logic [XLEN-1:0] mem_rdata;

  lsu_mem_src_t valid_load_src[3] = '{LSU_MEM_SRC_BOOTROM, LSU_MEM_SRC_DMEM, LSU_MEM_SRC_CLINT};
  lsu_mem_src_t valid_store_src[2] = '{LSU_MEM_SRC_DMEM, LSU_MEM_SRC_CLINT};

  logic we;
  logic we_dmem;
  logic we_clint;

  assign o_we_dmem = we && we_dmem;
  assign o_we_clint = we && we_clint;

  always_comb begin
    if ((BOOTROM_MEM_START == 0 || BOOTROM_MEM_START <= i_addr) && i_addr <= BOOTROM_MEM_END) begin
      mem_src = LSU_MEM_SRC_BOOTROM;
      o_addr = i_addr - BOOTROM_MEM_START;
    end else if ((DATA_MEM_START == 0 || DATA_MEM_START <= i_addr) && i_addr <= DATA_MEM_END) begin
      mem_src = LSU_MEM_SRC_DMEM;
      o_addr = i_addr - DATA_MEM_START;
    end else if ((CLINT_MEM_START == 0 || CLINT_MEM_START <= i_addr) && i_addr <= CLINT_MEM_END) begin
      mem_src = LSU_MEM_SRC_CLINT;
      o_addr = i_addr - CLINT_MEM_START;
    end else begin
      mem_src = LSU_MEM_SRC_UNKNOWN;
      o_addr = '0;
    end
  end

  always_comb begin
    we_dmem = '0;
    we_clint = '0;

    unique case (mem_src)
      LSU_MEM_SRC_BOOTROM : begin
        mem_rdata = i_rdata_bootrom;
      end
      LSU_MEM_SRC_DMEM    : begin
        mem_rdata = i_rdata_dmem;
        we_dmem = '1;
      end
      LSU_MEM_SRC_CLINT   : begin
        mem_rdata = i_rdata_clint;
        we_clint = '1;
      end
      default             : mem_rdata = '0;
    endcase
  end

  always_comb begin
    unique case (i_op)
      LSU_LOAD_B  : o_rdata = {{(3*XLEN/4){mem_rdata[XLEN/4-1]}}, mem_rdata[XLEN/4-1:0]};
      LSU_LOAD_H  : o_rdata = {{(XLEN/2){mem_rdata[XLEN/2-1]}}, mem_rdata[XLEN/2-1:0]};
      LSU_LOAD_W  : o_rdata = mem_rdata[XLEN-1:0];
      LSU_LOAD_BU : o_rdata = {{(3*XLEN/4){1'b0}}, mem_rdata[XLEN/4-1:0]};
      LSU_LOAD_HU : o_rdata = {{(XLEN/2){1'b0}}, mem_rdata[XLEN/2-1:0]};
      default     : o_rdata = mem_rdata[XLEN-1:0];
    endcase
  end

  always_comb begin
    unique case (i_op)
      LSU_STORE_B : begin
        o_wdata = {{(3*XLEN/4){1'b0}}, i_wdata[XLEN/4-1:0]};
        o_wstrb = 4'b0001;
        we = ~i_trap_req;
      end
      LSU_STORE_H : begin
        o_wdata = {{(XLEN/2){1'b0}}, i_wdata[XLEN/2-1:0]};
        o_wstrb = 4'b0011;
        we = ~i_trap_req;
      end
      LSU_STORE_W : begin
        o_wdata = i_wdata[XLEN-1:0];
        o_wstrb = 4'b1111;
        we = ~i_trap_req;
      end
      default     : begin
        o_wdata = i_wdata[XLEN-1:0];
        o_wstrb = 4'b0000;
        we = '0;
      end
    endcase
  end

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
      default: begin /* NOP */ end
    endcase
  end

  always_comb begin
    o_t_load_access_fault = '0;
    o_t_store_access_fault = '0;

    unique case (i_op)
      LSU_LOAD_B, LSU_LOAD_BU, LSU_LOAD_H, LSU_LOAD_HU, LSU_LOAD_W: begin
        if (!(mem_src inside {valid_load_src})) begin
          o_t_load_access_fault = '1;
        end
      end
      LSU_STORE_B, LSU_STORE_H, LSU_STORE_W: begin
        if (!(mem_src inside {valid_store_src})) begin
          o_t_store_access_fault = '1;
        end
      end
      default: begin /* NOP */ end
    endcase
  end

endmodule
