#include <LiquidCrystal.h>
#include <Keypad.h>

const byte ROWS = 4; // four rows
const byte COLS = 3; // three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {13, 9, 10, 12}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {19, 11, 17}; // connect to the column pinouts of the keypad
int Arduino = 3; // digital pin 11 is connected to Arduino
int product[10]; // products that are selected and then transered to Arduino for optimal path calculation
int count = 0; // cound the number of items entered
int done = 0; // set to 1 if finish entering

// initialize the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(5, 6, 4, 18, 2, 16);

void setup(){
  Serial.begin(9600);
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print the ready signal to the LCD.
  lcd.print("Please enter:");
}

void loop(){
  char key = keypad.getKey();
  // set the cursor to column 0, line 1 (second row)
  lcd.setCursor(0, 1);
  
  // if all possible items are selected
  if(count > 10){
    lcd.clear(); 
    lcd.print("Select all");
    if(done == 0)
      done = 1; // the customer didn't press '#', but maximum number of items is reached
  }

  // if any key is pressed and not all possible items are selected
  // if no key is pressed, nothing happens
  if(key != NO_KEY && count <= 9 && done == 0){
    lcd.clear();
    
    if(key == '0'){
      lcd.print("Grocery");
      product[count] = 0;
    }
    if(key == '1'){
      lcd.print("Kids");
      product[count] = 1;
    }
    if(key == '2'){
      lcd.print("Books");
      product[count] = 2;
    }
    if(key == '3'){
      lcd.print("Office");
      product[count] = 3;
    }
    if(key == '4'){
      lcd.print("Health");
      product[count] = 4;
    }
    if(key == '5'){
      lcd.print("Beauty");
      product[count] = 5;
    }
    if(key == '6'){
      lcd.print("Sports");
      product[count] = 6;
    }
    if(key == '7'){
      lcd.print("Electronics");
      product[count] = 7;
    }
    if(key == '8'){
      lcd.print("Apparel");
      product[count] = 8;
    }
    if(key == '9'){
      lcd.print("Home");
      product[count] = 9;
    }
    // finish entering
    if(key == '#'){
      lcd.print("Done");
      done = 1;
      count = count - 1; // no product entered
    }
    // clear the last item
    if(key == '*'){
      lcd.print("Clear all");
      done = 0;
      count = -1; // it will be added by 1 and get 0
    }
    count = count + 1; // increase the number of items entered
  }
  
  if(done == 1){
    while(count > 0){
      analogWrite(Arduino, (product[count] + 1) * 23);
      count --;
      delay(100);
    }
    done = 0; // finish transfering the list
  }
  
  delay(100);
}

