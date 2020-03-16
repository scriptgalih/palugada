void pid_exe() {
//  show_value();
  //  Serial.print(pitchPID.showP()); Serial.print("\t");
  //  Serial.print(pitchPID.showI()); Serial.print("\t");
  //  Serial.print(pitchPID.showD()); Serial.print("\t");
  //
  //  Serial.print(out_pitch_pid); Serial.print("\t");
  //  Serial3.println(inRoll);
  //    Serial3.println(t
  
  new_pwm = map(current_output_throttle, 1075, 1900, 650, 1800);
//      Serial.print(new_pwm); Serial.print("\r\n");

  //  update_pwm();
  if (new_pwm >= 830) {
    //    new_pwm = map(val_receiver[2], 1150, 1900, 750, 1800);
    out_pitch_pid = pitchPID.calc(inPitch);
    out_roll_pid = rollPID.calc(inRoll);
    PWM[0] = constrain(new_pwm + out_pitch_pid - out_roll_pid, 830, 1400);
    PWM[1] = constrain(new_pwm - out_pitch_pid + out_roll_pid, 830, 1400);
    PWM[2] = constrain(new_pwm + out_pitch_pid + out_roll_pid, 830, 1400);
    PWM[3] = constrain(new_pwm - out_pitch_pid - out_roll_pid, 830, 1400);
    for (int x = 0; x <= 3; x++) {
      //      Serial.print(PWM[x]); Serial.print("\t");
    }
    update_pwm();
  } else {
    pitchPID.resetOutput();
    minPWM();
    //    Serial.print("\t"); Serial.print("MATI");
  }


}
