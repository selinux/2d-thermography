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

clear all
close all

% load pakage for serial communication
% http://wiki.octave.org/Instrument_control_package
% pkg load instrument_control



%s0 = serial();
x = 41;
y = 21;
min = 1000;
max = 0;
s0 = serial("/dev/ttyACM0", 115200)
srl_flush(s0);
srl_write(s0, "40");
srl_write(s0, "20");

while(char(srl_read(s0,1)) != "#" )
  %fprintf("not started yet\n");
endwhile

srl_read(s0,1); % skip one more

l = 1;
while( l <= y+1 )
  k = 1;
  
  while( k <= x)
  
    data = srl_read(s0, 4);
    mesure = str2num(char(data)); % Convert uint8 array to string, 
    mesure = mesure/100;
    % values are read in both direction
    if(mod(l,2) == 0 )     
      img(l,k) = mesure;
    else
      img(l,x-k+2) = mesure; % enter value backward
    endif
    
    % hold min ans max mesure
    if( min > mesure )
      min = mesure;
    elseif ( max < mesure )
      max = mesure;
    endif
    
    k++;
    
  endwhile
  data = srl_read(s0,4);
  %img(j,x+1) = str2num(char(data));
  l++;
  %srl_flush(s0);
endwhile

img = img;

% normalize image values from 0 to 255
%img1 = img .- min;
%img1 = img1./(max-min);
img1 = img;
img1 = img1./(50);

img1 = img1.*255;

% print result
image(img1)

