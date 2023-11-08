// 터치센서 -> 전원 ON/OFF -> LED 색 표현 & 열선 가열
// 열선 온도 측정 -> LCD에 온도 표시

// 터치센서, 온도센서, 열선onoff, led, lcd

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// 터치센서
const int touchsensor = <<N4>>;
const int touchstate = 0;
boolean flag_touch = false;

// lcd
LiquidCrystal_I2C lcd(0x27,20,4); // 0x3F

// led
const int RED = <<N1>>;
const int GREEN = <<N2>>;
const int BLUE = <<N3>>;

// 온도센서
const int TEMP0 = <<기준온도>>;
double temp;

// 열선
const int heatPad = <<N5>>;



void makeColor(int REDVALUE, int GREENVALUE, int BLUEVALUE) {
  analogWrite(RED, REDVALUE);
  analogWrite(GREEN, GREENVALUE);
  analogWrite(BLUE, BLUEVALUE);
}

void setup() {
  Serial.begin(9600);
  pinMode(touchsensor, INPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(heatPad, OUTPUT);
}

void loop() {

  if(touchstate == HIGH) { // 터치센서 접촉

    if(flag_touch == false) { // 꺼져 있던 경우
      flag_touch = true;
      Serial.println("touchsensor ON");

      // lcd initialization
      lcd.init();
      lcd.backlight();
      lcd.noCursor();

      // led = blue
      makeColor(0, 0, 255);
      Serial.println("LED: BLUE");

      // 열선 가열 시작
      digitalWrite(heatPad, HIGH);
    }

    else { // 켜져 있던 경우
      flag_touch = false;
      Serial.println("touchsensor OFF");

      // lcd off
      lcd.clear();
      lcd.noCursor();
      lcd.noBacklight();

      // led off
      digitalWrite(<<핀번호>>, LOW);///////

      // 열선 가열 종료
      digitalWrite(heatPad, LOW);
    }

  }

  // 온도 측정 => lcd/serial 출력
  temp = analogRead(A0); // ANALOG_0
  lcd.setCursor(<<col,row>>); // <<좌표설정>>
  lcd.noCursor();
  lcd.printf("%.1f℃", temp);
  Serial.println("LCD: " + temp + "℃");

  if(temp >= TEMP0) {
    makeColor(255, 0, 0); // led = red
    Serial.println("LED: RED");
  }





  delay(100);
}