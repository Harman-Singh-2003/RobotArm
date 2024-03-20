#include <Wire.h> //This is for i2C
#include <AccelStepper.h>

int stepPin = 3;
int dirPin = 2;

AccelStepper motor1(1, stepPin, dirPin);

// i2C OLED
#define I2C_ADDRESS 0x3C
#define RST_PIN -1
float OLEDTimer = 0; //Timer for the screen refresh
//I2C pins:
//STM32: SDA: PB7 SCL: PB6
//Arduino: SDA: A4 SCL: A5

//---------------------------------------------------------------------------
//Magnetic sensor things
int magnetStatus = 0; //value of the status register (MD, ML, MH)

int lowbyte; //raw angle 7:0
word highbyte; //raw angle 7:0 and 11:8
int rawAngle; //final raw angle 
float degAngle; //raw angle in degrees (360/4096 * [value between 0-4095])

int quadrantNumber, previousquadrantNumber; //quadrant IDs
float numberofTurns = 0; //number of turns
float correctedAngle = 0; //tared angle - based on the startup value
float startAngle = 0; //starting angle
float totalAngle = 0; //total absolute angular displacement
float previoustotalAngle = 0; //for the display printing

float degree = 0;
int input;

const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data

bool newData = false;

int dataNumber = 0;             // new for this version

void setup()
{
  motor1.setMaxSpeed(800);
  pinMode(12, OUTPUT);
  digitalWrite(12,HIGH);
  Serial.begin(115200); //start serial - tip: don't use serial if you don't need it (speed considerations)
  Wire.begin(); //start i2C  
	Wire.setClock(800000L); //fast clock

  // checkMagnetPresence(); //check the magnet (blocks until magnet is found)

  // ReadRawAngle(); //make a reading so the degAngle gets updated
  // startAngle = degAngle; //update startAngle with degAngle - for taring
  

  //------------------------------------------------------------------------------
  motor1.setSpeed(900);
  motor1.setAcceleration(100);
  
}

void loop()
{    
    // ReadRawAngle(); //ask the value from the sensor
    // correctAngle(); //tare the value
    // checkQuadrant(); //check quadrant, check rotations, check absolute angular position
    // refreshDisplay();

    // ReadValue();
    // SetDegree();
    //Serial.println(degree);

    //delay(100); //wait a little - adjust it for "better resolution"
    // Serial.print("Total angle: ");
    // Serial.println(totalAngle, 2); //absolute position of the motor expressed in degree angles, 2 digits

    motor1.moveTo(degree);
    motor1.run();
   

    recvWithEndMarker();
    showNewNumber();
}

// void ReadValue(){
//   if (Serial.available() > 0){
//     input = Serial.read();
//     newData = true;
//   }
// }

// void SetDegree(){
//   if(newData){
//     if(input == 'f'){
//       degree = 200;
//     }
//     else if(input == 'b'){
//       degree = -200;
//     }
//   }
//   newData = false;
// }

void recvWithEndMarker() {
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
    
    if (Serial.available() > 0) {
        rc = Serial.read();

        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
        }
    }
}

void showNewNumber() {
    if (newData == true) {
        dataNumber = 0;             // new for this version
        dataNumber = atoi(receivedChars);   // new for this version
        degree = dataNumber/1.8;
        newData = false;
    }
}