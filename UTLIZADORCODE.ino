#include <Arduino.h>
#include "VSync.h"

#include "rf69_module.h"
#include "TimerOne.h"

void callback(void);

ValueSender<1> sender;
ValueReceiver<2> receiver;

const int ledPin =  13;
int ledState = LOW;

uint8_t cont = 0;

uint8_t volatile canSend = 0;

uint8_t rcv = 65;
uint8_t ackn;

uint8_t ack_received = 0;
uint8_t velocidade =20;
uint8_t sentido = 1;

uint8_t velc = 0;
uint8_t carstate = 0;

int vel_ard = velc;
int sent_pro = sentido;
int vel_pro = velocidade;

volatile uint8_t timeout = 0;

long tt=0;
uint8_t l;
int numb_packet = 0;

void setup() {

pinMode(ledPin, OUTPUT);
Serial.begin(19200);

 while(!Serial);

 configSPI();
 configModule();
 
 if(checkModule() == 1)
  velc = 0;

  Timer1.initialize(333333);
  Timer1.attachInterrupt(callback);


   sender.observe(vel_ard);


   receiver.observe(vel_pro);
  receiver.observe(sent_pro);
  
}

void loop(){

if(canSend == 1)
  {
    canSend= 0;
    if(ack_received == 0)
      cont++;
     else
     {
        cont = 0;
       ack_received = 0;
     }
  

    if(cont > 5)
      //Serial.println("LOST CONNECTION");

    receiver.sync();

    velocidade = (uint8_t)vel_pro;
    sentido = (uint8_t) sent_pro;
   // Serial.println("A ENVIAR: ");
   // Serial.print("velocidade % : "); Serial.print(velocidade); Serial.print(" Sentido : "); Serial.println(sentido);
    sendMessage(velocidade,sentido);
   
    waiToReceive();

 }

while(canSend == 0 && ack_received == 0)
{
  if(receiveDone() == 1)
  {
    readMessage(velc,carstate);
    //Serial.println("recebi");
   // Serial.print("velocidade % : "); Serial.print(velc); Serial.print(" Estado : "); Serial.print(carstate); Serial.print("sentido: "); Serial.println( sentido);
    ack_received == 1;
    vel_ard=velc;
    sender.sync();
  }
}

   if(millis() - tt >= 100000)
   {
    if(sentido == 1)
      sentido = 0;
    else
      sentido = 1;

      tt = millis();
   }
   
}
/*
void serialEvent() {
 
    uint8_t in = Serial.parseInt();

    if(in <= 100 && in>= 0)
      velocidade = in;
    
  }
/*

while(smph)
{
while(sendwStopWait(velc, seq_FRAME)==0);

readFifo(&velocidade , &ack_received);
Serial.print("Recebi: ");
Serial.print("Velocidade: ");
Serial.print(velocidade);
Serial.print("ack: ");
Serial.println(ack_received);

switch (ack_received) {
  case ACK0:
    if(seq_FRAME == FRAME1){
      seq_FRAME = FRAME0;
      Serial.println("ack");
      smph= 0;
    }
    else if(seq_FRAME == FRAME0)
    {
      Serial.print("ack");
    }
    else{
      Serial.print("NOT EVEN A ACKNOWLEDGE");
    }
    break;
  case ACK1:
    if(seq_FRAME == FRAME0)
    {
      Serial.print("ACK");
      seq_FRAME = FRAME1;
      smph = 0;
    }
    else if(seq_FRAME == FRAME1){
      Serial.print("NOT ACKNOWLEDGE");
    }
    else{
      Serial.print("NOT ACKNOWLEDGE)");
    }
    break;
  case NACK:
    Serial.println("NOT ACKNOWLEDGE");
    break;
}

}
Serial.println(velc);
smph = 1;
rcv++;
*/


void callback(void)
{
  canSend = 1;
}

/*
void serialEvent() {
  Serial.println("HERE");
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    Serial.print(inChar);
    }
  }
*/
