#include "Servo.h"

void setup() {
  // put your setup code here, to run once:

  Servo myservo;

  myservo.attach(A0);


//  myservo.write(90);
//  delay(5000);

//  myservo.write(180);
//  delay(5000);

  myservo.write(145);
  delay(5000);
  
//  myservo.write(180);
//  delay(5000);
  
//  myservo.write(180);
//  delay(2000);
//  myservo.write(90);
//  delay(2000);
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
