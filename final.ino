#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define col 5
#define row 1

const int touchsensor = <<pin_num>>;
int touchstate = 0;
boolean flag_touch = false;

LiquidCrystal_I2C lcd(0x27,16,2);

const int RED = <<pin_num>>;
const int GREEN = <<pin_num>>;
const int BLUE = <<pin_num>>;

const int tempsensor = <<pin_num>>;
const int TEMP0 = <<기준온도>>;
int temp;
boolean temp_flag = false;

const int heatPad = <<pin_num>>;


void setup() {
  Serial.begin(9600);
  pinMode(touchsensor, INPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(tempsensor, INPUT);
  pinMode(heatPad, OUTPUT);
}

void makeColor(int REDVALUE, int GREENVALUE, int BLUEVALUE) {
  analogWrite(RED, REDVALUE);
  analogWrite(GREEN, GREENVALUE);
  analogWrite(BLUE, BLUEVALUE);
}

int conv(double readVal) {
  double voltage = readVal*5/1024;
  double temp = voltage*100-50;
  return (int)temp;
}

void loop() {

  if(touchstate == HIGH) {

    if(flag_touch == false) {
      flag_touch = true;
      Serial.println("touchsensor ON");

      lcd.init();
      lcd.backlight();
      lcd.noCursor();

      makeColor(0, 0, 255);
      Serial.println("LED: BLUE");

      digitalWrite(heatPad, HIGH);
      temp_flag = true;
    }

    else {
      flag_touch = false;
      Serial.println("touchsensor OFF");

      lcd.clear();
      lcd.noCursor();
      lcd.noBacklight();

      makeColor(0, 0, 0);

      digitalWrite(heatPad, LOW);
    }

  }

  if(flag_touch == true) {
    temp = conv(analogRead(tempsensor));
    lcd.setCursor(col, row);
    lcd.print(temp);
    lcd.setCursor(col+3, row);
    lcd.print("'C");
    lcd.noCursor();
    Serial.print("LCD: ");
    Serial.print(temp);
    Serial.println("'C");

    if(temp_flag == true && temp >= TEMP0) {
      makeColor(255, 0, 0);
      Serial.println("LED: RED");
      temp_flag = false;
    }

  }
  
  delay(100);
}