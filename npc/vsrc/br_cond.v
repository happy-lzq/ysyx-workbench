module br_cond (
    rs1_data
    ,rs2_data
    ,br_type
    ,br_taken
);
    input wire [2:0] br_type;
    input wire [31:0] rs1_data;
    input wire [31:0] rs2_data;
    output reg [0: 0] br_taken;

    always @(*) begin
        case (br_type)
            3'b000 : br_taken = (rs1_data == rs2_data) ? 1'b1 : 1'b0;                   // beq
            3'b001 : br_taken = (rs1_data != rs2_data) ? 1'b1 : 1'b0;                   // bne
            3'b010 : br_taken = ($signed(rs1_data) <  $signed(rs2_data)) ? 1'b1 : 1'b0; // blt
            3'b011 : br_taken = ($signed(rs1_data) >= $signed(rs2_data)) ? 1'b1 : 1'b0; // bge
            3'b100 : br_taken = (rs1_data <  rs2_data) ? 1'b1 : 1'b0;                   // bltu
            3'b101 : br_taken = (rs1_data >= rs2_data) ? 1'b1 : 1'b0;                   // bgeu
            default: 
                br_taken = 1'b0;    
        endcase
    end
endmodule
