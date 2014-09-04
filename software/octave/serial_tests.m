%/
 %=============================================================================
 %
 %       Filename:  serial_test.m
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

clear all
close all

%colormap(hsv (128));

% load pakage for serial communication
% http://wiki.octave.org/Instrument_control_package
% pkg load instrument_control



%s0 = serial();
x = 40;
y = 20;
min = 1000;
max = 0;

s0 = serial("/dev/ttyACM2", 115200);
srl_flush(s0);
srl_write(s0, "40,20,34");

%while(1)
%  data = srl_read(s0,4);
%  mesure = str2num(char(data))
  %fprintf(mesure)
  %fprintf("\n")

%endwhile
