//#include <Adafruit_HMC5883_U.h>
//#include <Wire.h>
#include <DynamixelSerial.h>
#include <Servo.h>
#include <Kalman.h>
#include <Adafruit_BMP085.h>
#include <TimerOne.h>
byte IMU_Data[100];
int IMU_Data_point = 0;

int16_t  data, yaw1, pitch1, roll1;

// For teensy
//IntervalTimer myTimer;
#define cycle_time1 10000



#define RESTRICT_PITCH // Comment out to restrict roll to ±90deg instead - please read: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf


Adafruit_BMP085 bmp;
Servo motor1, motor2;

#define MOTOR1_PIN    22
#define MOTOR2_PIN    23
#define buzzer 17

//IMU variable
Kalman kalmanX; // Create the Kalman instances
Kalman kalmanY;

float parameterLowpass = 0;
float filter_low_pass = 0;
float roll = 0;

/* IMU Data */
double accX, accY, accZ;
double gyroX, gyroY, gyroZ;
int16_t tempRaw;

double gyroXangle, gyroYangle; // Angle calculate using the gyro only
double compAngleX, compAngleY; // Calculated angle using a complementary filter
double kalAngleX, kalAngleY; // Calculated angle using a Kalman filter


float raw_pitch = 0;
float lowpass_pitch = 0;
float complementary_pitch = 0;
float kalman_pitch = 0;

float raw_roll = 0;
float lowpass_roll = 0;
float complementary_roll = 0;
float kalman_roll = 0;

uint32_t timer;
uint8_t i2cData[14]; // Buffer for I2C data

void setup() {

  Serial.begin(115200);
  Serial2.begin(115200);
  Timer1.initialize(cycle_time1);
  Timer1.attachInterrupt(printvalue);

  Dynamixel.setSerial(&Serial1); // &Serial - Arduino UNO/NANO/MICRO, &Serial1, &Serial2, &Serial3 - Arduino Mega
  Dynamixel.begin(1000000, 2); // Inicialize the servo at 1 Mbps and Pin Control 2

  motor1.attach(MOTOR1_PIN);
  motor2.attach(MOTOR2_PIN);

  Dynamixel.moveSpeed(0, deg2dyna(150), 300);
  Dynamixel.moveSpeed(1, deg2dyna(150), 300);

  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  delay(10);
  digitalWrite(buzzer, HIGH);
  delay(10);

  //  //IMU Sensor
  //  setup_kalman();
  //  for (int i = 0; i <= 1000; i++) {
  //    Read_Sensor_IMU();
  //    delay(1);
  //    Serial.println("wait IMU");
  //  }
  //
  //  //BMP Sensor
  //  if (!bmp.begin()) {
  //    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  //    while (1) {}
  //  }

  Serial.println("wait ESC Motor");

  delay(4000);
  motor1.writeMicroseconds(700);
  motor2.writeMicroseconds(700);
  delay(4000);


}

void propeller_off()
{
  motor1.writeMicroseconds(700);
  motor2.writeMicroseconds(700);
}

void propeller_on()
{
  motor1.writeMicroseconds(800);
  motor2.writeMicroseconds(800);
}

void loop() {

  // Read_Sensor_IMU();
  // Serial.print(kalAngleX); Serial.print("\t");
  // Serial.print(kalAngleY); Serial.println();


  //Read_Sensor_BMP();
  //


  propeller_on();

  for (int i = 0; i <= 2; i++) {
    cek_move();
  }

  for (int i = 0; i <= 2; i++) {
    cek_move_fast();
  }

  berhenti();
  delay (6000);

}

int deg2dyna(float deg) {
  int dyna = map(deg, 0, 300, 0, 1023);
  return dyna;
}


void berhenti()
{
  propeller_off();
  center();
}

void center()
{
  Dynamixel.moveSpeed(0, deg2dyna(150), 500);
  Dynamixel.moveSpeed(1, deg2dyna(150), 500);
}

void cek_move()
{
  //kiri
  Dynamixel.moveSpeed(0, deg2dyna(170), 100);
  Dynamixel.moveSpeed(1, deg2dyna(170), 100);
  delay(3000);
  Dynamixel.moveSpeed(0, deg2dyna(130), 100);
  Dynamixel.moveSpeed(1, deg2dyna(130), 100);
  delay(3000);
}

void cek_move_fast()
{
  //kiri
  Dynamixel.moveSpeed(0, deg2dyna(170), 1000);
  Dynamixel.moveSpeed(1, deg2dyna(170), 1000);
  delay(1000);
  Dynamixel.moveSpeed(0, deg2dyna(130), 1000);
  Dynamixel.moveSpeed(1, deg2dyna(130), 1000);
  delay(1000);
}
