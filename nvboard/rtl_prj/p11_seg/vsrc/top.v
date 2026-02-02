module top (
    input        en,
    input  [7:0]  in,
    output [2:0] ledr,
    output [7:0] seg0
);
    // 中间信号：encode_out
    wire [2:0] encode_out;

    p_encode8_3 my_p_encode8_3(
        .en(en),
        .in(in),
        .ledr_out(encode_out)
    );
    
    assign ledr = encode_out;

    seg my_seg(
        .seg_in(encode_out),
        .seg_out(seg0)
    );
endmodule

/*
in[7:0]  --\
            ->  p_encode8_3  -> code[2:0]  -> 3 个 LED （ldr）
en       --/                            \
                                          -> seg -> seg0[7:0]
*/
