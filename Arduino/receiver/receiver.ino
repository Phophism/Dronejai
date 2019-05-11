#include <SPI.h>
#include <nRF24L01p.h>
nRF24L01p transmitter(7,8);//CSN,CE

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,16,2);

void setup(){
delay(150);
Serial.begin(115200);
SPI.begin();
SPI.setBitOrder(MSBFIRST);
transmitter.channel(90); // ตั้งช่องความถี่ให้ตรงกัน
transmitter.TXaddress("TWO"); // ตั้งชื่อตำแหน่งให้ตรงกัน ชื่อตั้งได้สูงสุด 5 ตัวอักษร
transmitter.RXaddress("ONE");
transmitter.init();
}

char  message[32];
String receiveData;
int len;

void loop(){

  if(len=Serial.available()>0){
    Serial.readBytes(message,len);
    String str(message);
    transmitter.txPL("Hee");
    transmitter.send(FAST); // สั่งให้ส่งออกไป
  }

  if(transmitter.available()){
    transmitter.read();
    transmitter.txPL(receiveData);
    lcd.setCursor(0,0);
    lcd.print(receiveData);
    receiveData="";
  }
}
