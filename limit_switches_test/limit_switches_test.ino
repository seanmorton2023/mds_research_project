#include <AccelStepper.h>
#include <MultiStepper.h>

#include <Servo.h>
#include <math.h>

#define limitSwitch1 11
#define limitSwitch2 10
#define limitSwitch3 9
#define limitSwitch4 A3

const int StepJ1 = 2;
const int DirJ1 = 5;
const int StepJ2 = 3;
const int DirJ2 = 6;
const int StepJ3 = 4;
const int DirJ3 = 7;

//extra stuff for Z and gripper
const int StepZ = 12;
const int DirZ = 13;
const int Gripper = A0;

int limitVal1 = 0;
int limitVal2 = 0;
int limitVal3 = 0;
int limitVal4 = 0;

Servo gripperServo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  ///*

  pinMode(limitSwitch1, INPUT_PULLUP);
  pinMode(limitSwitch2, INPUT_PULLUP);
  pinMode(limitSwitch3, INPUT_PULLUP);
  pinMode(limitSwitch4, INPUT_PULLUP);

  //main three stepper motors
  pinMode(StepJ1,OUTPUT);
  pinMode(DirJ1,OUTPUT);
  pinMode(StepJ2,OUTPUT);
  pinMode(DirJ2,OUTPUT);
  pinMode(StepJ3,OUTPUT);
  pinMode( DirJ3,OUTPUT);

  //Z axis
  pinMode(StepZ, OUTPUT);
  pinMode(DirZ, OUTPUT);


  //we want this code to only run once, so put it in setup
  //not in loop

  //LIMIT SWITCH 2

  /*
  digitalWrite(DirJ2, HIGH);
  while (digitalRead(limitSwitch2) != 1) {

    digitalWrite(StepJ2, HIGH);
    delayMicroseconds(1100);
    digitalWrite(StepJ2,LOW); 
    delayMicroseconds(1100);
  }
  */
  //gripperServo.attach(A0, 771, 2740);
  gripperServo.attach(A0);

  //findings: when I used default settings for servo,
  //or (600, 2200), or (771, 2740), literally any value
  //of the servo would 
  


  /*
  digitalWrite(DirJ1, HIGH);
  while(digitalRead(limitSwitch1) != 1) {
    digitalWrite(StepJ1,HIGH);
    delayMicroseconds(700);
    digitalWrite(StepJ1,LOW); 
    delayMicroseconds(700);
   }
  delay(1000); // delay for 1 second
  *
  *
   */
   
  //gripperServo.write(0);
//  gripperServo.write(700);
//  delay(5000);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  
  //first let's see if the limit switches are giving the right data

  //findings from limit switch testing:
  // - limit switch 2 works
  // - limit switch 4 starts at 0 and doesn't change state
  // - limit switches 1, 3 start at 1 and don't change state
  // limit switch 1 is also physically disconnected - need to re-solder


  
  limitVal1 = digitalRead(limitSwitch1);
  limitVal2 = digitalRead(limitSwitch2);
  limitVal3 = digitalRead(limitSwitch3);
  limitVal4 = digitalRead(limitSwitch4);


//  Serial.write("\nLimit switches 1,2,3,4:\n");
 // Serial.println(limitVal1);
//  Serial.println(limitVal2);
  Serial.println(limitVal3);
//  Serial.println(limitVal4);
  
  //Serial.write("test");
  delay(100);
  

}
