#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <dht.h>

dht DHT;
RF24 radio(9, 10); 
const uint64_t my_radio_pipe = 0xE8E8F0F0E1LL;  

float val[9] = {123.11, 0.12, 1230, 21, 34.50, 101.15, 57.10, 0.01, 18.3};
float density= 0.14;
byte n[] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x2E};

#define        COV_RATIO                       0.2            //ug/mmm / mv
#define        NO_DUST_VOLTAGE                 400            //mv
#define        SYS_VOLTAGE                     5000   

struct Data_to_be_sent {
  byte ch1,ch2,ch3,ch4,ch5,ch6,ch7,ch8,ch9; 
};

Data_to_be_sent sent_data;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*pins- A0 dust sensor
 */

int Filter(int m)
{
  static int flag_first = 0, _buff[10], sum;
  const int _buff_max = 10;
  int i;
  
  if(flag_first == 0)
  {
    flag_first = 1;
    for(i = 0, sum = 0; i < _buff_max; i++)
    {
      _buff[i] = m;
      sum += _buff[i];
    }
    return m;
  }
  else
  {
    sum -= _buff[0];
    for(i = 0; i < (_buff_max - 1); i++)
    {
      _buff[i] = _buff[i + 1];
    }
    _buff[9] = m;
    sum += _buff[9];
    
    i = sum / 10.0;
    return i;
  }
}

void command(int val,byte addr){
  int a=0,b=0,c=0,d=0,e=0;
  byte cmd;

  int num =val,count=0;
  while(num/10){
    num/=10;
    count++;
  }
  count+=1*(val!=0);

  a = val/10000;
  b = (val/1000)%10;
  c = (val/100)%10;
  d = (val/10)%10;
  e = val%10;

  Serial.print(a);
  Serial.print(b);
  Serial.print(c);
  Serial.print(d);
  Serial.println(e);

  if(count==6){
    byte cmd[] = {0x5A, 0xA5, 0x05, 0x82, 0x00, addr, n[a], n[b],
                0x5A, 0xA5, 0x05, 0x82, 0x00, addr+1, n[c], n[d],
                0x5A, 0xA5, 0x05, 0x82, 0x00, addr+2, n[10], n[e]};
    Serial.write(cmd,sizeof(cmd));       
  }

  if(count==5){
    byte cmd[] = {0x5A, 0xA5, 0x05, 0x82, 0x00, addr, n[a], n[b],
                0x5A, 0xA5, 0x05, 0x82, 0x00, addr+1, n[c], n[10],
                0x5A, 0xA5, 0x05, 0x82, 0x00, addr+2, n[d], n[e]};
    Serial.write(cmd,sizeof(cmd));       
  }
  else if(count==4){
    byte cmd[] = {0x5A, 0xA5, 0x05, 0x82, 0x00, addr, n[b], n[c],
                0x5A, 0xA5, 0x05, 0x82, 0x00, addr+1, n[10], n[d],
                0x5A, 0xA5, 0x05, 0x82, 0x00, addr+2, n[e]};
    Serial.write(cmd,sizeof(cmd));
  }
  else if(count==3){
    byte cmd[] = {0x5A, 0xA5, 0x05, 0x82, 0x00, addr, n[c], n[10],
                0x5A, 0xA5, 0x05, 0x82, 0x00, addr+1, n[d], n[e]};
    Serial.write(cmd,sizeof(cmd));
  }
  else if(count<=2){
    byte cmd[] = {0x5A, 0xA5, 0x05, 0x82, 0x00, addr, n[0], n[10],
                0x5A, 0xA5, 0x05, 0x82, 0x00, addr+1, n[d], n[e]};
    Serial.write(cmd,sizeof(cmd));
  }
}

void display_control(){
  val[0] = analogRead(A3);
  val[1] = analogRead(A2);
  val[2] = analogRead(A2);
  val[5] = density;
  val[8] = analogRead(A1);

  command(val[0]*100,0x00);   //ch4
  command(val[1]/10,0x10);    //co
  command(val[2]*100,0x20);   //co2
  command(val[3]*100,0x30);   //o2
  command(val[4]*100,0x40);   //temperature
  command(val[5]*100,0x50);   //dust
  command(val[6]*100,0x60);   //humidity
  command(val[7]*100,0x70);   //air velocity                
  command(val[8]*100,0x80);   //illumination
}

void dust(){
  const int iled = 7;                                            //drive the led of sensor
  const int vout = 0; 
  float voltage;
  int   adcvalue;

  int val[5];
  digitalWrite(iled, HIGH);
  delayMicroseconds(280);
  adcvalue = analogRead(vout);
  digitalWrite(iled, LOW);
  
  adcvalue = Filter(adcvalue);

  /*
  covert voltage (mv)
  */
  voltage = (SYS_VOLTAGE / 1024.0) * adcvalue * 11;

  /*
  voltage to density
  */
  if(voltage >= NO_DUST_VOLTAGE)
  {
    voltage -= NO_DUST_VOLTAGE;
    
    density = voltage * COV_RATIO;
  }
  else
    density = 0;
}

void Temp(){
    DHT.read11(A0);
    val[5] = DHT.humidity;
    val[6] = DHT.temperature;
}

void send_data(){
  sent_data.ch1 = val[0];
  sent_data.ch2 = val[1];
  sent_data.ch3 = val[2];
  sent_data.ch4 = val[3];
  sent_data.ch5 = val[4];
  sent_data.ch6 = val[5];
  sent_data.ch7 = val[6];
  sent_data.ch8 = val[7];
  sent_data.ch9 = val[8];

  radio.write(&sent_data, sizeof(Data_to_be_sent));  
}

void alert(){
  
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void setup() 
{
pinMode(2,OUTPUT);
pinMode(3,OUTPUT);
pinMode(4,OUTPUT);
pinMode(5,OUTPUT);
  
Serial.begin(115200);
radio.begin();                 
radio.openWritingPipe(my_radio_pipe);
radio.setPALevel(RF24_PA_MIN); 
radio.stopListening(); 
radio.setAutoAck(false);
}

int count=0;
void loop()
{
alert();
Temp();
dust();
if(count==5){
  display_control();
  count=0;
}
count++;
  
send_data();

delay(100);

}
