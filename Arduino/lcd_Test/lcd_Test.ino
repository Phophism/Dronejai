#include <Wire.h> 

#include <LiquidCrystal_I2C.h> //ประกาศ Library ของจอ I2C
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3F, 16, 2);
void setup()
{
// initialize the LCD
lcd.begin();
// Print a message to the LCD.
lcd.print("Hello !!!"); //ฟังก์ชั่นในการกำหนดข้อความที่ต้องการแสดงผล
lcd.setCursor(0, 1); //ฟังก์ชั่นในการกำหนดตำแหน่ง Cursor
lcd.print("ThaiEasyElec");
}
void loop() 
{
}
