#include <SPI.h>
#include <nRF24L01p.h>

nRF24L01p transmitter(7,8);//CSN,CE

void setup(){
delay(150);
Serial.begin(9600);
SPI.begin();
SPI.setBitOrder(MSBFIRST);
transmitter.channel(90); // ตั้งช่องความถี่ให้ตรงกัน
transmitter.TXaddress("GET"); // ตั้งชื่อตำแหน่งให้ตรงกัน ชื่อตั้งได้สูงสุด 5 ตัวอักษร
transmitter.init();
}

String message;

void loop(){
 transmitter.txPL("test"); // ค่าที่ต้องการส่ง
transmitter.send(FAST); // สั่งให้ส่งออกไป
delay(1000);
}
