#include <Servo.h>
#include "MillisTimer.h"
#include "config.h"

#define MAX_SIGNAL 2000
#define MIN_SIGNAL 700

#include "SG_PID.h"

Servo esc1, esc2, esc3, esc4;
pid roll_pid;
unsigned long time_ch[8];
int val_receiver[8];
int new_pwm;
boolean NOT_ARMING = true;
float sensor_value;
byte IMU_Data[100];
int IMU_Data_point = 0;
int16_t  data, yaw1, pitch1, roll1;

int PWM_ESC_1, PWM_ESC_2, PWM_ESC_3, PWM_ESC_4;
int throttle;
MillisTimer timer1 = MillisTimer(10);
void myTimerFunction(MillisTimer &mt)
{
  //  printvalue();
  Serial.print(0 - sensor_value); Serial.print("\t");
  Serial.print(PWM_ESC_1); Serial.print("\t");
  Serial.print(PWM_ESC_2); Serial.print("\t");
  Serial.print(PWM_ESC_3); Serial.print("\t");
  Serial.print(PWM_ESC_4); Serial.print("\t");
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  timer1.setInterval(10);
  timer1.expiredHandler(myTimerFunction);
  timer1.start();
  pinMode(BUZZER, 1);
  digitalWrite(BUZZER, 1);

  Serial.println("Program begin...");
  Serial.println("This program will calibrate the ESC.");
  //  init_remote();
  esc1.attach(OUT_1);
  esc2.attach(OUT_2);
  esc3.attach(OUT_3);
  esc4.attach(OUT_4);

  esc1.writeMicroseconds(0);
  esc2.writeMicroseconds(0);
  esc3.writeMicroseconds(0);
  esc4.writeMicroseconds(0);
  delay(2000);
  //  while (NOT_ARMING) {
  //    show_value();
  //
  //    if (val_receiver[0] < 1100 && val_receiver[1] >= 1800 && val_receiver[2] <= 1100) {
  //      Serial.println("masuk arming");
  //      unsigned long start_arming_time = millis();
  //      while (val_receiver[0] < 1100 && val_receiver[1] >= 1800 && val_receiver[2] <= 1100) {
  //        Serial.print("ngitung arming");
  //        Serial.print("\t");
  //        Serial.println(millis() - start_arming_time);
  //        if (millis() - start_arming_time >= 2000) {
  //          NOT_ARMING = false;
  //          break;
  //        }
  //      }
  //    }
  //  }
  digitalWrite(BUZZER, !digitalRead(BUZZER));
  delay(100);
  digitalWrite(BUZZER, !digitalRead(BUZZER));
  Serial.println("Now writing maximum output.");
  Serial.println("Turn on power source, then wait 2 seconds and press any key.");
  esc1.writeMicroseconds(MAX_SIGNAL);
  esc2.writeMicroseconds(MAX_SIGNAL);
  esc3.writeMicroseconds(MAX_SIGNAL);
  esc4.writeMicroseconds(MAX_SIGNAL);

  delay(2000);
  // Send min output
  Serial.println("Sending minimum output");
  esc1.writeMicroseconds(MIN_SIGNAL);
  esc2.writeMicroseconds(MIN_SIGNAL);
  esc3.writeMicroseconds(MIN_SIGNAL);
  esc4.writeMicroseconds(MIN_SIGNAL);
  delay(2000);
  throttle = 900;
  //  esc1.writeMicroseconds(800);
  //  esc2.writeMicroseconds(MIN_SIGNAL);
  //  esc3.writeMicroseconds(MIN_SIGNAL);
  //  esc4.writeMicroseconds(MIN_SIGNAL);
  //  delay(2000);
  //  esc1.writeMicroseconds(MIN_SIGNAL);
  //  esc2.writeMicroseconds(800);
  //  esc3.writeMicroseconds(MIN_SIGNAL);
  //  esc4.writeMicroseconds(MIN_SIGNAL);
  //  delay(2000);
  //  esc1.writeMicroseconds(MIN_SIGNAL);
  //  esc2.writeMicroseconds(MIN_SIGNAL);
  //  esc3.writeMicroseconds(800);
  //  esc4.writeMicroseconds(MIN_SIGNAL);
  //  delay(2000);
  //  esc1.writeMicroseconds(MIN_SIGNAL);
  //  esc2.writeMicroseconds(MIN_SIGNAL);
  //  esc3.writeMicroseconds(MIN_SIGNAL);
  //  esc4.writeMicroseconds(800);
  //  delay(2000);
  //    esc1.writeMicroseconds(MIN_SIGNAL);
  //    esc2.writeMicroseconds(MIN_SIGNAL);
  //    esc3.writeMicroseconds(MIN_SIGNAL);
  //    esc4.writeMicroseconds(MIN_SIGNAL);

  esc1.writeMicroseconds(throttle);
  esc2.writeMicroseconds(throttle);
  esc3.writeMicroseconds(throttle);
  esc4.writeMicroseconds(throttle);

  roll_pid.param(3.5, 0.001, 0.25, NORMAL); ///////////////////////////////////////////////////////////////
  roll_pid.constraint(-400, 400);
  roll_pid.setPoint(0);
  roll_pid.readSensor(0);
  roll_pid.timeSampling(1);
  roll_pid.showParam();
}
void loop() {
  timer1.run();
  sensor_value = roll1 / 100.0;
  roll_pid.readSensor(sensor_value);
  //  Serial.print(0-sensor_value); Serial.print("\t");
  //  Serial.println();
  roll_pid.calc();
  //  roll_pid.showUnitPID();
  int val_roll_pid = roll_pid.showPID();
  //  Serial.print(roll_pid.showPID(), 4);

  PWM_ESC_1 = constrain(throttle + val_roll_pid, 780, 1500);
  PWM_ESC_2 = constrain(throttle - val_roll_pid, 780, 1500);
  PWM_ESC_3 = constrain(throttle - val_roll_pid, 780, 1500);
  PWM_ESC_4 = constrain(throttle + val_roll_pid, 780, 1500);


  updateESC();
}

void updateESC() {
  esc1.writeMicroseconds(PWM_ESC_1);
  esc2.writeMicroseconds(PWM_ESC_2);
  esc3.writeMicroseconds(PWM_ESC_3);
  esc4.writeMicroseconds(PWM_ESC_4);
}
