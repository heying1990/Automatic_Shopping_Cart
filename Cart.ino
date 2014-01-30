#include <RFM12B.h>
#include <avr/sleep.h>

// The NodeID takes values from 1-127, 0 is reserved for sending broadcast messages (send to all nodes)
// The Network ID takes values from 0-255
// By default the SPI-SS line used is D10 on Atmega328. You can change it by calling .SetCS(pin) where pin can be {8,9,10}
#define NODEID           2  //network ID for cart
#define NETWORKID       99  //the network ID we are on
#define SERIAL_BAUD 115200

long distance_c[2];
long distance_h[2];

// Need an instance of the Radio Module
RFM12B radio;

void setup()
{
  radio.Initialize(NODEID, RF12_433MHZ, NETWORKID);
  Serial.begin(SERIAL_BAUD);
  Serial.println("Listening...");
}

void loop()
{
  if (radio.ReceiveComplete())
  {
    if (radio.CRCPass())
    {
      Serial.print('[');
      Serial.println(radio.GetSender());
      Serial.print("] ");
      for (byte i = 0; i < radio.GetDataLen(); i++){ 
        //distance_h[i] = radio.Data[i];
        Serial.println((char)radio.Data[i]);
      }
      if (radio.ACKRequested())
      {
        radio.SendACK();
        Serial.print(" - ACK sent");
      }
    }
    else
      Serial.print("BAD-CRC");
  }
}
