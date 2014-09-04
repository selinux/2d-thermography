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
#include <stdlib.h>


//#define INVERT_X
//#define INVERT_Y

#ifdef INVERT_X
#define FW_X    1
#define BW_X    0
#else
#define FW_X    0
#define BW_X    1
#endif

#ifdef INVERT_Y
#define FW_Y    1
#define BW_Y    0
#else
#define FW_Y    0
#define BW_Y    1
#endif


#define PIN_STPX        2
#define PIN_DIRX        3
#define MS1_X           4
#define MS2_X           5
#define PIN_STPY        6
#define PIN_DIRY        7
#define MS1_Y           8
#define MS2_Y           9

#define PIN_ENDX        10
#define PIN_ENDY        11
#define PIN_LASER       12

#define MOTX            0
#define MOTY            1


enum MS { FULL, HALF, QUART, EIGHTH };

enum MS ms_x = QUART;
enum MS ms_y = FULL;


int a = 0;     //  gen counter

/* size to scan */
//int size_x = 0;
//int size_y = 0;

boolean start_scan = false;


void setup() 
{            

    Serial.begin(115200);
    Serial.println("Init MLX90614 temperature Sensor");
  
    PORTC = (1 << PORTC4) | (1 << PORTC5);  //enable internal pullup resistors on i2c ports
  
    Serial.println("Init motors");
    /* init motors 
     * *****************/
    DDRD = B11001100;         // set pins 2-7 as OUTPUT
    DDRB = B00010000;         // set pins 8-11 as OUTPUT
  
    /* Set micro stepping 
    * sample : pins 4,5,8,9 HIGH (1/8 step) 
    * MS1 = L, MS2 = L   >  Full step 
    * MS1 = H, MS2 = L   >  Half step 
    * MS1 = L, MS2 = H   >  Quarter step 
    * MS1 = H, MS2 = H   >  Eighth step */

    switch( ms_x ){
        case HALF:
            digitalWrite(MS1_X, HIGH);
            break;
        case QUART:
            digitalWrite(MS2_X, HIGH);
            break;
        case EIGHTH:
            digitalWrite(MS1_X, HIGH);
            digitalWrite(MS2_X, HIGH);
            break;
        case FULL:
            break;
    }

    switch( ms_y ){
        case HALF:
            digitalWrite(MS1_Y, HIGH);
            break;
        case QUART:
            digitalWrite(MS2_Y, HIGH);
            break;
        case EIGHTH:
            digitalWrite(MS1_Y, HIGH);
            digitalWrite(MS2_Y, HIGH);
            break;
        case FULL:
            break;
    }

    digitalWrite(PIN_DIRX, FW_X);
    digitalWrite(PIN_DIRY, FW_Y);

}


void loop(){

    int i = 0;
    int j = 0;
    long int tpl;

    while (Serial.available() > 0) {

        // look for the next valid integer in the incoming serial stream:
        int size_x = Serial.parseInt();
        step(size_x, FW_X, MOTX);
        // do it again:
        int size_y = Serial.parseInt();
        Serial.println(size_y);
    
        if (Serial.read() == '\n' && size_x != 0 && size_y != 0 )
            start_scan == true;
    
    
        if(start_scan == true){
            Serial.println("start scan #");
            // Go to upper line (to do so motor goes down)
            step( (size_y/2), BW_Y, MOTY);
            // Go to the beging of line 
            step( (size_x/2), BW_X, MOTX);
        
            for ( j=0; j < size_y; j++ ){
        
                for ( i=0; i < size_x; i++ ){
                    if( j%2 == 0 )
                        step(1, FW_X, MOTX);
                    else
                        step(1, BW_X, MOTX);
    
                    tpl = readMLXtemperature(0);
                    Serial.print(tpl);
                }
        
                tpl = readMLXtemperature(1);
                Serial.print(tpl);
        
                // Go one line down (motor up)
                step(1, FW_Y, MOTY);
            }
        
            start_scan = false;
    
        }else{
            Serial.println("Scan termineted...");
            start_scan = false;
        }
    }
}


/*  move the motor 0 or 1 (X/Y)
 *  int nb = signed number of step (sign = direction )
 */
int step( int nb, int dir, byte motor ){

    int i = 0;
    if( motor == MOTX ){     
 
        /* select direction */
        digitalWrite(PIN_DIRX, dir);
        delay(1);               
 
        for(i = 0; i <= nb; i++){
            PORTD |= (1<<PIN_STPX);    // X step high
            delay(3);               
            PORTD &= ~(1<<PIN_STPX);    // X step low
            delay(3);               
        }
 
    }else if( motor == MOTY){
 
        /* select direction */
        digitalWrite(PIN_DIRY, dir);
        delay(1);               
 
        for(i = 0; i <= nb; i++){
            PORTD |= (1<<PIN_STPY);    // X step high
            delay(3);               
            PORTD &= ~(1<<PIN_STPY);    // X step low
            delay(3);               
        }
        
 
    }else{

        return 1;
    }
 
    return 0;
}


/****************************************************************
 *
 * read MLX90614 i2c ambient or object temperaturei
 *
 ****************************************************************/

long int readMLXtemperature(int TaTo) {
    long int lii = 0;
    int dlsb,dmsb,pec;
    int dev = 0x5A<<1;

    i2c_init();
    i2c_start_wait(dev+I2C_WRITE);  // set device address and write mode

    (TaTo)? i2c_write(0x06) : i2c_write(0x07); // or command read object or ambient temperature
    i2c_rep_start(dev+I2C_READ);    // set device address and read mode
    dlsb = i2c_readAck();           // read data lsb
    dmsb = i2c_readAck();           // read data msb
    pec = i2c_readNak();
    i2c_stop();

    lii=dmsb*0x100+dlsb;
    
    
    return(lii*2-27315);            // return degre*100 
}

void initPos( byte motor ){

    boolean end;
 
    if( motor == MOTX ){
        end = digitalRead(PIN_ENDX);

        while( end != HIGH ){
            step(1, BW_X, MOTX);
            delay(3);
            end = digitalRead(PIN_ENDX);
        }

        step(160, BW_X, MOTX);

    }

}

