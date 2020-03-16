int RECEIVER_PIN[8] = {IN_CH_1, IN_CH_2, IN_CH_3, IN_CH_4, IN_CH_5, IN_CH_6, IN_CH_7, IN_CH_8};

void init_remote() {
  attachInterrupt(digitalPinToInterrupt(IN_CH_1), pulse1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(IN_CH_2), pulse2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(IN_CH_3), pulse3, CHANGE);
  attachInterrupt(digitalPinToInterrupt(IN_CH_4), pulse4, CHANGE);
  //  attachInterrupt(digitalPinToInterrupt(IN_CH_5), pulse5, CHANGE);
  //  attachInterrupt(digitalPinToInterrupt(IN_CH_6), pulse6, CHANGE);
  //  attachInterrupt(digitalPinToInterrupt(IN_CH_7), pulse7, CHANGE);
  //  attachInterrupt(digitalPinToInterrupt(IN_CH_8), pulse8, CHANGE);
}

void calculateReceiver(int idx) {
  if (digitalRead(RECEIVER_PIN[idx]) == HIGH) {
    time_ch[idx] = micros();
  } else {
    //    int aa = movAvg(idx, (int)(micros() - time_ch[idx]));
    long t_now = micros() - time_ch[idx];
    if (abs(t_now - last_time_ch[idx]) > 30){
      last_time_ch[idx] = t_now;  
      return;
    }
    val_receiver[idx] = (int)t_now;
    
    //    val_receiver[idx] = aa;
  }
}

void show_value() {

  for (int x = 0; x <= 3 ; x++) {
    Serial.print(val_receiver[x]); Serial.print("\t");
  }
  Serial.print(new_pwm); Serial.print("\t");
  Serial.println();
}
void pulse1() {
  calculateReceiver(0);
}
void pulse2() {
  calculateReceiver(1);
}
void pulse3() {
  calculateReceiver(2);
}
void pulse4() {
  calculateReceiver(3);
}
void pulse5() {
  calculateReceiver(4);
}
void pulse6() {
  calculateReceiver(5);
}
void pulse7() {
  calculateReceiver(6);
}
void pulse8() {
  calculateReceiver(7);
}

float movAvg(int idx, float in1) {
  float sum = 0;
  for (int x = (LEN_AVG - 1); x >= 1; x--) {
    tempIMU[idx][x] = tempIMU[idx][x - 1];
    sum += tempIMU[idx][x];
  }
  tempIMU[idx][0] = in1;
  sum += in1;
  return sum / (float)LEN_AVG;
}
