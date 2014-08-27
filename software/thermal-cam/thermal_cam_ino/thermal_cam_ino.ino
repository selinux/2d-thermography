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


#define END_X    2
#define END_Y    3
#define DIR_X    5
#define STP_X    4
#define DIR_Y    7
#define STP_Y    6


int a = 0;     //  gen counter

void setup() 
{            
  Serial.begin(115200);
  
  pinMode(DIR_X, OUTPUT);
  pinMode(STP_X, OUTPUT);       
  pinMode(DIR_Y, OUTPUT);
  pinMode(STP_Y, OUTPUT);       
  pinMode(END_X, INPUT);
  pinMode(END_Y, INPUT);
}


void loop() 
{
  if (a <  400)  //sweep 200 step in dir 1
   {
    a++;
    digitalWrite(STP_X, HIGH);   
    digitalWrite(STP_Y, HIGH);   
    delay(1);               
    digitalWrite(STP_X, LOW);  
    digitalWrite(STP_Y, LOW);
    delay(1);              
   }
  else 
   {
    digitalWrite(DIR_X, HIGH);
    digitalWrite(DIR_Y, HIGH);    
    a++;
    digitalWrite(STP_X, HIGH);  
    digitalWrite(STP_Y, HIGH);
    delay(1);
    digitalWrite(STP_X, LOW);  
    digitalWrite(STP_Y, LOW); 
    delay(1);
    
    if (a>800)    //sweep 200 in dir 2
     {
      a = 0;
      digitalWrite(DIR_X, LOW);
      digitalWrite(DIR_Y, LOW);
      
      Serial.println("Fin du cycle");
     }
    }
}
