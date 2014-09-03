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

#define DIRECTION_X     1       // 1 or -1
#define DIRECTION_Y     1       // 1 or -1
#define STP_X           2
#define DIR_X           3
#define MS1_X           4
#define MS2_X           5
#define STP_Y           6
#define DIR_Y           7
#define MS1_Y           8
#define MS2_Y           9

#define END_X           10
#define END_Y           11

#define MOTX            0
#define MOTY            1


int a = 0;     //  gen counter

/* size to scan */
int size_x = 40;
int size_y = 20;

boolean start_scan = false;


void setup() 
{            

    Serial.begin(115200);
    Serial.println("Init MLX90614 temperature Sensor");
  
    PORTC = (1 << PORTC4) | (1 << PORTC5);  //enable internal pullup resistors on i2c ports
  
    Serial.println("Init motors");
    /* init motors 
     * *****************/
    DDRD = B11111100;         // set pins 2-7 as OUTPUT
    DDRB = B00000011;         // set pins 8-11 as OUTPUT
  
    /* Set micro stepping 
    * pins 4,5,8,9 HIGH (1/8 step) 
    * MS1 = L, MS2 = L   >  Full step 
    * MS1 = H, MS2 = L   >  Half step 
    * MS1 = L, MS2 = H   >  Quarter step 
    * MS1 = H, MS2 = H   >  Eighth step */
    //PORTD = B00110000;        // pins 4,5 HIGH (1/8 X step)
    //PORTB = B00000011;        // pins 8,9 HIGH (1/8 Y step)


    if( DIRECTION_X > 0 )
        PORTD |= (1<<DIR_X);  // set dir to HIGH
    if( DIRECTION_Y > 0 )
        PORTD |= (1<<DIR_Y);  // set dir to HIGH

   // initPos(0);

}


void loop(){

    int i = 0;
    int j = 0;
    long int tpl;

    int start_read = 0;
    if (Serial.available() > 0) {
        start_read = Serial.read();
        if(start_read != 0)
            start_scan = true;
    }


    if(start_scan == true){
        Serial.println("start scan #");
        // Go to upper line (to do so motor goes down)
        step( (size_y/2)*DIRECTION_Y*-1, MOTY);
        // Go to the beging of line 
        step( (size_x/2)*DIRECTION_X, MOTX);
    
        for ( j=0; j < size_y; j++ ){
    
            for ( i=0; i < size_x; i++ ){
                step(-1, MOTX);
                tpl = readMLXtemperature(0);
                Serial.print(tpl);
                //Serial.print(",");
            }
    
            tpl = readMLXtemperature(1);
            //Serial.print(":");
            Serial.println(tpl);
            //Serial.println(";");
    
            // Go one line down (motor up)
            step(-1, MOTY);
            // return to the begining of next line
            step(size_x, MOTX);
        }
    
        start_scan = false;

    }else{
        Serial.println("Scan termineted...");
        start_scan = false;
    }
    
}


/*  move the motor 0 or 1 (X/Y)
 *  int nb = signed number of step (sign = direction )
 */
int step( int nb, byte motor ){

    int i = 0;
    if( motor == MOTX ){     
 
        /* toggle direction */
        if( nb > 0 ){
            PORTD |= (1<<DIR_X);    
        }else{
            PORTD &= ~(1<<DIR_X);    // toggle direction
            nb *= -1;
        }
 
        for(i = 0; i <= nb; i++){
            PORTD |= (1<<STP_X);    // X step high
            delay(3);               
            PORTD &= ~(1<<STP_X);    // X step low
            delay(3);               
        }
 
    }else if( motor == MOTY){
 
        /* toggle direction */
        if( nb > 0 ){
            PORTD |= (1<<DIR_Y);    
        }else{
            PORTD &= ~(1<<DIR_Y);    // toggle direction
            nb *= -1;
        }
 
        for(i = 0; i <= nb; i++){
            PORTD |= (1<<STP_Y);    // X step high
            delay(3);               
            PORTD &= ~(1<<STP_Y);    // X step low
            delay(3);               
        }
        
 
    }else{
        return 1;
    }
 
    return 0;
}


void readPoint(){

    long int tpl;

    tpl = readMLXtemperature(0);
//    Serial.print("#");
    Serial.print(tpl);
//    tpl = readMLXtemperature(1);
//    Serial.print(",");
//    Serial.println(tpl);
//    Serial.println("Fin du cycle");


}

//****************************************************************
// read MLX90614 i2c ambient or object temperature
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
        end = digitalRead(END_X);

        while( end != HIGH ){
            step(-1, MOTX);
            delay(3);
            end = digitalRead(END_X);
        }

        step(160, MOTX);

    }



}

