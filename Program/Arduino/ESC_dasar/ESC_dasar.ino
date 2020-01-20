#include <Servo.h>
#include "config.h"
#define MAX_SIGNAL 2000
#define MIN_SIGNAL 700
Servo esc1, esc2, esc3, esc4;
void setup() {
  Serial.begin(115200);
  Serial.println("Program begin...");
  Serial.println("This program will calibrate the ESC.");

  esc1.attach(OUT_1);
  esc2.attach(OUT_4);
  esc3.attach(OUT_5);
  esc4.attach(OUT_6);

  Serial.println("Now writing maximum output.");
  Serial.println("Turn on power source, then wait 2 seconds and press any key.");

  esc1.writeMicroseconds(MAX_SIGNAL);
  esc2.writeMicroseconds(MAX_SIGNAL);
  esc3.writeMicroseconds(MAX_SIGNAL);
  esc4.writeMicroseconds(MAX_SIGNAL);

  // Wait for input
  while (!Serial.available());
  Serial.read();

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

}
