%/
 %=============================================================================
 %
 %       Filename:  read_arduino.m
 %
 %    Description:  read serial data from arduino
 %
 %       Version:   1.0
 %       Created:   31. 08. 14 19:10:11
 %       Revision:  none
 %       Compiler:  GNU octave
 %
 %       Author:  Sebastien Chassot (sinux), sebastien.chassot@etu.hesge.ch
 %       Company:  HES-SO hepia section ITI (soir)
 %
 % ============================================================================
 %/

% load pakage for serial communication
% http://wiki.octave.org/Instrument_control_package
% pkg load instrument_control

%s0 = serial();

s0 = serial("/dev/ttyACM0", 115200)
srl_flush(s0);
srl_write(s0, "s");

while(char(srl_read(s0,1)) != "#" )
  %fprintf("not started yet\n");
endwhile

srl_read(s0,1); % skip one

j = 0;
while( j < 20 )
  i = 0;
  while( i < 20)
    % srl_write(s1, "Hello world!")
    data = srl_read(s0, 10);
    # Convert uint8 array to string, 
    char(data)
    i++;
  endwhile
  data = srl_read(s0,8);
  char(data)
  j++;
endwhile

