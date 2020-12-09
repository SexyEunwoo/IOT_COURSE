#include <SoftwareSerial.h>

SoftwareSerial pms(2, 3);

unsigned char datas[32] = { 0, };

uint16_t PM1_AE;
uint16_t PM2P5_AE;
uint16_t PM10_AE;
uint16_t checksum;


void setup() {
  // put your setup code here, to run once:
  pms.begin(9600);
  Serial.begin(9600);

  /*
  for(int i = 0; i < 8; i++) {
    pms.write(init[i]);
  }
  */
}

void loop() {
  // put your main code here, to run repeatedly:
  if(pms.available() >= 32)
  {
    checksum = 0;
    
    for(int i = 0; i < 32; i++)
    {
      datas[i] = pms.read();

      if(i < 30)
      {
        checksum += datas[i];
      }
    }

    PM1_AE = (datas[10] << 8) + datas[11];
    PM2P5_AE = (datas[12] << 8) + datas[13];
    PM10_AE = (datas[14] << 8) + datas[15];

    /*
    Serial.print("Start code1 : "); Serial.println(datas[0], HEX);
    Serial.print("Start code2 : "); Serial.println(datas[1], HEX);
    Serial.print("Check High : "); Serial.println(datas[30], HEX);
    Serial.print("Check Low  : "); Serial.println(datas[31], HEX);
    Serial.print("CheckSum : ");   Serial.println(checksum, HEX);
    Serial.print("PM1_AE : "); Serial.println(PM1_AE, DEC);
    Serial.print("PM2P5_AE : "); Serial.println(PM2P5_AE, DEC);
    Serial.print("PM10_AE : "); Serial.println(PM10_AE, DEC);

    Serial.println();
    Serial.println();
    */
    
    if(checksum != (datas[30] << 8 | datas[31]))
    {
      Serial.println("checksum error!");
      Serial.print("checksum : "); Serial.println(checksum);
      Serial.print("30, 31 : "); Serial.println((datas[30] << 8 | datas[31]));
      return;
    }

    if(datas[0] != 0x42 || datas[1] != 0x4d)
    {
      Serial.println("Start code Error!");
      return;
    }

    Serial.print("PM1(μg/m3) : "); Serial.println(PM1_AE);
    Serial.print("PM2P5(μg/m3) : "); Serial.println(PM2P5_AE);
    Serial.print("PM10(μg/m3) : "); Serial.println(PM10_AE);
    Serial.println(); Serial.println();
  }
}
