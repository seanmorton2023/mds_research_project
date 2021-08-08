
// CNC Shield Stepper  Control Demo
// Superb Tech
// www.youtube.com/superbtech

const int StepJ1 = 2;
const int DirJ1 = 5;
const int StepJ2 = 3;
const int DirJ2 = 6;
const int StepJ3 = 4;
const int DirJ3 = 7;

//two grippers remaining

//code says that the gripper is controlled at A0, 
//stepper 4 is at stepj4 = 12, dirj4 = 13

const int StepZ = 12;
const int DirZ = 13;
const int Gripper = A0;


void setup() {
  pinMode(StepJ1,OUTPUT);
  pinMode(DirJ1,OUTPUT);
  pinMode(StepJ2,OUTPUT);
  pinMode(DirJ2,OUTPUT);
  pinMode(StepJ3,OUTPUT);
  pinMode( DirJ3,OUTPUT);

  //extra grippers
  pinMode(StepZ, OUTPUT);
  pinMode(DirZ, OUTPUT);

}

void loop() {
 digitalWrite(DirJ1, HIGH); // set direction, HIGH for clockwise, LOW for anticlockwise
 digitalWrite(DirJ2, HIGH);
 digitalWrite(DirJ3, HIGH);


  ///////////////////////////////////////////////////////////////////////////
  
  //J1 TESTING

  /*
 
 for(int x = 0; x < 400; x++) { // loop for 200 steps
  digitalWrite(StepJ1,HIGH);
  delayMicroseconds(700);
  digitalWrite(StepJ1,LOW); 
  delayMicroseconds(700);
 }
delay(1000); // delay for 1 second

  //now make it turn counterclockwise
  digitalWrite(DirJ1, LOW);
  for (int ii = 0; ii < 400; ++ii) {
    digitalWrite(StepJ1, HIGH);
    delayMicroseconds(700);
    digitalWrite(StepJ1, LOW);
    delayMicroseconds(700);
  }
delay(1000); //delay 1 second

*/

///////////////////////////////////////////////////////////////////

//J2 TESTING

/*

//set direction to clockwise first
digitalWrite(DirJ2, HIGH);

for (int ii = 0; ii < 200; ++ii) {
  digitalWrite(StepJ2, HIGH);
  delayMicroseconds(700);
  digitalWrite(StepJ2, LOW);
  delayMicroseconds(700);
}
delay(1000);

//now go CCW
digitalWrite(DirJ2, LOW);

for (int ii = 0; ii < 200; ++ii) {
  digitalWrite(StepJ2, HIGH);
  delayMicroseconds(700);
  digitalWrite(StepJ2, LOW);
  delayMicroseconds(700);
}
delay(1000);

*/

////////////////////////////////////////////////////////

//J3 Testing

/*

//set direction to clockwise first
digitalWrite(DirJ3, HIGH);

for (int ii = 0; ii < 200; ++ii) {
  digitalWrite(StepJ3, HIGH);
  delayMicroseconds(700);
  digitalWrite(StepJ3, LOW);
  delayMicroseconds(700);
}
delay(1000);

//now go CCW
digitalWrite(DirJ3, LOW);

for (int ii = 0; ii < 200; ++ii) {
  digitalWrite(StepJ3, HIGH);
  delayMicroseconds(700);
  digitalWrite(StepJ3, LOW);
  delayMicroseconds(700);
}
delay(1000);

*/

//////////////////////////////////////////////////////////

//Z TESTING

//set direction to clockwise
digitalWrite(DirZ, HIGH);

for (int ii = 0; ii < 50; ++ii) {
  digitalWrite(StepZ, HIGH);
  delayMicroseconds(700);
  digitalWrite(StepZ, LOW);
  delayMicroseconds(700);
}
delay(1000);

//now go CCW
digitalWrite(DirZ, LOW);

for (int ii = 0; ii < 50; ++ii) {
  digitalWrite(StepZ, HIGH);
  delayMicroseconds(700);
  digitalWrite(StepZ, LOW);
  delayMicroseconds(700);
}
delay(1000);




//GRIPPER TESTING




//for(int x = 0; x<200; x++) { // loop for 200 steps
//  digitalWrite(StepJ2,HIGH);
//  delayMicroseconds(500);
//  digitalWrite(StepJ2,LOW); 
//  delayMicroseconds(500);
// }
//delay(1000); // delay for 1 second
//
//for(int x = 0; x<200; x++) { // loop for 200 steps
//  digitalWrite(StepJ3,HIGH);
//  delayMicroseconds(500);
//  digitalWrite(StepJ3,LOW); 
//  delayMicroseconds(500);
// }
//delay(1000); // delay for 1 second

}
