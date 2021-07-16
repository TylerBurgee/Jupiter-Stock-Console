#include <Keypad.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <OneButton.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// BOOLEANS FOR LETTER SHIFTING
bool firstRowLetters = true;
bool secondRowLetters = false;
bool thirdRowLetters = false;

// SETUP KEYPAD 
const byte ROWS = 4; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'S'}
};

byte rowPins[ROWS] = {6, 7, 8 , 9}; 
byte colPins[COLS] = {5, 4, 3, 2}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

// BYTE DATA FROM SERIAL
int data = 0;

// STRING DATA FROM SERIAL
String stockDataArr[6];
// CHARACTERS TO BE ADDED TO stockDataArr
String charsToAdd = "";
// COUNTER VARIABLE FOR stockDataArr
int i = 0;


// LCD CURSOR POSITION
int cursor_pos = 0;

// STORES DATA DECODED FROM BYTES TO STRING
String strData;

// CHECKS IF MESSAGE HAS BEEN RECEIVED FROM SERIAL
bool msgReceived;
bool new_search;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ticker:");
  lcd.setCursor(7, 0);
}

// CHECK WHICH LETTER-SHIFT LEVEL IS CURRENTLY ACTIVATED
void checkLetterRow(char firstLetter, char secondLetter, char thirdLetter) {
  if (firstRowLetters) {
      handleButtonEvent(firstLetter);
    }
    else if (secondRowLetters) {
      handleButtonEvent(secondLetter);
    }
    else if (thirdRowLetters) {
      handleButtonEvent(thirdLetter);
    }
}

void loop() {
  // VARIABLE THAT STORES WHICH BUTTON HAS BEEN PRESSED
  char customKey = customKeypad.getKey();

  // ASSIGN FUNCTIONS TO EACH BUTTON ON THE KEYPAD
  if (customKey == '1') {
    checkLetterRow('A', 'B', 'C');
  }
  else if (customKey == '2') {
    checkLetterRow('D', 'E', 'F');
  }
  else if (customKey == '3') {
    checkLetterRow('G', 'H', 'I');
  }
  else if (customKey == '4') {
    checkLetterRow('J', 'K', 'L');
  }
  else if (customKey == '5') {
    checkLetterRow('M', 'N', 'O');
  }
  else if (customKey == '6') {
    checkLetterRow('P', 'Q', 'R');
  }
  else if (customKey == '7') {
    checkLetterRow('S', 'T', 'U');
  }
  else if (customKey == '8') {
    checkLetterRow('V', 'W', 'X');
  }
  else if (customKey == '9') {
    checkLetterRow('Y', 'Z', '.');
  }
  else if (customKey == '0') {
    
  }
  else if (customKey == '*') {
    Serial.println("clear");
    delay(500);
    Serial.flush();
    cursor_pos = 0;
    setup();
  } else if (customKey == '#') {
    
  }
  else if (customKey == 'A') {
    firstRowLetters = true;
    secondRowLetters = false;
    thirdRowLetters = false;
  }
  else if (customKey == 'B') {
    firstRowLetters = false;
    secondRowLetters = true;
    thirdRowLetters = false;
  }
  else if (customKey == 'C') {
    firstRowLetters = false;
    secondRowLetters = false;
    thirdRowLetters = true;
  }
  else if (customKey == 'S') {
    Serial.println("search");
    lcd.setCursor(0, 1);
    lcd.print("searching...");
    Serial.flush();
    delay(100);
    readSerial();
  }
}

// DECODE BYTE DATA TO STRING DATA
void decodeData(int data) {
  
  if (data == 48) {
    strData = "0";
  }
  else if (data == 49) {
    strData = "1";
  }
  else if (data == 50) {
    strData = "2";
  }
  else if (data == 51) {
    strData = "3";
  }
  else if (data == 52) {
    strData = "4";
  }
  else if (data == 53) {
    strData = "5";
  }
  else if (data == 54) {
    strData = "6";
  }
  else if (data == 55) {
    strData = "7";
  }
  else if (data == 56) {
    strData = "8";
  }
  else if (data == 57) {
    strData = "9";
  }
  else if (data == 46) {
    strData = ".";
  }
  // IF (DATA == 95 a.k.a "_"), ASSIGN CharsToAdd to stockDataArr, 
  //then CLEAR temp. variables and INCREMENT counter variable for stockDataArr.
  else if (data == 95) {
    stockDataArr[i] = (charsToAdd);
    i++;
    strData = "";
    charsToAdd = "";
  }
  else if (data == 43) {
    strData = "+";
  }
  else if (data == 45) {
    strData = "-";
  } 
  else if (data == 44) {
    strData = ",";
  }
  else if (data == 32) {
    strData = "(";
  }
  else if (data == 94) {
    msgReceived = true;
  }
  else if (data == 97) {
    strData = "A";
  }
  else if (data == 66) {
    strData = "B";
  }
  else if (data == 101) {
    strData = "E";
  }
  else if (data == 104) {
    strData = "H";
  }
  else if (data == 105) {
    strData = "I";
  }
  else if (data == 108) {
    strData = "L";
  }
  else if (data == 114) {
    strData = "R";
  }
  else if (data == 115) {
    strData = "S";
  }
  else if (data == 117) {
    strData = "U";
  }

  // ADD strData TO charsToAdd
  charsToAdd = charsToAdd + strData;
}

// INFINITELY LOOP OVER 6 DIFFERENT MESSAGES ON LCD
void displayLoop() {
  new_search = false;
  
  while (!new_search) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("MV:$" + stockDataArr[0]);
    lcd.setCursor(0, 1);
    lcd.print("OPP:$" + stockDataArr[1]);
    delay(5000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Chg:$" + stockDataArr[2]);
    lcd.setCursor(0, 1);
    lcd.print("Vol:" + stockDataArr[3]);
    delay(5000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PT:$" + stockDataArr[4]);
    lcd.setCursor(0, 1);
    lcd.print("RCP:" + stockDataArr[5]);
    delay(5000);
  }
}

// READ DATA FROM SERIAL
void readSerial() {
  msgReceived = false;

  while (!msgReceived) {
    if (Serial.available() > 0) {
      data = Serial.read();
      decodeData(data);
    }
  }
  displayLoop();
}

// HANDLE BUTTON EVENTS
void handleButtonEvent(char button) {
  Serial.println(button);
  lcd.print(button);
  delay(500);
  Serial.flush();
  cursor_pos += 1;
  lcd.setCursor(7 + cursor_pos, 0);
}
