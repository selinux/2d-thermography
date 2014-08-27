/*
 * =====================================================================================
 *
 *       Filename:  thermal-cam.ino
 *
 *    Description:  move and read an IR MLX90614 (ESF-DCI)
 *
 *        Version:  1.0
 *        Created:  26. 08. 14 16:59:30
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sebastien Chassot (sinux), sebastien.chassot@etu.hesge.ch
 *        Company:  HES-SO hepia section ITI (soir)
 *
 * =====================================================================================
 */


#include <i2cmaster.h>


#define END_X     2
#define END_Y     3
#define DIR_X    4
#define STP_X    5
#define DIR_Y    6
#define STP_Y    7


int a = 0;     //  gen counter

void setup() 
{                
  pinMode(DIR_X, OUTPUT);
  pinMode(STP_X, OUTPUT);       
  pinMode(DIR_Y, OUTPUT);
  pinMode(STP_Y, OUTPUT);       
  pinMode(END_X, INPUT);
  pinMode(END_Y, INPUT);
}


void loop() 
{
  if (a <  200)  //sweep 200 step in dir 1
   {
    a++;
    digitalWrite(STP_X, HIGH);   
//    delay(10);               
    digitalWrite(STP_X, LOW);  
//    delay(10);              
   }
  else 
   {
    digitalWrite(DIR_X, HIGH);
    a++;
    digitalWrite(STP_X, HIGH);  
//    delay(10);
    digitalWrite(STP_X, LOW);  
//    delay(10);
    
    if (a>400)    //sweep 200 in dir 2
     {
      a = 0;
      digitalWrite(DIR_X, LOW);
     }
    }
}
