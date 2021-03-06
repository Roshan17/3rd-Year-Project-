
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define BACKLIGHT_PIN  13

const char address[4] = {'0', '0', '0', '1'};
String routingTable[5] = {"0001", "0002"};
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // Set the LCD I2C address



#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
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
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  pinMode(AUX, INPUT);
  lcd.begin(16, 2);
  lcd.backlight();
  Serial.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);

  currentMillis = millis();
}


bool pqr = true;

char  destinationArr[4];


int arrayIndex = 0;

char  sourceArr[4];

bool isGeatWayMassage = false;
char getwayMassage[32];
int getwayCount = 0;
String TTLValue;
int msgId = 0; //every send massage to get way has a id
String ResivedMsgId;
void loop() {
  // int AUXState = digitalRead(AUX);
  int  n = 0;
  while (Serial2.available())
  {
    if (isTypingMsg == 1) {
      isTypingMsg = 0;
      clearDisplayRemainPart(0);
    }
    char x = Serial2.read();
    Serial.print(x);
    Serial.print("  arrayIndex=>>>>");
    Serial.println(arrayIndex);
    if (arrayIndex < 4) {
      destinationArr[arrayIndex] = x;
      arrayIndex++;

    }
    else if (arrayIndex < 8) {
      sourceArr[arrayIndex - 4] = x;
      arrayIndex++;
    }
    else if (x == '$' && arrayIndex == 8) {
      destinationArr[4] = '\0';
      if (String(destinationArr) == "9999") {
        // Serial.print("destinationArr= ");
        isGeatWayMassage = true;
        arrayIndex++;
      }
    }
    else if (isGeatWayMassage) {

      if (x == '@') {
        Serial.print("TTLValue---> ");
        Serial.println(TTLValue);
        int ttl = TTLValue.toInt();
        if (ttl > 0) {
          sourceArr[4] = '\0';
          if (String(sourceArr) == String(address)) {
            Serial.println(" massage Destroied ");
          } else {
            if (SearchRoutingTable()) {
              //if (ttl != 0) {
              sourceArr[4] = '\0';
              SendTOGetWay(getwayMassage, String(sourceArr), "9999", (ttl - 1), ResivedMsgId);

              //}
            }
            else {
              //if (ttl != 0) {
              sourceArr[4] = '\0';
              SendTOGetWay(getwayMassage, String(sourceArr), "9999", (ttl - 1), ResivedMsgId);

              delay(500);



              //}
            }
          }

        } else {
          Serial.println("massage Destroied ");
        }
        getwayCount = 0;
        arrayIndex = 0;
        isGeatWayMassage = false;
      }
      else if (arrayIndex == 9) {
        TTLValue = x;
        arrayIndex++;
      }
      else if (arrayIndex == 10) {
        ResivedMsgId = x;
        arrayIndex++;
      }
      else {
        getwayMassage[getwayCount] = x;
        getwayCount++;
      }
    }
    else {
      destinationArr[4] = '\0';
      Serial.print("destinationArr= ");
      Serial.print(String(destinationArr));
      Serial.print("  address= ");
      Serial.println(String(address));
      if ((String(destinationArr) == String(address)) || (String(destinationArr) == "1111")) {
        if (isAlphaNumeric(x) || (x == ' ') || (x == ';') || (x == '.')) {

          if (n <= 15) {
            lcd.setCursor(n, 0);
            delay(30);//if we did not put if input=1 not display
          }
          else {

            lcd.setCursor(n - 16, 1);
            delay(30);//if we did not put if input=1 not display
          }
          // lcd.setCursor(n, 0);
          n++;
          lcd.print(x);
        }

      }
      if (x == '@') {
        Serial.println("---------------------------");
        clearDisplayRemainPart(n);
        isMsgDisplay = 1;
        arrayIndex = 0;
        for (int i = 0; i < 4; i++) {
          destinationArr[i] = '\0';
          sourceArr[i] = '\0';
        }
      }
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
    } else if (customKey == 'D') {
      Serial.println("msgId=");
      Serial.println(msgId);
      msgId = msgId % 10;
      SendTOGetWay(Data, String(address), "9999", 5, String(msgId));
      msgId++;
      data_count = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Massage Send...");
      delay(1000);
      lcd.clear();
    }
    else if (customKey == 'C') {
      if (isMsgDisplay == 1) {
        isMsgDisplay = 0;
        isTypingMsg = 1;
        clearDisplayRemainPart(0);
        data_count = 0;
      }
      Serial.println("_");
      printLcdDisplay(' ', data_count);
      data_count++;
    }
    else if (customKey == 'A') {
      send(Data, String(address), "1111"); /////////////////////////
      data_count = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Massage Send...");
      delay(1000);
      lcd.clear();
    }
    else if (customKey == '#') {
      if (isMsgDisplay == 1) {
        isMsgDisplay = 0;
        isTypingMsg = 1;
        clearDisplayRemainPart(0);
        data_count = 0;
      }
      Serial.println(".");
      printLcdDisplay('.', data_count);
      data_count++;
    }
    else if (customKey == '*') {
      if (isMsgDisplay == 1) {
        isMsgDisplay = 0;
        isTypingMsg = 1;
        clearDisplayRemainPart(0);
        data_count = 0;
      }
      Serial.println(";");
      printLcdDisplay(';', data_count);
      data_count++;
    } else if (customKey == '0') {
      if (isMsgDisplay == 1) {
        isMsgDisplay = 0;
        isTypingMsg = 1;
        clearDisplayRemainPart(0);
        data_count = 0;
      }
      Serial.println("0");
      printLcdDisplay('0', data_count);
      data_count++;
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

void SendTOGetWay(char data[], String sourceAddress, String getwayAddress, int TTLValue, String msgId) {
  Serial.print("SendTOGetWay");
  Serial.print(data);
  Serial.print(" TTLValue= ");
  Serial.println(TTLValue);

  Serial2.print(getwayAddress);//getway address
  Serial2.print(sourceAddress);//source
  Serial2.print('$');//TTL Value available
  Serial2.print(String(TTLValue));//TTL Value

  Serial2.print(msgId);//TTL Value

  Serial2.print(Data);//massage
  Serial2.print('@');

  Serial2.end();
  delay(300);
  Serial2.begin(9600);
  //Serial3.begin(9600);//----------
  delay(700);
  for (int i = 0; i < 32; i++) {
    Data[i] = '\0';
  }
}
void send(char data[], String sourceAddress, String destinationAddress) {
  Serial.print("send funtion");
  Serial.println(data);

  Serial2.print(destinationAddress);//destination
  Serial2.print(sourceAddress);//source
  Serial2.print(Data);//massage
  Serial2.print('@');

  Serial2.end();
  delay(300);
  Serial2.begin(9600);
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
    // Serial.println(customKey);
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
bool SearchRoutingTable() {
  for (int i = 0; i < 5; i++) {
    if (routingTable[i] == "9999") {
      return true;
    }
  }
  return false;
}

