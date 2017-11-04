
// サーボモータ動作確認用プログラム
// セルコンプロジェクト　https://github.com/cellucon/arduino.mini
/*  シリアルモニタからサーボを操作可能
  r: データ送信
  a: attach
  d: detach
PWMの制御幅で角度コントロール
  http://tetsuakibaba.jp/index.php?page=workshop/ServoBasis/main
  800-2200: 0度〜90度設定
  PWM　9Pin 方向、10Pin　ESC
*/
#include <Servo.h>
Servo sv9;
Servo sv10;

int analog0 = 0;
int analog1 = 0;
int i = 7;

unsigned long lastTime;
boolean servoAttached = false;

void setup()
{
  Serial.begin(115200);
  pinMode(13, OUTPUT); //LED表示
  lastTime = millis();
}

void loop()
{
  //Bt通信監視
  if (servoAttached && lastTime > millis() + 1100) {
    sv9.writeMicroseconds(1500);
    sv10.writeMicroseconds(1500);
  }

  int ret;
  if ( Serial.available() > 0 ) {
    lastTime = millis();
    digitalWrite(13, HIGH); //LED点灯
    int d = Serial.read();

    if ( d == 'a' ) {
      sv9.attach(9, 800, 2200);
      sv10.attach(10, 800, 2200);
      servoAttached = true;
      Serial.println("Servo Attached");
    }
    else if ( d == 'd' ) {
      sv9.detach();
      sv10.detach();
      servoAttached = false;
      Serial.println("Servo Detached");
    }
    else if ( d == 'r' ) {
        Serial.println("RPS:0");
        int batValue = 100; //analogRead(A3);
        Serial.println("BAT:" + String(batValue, DEC));
     }
    else if ( '0' <= d && d <= '9' ) {
      d -= '0';
      if (i == 7) {
        analog0 = d * 1000;
      }
      if (i == 6) {
        analog0 += d * 100;
      }
      if (i == 5) {
        analog0 += d * 10;
      }
      if (i == 4) {
        analog0 += d;
      }
      if (i == 3) {
        analog1 = d * 1000;
      }
      if (i == 2) {
        analog1 += d * 100;
      }
      if (i == 1) {
        analog1 += d * 10;
      }
      if (i == 0) {
        analog1 += d;
        Serial.println("analog0  : " + String(analog0, DEC));
        Serial.println("analog1 : " + String(analog1, DEC));
      }
      i--;
    }
    else if ( d == 'm' ) {
      sv11.writeMicroseconds(analog0);
      sv12.writeMicroseconds(analog1);
      analog0 = 0;
      analog1 = 0;
      i = 7;
    } else if ( d == 'x' ) {
      //sv5.writeMicroseconds(analog9);
      //moveShaft(sv5, analog0, 5);
      //Serial.println("camera x  : " + String(analog9, DEC));
      analog0 = 0;
      i = 7;
    } else if ( d == 'y' ) {
      //sv6.writeMicroseconds(analog9);
      //moveShaft(sv6, analog0, 5);
      //Serial.println("camera y  : " + String(analog9, DEC));
      analog0 = 0;
      i = 7;
    }
    digitalWrite(13, LOW);   //LED消灯
  }
}

