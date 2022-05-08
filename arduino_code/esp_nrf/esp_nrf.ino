#include "FirebaseESP8266.h"  // Install Firebase ESP8266 library
#include <ESP8266WiFi.h>

#define FIREBASE_HOST "cgms-dac55-default-rtdb.firebaseio.com" //Without http:// or https:// schemes
#define FIREBASE_AUTH "CrSeANPuBV3d5mrrzT8sVr8Fl9saCkcUy9ejZ7it"
#define WIFI_SSID "realme6"
#define WIFI_PASSWORD "asdfghjk"

int led = LED_BUILTIN;     // Connect LED to D5

FirebaseData firebaseData, state, code, message;

FirebaseJson json;

float h=0;

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(4, 2);

const uint64_t pipeIn = 0xE8E8F0F0E1LL; 

float val[9];

struct Received_data {
  byte ch1,ch2,ch3,ch4,ch5,ch6,ch7,ch8,ch9;
};

Received_data received_data;

unsigned long last_Time = 0;

//We create the function that will read the data each certain time
void receive_the_data()
{
  while ( radio.available() ) {
  radio.read(&received_data, sizeof(Received_data));
  last_Time = millis(); //Here we receive the data
}

  val[0] = received_data.ch1;
  val[1] = received_data.ch2;
  val[2] = received_data.ch3;
  val[3] = received_data.ch4;
  val[4] = received_data.ch5;
  val[5] = received_data.ch6;
  val[6] = received_data.ch7;
  val[7] = received_data.ch8;
  val[8] = received_data.ch9;

  Serial.println(val[0]);
}

void setup() 
{
pinMode(2, OUTPUT);
Serial.begin(9600);
radio.begin();
radio.openReadingPipe(1,pipeIn);
radio.setPALevel(RF24_PA_MIN);       
radio.startListening();   
radio.setAutoAck(false);

  pinMode(led,OUTPUT);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  //Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

String Temp = "/TrackData/temperature/";
String Hum = "/TrackData/humidity/";
String Dust = "/TrackData/dust/";
String Co2 = "/TrackData/GasData/co2/";
String o2 = "/TrackData/GasData/o2/";
String Co = "/TrackData/GasData/co/";
String Ch4 = "/TrackData/GasData/ch4/";
String illumination = "/TrackData/illumination/";


int a=0;

void loop()
{
while(!radio.available())            
{}

receive_the_data();

a+=1;
  long int t1,t2;
  t1 = millis();

  //set live values
  Firebase.setFloatAsync(firebaseData, "/live_data/temp", val[4]);
  Firebase.setFloatAsync(firebaseData, "/live_data/hum", val[6]);
  Firebase.setFloatAsync(firebaseData, "/live_data/dust", val[5]);
  Firebase.setFloatAsync(firebaseData, "/live_data/gasdata/co2", val[2]);
  Firebase.setFloatAsync(firebaseData, "/live_data/gasdata/co", val[1]);
  Firebase.setFloatAsync(firebaseData, "/live_data/gasdata/ch4", val[0]);
  Firebase.setFloatAsync(firebaseData, "/live_data/illumination", val[8]);
  Firebase.setFloatAsync(firebaseData, "/live_data/o2", val[3]);

  //track data
  Firebase.setFloatAsync(firebaseData, Temp + String(a), val[4]);
  Firebase.setFloatAsync(firebaseData, Hum + String(a), val[6]);
  Firebase.setFloatAsync(firebaseData, Dust + String(a), val[5]);
  Firebase.setFloatAsync(firebaseData, Co2 + String(a), val[2]);
  Firebase.setFloatAsync(firebaseData, Co + String(a), val[1]);
  Firebase.setFloatAsync(firebaseData, Ch4 + String(a), val[0]);
  Firebase.setFloatAsync(firebaseData, illumination + String(a), val[8]);
  Firebase.setFloatAsync(firebaseData, o2 + String(a), val[3]);

  /*//read values
  Firebase.getInt(state, "/read_data/state");
  Firebase.getInt(code, "/read_data/code");
  Firebase.getString(message, "/read_data/message");*/

  //delay of 1sec
  t2 = millis();
  if(t2-t1 < 1000)
    delay(1000-t2+t1); 
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
