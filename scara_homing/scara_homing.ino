/*
   Arduino based SCARA Robot 
   by Dejan, www.HowToMechatronics.com
   AccelStepper: http://www.airspayce.com/mikem/arduino/AccelStepper/index.html

*/
#include <AccelStepper.h>
#include <Servo.h>
#include <math.h>

#define limitSwitch1 11
#define limitSwitch2 10
#define limitSwitch3 9
#define limitSwitch4 A3

// Define the stepper motors and the pins the will use
AccelStepper stepper1(1, 2, 5); // (Type:driver, STEP, DIR)
AccelStepper stepper2(1, 3, 6);
AccelStepper stepper3(1, 4, 7);
AccelStepper stepper4(1, 12, 13); 

Servo gripperServo;  // create servo object to control a servo


//double x = 10.0;
//double y = 10.0;
//double L1 = 228; // L1 = 228mm
//double L2 = 136.5; // L2 = 136.5mm

double x = 0.0;
double y = 13.0;
double L1 = 228; //inches
double L2 = 136.5; //inches

double theta1, theta2, phi, z;

int stepper1Position, stepper2Position, stepper3Position, stepper4Position;

//const float theta1AngleToSteps = 44.444444;
//const float theta2AngleToSteps = 35.555555;
//const float phiAngleToSteps = 10;
//const float zDistanceToSteps = 100;

//const float theta1AngleToSteps = 9.4882;
const float theta1AngleToSteps = -11.2;
const float theta2AngleToSteps = -9.1722;
const float phiAngleToSteps = 2.42;
const float zDistanceToSteps = 540;

byte inputValue[5];
int k = 0;

String content = "";
int data[10];

int theta1Array[100];
int theta2Array[100];
int phiArray[100];
int zArray[100];
int gripperArray[100];
int positionsCounter = 0;

void setup() {
  //Serial.begin(115200);
  Serial.begin(9600);

  ///*

  pinMode(limitSwitch1, INPUT_PULLUP);
  pinMode(limitSwitch2, INPUT_PULLUP);
  pinMode(limitSwitch3, INPUT_PULLUP);
  pinMode(limitSwitch4, INPUT_PULLUP);

  // Stepper motors max speed
  stepper1.setMaxSpeed(2000); //tried changing this from 4000 and 2000 but didn't do anything
  stepper1.setAcceleration(500);
  stepper2.setMaxSpeed(4000);
  stepper2.setAcceleration(2000);
  stepper3.setMaxSpeed(4000);
  stepper3.setAcceleration(2000);
  stepper4.setMaxSpeed(500); //edited this bc the Z was rising too fast
  stepper4.setAcceleration(300);

  gripperServo.attach(A0, 600, 2500);
  // initial servo value - open gripper
  
//  data[6] = 180;
//  gripperServo.write(data[6]);
//  delay(1000);
//  data[5] = 100;
  
  homing();  
}

void loop() {
}

void serialFlush() {
  while (Serial.available() > 0) {  //while there are characters in the serial buffer, because Serial.available is >0
    Serial.read();         // get one character
  }
}

void homing() {
  
  // Homing Stepper4
  stepper4.setSpeed(450);

  while (digitalRead(limitSwitch4) != 1) {
    stepper4.runSpeed();
  }
  stepper4.setCurrentPosition(2400);
  delay(1000);
  
  stepper4.moveTo(0);
  while (stepper4.currentPosition() != 0){
    stepper4.run();
  }

  delay(1000);

  //--------------------------//
  
  // Homing Stepper3
  stepper3.setSpeed(-200);
  while (digitalRead(limitSwitch3) != 1) {
    stepper3.runSpeed();
  }
  stepper3.setCurrentPosition(-300);
  delay(1000);

  stepper3.moveTo(0);
  while (stepper3.currentPosition() != 0) {
    stepper3.run();
  }

  delay(1000);

  //------------------------//

  // Homing Steppers 1 and 2: arm has to be folded up to home Stepper 1 
  stepper2.setSpeed(700);
  while (digitalRead(limitSwitch2) != 1) {
    stepper2.runSpeed();
  }
  stepper2.setCurrentPosition(1420); 
  delay(1000);
  
  stepper1.setSpeed(600);
  while (digitalRead(limitSwitch1) != 1) {
    stepper1.runSpeed();
  }
//  stepper1.setCurrentPosition(1905); //to make robot aligned with y axis
  stepper1.setCurrentPosition(920);

  delay(1000);

  //--------------------------------------//
  
  //Zeroing steppers 1 and 2
  int stepper1pos = 0;
  //int stepper1pos = -(1905-920); //to align w/ y axis
  
  stepper1.moveTo(stepper1pos);
  while (stepper1.currentPosition() != stepper1pos) {
    stepper1.run();
  }
  delay(1000);

  stepper2.moveTo(0);
  while (stepper2.currentPosition() != 0) {
    stepper2.run();
  }
  delay(1000);

  
}
