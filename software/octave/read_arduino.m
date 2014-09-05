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


x = 200;
y = 60;

DATA = strcat(num2str(x),",",num2str(y),"\n")

img = zeros(y,x);
s0 = serial("/dev/ttyACM0", 115200)
pause(5);
%srl_flush(s0);
srl_write(s0, DATA);
srl_flush(s0);
%srl_write(s0, "20,10\n")
%srl_write(s0, '\n');

while(char(srl_read(s0,1)) != "#" )
  %fprintf("not started yet\n");
endwhile

srl_read(s0,2); % skip one more

l = 1;
while( l <= y )
  k = 1;
  
  while( k <= x)

    %srl_flush(s0);  
    data = srl_read(s0, 4);

    %char(data)
    mesure = str2num(char(data)); % Convert uint8 array to string, 
    mesure = mesure/100;
    %dump = srl_read(s0,1);
    % values are read in both direction
    if(mod(l,2) == 1 )     
      img(l,k) = mesure;
    else
      %img(l,k) = mesure;
      img(l,x-k+1) = mesure; % enter value backward
    endif
    
    
    k++;
    
  endwhile
  data = srl_read(s0,4);
  %img(j,x+1) = str2num(char(data));
  l++;
  %srl_flush(s0);
endwhile

srl_close(s0);

%img = img;
minimum = min(min(img));
maximum = max(max(img));

% normalize image values from 0 to 255
%img1 = img .- min;
colormap(jet (128));
img1 = img./maximum.*128;

img2 = img .- minimum;
img2 = img2./(maximum-minimum).*128;

%img1 = img;
%img1 = img1./(50);

%img1 = img1.*128;

% print result
%image(img1)
image(img2)
