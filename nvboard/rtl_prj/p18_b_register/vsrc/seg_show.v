module seg_show (
 
     seg_in     // < i
    ,seg0       // > o
    ,seg1       // > o
    ,seg2       // > o
    ,seg3       // > o
    ,seg4       // > o
    ,seg5       // > o
    ,seg6       // > o
    ,seg7       // > o
);
    input    [7:0]                                    seg_in;
    output   [7:0]   seg0,seg1,seg2,seg3,seg4,seg5,seg6,seg7;
    
    // 7段译码函数
    function  [7:0]   hex_seg;
        input [3:0]       hex;
        begin
            case (hex)
                4'h0: hex_seg = 8'b00000011; // 0
                4'h1: hex_seg = 8'b10011111; // 1
                4'h2: hex_seg = 8'b00100101; // 2
                4'h3: hex_seg = 8'b00001101; // 3
                4'h4: hex_seg = 8'b10011001; // 4
                4'h5: hex_seg = 8'b01001001; // 5
                4'h6: hex_seg = 8'b01000001; // 6
                4'h7: hex_seg = 8'b00011111; // 7
                4'h8: hex_seg = 8'b00000001; // 8
                4'h9: hex_seg = 8'b00001001; // 9
                4'hA: hex_seg = 8'b00010001; // A
                4'hB: hex_seg = 8'b11000001; // B
                4'hC: hex_seg = 8'b01100011; // C
                4'hD: hex_seg = 8'b10000101; // D
                4'hE: hex_seg = 8'b01100001; // E
                4'hF: hex_seg = 8'b01110001; // F
            endcase
        end
    endfunction
    assign seg0 = hex_seg(seg_in[3:0]);
    assign seg1 = hex_seg(seg_in[7:4]);
    assign seg2 = 8'b00000011; 
    assign seg3 = 8'b00000011; 
    assign seg4 = 8'b00000011; 
    assign seg5 = 8'b00000011; 
    assign seg6 = 8'b00000011; 
    assign seg7 = 8'b00000011; 
endmodule
