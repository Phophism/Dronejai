#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3F, 16, 2);
 
void setup()
{
// initialize the LCD
lcd.begin();
// Turn on the blacklight and print a message.
lcd.backlight();
lcd.print("Hello, world!");
lcd.setCursor(0, 1);
lcd.print("Arduitronics.com");        
}
void loop()
{
// Do nothing here...
}
