`ifndef CTRL_DEFS_VH
`define CTRL_DEFS_VH

// EX ctrl
`define EX_CTRL_WIDTH           11
`define EX_CTRL_ALU_OP_MSB      10
`define EX_CTRL_ALU_OP_LSB      6
`define EX_CTRL_ALU_SRC_A       5
`define EX_CTRL_ALU_SRC_B_MSB   4
`define EX_CTRL_ALU_SRC_B_LSB   3
`define EX_CTRL_BR_TYPE_MSB     2
`define EX_CTRL_BR_TYPE_LSB     0

// MEM ctrl
`define MEM_CTRL_WIDTH          5
`define MEM_CTRL_READ           4
`define MEM_CTRL_WRITE          3
`define MEM_CTRL_LSU_MSB        2
`define MEM_CTRL_LSU_LSB        0

// WB ctrl
`define WB_CTRL_WIDTH           8
`define WB_CTRL_REG_WRITE       7
`define WB_CTRL_WDATA_SRC_MSB   6
`define WB_CTRL_WDATA_SRC_LSB   5
`define WB_CTRL_RD_ADDR_MSB     4
`define WB_CTRL_RD_ADDR_LSB     0    

// CSR ctrl（控制 + 数据）
`define CSR_CTRL_WIDTH          49
`define CSR_CTRL_READ           48
`define CSR_CTRL_WRITE          47
`define CSR_CTRL_OP_MSB         46
`define CSR_CTRL_OP_LSB         45
`define CSR_CTRL_IMM            44
`define CSR_CTRL_ADDR_MSB       43
`define CSR_CTRL_ADDR_LSB       32
`define CSR_CTRL_ZIMM_MSB       31    // ← 新增：csr_zimm 占低 32 位
`define CSR_CTRL_ZIMM_LSB       0

// SYS ctrl（系统/异常控制）
`define SYS_CTRL_WIDTH         35
`define SYS_CTRL_TRAP_ENTER    34
`define SYS_CTRL_MRET          33
`define SYS_CTRL_IS_EBREAK     32
`define SYS_CTRL_TRAP_CODE_MSB 31
`define SYS_CTRL_TRAP_CODE_LSB 0

// PC_ctrl
`define PC_CTRL_WIDTH           37
`define PC_SEL_MSB              36
`define PC_SEL_LSB              35
`define BR_TAKEN                34
`define IS_JAL                  33
`define IS_JALR                 32
`define TRAP_CODE_MSB           31
`define TRAP_CODE_LSB           0                       
`endif 
