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

a = 0;

while( a < 1000)
  % srl_write(s1, "Hello world!")
  data = srl_read(s0, 10);
  # Convert uint8 array to string, 
  char(data)
  a++;
endwhile