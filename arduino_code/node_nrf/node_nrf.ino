#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10);
const byte address1[6] = "00001"; // reading
const byte address2[6] = "00002"; // writing
bool led=1;

void setup() 
{
pinMode(2, OUTPUT);
Serial.begin(9600);
radio.begin();             
}

void loop()
{

radio.openReadingPipe(0, address1); 
radio.setPALevel(RF24_PA_MIN);       
radio.startListening();

while(!radio.available()){
  //
}

char text[32];                 
radio.read(&text, sizeof(text));

radio.openWritingPipe(address2);
radio.setPALevel(RF24_PA_MIN); 
radio.stopListening();

radio.write(&text, sizeof(text));

digitalWrite(2,led);
led=!led;
Serial.println(text);
delay(5);
}
