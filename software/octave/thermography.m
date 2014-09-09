%/
 %=============================================================================
 %
 %       Filename:  thermography.m
 %
 %    Description:  read serial data from arduino (temperatures) and render an image 
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


%-------------------------------------
%
% init variables
%
%=====================================

COLORS = 1024;
x = 100;
y = 60;
% micro stepping values [1,2,4,8] (1,1/2,1/4,1/8)
ms_x = 4;
ms_y = 4;

% delay between mesure from 1ms to Tms
% 1 is too short, 90-100ms give finest scan (higher value is a waste of time)
r_delay = 90;

% string to be passed to arduino
DATAS = strcat(num2str(x),",",num2str(y),",",num2str(ms_x),",",num2str(ms_y),",",num2str(r_delay),"\n");


% init empty matrice 
img = zeros(y,x);
capteur = zeros(1,y);     % une valeur capteur par ligne



%------------------------------------
% look for a valid serial port
%====================================
d = -1;

do 

    d++;
    device = strcat("/dev/ttyACM",num2str(d));
    
until ((exist(device) == 2) && d < 10)

% open it
s0 = serial(device, 115200);

% the arduino make a hard reset when a connexion is activated so wait 
% unitil the homing finished
pause(5);

% send the scan command
srl_write(s0, DATAS);
srl_flush(s0);

% wait for the begining of datas
while(char(srl_read(s0,1)) != "#" )
  % do nothing
endwhile

srl_read(s0,2); % skip \n

% time counter
start_time = time;


l = 1;
while( l <= y )
  k = 1;
  
  while( k <= x)

    data = srl_read(s0, 4);

    mesure = str2num(char(data)); % Convert uint8 array to string, 
    mesure = mesure/100;
    %dump = srl_read(s0,1);
    % values are read in both direction
    if(mod(l,2) == 1 )     
      img(l,k) = mesure;
    else
      img(l,x-k+1) = mesure; % enter value backward
    endif
    
    
    k++;
    
  endwhile
  data = srl_read(s0,4);
  
  capteur(1,l) = str2num(char(data));
  l++;
endwhile

srl_close(s0);

%img = img;
minimum = min(min(img));
maximum = max(max(img));
min_capt = min(capteur);
max_capt = max(capteur);
moy_capteur = sum(capteur(:,1))/size(capteur)(1)

% normalize image values from 0 to 255
%img1 = img .- min;
colormap(jet (COLORS));
img1 = img./maximum.*COLORS;

img2 = img .- minimum;
img2 = img2./(maximum-minimum).*COLORS;

%img1 = img;
%img1 = img1./(50);

%img1 = img1.*128;

% print result
%image(img1)
image(img2);

enlapsed_time = time - start_time;
minimum
maximum
delta = maximum - minimum

min_capt = min(capteur)/100
max_capt = max(capteur)/100
moy_capteur = sum(capteur(:,1))/size(capteur)(1)./100

title ("Thermogram");
xlabel ("largeur");
ylabel ("longueur");

caxis([minimum maximum]);
colorbar