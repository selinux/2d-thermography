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
#define PIN_DEBUG       13

#define MOTX            0
#define MOTY            1
#define INIT_STEPX      110
#define INIT_STEPY      110
#define SPEED           2
//#define READDELAY       90

enum MS { FULL=1, HALF=2, QUART=4, EIGHTH=8 };

enum MS ms_x = HALF;
enum MS ms_y = HALF;

void ms_step(MS ms_x, MS ms_y);

/* size to scan */
int size_x = 0;
int size_y = 0;
int r_delay = 90;

boolean start_scan = false;


void setup() 
{            

    Serial.begin(57600);
    Serial.println("Init MLX90614 temperature Sensor");
  
    PORTC = (1 << PORTC4) | (1 << PORTC5);  //enable internal pullup resistors on i2c ports
  
    Serial.println("Init motors");
    
    /* ----------------------
     * init motors 
     * **********************/
    DDRD = B11001100;         // set pins 2,3,6,7 as OUTPUT
    DDRB = B00110000;         // set pins 12 as OUTPUT + debug
  
  
    /* force laser LOW just in case !!!! */
    digitalWrite(PIN_LASER, LOW);
 
    ms_step(ms_x, ms_y);


    digitalWrite(PIN_DIRX, FW_X);
    digitalWrite(PIN_DIRY, FW_Y);
    digitalWrite(PIN_DEBUG, LOW);

    initPos(MOTX);
    initPos(MOTY);
    
    digitalWrite(PIN_LASER, HIGH);
}


void loop(){

    int i = 0;
    int j = 0;
    long int tpl;


    // look for the next valid integer in the incoming serial stream:
    size_x = Serial.parseInt();
    size_y = Serial.parseInt();
    
    ms_x = (MS)Serial.parseInt();
    ms_y = (MS)Serial.parseInt();
    
    int read_delay = Serial.parseInt();
    
    
    if ( size_x != 0 && size_y != 0 ){
        Serial.print(size_x);
        Serial.print(" ");
        Serial.print(size_y);
        Serial.println(" Ok pour commencer");
        start_scan = true;
    }


    if(start_scan == true){

        Serial.println("start scan #");

        digitalWrite(PIN_LASER, HIGH);
        ms_step(ms_x, ms_y);


        // Go to upper line (to do so motor goes down)
        step( (size_y/2), BW_Y, MOTY);
        // Go to the beging of line 
        step( (size_x/2), BW_X, MOTX);
    
        for ( j=0; j < size_y; j++ ){
    
            for ( i=0; i < size_x; i++ ){
              
                if( j%2 == 0 ){
                    step(1, FW_X, MOTX);
                    //tpl = 1000;   // debug value
                }else{
                    step(1, BW_X, MOTX);
                    //tpl = 2000;   // debug value
                }
                tpl = readMLXtemperature(0);
                Serial.print(tpl);
                delay(read_delay);

            }
    
            /* move backward */
            //step( size_x*2, BW_X, MOTX);

            /* lost step correction */
//            if( j%2 == 0 )
//                step(3, BW_X, MOTX);
//            else
//                step(3, FW_X, MOTX);

            /* at the EOL, ambient temperature is sent */
            tpl = readMLXtemperature(1);
            Serial.print(tpl);
            
            /* debug test 10 times to be sur the MLX buffer s empty */
            for(int u=0;u<10;u++){
                  tpl = readMLXtemperature(1);
                  delay(read_delay);
            }

            
            /* Go one line down (motor up) */
            step(1, FW_Y, MOTY);

        }

        /* move back to origin and turn the laser off */
        step( (size_y/2), BW_Y, MOTY);
        step( (size_x/2), BW_X, MOTX);

        start_scan = false;


    }else{
        Serial.println("Waiting...");
        start_scan = false;
    }
}

/**
 * @brief move motor 0 or 1 (X/Y) n step
 *
 * @param nb number of step
 *
 * @param dir 0 or 1 (BW_X,FW_X)
 *
 * @param motor motor number
 *
 */
void step( int nb, int dir, byte motor ){

   /* Set micro stepping 
    * sample : pins 4,5,8,9 HIGH (1/8 step) 
    * MS1 = L, MS2 = L   >  Full step 
    * MS1 = H, MS2 = L   >  Half step 
    * MS1 = L, MS2 = H   >  Quarter step 
    * MS1 = H, MS2 = H   >  Eighth step */
    int i = 0;
    if( motor == MOTX ){     
 
        /* select direction */
        digitalWrite(PIN_DIRX, dir);
        delay(1);
 
        for(i = 0; i <= nb; i++){
            PORTD |= (1<<PIN_STPX);    // X step high
            delay(SPEED);               
            PORTD &= ~(1<<PIN_STPX);    // X step low
            delay(SPEED);               
        }
 
    }else{
 
        /* select direction */
        digitalWrite(PIN_DIRY, dir);
        delay(1);               
 
        for(i = 0; i <= nb; i++){
            PORTD |= (1<<PIN_STPY);    // X step high
            delay(SPEED);
            PORTD &= ~(1<<PIN_STPY);    // X step low
            delay(SPEED);             
        }
    }
}


/**
 * @brief read MLX90614 i2c ambient or object temperature
 *
 * @param TaTo 0 = object temp and 1 = MLX90614 temp 
 *
 * @return the temperature in degre * 10^2
 *
 */
long int readMLXtemperature(int TaTo) {
    long int lii = 0;
    int dlsb,dmsb,pec;
    int dev = 0x5A<<1;

    i2c_init();
    i2c_start_wait(dev+I2C_WRITE);             // set device address and write mode

    (TaTo)? i2c_write(0x06) : i2c_write(0x07); // command read object or ambient temperature
    i2c_rep_start(dev+I2C_READ);               // set device address and read mode
    dlsb = i2c_readAck();                      // read data lsb
    dmsb = i2c_readAck();                      // read data msb
    pec = i2c_readNak();
    i2c_stop();

    lii=dmsb*0x100+dlsb;
    
    
    return(lii*2-27315);                       // return degre*100 (conversion done by octave)
}

/**
 * @brief bring the motor to the endstop and move in midle position
 *
 * @param motor number (0 or 1)
 *
 */
void initPos( byte motor ){

    boolean end;
 
    if( motor == MOTX ){
        end = digitalRead(PIN_ENDX);

        while( end != HIGH ){
            step(1, FW_X, MOTX);
            delay(1);
            end = digitalRead(PIN_ENDX);
        }

        while( end == HIGH ){
            step(1, BW_X, MOTX);
            delay(1);
            end = digitalRead(PIN_ENDX);
        }

        step(INIT_STEPX*ms_x, BW_X, MOTX);

    }else{

        end = digitalRead(PIN_ENDY);

        while( end != HIGH ){
            step(1, FW_Y, MOTY);
            delay(1);
            end = digitalRead(PIN_ENDY);
        }

        while( end == HIGH ){
            step(1, BW_Y, MOTY);
            delay(1);
            end = digitalRead(PIN_ENDY);
        }

        step(INIT_STEPY*ms_y, BW_Y, MOTY);
    }

}

void ms_step(MS ms_x, MS ms_y){
  
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
}
