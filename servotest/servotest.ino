#include "Servo.h"
Servo myservo;

void setup() {
  // put your setup code here, to run once:
   myservo.attach(A0);


//  myservo.write(90);
//  myservo.write(180);
//  gripperServo.write(0);
//  gripperServo.write(700); 
//  gripperServo.write(800);

    myservo.write(45); //grab
    delay(3000);
//    myservo.write(90);
//    delay(1000);
//
//    myservo.write(135);
//    delay(1000);
//    myservo.write(90);
//    delay(1000);
//    

 
}

void loop() {
  // put your main code here, to run repeatedly:

  //servo values greater than 90 seem to rotate the servo one way, and
  //values around 0 rotate the servo the other way

//  myservo.write(135);
//  delay(1000);
//  myservo.write(90);
//  delay(3000);
//  myservo.write(135);
//  delay(1000);
//  myservo.write(90);
//  delay(3000);

//    myservo.write(45); //grab
//    delay(3000);
//
//    myservo.write(135);
//    delay(1000);
//      myservo.write(90);
//    delay(1000);
//    

//    myservo.write(70);
//    myservo.write(45); //grab
//    delay(3000);
//    myservo.write(90);
//    delay(3000);
//    myservo.write(110);
//    delay(3000);


}
