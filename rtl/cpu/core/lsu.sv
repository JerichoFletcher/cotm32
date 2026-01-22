// Load-store unit
module lsu
  import cotm32_pkg::*;
  import cotm32_priv_pkg::*;
(
  input lsu_ls_t i_op,
  input logic [XLEN-1:0] i_addr,
  input logic [XLEN-1:0] i_wdata,
  input logic [XLEN-1:0] i_rdata_imem,
  input logic [XLEN-1:0] i_rdata_clint,
  input logic [XLEN-1:0] i_rdata_uart,
  input logic [XLEN-1:0] i_rdata_dmem,

  input priv_mode_t i_priv_mode,
  input logic i_trap_req,

  output logic [XLEN-1:0] o_addr,
  output logic [XLEN-1:0] o_wdata,
  output logic [XLEN-1:0] o_rdata,
  output logic [XLEN/BYTE_WIDTH-1:0] o_wstrb,

  output logic o_re_uart,

  output logic o_we_dmem,
  output logic o_we_uart,
  output logic o_we_clint,

  output logic o_t_load_addr_misaligned,
  output logic o_t_load_access_fault,
  output logic o_t_store_addr_misaligned,
  output logic o_t_store_access_fault
);

  import cotm32_pkg::*;

  lsu_mem_src_t mem_src;
  logic [XLEN-1:0] mem_rdata;

  logic re;
  logic we;
  logic src_dmem;
  logic src_uart;
  logic src_clint;

  assign o_re_uart = re && src_uart;

  assign o_we_dmem = we && src_dmem;
  assign o_we_uart = we && src_uart;
  assign o_we_clint = we && src_clint;

  always_comb begin
    if ((IMEM_MEM_START == 0 || IMEM_MEM_START <= i_addr) && i_addr <= IMEM_MEM_END) begin
      mem_src = LSU_MEM_SRC_IMEM;
      o_addr = i_addr - IMEM_MEM_START;
    end else if ((CLINT_MEM_START == 0 || CLINT_MEM_START <= i_addr) && i_addr <= CLINT_MEM_END) begin
      mem_src = LSU_MEM_SRC_CLINT;
      o_addr = i_addr - CLINT_MEM_START;
    end else if ((UART_MEM_START == 0 || UART_MEM_START <= i_addr) && i_addr <= UART_MEM_END) begin
      mem_src = LSU_MEM_SRC_UART;
      o_addr = i_addr - UART_MEM_START;
    end else if ((DATA_MEM_START == 0 || DATA_MEM_START <= i_addr) && i_addr <= DATA_MEM_END) begin
      mem_src = LSU_MEM_SRC_DMEM;
      o_addr = i_addr - DATA_MEM_START;
    end else begin
      mem_src = LSU_MEM_SRC_UNKNOWN;
      o_addr = '0;
    end
  end

  always_comb begin
    src_dmem = '0;
    src_uart = '0;
    src_clint = '0;

    unique case (mem_src)
      LSU_MEM_SRC_IMEM : begin
        mem_rdata = i_rdata_imem;
      end
      LSU_MEM_SRC_CLINT   : begin
        mem_rdata = i_rdata_clint;
        src_clint = '1;
      end
      LSU_MEM_SRC_UART    : begin
        mem_rdata = i_rdata_uart;
        src_uart = '1;
      end
      LSU_MEM_SRC_DMEM    : begin
        mem_rdata = i_rdata_dmem;
        src_dmem = '1;
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
    re = '0;
    we = '0;

    o_wdata = i_wdata[XLEN-1:0];
    o_wstrb = 4'b0000;

    unique case (i_op)
      LSU_LOAD_B, LSU_LOAD_H, LSU_LOAD_W, LSU_LOAD_BU, LSU_LOAD_HU: begin
        re = '1;
      end
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
      default     : begin /* NOP */ end
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
        unique case (i_priv_mode)
          PRIV_M: begin
            if (!(mem_src inside {LSU_MEM_SRC_IMEM, LSU_MEM_SRC_CLINT, LSU_MEM_SRC_UART, LSU_MEM_SRC_DMEM})) begin
              o_t_load_access_fault = '1;
            end
          end
          PRIV_U: begin
            if (!(mem_src inside {LSU_MEM_SRC_IMEM, LSU_MEM_SRC_DMEM})) begin
              o_t_load_access_fault = '1;
            end
          end
          default: o_t_load_access_fault = '1;
        endcase
      end
      LSU_STORE_B, LSU_STORE_H, LSU_STORE_W: begin
        unique case (i_priv_mode)
          PRIV_M: begin
            if (!(mem_src inside {LSU_MEM_SRC_CLINT, LSU_MEM_SRC_UART, LSU_MEM_SRC_DMEM})) begin
              o_t_store_access_fault = '1;
            end
          end
          PRIV_U: begin
            if (!(mem_src inside {LSU_MEM_SRC_DMEM})) begin
              o_t_store_access_fault = '1;
            end
          end
          default: o_t_store_access_fault = '1;
        endcase
      end
      default: begin /* NOP */ end
    endcase
  end

endmodule
