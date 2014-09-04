/*
 * =====================================================================================
 *
 *       Filename:  serial_only.ino
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

}


void loop(){

    //while (Serial.available() > 4) {

        // look for the next valid integer in the incoming serial stream:
        int size_x = Serial.parseInt();
        // do it again:
        int size_y = Serial.parseInt();

        Serial.println(size_x);
        Serial.println(size_y);
    
        if ( size_x != 0 && size_y != 0 ){
            Serial.println("Ok pour commencer");
            start_scan == true;
        }
    
    
        if(start_scan == true){
            Serial.println("start scan #");
            start_scan = false;
    
        }else{
            Serial.println("Scan termineted...");
            start_scan = false;
        }
    //}
}
