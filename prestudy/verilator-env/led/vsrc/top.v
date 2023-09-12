module top (
    input clk,
    input rst,
    // input ps2_clk,
    // input ps2_data,
    output [15:0] led
);

light light1(
    .clk(clk),
    .rst(rst),
    .led(led)
);


// ps2_keyboard my_keyboard(
//     .clk(clk),
//     .resetn(~rst),
//     .ps2_clk(ps2_clk),
//     .ps2_data(ps2_data)
// );

endmodule
