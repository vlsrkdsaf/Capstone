/*
schema
: 터치센서 -> 전원 ON/OFF -> LED 색 표현 & 열선 가열
: 열선 온도 측정 -> LCD에 온도 표시

구현 요소: 터치센서, 온도센서, 열선, led, lcd
(led red = 온도 충분을 의미)
*/

/*
업로드 전 설정할 항목
- P1~P6 핀번호 설정 (아날로그는 A*)
- 기준온도 설정 (line )
- LCD 커서 좌표 설정 (line )
*/


#include <Wire.h>
#include <LiquidCrystal_I2C.h>


// 터치센서
const int touchsensor = <<P1>>;
const int touchstate = 0;
boolean flag_touch = false;

// lcd
LiquidCrystal_I2C lcd(0x27,20,4); // 0x3F

// led
const int RED = <<P2>>;
const int GREEN = <<P3>>;
const int BLUE = <<P4>>;

// 온도센서
const int tempsensor = <<P6>>;
const int TEMP0 = <<기준온도>>;
double temp;
boolean temp_flag = false;

// 열선
const int heatPad = <<P5>>;


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
      temp_flag = true;
    }

    else { // 켜져 있던 경우
      flag_touch = false;
      Serial.println("touchsensor OFF");

      // lcd off
      lcd.clear();
      lcd.noCursor();
      lcd.noBacklight();

      // led off
      makeColor(0, 0, 0);

      // 열선 가열 종료
      digitalWrite(heatPad, LOW);
    }

  }

  if(flag_touch == true) {

    // 온도 측정 => lcd/serial 출력
    temp = analogRead(tempsensor);
    lcd.setCursor(<<col,row>>); // LCD 커서 좌표설정
    lcd.noCursor();
    lcd.printf("%.1f℃", temp);
    Serial.println("LCD: " + temp + "℃");

    if(temp_flag == true && temp >= TEMP0) {
      makeColor(255, 0, 0); // led = red
      // delay not required
      Serial.println("LED: RED");
      temp_flag = false;
    }

  }
  
  delay(100); // 터치센서 반응 느리면 delay 삭제할 것
}