const int LED = 8;
const int servo = 8;




void setup() {
  // put your setup code here, to run once:

  pinMode(LED, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(9, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:


//  //just some test code for the board
//  digitalWrite(LED, HIGH);
//  delay(100);
//  digitalWrite(LED, LOW);
//  delay(100);
//  digitalWrite(LED, HIGH);
//  delay(100);
//  digitalWrite(LED, LOW);
//  delay(100);
//  digitalWrite(LED, HIGH);
//  delay(400);
//  digitalWrite(LED, LOW);
//  delay(100);
//  

  //gonna try something to see if I can control the servo like this
//  digitalWrite(LED, HIGH);
//  delay(1000);
//  digitalWrite(LED, LOW);
//  delay(2000);

//  analogWrite(servo, 255);
//  delay(1000);
//  analogWrite(servo, 0);
//  delay(2000);

  //let's try some other ports
  analogWrite(1, 255);
  delay(2000);
  analogWrite(1, 0);
  analogWrite(2, 255);
  delay(2000);
  
  analogWrite(2, 0);
  analogWrite(3, 255);
  delay(2000);
  
  analogWrite(3, 0);
  analogWrite(4, 255);
  delay(2000);
  
  analogWrite(4, 0);
  analogWrite(5, 255);
  delay(2000);
  
  analogWrite(5, 0);
  analogWrite(6, 255);
  delay(2000);
  analogWrite(6,0);
}
