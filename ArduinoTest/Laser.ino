/*
  
*/
#include <LiquidCrystal.h>

#include <LiquidCrystal_I2C.h>
//Number of pulses to count
#define COUNT_NUMBER 3
// constants won't change. Used here to set a pin number:

const int laserPin = 13;// the number of the laser pin
const int sensorPin = 2;// Sensor pin
const int ledPinOut =7; //Control led pin
// Variables will change:
int laserState = LOW;             // laserState used to set the laser
int senseCounter = 0;
int senseTime = 0;
bool senseLaserFlag = false; // This flag is on wehn laser hit sensor
int sensorCount = 0;
int wasHitCount = 0;
int wasHitChangeFlag = false;
unsigned long timeSampels[COUNT_NUMBER] = {0}; 
unsigned long previousTimeSampel = 0;
int senseIndex = 0;
int laserSensorReadOut;
int flag = 0;
// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store

unsigned long previousMillis = 0;        // will store last time laser was updated


// constants won't change:
const int laserInterval = 500;       // laserInterval at which to blink (milliseconds)
const int laserIntervalEpsiolon = 20; //Error on sensing the laser 

LiquidCrystal_I2C lcd(0x27, 16, 2);  //LCD  Module 
void setup() {
    // set the digital pin as output:
    pinMode(laserPin, OUTPUT);
    pinMode(sensorPin, INPUT);
    pinMode(ledPinOut, OUTPUT);
    
    lcd.begin();
    lcd.backlight();
    lcd.clear();
   
}

void loop() {
    // here is where you'd put code that needs to be running all the time.

    // check to see if it's time to blink the laser; that is, if the difference
    // between the current time and last time you blinked the laser is bigger than
    // the laserInterval at which you want to blink the laser.
    unsigned long currentTime = millis();

     if (currentTime - previousMillis >= laserInterval) {
        // save the last time you blinked the laser
        previousMillis = currentTime;

        // if the laser is off turn it on and vice-versa:
        if (laserState == LOW) {
            laserState = HIGH;
        }
        else {
            laserState = LOW;
        }

        // Send Comand to laser
        digitalWrite(laserPin, laserState);
    }
    // ReadLaserSensor
    laserSensorReadOut = digitalRead(sensorPin);
    if (laserSensorReadOut & !senseLaserFlag){  //if laser sensors for first cycle 
        digitalWrite(ledPinOut, HIGH);
        senseCounter++; //count laser sensing
    
        senseLaserFlag = true; //set sense flag to true
        senseTime = currentTime; //save sense cycle time
       
        timeSampels[senseIndex] = senseTime; //add sensing time to array

        senseIndex++; //advance array to next position
        if (senseIndex > COUNT_NUMBER) { //reset to array beggning if reached the end
            senseIndex = 0;
        }



        lcd.setCursor(0, 0);
        lcd.print(senseCounter);
    }
    else
    { 
        if (!laserSensorReadOut & senseLaserFlag) { 
            senseLaserFlag = false;
     
            digitalWrite(ledPinOut, LOW); //Control led
        }

           
                     
    }
        flag = 0;
        if (previousTimeSampel != timeSampels[0]) { //check that new data is in array
            for (int i = 0; i < (COUNT_NUMBER - 1); i++) { //check diff between array

                if (((abs(timeSampels[i + 1] - timeSampels[i]) - 2 * laserInterval)) < laserIntervalEpsiolon) { //diff should be 2 times the interval
                    flag = 1;// good diff

                }
                else {
                    flag = 0; //bad diff


                }
            }
            
            if (flag) { //if diff in all of array is correct ----> Hit

                wasHitCount++;
                previousTimeSampel = timeSampels[0]; //for checking that new data has arrived
 

            }
        }
   
    lcd.setCursor(0, 1);
    lcd.print("Hit:");
    lcd.setCursor(5, 1);
    lcd.print(wasHitCount);

     
    


    
}