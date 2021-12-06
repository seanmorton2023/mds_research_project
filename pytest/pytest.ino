// Include the library:
#include <LiquidCrystal.h>
// Create an LCD object. Parameters: (RS, E, D4, D5, D6, D7):
LiquidCrystal lcd = LiquidCrystal(2, 3, 4, 5, 6, 7);
String x;
String input;
String obj;
float x_coord;
float y_coord;
int count = 0;
void setup() {
  
  //Startup serial to connect with python
  Serial.begin(115200);
  Serial.setTimeout(1);
  // Specify the LCD's number of columns and rows. Change to (20, 4) for a 20x4 LCD:
  lcd.begin(16, 2);
  
}
void loop() {
  if(Serial.available() > 0){
  lcd.clear();
  x = Serial.readString();
  Serial.print(x);
  int c = x.length();
  for (int i = 0; i < c; i++)
    {
      if(x[i] != ','){
        input += x[i]; 
      }
      else{
        if(count == 0){
          obj = input;
          input = "";
          count++;
        }
        else if(count == 1){
          x_coord = input.toFloat();
          input = "";
          count++;
        }
        else{
          y_coord = input.toFloat();
          input = "";  
        }
      }
      }
  
    }

  lcd.clear();

  
  // Set the cursor on the third column and the first row, counting starts at 0:
  lcd.setCursor(2, 0);
  // Print the string 'Hello World!':
  lcd.print(obj);
  // Set the cursor on the third column and the second row:
  lcd.setCursor(2, 1);
  // Print the string 'LCD tutorial':
  lcd.print(String(x_coord)+", "+String(y_coord));
  

}
