/* @file CustomKeypad.pde
  || @version 1.0
  || @author Alexander Brevig
  || @contact alexanderbrevig@gmail.com
  ||
  || @description
  || | Demonstrates changing the keypad size and key values.
  || #
*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define BACKLIGHT_PIN  13



LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // Set the LCD I2C address



#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {' ', '0', '#', 'E'}
};
byte rowPins[ROWS] = {2, 3, 4, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 7, 8, 9}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins , colPins, ROWS, COLS);

int data_count = 0;
char Data[32] = "";
int AUX = 11;
int isMsgDisplay = 0;
int isTypingMsg = 0;
long previous = 0;
unsigned long currentMillis = 0;
void setup() {

  pinMode(AUX, INPUT);
  lcd.begin(16, 2);
  lcd.backlight();
  Serial.begin(9600);
  Serial2.begin(57600);
  Serial3.begin(9600);

  currentMillis = millis();
}


bool pqr = true;

void loop() {
  // int AUXState = digitalRead(AUX);

  if (Serial2.available()) {
    char x = Serial2.read();
    delay(2000);
    Serial.print("t");
    Serial.print(x);
    lcd.setCursor(0, 0);
    lcd.print(x);
    // Serial.print("/n");


  }

  int  n = 0;

  while (Serial2.available())
  {
    if (isTypingMsg == 1) {
      isTypingMsg = 0;
      clearDisplayRemainPart(0);
    }
    char x = Serial2.read();
    //Serial.print(x);
    //if massage  reserved should display only the massage  is reserved
    if (x == '@') {
      clearDisplayRemainPart(n);
      isMsgDisplay = 1;
    }
    //delay(2);
    //Serial.print("t");
    if (isAlphaNumeric(x) || (x == ' ')) {
      Serial.print(x);
      if (n <= 15) {
        lcd.setCursor(n, 0);
      }
      else {

        lcd.setCursor(n - 16, 1);
      }
      // lcd.setCursor(n, 0);
      n++;
      lcd.print(x);
    }
  }
  char customKey = customKeypad.getKey();
  if (customKey) {
    //delete char at a time
    if (customKey == 'B') {
      Serial.println("delete");
      Serial.print("A datacount=");
      Serial.println(data_count);
      if (data_count > 0) {
        data_count = data_count - 1;
      }
      if (data_count < 16) {
        lcd.setCursor(data_count, 0);
        lcd.print(" ");
        Data[data_count] = '\0';

      }
      else {
        lcd.setCursor(data_count - 16, 1);
        lcd.print(" ");
        Data[data_count] = '\0';
      }
    }
    else if (customKey == 'A') {
      send();
      data_count = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Massage Send...");
      delay(1000);
      lcd.clear();
    }
    else {
      if (isMsgDisplay == 1) {
        isMsgDisplay = 0;
        isTypingMsg = 1;
        clearDisplayRemainPart(0);
        data_count = 0;
      }

      if (customKey == '1') {
        bool s = true;
        int count = 0;
        char tes[4] = {'1', 'a', 'b', 'c'};

        while (s) {
          char tempkey = customKeypad.getKey();
          if (tempkey == '1') {
            count = (count + 1) % 4;
          }
          Serial.println(tes[count]);
          printLcdDisplay(tes[count], data_count);
          //customKey = tes[sa];
          currentMillis = millis();
          if (currentMillis - previous > 3000) {
            s = false;
            previous = currentMillis;
          }
          currentMillis = millis();
        }
        data_count++;
      } else if (customKey == '2') {
        bool s = true;
        int count = 0;
        char tes[4] = {'2', 'd', 'e', 'f'};

        while (s) {
          char tempkey = customKeypad.getKey();
          if (tempkey == '2') {
            count = (count + 1) % 4;
          }
          Serial.println(tes[count]);
          printLcdDisplay(tes[count], data_count);
          //customKey = tes[sa];
          currentMillis = millis();
          if (currentMillis - previous > 3000) {
            s = false;
            previous = currentMillis;
          }
          currentMillis = millis();
        }
        data_count++;

      } else if (customKey == '3') {
        bool s = true;
        int count = 0;
        char tes[4] = {'3', 'g', 'h', 'i'};

        while (s) {
          char tempkey = customKeypad.getKey();
          if (tempkey == '3') {
            count = (count + 1) % 4;
          }
          Serial.println(tes[count]);
          printLcdDisplay(tes[count], data_count);
          //customKey = tes[sa];
          currentMillis = millis();
          if (currentMillis - previous > 3000) {
            s = false;
            previous = currentMillis;
          }
          currentMillis = millis();
        }
        data_count++;

      }
      else if (customKey == '4') {
        bool s = true;
        int count = 0;
        char tes[4] = {'4', 'j', 'k', 'l'};

        while (s) {
          char tempkey = customKeypad.getKey();
          if (tempkey == '4') {
            count = (count + 1) % 4;
          }
          Serial.println(tes[count]);
          printLcdDisplay(tes[count], data_count);
          //customKey = tes[sa];
          currentMillis = millis();
          if (currentMillis - previous > 3000) {
            s = false;
            previous = currentMillis;
          }
          currentMillis = millis();
        }
        data_count++;

      }
      else if (customKey == '5') {
        bool s = true;
        int count = 0;
        char tes[4] = {'5', 'm', 'n', 'o'};

        while (s) {
          char tempkey = customKeypad.getKey();
          if (tempkey == '5') {
            count = (count + 1) % 4;
          }
          Serial.println(tes[count]);
          printLcdDisplay(tes[count], data_count);
          //customKey = tes[sa];
          currentMillis = millis();
          if (currentMillis - previous > 3000) {
            s = false;
            previous = currentMillis;
          }
          currentMillis = millis();
        }
        data_count++;

      } else if (customKey == '6') {
        bool s = true;
        int count = 0;
        char tes[4] = {'6', 'p', 'q', 'r'};

        while (s) {
          char tempkey = customKeypad.getKey();
          if (tempkey == '6') {
            count = (count + 1) % 4;
          }
          Serial.println(tes[count]);
          printLcdDisplay(tes[count], data_count);
          //customKey = tes[sa];
          currentMillis = millis();
          if (currentMillis - previous > 3000) {
            s = false;
            previous = currentMillis;
          }
          currentMillis = millis();
        }
        data_count++;

      } else if (customKey == '7') {
        bool s = true;
        int count = 0;
        char tes[4] = {'7', 's', 't', 'u'};

        while (s) {
          char tempkey = customKeypad.getKey();
          if (tempkey == '7') {
            count = (count + 1) % 4;
          }
          Serial.println(tes[count]);
          printLcdDisplay(tes[count], data_count);
          //customKey = tes[sa];
          currentMillis = millis();
          if (currentMillis - previous > 3000) {
            s = false;
            previous = currentMillis;
          }
          currentMillis = millis();
        }
        data_count++;

      } else if (customKey == '8') {
        bool s = true;
        int count = 0;
        char tes[4] = {'8', 'v', 'w', 'x'};

        while (s) {
          char tempkey = customKeypad.getKey();
          if (tempkey == '8') {
            count = (count + 1) % 4;
          }
          Serial.println(tes[count]);
          printLcdDisplay(tes[count], data_count);
          //customKey = tes[sa];
          currentMillis = millis();
          if (currentMillis - previous > 3000) {
            s = false;
            previous = currentMillis;
          }
          currentMillis = millis();
        }
        data_count++;

      } else if (customKey == '9') {
        bool s = true;
        int count = 0;
        char tes[4] = {'9', 'y', 'z', '!'};

        while (s) {
          char tempkey = customKeypad.getKey();
          if (tempkey == '9') {
            count = (count + 1) % 4;
          }
          Serial.println(tes[count]);
          printLcdDisplay(tes[count], data_count);
          //customKey = tes[sa];
          currentMillis = millis();
          if (currentMillis - previous > 3000) {
            s = false;
            previous = currentMillis;
          }
          currentMillis = millis();
        }
        data_count++;
      }
    }
  }
}

void send() {
  Serial.print("send funtion");
  Serial.println(Data);
  //Serial2.print(Data);
  for (int i = 0; i < 32; i++) {
    if (Data[i] != '\0') {
      if (i != 31) {
        if ( Data[i + 1] == '\0') {
          //@ mark for no next data
          char send[2] = {Data[i], '@'};
          Serial2.print(send);
        }
        else {
          //$ mark for massage not end
          char send[2] = {Data[i], '$'};
          Serial2.print(send);
        }
      }
      else {
        //@ mark for no next data
        char send[2] = {Data[i], '@'};
        Serial2.print(send);
      }

    }
  }
  //Serial3.println(Data);//--------
  //Serial3.end();//-------
  Serial2.end();
  delay(300);
  Serial2.begin(57600);
  //Serial3.begin(9600);//----------
  delay(700);
  for (int i = 0; i < 32; i++) {
    Data[i] = '\0';
  }
}

void clearDisplayRemainPart(int coloum) {
  for (int i = coloum; i < 32; i++) {
    if (i <= 15) {
      lcd.setCursor(i, 0);
    }
    else {
      lcd.setCursor(i - 16, 1);
    }
    lcd.print(' ');
  }
}
void printLcdDisplay(char customKey , int data_count) {

  if (data_count < 32) {
    Serial.println(customKey);
    if (data_count < 16) {
      lcd.setCursor(data_count, 0);
      Data[data_count] = customKey;

    }
    else {
      lcd.setCursor(data_count - 16, 1);
      Data[data_count] = customKey;

    }
    lcd.print(customKey);
    //data_count++;
    // delay(1000);
    // lcd.clear();
    // delay(80);
  }
  else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sorry massage ");
    lcd.setCursor(0, 1);
    lcd.print("too long..");
    Serial.print("B & datacount=");
    Serial.println(data_count);
    int temp = data_count; //-------------
    Data[data_count] = '\n';
    //data_count--;
    // Serial.print("B datacount=");
    //Serial.println(data_count);
    delay(1000);
    lcd.clear();

    for (int i = 0; i < 32; i++) {
      if (i < 16) {
        lcd.setCursor(i, 0);
        lcd.print(Data[i]);
      }
      else {
        lcd.setCursor(i - 16, 1);
        lcd.print(Data[i]);
      }
      data_count = temp; //------
    }
  }

}


