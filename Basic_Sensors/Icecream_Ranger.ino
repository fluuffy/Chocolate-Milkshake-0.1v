#include <Wire.h>
#include <VL53L0X.h>
#include <LiquidCrystal.h>

VL53L0X sensor;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int data;

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  Wire.begin();

  sensor.init();
  sensor.setTimeout(500);
  
  /* Start continuous back-to-back mode (take readings as
   * fast as possible).  To use continuous timed mode
   * instead, provide a desired inter-measurement period in
   * ms (e.g. sensor.startContinuous(100)). */
  sensor.startContinuous();
}

void loop()
{
  Serial.print(sensor.readRangeContinuousMillimeters());
  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

  /* Dump data into 'data' */
  data = sensor.readRangeContinuousMillimeters();
  
  /* LCD Displays the VL53L0X's range */
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Milimeters");
  lcd.setCursor(0,1);
  lcd.print(data);
  
  Serial.println();
  lcd.cursor();
  
  /* So that the values are readable */
  delay(10);
}
