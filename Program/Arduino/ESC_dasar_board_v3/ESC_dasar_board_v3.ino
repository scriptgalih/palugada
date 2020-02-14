
#include <Servo.h>
#include "config.h"

#define MAX_SIGNAL 2000
#define MIN_SIGNAL 700
#include <i2c_t3.h>
#include <Kalman.h> // Source: https://github.com/TKJElectronics/KalmanFilter

#define RESTRICT_PITCH // Comment out to restrict roll to ±90deg instead - please read: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf

Kalman kalmanX; // Create the Kalman instances
Kalman kalmanY;

/* IMU Data */
double accX, accY, accZ;
double gyroX, gyroY, gyroZ;
int16_t tempRaw;

double gyroXangle, gyroYangle; // Angle calculate using the gyro only
double compAngleX, compAngleY; // Calculated angle using a complementary filter
double kalAngleX, kalAngleY; // Calculated angle using a Kalman filter

uint32_t timer;
uint8_t i2cData[14]; // Buffer for I2C data

Servo esc1, esc2, esc3, esc4;

unsigned long time_ch[8];
int val_receiver[8];
int new_pwm;
boolean NOT_ARMING = true;

byte IMU_Data[100];
int IMU_Data_point = 0;
int16_t  data, yaw1, pitch1, roll1;

//void myTimerFunction(MillisTimer &mt)
//{
//  printvalue();
//}

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
//      Serial.println("masuk arming");
      unsigned long start_arming_time = millis();
      while (val_receiver[0] < 1100 && val_receiver[1] >= 1800 && val_receiver[2] <= 1100) {
//        Serial.print("ngitung arming");
//        Serial.print("\t");
//        Serial.println(millis() - start_arming_time);
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
  esc1.writeMicroseconds(MAX_SIGNAL);
  esc2.writeMicroseconds(MAX_SIGNAL);
  esc3.writeMicroseconds(MAX_SIGNAL);
  esc4.writeMicroseconds(MAX_SIGNAL);
  initMPU6050();
  delay(2000);
  // Send min output
  Serial.println("Sending minimum output");
  esc1.writeMicroseconds(MIN_SIGNAL);
  esc2.writeMicroseconds(MIN_SIGNAL);
  esc3.writeMicroseconds(MIN_SIGNAL);
  esc4.writeMicroseconds(MIN_SIGNAL);
}
void loop() {
  calculateMPU6050();
  new_pwm = map(val_receiver[2], 1090, 1900, 750, 1800);
  esc1.writeMicroseconds(new_pwm);
  esc2.writeMicroseconds(new_pwm);
  esc3.writeMicroseconds(new_pwm);
  esc4.writeMicroseconds(new_pwm);
//  Serial.println(new_pwm);
//  Serial.print("*");Serial.print(",");
//  Serial.print(0);Serial.print(",");
  Serial.print(compAngleX);Serial.print("\t");
  Serial.print(compAngleY);Serial.print("\t");
  Serial.print("\r\n");
  delay(20);
}
