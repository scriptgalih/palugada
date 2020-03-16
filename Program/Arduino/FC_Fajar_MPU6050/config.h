/*
Definisi port palugada FC
*/
#define LED1 22
#define LED2 23
#define LED3 24
#define LED4 25
#define BUZZER 17

#define IN_CH_1 16
#define IN_CH_2 15
#define IN_CH_3 14
#define IN_CH_4 13
#define IN_CH_5 39
#define IN_CH_6 38
#define IN_CH_7 37
#define IN_CH_8 36

#define OUT_1 23
#define OUT_2 22
#define OUT_3 21
#define OUT_4 20
#define OUT_5 35
#define OUT_6 34

#define ADC_BAT 3536

#define SCL0 19
#define SDA0 18

#define SCL2 3
#define SDA2 4

#define RX1 0
#define TX1 1
#define RX2 9
#define TX2 10
#define RX3 31
#define TX3 32

#define TARGET_START 0x01
#define TARGET_END   0x7F

#define WIRE_PINS   I2C_PINS_18_19
#if defined(__MKL26Z64__)               // LC
#define WIRE1_PINS   I2C_PINS_22_23
#endif
#if defined(__MK20DX256__)              // 3.1-3.2
#define WIRE1_PINS   I2C_PINS_29_30
#endif
#if defined(__MK64FX512__) || defined(__MK66FX1M0__)  // 3.5/3.6
#define WIRE1_PINS   I2C_PINS_37_38
#define WIRE2_PINS   I2C_PINS_3_4
#endif
#if defined(__MK66FX1M0__)              // 3.6
#define WIRE3_PINS   I2C_PINS_56_57
#endif

//---------------------------------------------- GLOBAL VARIABLE ----------------------------------------------
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

//byte IMU_Data[100];
//int IMU_Data_point = 0;
//int16_t  data, yaw1, pitch1, roll1;
