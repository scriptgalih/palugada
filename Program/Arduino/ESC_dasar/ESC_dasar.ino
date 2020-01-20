#include <Servo.h>

#include "config.h"
#define MAX_SIGNAL 2000
#define MIN_SIGNAL 700
Servo esc1, esc2, esc3, esc4;

unsigned long time_ch[8];
int val_receiver[8];
int new_pwm;
boolean NOT_ARMING = true;
void setup() {
  Serial.begin(115200);
  Serial.println("Program begin...");
  Serial.println("This program will calibrate the ESC.");
  init_remote();
  esc1.attach(OUT_1);
  esc2.attach(OUT_4);
  esc3.attach(OUT_5);
  esc4.attach(OUT_6);

  esc1.writeMicroseconds(0);
  esc2.writeMicroseconds(0);
  esc3.writeMicroseconds(0);
  esc4.writeMicroseconds(0);
  while (NOT_ARMING) {
    show_value();
    if (val_receiver[0] < 1100 && val_receiver[1] >= 1800) {
      Serial.println("masuk arming");
      unsigned long start_arming_time = millis();
      while (val_receiver[0] < 1100 && val_receiver[1] >= 1800) {
        Serial.print("ngitung arming");
        Serial.print("\t");
        Serial.println(millis() - start_arming_time);
        if (millis() - start_arming_time >= 2000) {
          NOT_ARMING = false;
          break;
        }
      }
    }
  }
  Serial.println("Now writing maximum output.");
  Serial.println("Turn on power source, then wait 2 seconds and press any key.");
  esc1.writeMicroseconds(MAX_SIGNAL);
  esc2.writeMicroseconds(MAX_SIGNAL);
  esc3.writeMicroseconds(MAX_SIGNAL);
  esc4.writeMicroseconds(MAX_SIGNAL);

  // Wait for input
  //  while (!Serial.available()) {
  //    show_value();
  //    Serial.read();
  //  }
  delay(2000);
  // Send min output
  Serial.println("Sending minimum output");
  esc1.writeMicroseconds(MIN_SIGNAL);
  esc2.writeMicroseconds(MIN_SIGNAL);
  esc3.writeMicroseconds(MIN_SIGNAL);
  esc4.writeMicroseconds(MIN_SIGNAL);
}
void loop() {
  if (Serial.available()) {
    int pwm1 = Serial.parseInt();
    esc1.writeMicroseconds(pwm1);
    esc2.writeMicroseconds(pwm1);
    esc3.writeMicroseconds(pwm1);
    esc4.writeMicroseconds(pwm1);
    Serial.println(pwm1);
  }
  new_pwm = map(val_receiver[2], 1090, 1900, 750, 1800);
  esc1.writeMicroseconds(new_pwm);
  esc2.writeMicroseconds(new_pwm);
  esc3.writeMicroseconds(new_pwm);
  esc4.writeMicroseconds(new_pwm);
  show_value();
}
