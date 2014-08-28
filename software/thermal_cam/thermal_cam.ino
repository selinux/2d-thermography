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

#define DIRECTION_X     true
#define DIRECTION_Y     true
#define STP_X    2
#define DIR_X    3
#define MS1_X    4
#define MS2_X    5
#define STP_Y    6
#define DIR_Y    7
#define MS1_Y    8
#define MS2_Y    9

#define END_X    10
#define END_Y    11

int a = 0;     //  gen counter

void setup() 
{            
  Serial.begin(115200);
  Serial.println("Init MLX90614 temperature Sensor");

  PORTC = (1 << PORTC4) | (1 << PORTC5);  //enable internal pullup resistors on i2c ports

  Serial.println("Init motors");
  /* init motors 
   * *****************/
  DDRD = B11111100;         // set pins 2-7 as OUTPUT
  DDRB = B00001111;         // set pins 8-11 as OUTPUT

 /* Set micro stepping 
  * pins 4,5,8,9 HIGH (1/8 step) */
  PORTD = B00000011;        // pins 4,5 HIGH (1/8 X step)
  PORTB = B00110000;        // pins 8,9 HIGH (1/8 Y step)

  if( DIRECTION_X )
      PORTD |= (1<<DIR_X);  // toggle high/low
  if( DIRECTION_Y )
      PORTD |= (1<<DIR_Y);  // toggle high/low

}


void loop(){


    if (a < 250){  //sweep 200 step in dir 1
    
        a++;
        PORTD |= (1<<STP_X);    // X step high
        PORTD |= (1<<STP_Y);
        delay(1);               
        PORTD &= ~(1<<STP_X);    // X step low
        PORTD &= ~(1<<STP_Y);
        delay(16);              
//
        readPoint();

    }else{
        PORTD &= ~(1<<DIR_X);    // toggle direction
        PORTD &= ~(1<<DIR_Y);
        a++;
        PORTD |= (1<<STP_X);    // X step high
        PORTD |= (1<<STP_Y);
        delay(1);
        PORTD &= ~(1<<STP_X);    // X step low
        PORTD &= ~(1<<STP_Y);
        delay(16);
        readPoint();
    
        if (a>500){    //sweep 200 in dir 2
            delay(1000);
            a = 0;
            PORTD ^= (1<<DIR_X);    // toggle direction
            PORTD ^= (1<<DIR_Y);
      

        }
    }
}

void readPoint(){

    long int tpl;

    tpl = readMLXtemperature(0);
    Serial.print("La temps est de : ");
    Serial.print(tpl);
    tpl = readMLXtemperature(1);
    Serial.print(" ambient ");
    Serial.println(tpl);
    //Serial.println("Fin du cycle");


}

//****************************************************************
// read MLX90614 i2c ambient or object temperature
long int readMLXtemperature(int TaTo) {
    long int lii = 0;
    int dlsb,dmsb,pec;
    int dev = 0x5A<<1;

    i2c_init();
    i2c_start_wait(dev+I2C_WRITE);  // set device address and write mode

    (TaTo)? i2c_write(0x06) : i2c_write(0x07);                // or command read object or ambient temperature
    i2c_rep_start(dev+I2C_READ);    // set device address and read mode
    dlsb = i2c_readAck();           // read data lsb
    dmsb = i2c_readAck();           // read data msb
    pec = i2c_readNak();
    i2c_stop();

    lii=dmsb*0x100+dlsb;
    
    
    return(lii*2-27315);
}
