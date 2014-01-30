#include <RFM12B.h>
#include <avr/sleep.h>

// Initialize the radio by telling it what ID it has and what network it's on
// The NodeID takes values from 1-127, 0 is reserved for sending broadcast messages (send to all nodes)
// The Network ID takes values from 0-255
// By default the SPI-SS line used is D10 on Atmega328. You can change it by calling .SetCS(pin) where pin can be {8,9,10}
#define NODEID        1 // network ID for customer
#define NETWORKID    99  // the network ID we are on
#define GATEWAYID_c   2  // the node ID we're sending to: cart
#define ACK_TIME     50000 // wait 50 ms for ACK signal to come back
#define SERIAL_BAUD  115200
//#define echoPin       2

long duration = 0;
long delay_c = 0;
long sum = 0;
int count = 0;
//long distance_h[2];'; // distance[0] = distance_l, distance[1] = distance_r 

// Need an instance of the Radio Module
RFM12B radio;
char signal[] = "B";  // the cart sends "B" to store transceivers
bool requestACK = true;
long input;
long interPacketDelay;

void setup()
{
  Serial.begin(SERIAL_BAUD);
  //pinMode(echoPin, INPUT);
  radio.Initialize(NODEID, RF12_433MHZ, NETWORKID);
  radio.Sleep(); // sleep right away to save power
  Serial.println("Transmitting...\n\n");
}

void loop()
{
  //serial input of [0-9] will change the transmit delay between 100-1000ms
  if (Serial.available() > 0) {
    input = Serial.read();
    if (input >= 48 && input <= 57) //[1..9] = {100..900}ms; [0]=1000ms
    {
      interPacketDelay = 100 * (input-48);
      if (interPacketDelay == 0) interPacketDelay = 1000;
      Serial.print("\nChanging delay to ");
      Serial.println(interPacketDelay);
      Serial.print("ms\n");
    }
  }
  Serial.println("");
  Serial.print("Sending:");
  Serial.println((char)signal[0]); // print out the signal sent
  
  radio.Wakeup();
  // talk to cart
  
  radio.Send(GATEWAYID_c, signal, 1, requestACK);
  long now = micros(); 
  
  while (micros() - now <= ACK_TIME){ // still wait for ack to return
     Serial.print("Loop?");
     Serial.println(micros()-now);
     
    if (radio.ACKReceived(GATEWAYID_c)){
      delay_c = micros() - now;
      Serial.println(delay_c);
      break; // jump out of the loop as soon as ACK is received
    }
  }
   sum = sum + delay_c;
   count ++;
  
  if (count >= 100){
    long ave = sum / count;
    Serial.println(ave);
  } 
   
  if (delay_c < ACK_TIME && delay_c > 0)
  {
    Serial.print("Transmission succeeded.");
    Serial.println(delay_c);
  }
  else
    Serial.print("Transmission failed.");
    
  radio.Sleep();
  
  delay(1000);
}
