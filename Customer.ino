#include <RFM12B.h>
#include <avr/sleep.h>

// Initialize the radio by telling it what ID it has and what network it's on
// The NodeID takes values from 1-127, 0 is reserved for sending broadcast messages (send to all nodes)
// The Network ID takes values from 0-255
// By default the SPI-SS line used is D10 on Atmega328. You can change it by calling .SetCS(pin) where pin can be {8,9,10}
#define NODEID        1  // network ID for customer
#define NETWORKID    99  // the network ID we are on
#define GATEWAYID_c   2  // the node ID we're sending to and receiving from: cart
#define GATEWAYID_l   3  // the node ID we're sending to: left store transceiver
#define GATEWAYID_r   4  // the node ID we're sending to: right store transceiver
#define ACK_TIME     50 // wait 50 ms for ACK signal to come back
#define SERIAL_BAUD  115200
#define LED 13 // LED pin

int interPacketDelay = 1000; // wait for 1 s before another position update
char input = 0;
long delay_c = 100;
long delay_l = 100;
long delay_r = 100;

// Need an instance of the Radio Module
RFM12B radio;
char signal[] = "A";  // the cart sends "A" to store transceivers
bool requestACK = false;

void setup()
{
  Serial.begin(SERIAL_BAUD);
  pinMode(LED, OUTPUT);
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
      Serial.print(interPacketDelay);
      Serial.println("ms\n");
    }
  }

  Serial.print("Sending:");
  Serial.print((char)signal[0]); // print out the signal sent
  
  requestACK = true; // request ACK for each transmission
  
  radio.Wakeup();
  // talk to left store transceiver
  radio.Send(GATEWAYID_l, signal, 1, requestACK); 
  long now_l = millis(); // record the moment when the signal is sent
  
  Serial.print(" - waiting for ACK from left...");

  while (millis() - now_l <= ACK_TIME){ // still wait for ack to return
    if (radio.ACKReceived(GATEWAYID_l)){
      delay_l= millis() - now_l;
      break; // jump out of the loop as soon as ACK is received
    }
  }
  
  // talk to right store transceiver  
  radio.Send(GATEWAYID_r, signal, 1, requestACK); 
  long now_r = millis(); // record the moment when the signal is sent
  
  Serial.print(" - waiting for ACK from right...");

  while (millis() - now_r <= ACK_TIME){ // still wait for ack to return
    if (radio.ACKReceived(GATEWAYID_r)){
      delay_r= millis() - now_r;
      break; // jump out of the loop as soon as ACK is received
    }
  }
  if(delay_l <= ACK_TIME && delay_r <= ACK_TIME){  
    Serial.println("Distance from left: ");
    Serial.println(delay_l * 3e5); // wireless wave speed = speed of light: 3e5 m/ms
    Serial.println("m\n");
    Serial.println("Delay from right: ");
    Serial.println(delay_r * 3e5);
    Serial.println("m");
  }
  else
    Serial.println("Transmission failed.");
  
  // send both distances to cart
  long distance_h[] = {delay_l/2 * 3e5, delay_r/2 * 3e5}; 
  
  radio.Send(GATEWAYID_c, distance_h, 2, requestACK); 
  long now_c = millis();
  Serial.print(" - waiting for ACK from cart...");
  
  // calculate distance between customer and cart
  while (millis() - now_c <= ACK_TIME){ // still wait for ack to return
    if (radio.ACKReceived(GATEWAYID_c)){
      delay_c = millis() - now_c;
      break; // jump out of the loop as soon as ACK is received
    }
  }
  
  long distance_ch = delay_c/2 * 3e5;
  
  if(distance_ch > 3)
    digitalWrite(LED, HIGH); // the cart falls more than 3 m behind, blink red LED
  else
    digitalWrite(LED, LOW); 
    
  radio.Sleep();
  
  delay(interPacketDelay);
}
