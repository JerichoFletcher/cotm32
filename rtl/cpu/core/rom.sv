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
      for (int j = 0; j < XLEN/BYTE_WIDTH; j++) begin
        if (i_addr[i] + j < MEM_SIZE) begin
          o_rdata[i][j*BYTE_WIDTH+:BYTE_WIDTH] = mem_bytes[i_addr[i] + j];
        end
      end
    end
  end

endmodule
