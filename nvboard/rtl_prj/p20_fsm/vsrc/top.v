module top (
     clk
    ,rst_n
    ,one
    ,half
    ,pi_quit
    ,po_money
    ,po_cola
    ,quit_clk
    ,dout
);
    input            clk,rst_n,one,half,pi_quit;
    output       quit_clk,po_money,po_cola,dout;

fsm u_fsm (
     .clk            (clk     )
    ,.rst_n          (rst_n   )
    ,.din            (half    )
    ,.dout           (dout    )
);

com_fsm u_com_fsm (
     .clk            (clk     )
    ,.rst_n          (rst_n   )
    ,.one            (one     )
    ,.half           (half    )
    ,.pi_quit        (pi_quit )
    ,.po_money       (po_money)
    ,.po_cola        (po_cola )
    ,.quit_clk       (quit_clk)
);

endmodule
