import cotm32_pkg::XLEN;

// Read-only memory with variable read ports
module rom #(
  parameter N_READ_PORTS = 1,
  parameter MEM_SIZE
) (
  input logic [XLEN-1:0] i_addr [0:N_READ_PORTS-1],
  output logic [XLEN-1:0] o_rdata [0:N_READ_PORTS-1]
);

  import cotm32_pkg::*;

  logic [BYTE_WIDTH-1:0] mem_bytes [0:MEM_SIZE-1] /* verilator public_flat_rw */;
  
  always_comb begin
    for (int i = 0; i < N_READ_PORTS; i++) begin
      logic [XLEN-1:0] addr = {i_addr[i][XLEN-1:2], 2'b0};

        if (addr >= MEM_SIZE) begin
          o_rdata[i] = '0;
        end else begin
          o_rdata[i] = {mem_bytes[addr+3], mem_bytes[addr+2], mem_bytes[addr+1], mem_bytes[addr]};
        end
    end
  end

endmodule
