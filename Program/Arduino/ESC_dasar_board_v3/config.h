/*
Definisi port palugada FC
*/
#define LED1 35
#define LED2 34
#define LED3 33
#define BUZZER 29

#define IN_CH_1 20
#define IN_CH_2 19
#define IN_CH_3 18
#define IN_CH_4 17
#define IN_CH_5 16
#define IN_CH_6 15
#define IN_CH_7 14
#define IN_CH_8 13

#define OUT_1 39
#define OUT_2 38
#define OUT_3 37
#define OUT_4 36
#define OUT_5 35
#define OUT_6 34

#define ADC_BAT 36

#define SCL0 19
#define SDA0 18

#define SCL2 3
#define SDA2 4

#define RX1 0
#define TX1 1
#define RX2 9
#define TX2 10
#define RX3 7
#define TX3 8
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
