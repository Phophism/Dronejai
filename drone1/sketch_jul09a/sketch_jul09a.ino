
int buzzPin= 3 ; // I/O-pin from buzzer connects here 
const int wpm = 20; // Morse speed in WPM
const int dotL = 1200/wpm; // Calculated dot-length
const int dashL = 3*dotL; // Dash = 3 x dot
const int sPause = dotL; // Symbol pause = 1 dot
const int lPause = dashL; // Letter pause = 3 dots
const int wPause = 7*dotL; // Word pause = 7 dots

void setup()
{
pinMode(buzzPin,OUTPUT); // Set buzzer-pin as output
}
void loop()
{
dash();
dot();
dash();
dot();
delay(lPause-sPause); // Subtracts pause already taken

dash();
dash();
dot();
dash();
delay(wPause-sPause); // Subtracts pause already taken

}

void dot(){
digitalWrite(buzzPin, LOW); // Tone ON
delay(dotL); // Tone length
digitalWrite(buzzPin, HIGH); // Tone OFF
delay(sPause); // Symbol pause
return;
}

void dash(){
digitalWrite(buzzPin, LOW); // Tone ON
delay(dashL); // Tone length
digitalWrite(buzzPin, HIGH); // Tone OFF
delay(sPause); // Symbol pause
return;
}
