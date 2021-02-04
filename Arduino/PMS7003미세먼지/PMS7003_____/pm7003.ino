#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <PMS.h>

#define HEAD1 0x42
#define HEAD2 0x4d

SoftwareSerial pms_serial(2, 3);
LiquidCrystal_I2C lcd(0x27, 16, 2);

char str[10];

PMS pms(pms_serial);
PMS::DATA data;

void setup() {
  Serial.begin(9600);
  pms_serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.print("turn on...");
  delay(3000);

  lcd.clear();
}

void loop() {
  if (pms.read(data))
  {
    Serial.print("PM1(μg/m3) : "); Serial.println(data.PM_AE_UG_1_0);
    
    sprintf(str, "PM2.5 : %d", data.PM_AE_UG_2_5);
    lcd.clear(); lcd.setCursor(0, 0);
    lcd.print(str);
    Serial.print("PM2.5(μg/m3) : "); Serial.println(data.PM_AE_UG_2_5);
    
    sprintf(str, "PM10  : %d", data.PM_AE_UG_10_0);
    lcd.setCursor(0, 1);
    lcd.print(str);
    Serial.print("PM10(μg/m3) : "); Serial.println(data.PM_AE_UG_10_0);
    
    Serial.println(); Serial.println();
  }

//    Serial1.print("PM 1.0 (ug/m3): ");
//    Serial1.println(data.PM_AE_UG_1_0);
//
//    Serial1.print("PM 2.5 (ug/m3): ");
//    Serial1.println(data.PM_AE_UG_2_5);
//
//    Serial1.print("PM 10.0 (ug/m3): ");
//    Serial1.println(data.PM_AE_UG_10_0);
//
//    Serial1.println();
}
