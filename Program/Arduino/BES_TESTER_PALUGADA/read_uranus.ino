void printvalue()
{
  
  Serial.write('*');         Serial.print(',');
  Serial.print(yaw1/10.0);   Serial.print(',');
  Serial.print(pitch1/100.0); Serial.print(',');
  Serial.println(roll1/100.0);
}


void serialEvent2(){
  if(Serial2.available())
  {
    IMU_Data[IMU_Data_point] =   Serial2.read(); 
    if(IMU_Data_point >= 100) IMU_Data_point = 0;

      if (IMU_Data_point >= 31)
      {
        if((IMU_Data[IMU_Data_point-31] == 0x88) && (IMU_Data[IMU_Data_point-30] == 0xAF) )
        {
                  yaw1   = (IMU_Data[IMU_Data_point-6]  << 8) | IMU_Data[IMU_Data_point-5];
                  pitch1 = (IMU_Data[IMU_Data_point-8]  << 8) | IMU_Data[IMU_Data_point-7];
                  roll1  = (IMU_Data[IMU_Data_point-10] << 8) | IMU_Data[IMU_Data_point-9];
                  IMU_Data_point = 0;
        }
      }
      
   IMU_Data_point++;
  }
}
