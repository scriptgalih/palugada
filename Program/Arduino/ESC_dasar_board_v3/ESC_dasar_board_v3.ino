
#include <Servo.h>
#include <Kalman.h>
#include <pid2.h>
#include "config.h"

#define MAX_SIGNAL 2000
#define MIN_SIGNAL 650
#include <i2c_t3.h>
// Source: https://github.com/TKJElectronics/KalmanFilter

#define RESTRICT_PITCH // Comment out to restrict roll to ±90deg instead - please read: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf
#define LEN_AVG 10
float tempIMU[2][LEN_AVG];
float inPitch, inRoll;

IntervalTimer MPUTimer, pidTimer; // SETTING TIMER
pid2 pitchPID, rollPID;
int out_pitch_pid, out_roll_pid;
Servo esc1, esc2, esc3, esc4;

unsigned long time_ch[8];
int val_receiver[8];
int new_pwm;
boolean NOT_ARMING = true;
int PWM[4];

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  Wire2.begin(I2C_MASTER, 0x00, WIRE2_PINS, I2C_PULLUP_EXT, 400000);
  Wire2.setDefaultTimeout(10000); // 10ms
  pinMode(BUZZER, 1);
  digitalWrite(BUZZER, 1);

  Serial.println("Program begin...");
  Serial.println("This program will calibrate the ESC.");

  init_remote();
  esc1.attach(OUT_1);
  esc2.attach(OUT_2);
  esc3.attach(OUT_3);
  esc4.attach(OUT_4);

  esc1.writeMicroseconds(0);
  esc2.writeMicroseconds(0);
  esc3.writeMicroseconds(0);
  esc4.writeMicroseconds(0);

  while (NOT_ARMING) {
//    show_value();
    delay(1);
    if (val_receiver[0] < 1100 && val_receiver[1] >= 1800 && val_receiver[2] <= 1100) {
      Serial.println("masuk arming");
      unsigned long start_arming_time = millis();
      while (val_receiver[0] < 1100 && val_receiver[1] >= 1800 && val_receiver[2] <= 1100) {
        //                Serial.print("ngitung arming");
        //                Serial.print("\t");
        //                Serial.println(millis() - start_arming_time);
        yield();
        if (millis() - start_arming_time >= 2000) {
          NOT_ARMING = false;
          break;
        }
      }
    }
  }
  digitalWrite(BUZZER, !digitalRead(BUZZER));
  delay(100);
  digitalWrite(BUZZER, !digitalRead(BUZZER));
  Serial.println("Now writing maximum output.");
  Serial.println("Turn on power source, then wait 2 seconds and press any key.");

  //    esc1.writeMicroseconds(MAX_SIGNAL);
  //    esc2.writeMicroseconds(MAX_SIGNAL);
  //    esc3.writeMicroseconds(MAX_SIGNAL);
  //    esc4.writeMicroseconds(MAX_SIGNAL);
  initMPU6050();
  MPUTimer.begin(calculateMPU6050, 50000);
  pidTimer.begin(pid_exe, 100000);

  pitchPID.setParam(0, 0, 0);
  pitchPID.setLimit(400);
  rollPID.setParam(0, 0, 0);
  rollPID.setLimit(400);
  //  delay(2000);
  // Send min output
  Serial.println("Sending minimum output");
  Serial.println("Pitch Roll");
  minPWM();
}
void loop() {

}

void update_pwm() {
  esc1.writeMicroseconds(PWM[0]); //-
  esc2.writeMicroseconds(PWM[1]); //+
  esc3.writeMicroseconds(PWM[2]); //-
  esc4.writeMicroseconds(PWM[3]); //+
}

void minPWM() {
  esc1.writeMicroseconds(MIN_SIGNAL);
  esc2.writeMicroseconds(MIN_SIGNAL);
  esc3.writeMicroseconds(MIN_SIGNAL);
  esc4.writeMicroseconds(MIN_SIGNAL);
}
