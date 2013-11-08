module galController(Row,clkout,enable,reset,clock);
	input enable, reset, clock;
	output Row[0:7];
  
  	clockDivider(clkout, clock);
  
  	and and0(d0,q1,q2,q3);
  	Dfflip DFF0(q0,nq0,d0,clock,reset,enable);
  
  	and and10(y10,q1,nq2);
  	and and11(y11,nq1,q2,q3);
  	and and12(y12,q1,q2,nq3);
  	or  or1(d1,y10,y11,y12);
  	Dfflip DFF1(q1,nq1d1,clock,reset,enable);
  
  	and and20(y20,nq2,q3);
  	and and21(y21,q2,nq3);
  	or  or2(d2,y20,y21);
  	Dfflip DFF2(q2,nq2,d2,clock,reset,enable);
  
  	
  	and out0(Row[0],nq2,nq1,nq0);
  	and out1(Row[1],nq2,nq1,q0);
  	and out2(Row[2],nq2,q1,nq0);
  	and out3(Row[3],nq2,q1,q0);
  	
  	and out4(Row[4],q2,nq1,nq0);
  	and out5(Row[5],q2,nq1,q0);
  	and out6(Row[6],q2,q1,nq0);
  	and out7(Row[7],q2,q1,q0);
		
endmodule

module Dfflip(q, qBar, D, clk, rst, enable);
  input D, clk, rst;
  output q, qBar;

  reg q;

  not n1 (qBar, q);
  not n2 (nenable, enable);

  always@ (posedge rst or posedge clk)
  begin
    if(rst or nenable)
      q = 0;

    else
      q = D;
 
  end

endmodule

clockDivider(clkout, clock){
//600kHz out 
//20Mhz in
input clock;
output clkout;
reg counter[6:0];

initial counter = 0;

always@ (posedge clock)
begin
	if(counter < 39)
		counter <= counter + 1;
	else 
		counter <= 0;
end

always@ (counter)
begin 
	if (counter < 20)
		clkout <= 1;
	else 
		clkout <= 0;
end
}

  

