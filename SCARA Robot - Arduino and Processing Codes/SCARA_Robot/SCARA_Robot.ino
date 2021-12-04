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
const float zDistanceToSteps = 27;

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
  stepper4.setMaxSpeed(800); //edited this bc the Z was rising too fast
  stepper4.setAcceleration(500);

  gripperServo.attach(A0, 600, 2500);
  // initial servo value - open gripper
  
//  data[6] = 180;
//  gripperServo.write(data[6]);
//  delay(1000);
//  data[5] = 100;
  
  homing();  
}

void loop() {

  //Serial.println(digitalRead(limitSwitch3));


  if (Serial.available()) {
    content = Serial.readString(); // Read the incoming data from Processing
    // Extract the data from the string and put into separate integer variables (data[] array)
    for (int i = 0; i < 10; i++) {
      int index = content.indexOf(","); // locate the first ","
      data[i] = atol(content.substring(0, index).c_str()); //Extract the number from start to the ","
      content = content.substring(index + 1); //Remove the number from the string
    }
    /*
     data[0] - SAVE button status
     data[1] - RUN button status
     data[2] - Joint 1 angle
     data[3] - Joint 2 angle
     data[4] - Joint 3 angle
     data[5] - Z position
     data[6] - Gripper value
     data[7] - Speed value
     data[8] - Acceleration value
    */
    // If SAVE button is pressed, store the data into the appropriate arrays
    if (data[0] == 1) {
      theta1Array[positionsCounter] = data[2] * theta1AngleToSteps; //store the values in steps = angles * angleToSteps variable
      theta2Array[positionsCounter] = data[3] * theta2AngleToSteps;
      phiArray[positionsCounter] = data[4] * phiAngleToSteps;
      zArray[positionsCounter] = data[5] * zDistanceToSteps;
      //gripperArray[positionsCounter] = data[6];
      positionsCounter++;
    }
    // clear data
    if (data[0] == 2) {
      // Clear the array data to 0
      memset(theta1Array, 0, sizeof(theta1Array));
      memset(theta2Array, 0, sizeof(theta2Array));
      memset(phiArray, 0, sizeof(phiArray));
      memset(zArray, 0, sizeof(zArray));
      memset(gripperArray, 0, sizeof(gripperArray));
      positionsCounter = 0;
    }
  }
  // If RUN button is pressed
  while (data[1] == 1) {
    stepper1.setSpeed(data[7]);
    stepper2.setSpeed(data[7]);
    stepper3.setSpeed(data[7]);
    stepper4.setSpeed(data[7]);
    stepper1.setAcceleration(data[8]);
    stepper2.setAcceleration(data[8]);
    stepper3.setAcceleration(data[8]);
    stepper4.setAcceleration(data[8]);

    // execute the stored steps
    for (int i = 0; i <= positionsCounter - 1; i++) {
      if (data[1] == 0) {
        break;
      }
      stepper1.moveTo(theta1Array[i]);
      stepper2.moveTo(theta2Array[i]);
      stepper3.moveTo(phiArray[i]);
      stepper4.moveTo(zArray[i]);
      while (stepper1.currentPosition() != theta1Array[i] 
      || stepper2.currentPosition() != theta2Array[i] 
      || stepper3.currentPosition() != phiArray[i] 
      || stepper4.currentPosition() != zArray[i]) {
        stepper1.run();
        stepper2.run();
        stepper3.run();
        stepper4.run();
      }
      if (i == 0) {
        //gripperServo.write(gripperArray[i]);
      }
      else if (gripperArray[i] != gripperArray[i - 1]) {
        //gripperServo.write(gripperArray[i]);
        //delay(800); // wait 0.8s for the servo to grab or drop - the servo is slow
      }

      //check for change in speed and acceleration or program stop
      if (Serial.available()) {
        content = Serial.readString(); // Read the incomding data from Processing
        // Extract the data from the string and put into separate integer variables (data[] array)
        for (int i = 0; i < 10; i++) {
          int index = content.indexOf(","); // locate the first ","
          data[i] = atol(content.substring(0, index).c_str()); //Extract the number from start to the ","
          content = content.substring(index + 1); //Remove the number from the string
        }

        if (data[1] == 0) {
          break;
        }
        // change speed and acceleration while running the program
        stepper1.setSpeed(data[7]);
        stepper2.setSpeed(data[7]);
        stepper3.setSpeed(data[7]);
        stepper4.setSpeed(data[7]);
        stepper1.setAcceleration(data[8]);
        stepper2.setAcceleration(data[8]);
        stepper3.setAcceleration(data[8]);
        stepper4.setAcceleration(data[8]);
      }
    }
    /*
      // execute the stored steps in reverse
      for (int i = positionsCounter - 2; i >= 0; i--) {
      if (data[1] == 0) {
        break;
      }
      stepper1.moveTo(theta1Array[i]);
      stepper2.moveTo(theta2Array[i]);
      stepper3.moveTo(phiArray[i]);
      stepper4.moveTo(zArray[i]);
      while (stepper1.currentPosition() != theta1Array[i] || stepper2.currentPosition() != theta2Array[i] || stepper3.currentPosition() != phiArray[i] || stepper4.currentPosition() != zArray[i]) {
        stepper1.run();
        stepper2.run();
        stepper3.run();
        stepper4.run();
      }
      //gripperServo.write(gripperArray[i]);

      if (Serial.available()) {
        content = Serial.readString(); // Read the incomding data from Processing
        // Extract the data from the string and put into separate integer variables (data[] array)
        for (int i = 0; i < 10; i++) {
          int index = content.indexOf(","); // locate the first ","
          data[i] = atol(content.substring(0, index).c_str()); //Extract the number from start to the ","
          content = content.substring(index + 1); //Remove the number from the string
        }
        if (data[1] == 0) {
          break;
        }
      }
      }
    */
  }

  stepper1Position = data[2] * theta1AngleToSteps;
  stepper2Position = data[3] * theta2AngleToSteps;
  stepper3Position = data[4] * phiAngleToSteps;
  stepper4Position = data[5] * zDistanceToSteps;

  stepper1.setSpeed(data[7]);
  stepper2.setSpeed(data[7]);
  stepper3.setSpeed(data[7]);
  stepper4.setSpeed(data[7]);

  stepper1.setAcceleration(data[8]);
  stepper2.setAcceleration(data[8]);
  stepper3.setAcceleration(data[8]);
  stepper4.setAcceleration(data[8]);

  stepper1.moveTo(stepper1Position);
  stepper2.moveTo(stepper2Position);
  stepper3.moveTo(stepper3Position);
  stepper4.moveTo(stepper4Position);

  while (stepper1.currentPosition() != stepper1Position 
  || stepper2.currentPosition() != stepper2Position 
  || stepper3.currentPosition() != stepper3Position 
  || stepper4.currentPosition() != stepper4Position) {
    stepper1.run();
    stepper2.run();
    stepper3.run();
    stepper4.run();
  }
  delay(100);
  //gripperServo.write(data[6]);
  delay(300);
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
//  int stepper1pos = 0;
  int stepper1pos = -(1905-920); //to align w/ y axis
  
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
