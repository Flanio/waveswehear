#include <Wire.h>

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,20,4); // set the LCD address to 0x27 for a 20 chars and 4 line display

void setup()

{

lcd.init(); 

lcd.backlight();

lcd.print("Hello, world!");

}

void loop(){}

