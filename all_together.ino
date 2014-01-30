#include <RFM12B.h>
#include <avr/sleep.h>

#define trigPin_l 14
#define trigPin_f 9
#define trigPin_r 4
#define echoPin_l 7 //A0 used as digital input 
#define echoPin_f 8
#define echoPin_r 3
#define transistorPin_l 5
#define transistorPin_r 6
#define readPin 3 // read from atmega328 through anaglog pin 3 
#define sendreadyPin 15 //A1 used as digital input, connected to atmega

#define NODEID        1 // network ID for customer
#define NETWORKID    99  // the network ID we are on
#define cartID   2  // the node ID we're sending to: human
#define ACK_TIME     50 // wait 50 ms for ACK signal to come back
#define SERIAL_BAUD  9600

int t_07 = 1300; // time needed to cover 0.7 m in x-direction
int t_90deg_left = 1130; //time needed to turn 90 degree when walking
int t_90deg_right = 970; //time needed to turn 90 degree when rest
int t_180 = 2020;
int t_21 = 4050;
int t_14 = 3050;
int t_sensor = 200;
float v_straight = 0.5; //meter per sec
int current_head;

int alarm_f = 0, alarm_l = 0, alarm_r = 0; // 0 if no objects within 35 - 45 cm
float cart_x = 1.3; // store the cart position
float cart_y = 0.5;
int cart_head = 0;
long duration_f, distance_f;
long duration_l, distance_l;
long duration_r, distance_r;
int readytogo = 0; // set to 1 after collecting all store information

// wait for user input when starting up
int sum = 0;
int ave = 0;
int count = 0; // count the numver of items selected
int i = 0;
int j = 0;
int store[10]; // store the store number
float store_location[10][2]; // store location
//char store_name[10]; // store name

// determine optimal path
int sort = 0; // store list is unsorted, set to 1 if sorted
int goal;
int nextstore = 0;
float next_x;
float next_y;

RFM12B radio;
char alarm[1]; // set to 'Y' if front sensor detects something
bool requestACK = true;

void setup() {
   Serial.begin (9600);
   pinMode(trigPin_f, OUTPUT);
   pinMode(trigPin_l, OUTPUT);
   pinMode(trigPin_r, OUTPUT);
   pinMode(echoPin_f, INPUT);
   pinMode(echoPin_l, INPUT);
   pinMode(echoPin_r, INPUT);
   pinMode(19, OUTPUT); // front sensor LED indicator
   
   pinMode(sendreadyPin, INPUT);
   pinMode(transistorPin_l, OUTPUT);
   pinMode(transistorPin_r, OUTPUT);
   
   pinMode(readPin,INPUT);
   
   radio.Initialize(NODEID, RF12_433MHZ, NETWORKID);
   radio.Sleep(); // sleep right away to save power
   // Serial.println("Transmitting...\n\n");
 }
 
void loop() {
  if(digitalRead(sendreadyPin) == 1){
    delay(500);
  while(i < 10)
  {
    delay(500);
    while(j < 10){ 
      delay(25);
      int val = analogRead(readPin);
      sum = sum + val;
      j ++;
      //Serial.print("value =");
      //Serial.println(val);
    }
    ave = sum / 10; // average out to get user input
    //Serial.print("average =");
    //Serial.println(ave);
    //Serial.print("");

    
    if(ave < 70){
      i --; // no coming signal
    }
    if(ave < 70 && count != 0){
      break; // no coming signal
    }
    if(70 < ave && ave < 160){
      store[count] = 0;
      store_location[count][0] = 1.3;
      store_location[count][1] = 0.5;
      //store_name[count] = "Grocery";
      Serial.print("grocery");
      count ++;
    }
    if(160 < ave && ave < 250){
      store[count] = 1;
      store_location[count][0] = 2.7;
      store_location[count][1] = 0.5;
      //store_name[count] = "Kids";
      Serial.print("kids");
      Serial.print("location:");
      Serial.println(store_location[count][0]);
      Serial.println(store_location[count][1]);
      count ++;
    } 
    if(250 < ave && ave < 340){
      store[count] = 2;
      store_location[count][0] = 2.7;
      store_location[count][1] = 1.9;
      //store_name[count] = "Books";
      Serial.print("books");
      count ++;
    }
    if(340 < ave && ave < 430){
      store[count] = 3;
      store_location[count][0] = 1.3;
      store_location[count][1] = 1.9;
      //store_name[count] = "Office";
      Serial.print("office");
      count ++;
    }  
    if(430 < ave && ave < 520){
      store[count] = 4;
      store_location[count][0] = 1.3;
      store_location[count][1] = 3.3;
     // store_name[count] = "Health";
      Serial.print("Health");
      count ++;
    }   
    if(520 < ave && ave < 610){
      store[count] = 5;
      store_location[count][0] = 2.7;
      store_location[count][1] = 3.3;
      //store_name[count] = "Beauty";
      Serial.print("beauty");
      count ++;
    }  
    if(610 < ave && ave < 700){
      store[count] = 6;
      store_location[count][0] = 2.7;
      store_location[count][1] = 4.7;
      //store_name[count] = "Sports";
      Serial.print("sports");
      count ++;
    }   
    if(700 < ave && ave < 790){
      store[count] = 7;
      store_location[count][0] = 1.3;
      store_location[count][1] = 4.7;
      //store_name[count] = "Electronics"; 
      Serial.print("electronics");
      count ++;
    }
    if(790 < ave && ave < 880){
      store[count] = 8; 
      store_location[count][0] = 2.7;
      store_location[count][1] = 6.1;
      //store_name[count] = "Apparel"; 
      Serial.print("apparel");
      count ++;
    }
    if(880 < ave && ave < 970){
      store[count] = 9;
      store_location[count][0] = 1.3;
      store_location[count][1] = 6.1;
      //store_name[count] = "Home"; 
      Serial.print("home");
      count ++;  
    }
   
    i ++; // increase the index
    delay(250);
    sum = 0;
    j = 0;
  }
  
  delay(1000);
    //Serial.print("stores we chose:\n");
    //Serial.print("count:");
    //Serial.println(count);
   // for (int a =0; a < count; a++){
     // Serial.println(store[a]);
      //Serial.print("");
   // }
   // determine optimal path  
   if (sort == 0){
     for(int k = 0; k < count; k ++){
        for(int h = 0; h < count - k -1; h++){
           if(store[h] > store[h+1]){
               int tmp=store[h];
               store[h]=store[h+1];
               store[h+1]=tmp;
               //flip x
               float flip_x = store_location[h][0];
               store_location[h][0] = store_location[h+1][0];
               store_location[h+1][0] = flip_x;
               //flip y
               float flip_y = store_location[h][1];
               store_location[h][1] = store_location[h+1][1];
               store_location[h+1][1] = flip_y;
            }
        }
     }
   }

   
   /*Serial.print("sorted stores:");
   for (int b =0; b < count; b++){
      Serial.println(store[b]);
      Serial.print("x_axis:");
      Serial.println(store_location[b][0]);
      Serial.print("y_axis:");
      Serial.println(store_location[b][1]);
    } */
   delay(1000);
   sort = 1; // finish sorting the list
   readytogo =1;
  }

if(readytogo == 1){
   goal = store[nextstore]; // find the next destination
   
   if (nextstore < count){
     next_x = store_location[nextstore][0]; // get the next store location
     next_y = store_location[nextstore][1];
     nextstore++;
     //test
     Serial.print("next loaction_x:");
     Serial.println(next_x,2);
     Serial.print("next loaction_y:");
     Serial.println(next_y,2);
     delay(1000);
   }
   else {
     next_x = 1.3; // return to starting spot
     next_y = 0.5;
   }
     
   // set out to the destination!!!
   float delta_x = next_x - cart_x; // calcualate the length needs to go
   float delta_y = next_y - cart_y;

   // U_turn necessary? 
   if ((cart_head == 180 && (cart_y == 0.5 || cart_y == 3.3 || cart_y == 6.1)) || (cart_head == 0 && (cart_y == 1.9 || cart_y == 4.7))){
     // stop to get ready for U_turn

     // front sensor needed
     digitalWrite(trigPin_f, LOW);
     delayMicroseconds(2);
     digitalWrite(trigPin_f, HIGH);
     delayMicroseconds(10); // Added this line
     digitalWrite(trigPin_f, LOW);
  
     duration_f = pulseIn(echoPin_f, HIGH);
     distance_f = (duration_f/2) / 29.1;
     Serial.print("\ndistance_f: ");
     Serial.println(distance_f);
     
     while(distance_f < 20 && distance_f > 0){
       digitalWrite(19, HIGH);
       analogWrite(transistorPin_l, 0);
       analogWrite(transistorPin_r, 0);
       delay(150);
       
       //talk to cautomer
       radio.Wakeup();
  
       radio.Send(cartID, alarm, 1, requestACK);
       delay(50); // wait for receive send back ACK
       
       if (radio.ACKReceived(cartID)){
         Serial.print("successful");
       }
    
       radio.Sleep();
  
       delay(500);
     
       digitalWrite(trigPin_f, LOW);
       delayMicroseconds(2);
       digitalWrite(trigPin_f, HIGH);
       delayMicroseconds(10); // Added this line
       digitalWrite(trigPin_f, LOW);

       duration_f = pulseIn(echoPin_f, HIGH);
       distance_f = (duration_f/2) / 29.1; //update distance front 
       Serial.print("new front distance:");
       Serial.println(distance_f);
     } 
       digitalWrite(19, LOW);
       analogWrite(transistorPin_r,0);
       analogWrite(transistorPin_l,0);
       delay(300);
     
     // U_turn
     analogWrite(transistorPin_r,0);
     analogWrite(transistorPin_l,169);
     delay(t_180);
     analogWrite(transistorPin_r,0);
     analogWrite(transistorPin_l,0);
     delay(100);
     cart_head = 180 - cart_head; // update the cart head angle
     
     // right sensor needed
     digitalWrite(trigPin_r, LOW); 
     delayMicroseconds(2); // stay low for 2ms
     digitalWrite(trigPin_r, HIGH);
     delayMicroseconds(10); // stay high for 10 ms
     digitalWrite(trigPin_r, LOW);
     duration_r = pulseIn(echoPin_r, HIGH);
     distance_r = (duration_r/2) / 29.1;
     
     if(distance_r < 20 && distance_r > 0){
       analogWrite(transistorPin_l,0);
       analogWrite(transistorPin_r,195);
       delay(t_sensor);
       Serial.print("direction modified!");
     }    
   }

   //return to the origin
   if(delta_y < 0){
     if(cart_head == 0){
       analogWrite(transistorPin_r,0);
       analogWrite(transistorPin_l,169);
       delay(t_180); // turn 180 degrees
       analogWrite(transistorPin_r,0);
       analogWrite(transistorPin_l,0);
       delay(300);
       
       //right sensor needed
       digitalWrite(trigPin_r, LOW); 
       delayMicroseconds(2); // stay low for 2ms
       digitalWrite(trigPin_r, HIGH);
       delayMicroseconds(10); // stay high for 10 ms
       digitalWrite(trigPin_r, LOW);
       duration_r = pulseIn(echoPin_r, HIGH);
       distance_r = (duration_r/2) / 29.1;
     
     if(distance_r < 20 && distance_r > 0){
       analogWrite(transistorPin_l,0);
       analogWrite(transistorPin_r,195);
       delay(t_sensor);
       Serial.print("direction modified!");
     }
       cart_head = 180;
     }
       analogWrite(transistorPin_r,195);
       analogWrite(transistorPin_l,169);
       delay(t_07 * 0.9);
       //turn left 90 degree
       analogWrite(transistorPin_r,195);
       analogWrite(transistorPin_l,0);
       delay(t_90deg_left);
       analogWrite(transistorPin_r,0);
       analogWrite(transistorPin_l,0);
       delay(300);
       analogWrite(transistorPin_r,195);
       analogWrite(transistorPin_l,169);
       delay((float)(-(delta_y+0.15)/v_straight)*1000);
       
       if(delta_x ==0){
       //turn left by 90 degree
         analogWrite(transistorPin_r,195);
         analogWrite(transistorPin_l,0);
         delay(t_90deg_left);
         analogWrite(transistorPin_r,195);
         analogWrite(transistorPin_l,169);
         delay(t_07);
         analogWrite(transistorPin_r,0);
         analogWrite(transistorPin_l,0);
         delay(300);
       }
       //turn right by 90 degree
       else if(delta_x < 0){
         analogWrite(transistorPin_r,0);
         analogWrite(transistorPin_l,169);
         delay(t_90deg_right);
         analogWrite(transistorPin_r,195);
         analogWrite(transistorPin_l,169);
         delay(t_07);
         analogWrite(transistorPin_r,0);
         analogWrite(transistorPin_l,0);
         delay(300);
     }
     readytogo = 0;  // stop
   }
   
   // go in the x direction for 1.4 m if the destination is in the same lane
   
   else if(delta_y == 0){
     // return from 1 to 0
     if(cart_x == 2.7 && cart_y == 0.5){
     
     // U_turn
       analogWrite(transistorPin_r,0);
       analogWrite(transistorPin_l,169);
       delay(t_180);
       analogWrite(transistorPin_r,0);
       analogWrite(transistorPin_l,0);
       delay(100);
       cart_head = 180 - cart_head;
       //go for 1.4m
       analogWrite(transistorPin_r,195);
       analogWrite(transistorPin_l,169);
       delay(t_14);
       readytogo = 0;  // stop
   }
   //go straight for 1.4m
    else{
     //Serial.print("in loop?");
     analogWrite(transistorPin_r,195);
     analogWrite(transistorPin_l,169);
     delay(t_14);
     //Serial.print("done");
    }
     
     digitalWrite(trigPin_r, LOW); 
     delayMicroseconds(2); // stay low for 2ms
     digitalWrite(trigPin_r, HIGH);
     delayMicroseconds(10); // stay high for 10 ms
     digitalWrite(trigPin_r, LOW);
     duration_r = pulseIn(echoPin_r, HIGH);
     distance_r = (duration_r/2) / 29.1;
     
     if(distance_r < 20 && distance_r > 0){
       analogWrite(transistorPin_l,0);
       analogWrite(transistorPin_r,195);
       delay(t_sensor);
     }
   } // delta_y == 0
   
   // the cart needs to move into another lane
   else if (delta_y > 0){
     
     //in x direction 0.7m
     analogWrite(transistorPin_r,195);
     analogWrite(transistorPin_l,169);
     delay(t_07);
     
     
     // turn 90 defrees left 
     if(cart_y == 0.5 || cart_y == 3.3 || cart_y == 6.1){
       analogWrite(transistorPin_r,195);
       analogWrite(transistorPin_l,0);
       delay(t_90deg_left);
       analogWrite(transistorPin_r,0);
       analogWrite(transistorPin_l,0);
       delay(300);
     }
     
     // turn 90 degrees right
     else if(cart_y == 1.9 || cart_y == 4.7){
       analogWrite(transistorPin_r,0);
       analogWrite(transistorPin_l,169);
       delay(t_90deg_right);
       analogWrite(transistorPin_r,0);
       analogWrite(transistorPin_l,0);
       delay(300);
       
     } 
     // move in y direction
     
     analogWrite(transistorPin_r,195);
     analogWrite(transistorPin_l,169);
     delay(t_07);
     
     analogWrite(transistorPin_r,195);
     analogWrite(transistorPin_l,169);
     delay((float)((delta_y) / v_straight) * 1000 - t_07);
     Serial.print("time measured:");
     Serial.println(delta_y / v_straight * 1000);
     Serial.print("delta x:");
     Serial.println(delta_x);
     Serial.print("cart_head:");
     Serial.println(cart_head);
     
     //move in remaining x direction
     if(delta_x == 0 && cart_head == 0){
       analogWrite(transistorPin_r,195);
       analogWrite(transistorPin_l,0);
       delay(t_90deg_left);
       analogWrite(transistorPin_r,0);
       analogWrite(transistorPin_l,0);
       delay(300);
       current_head = 180;
     }
     if(delta_x > 0 && cart_head == 0){
       Serial.print("turn right?");
       analogWrite(transistorPin_r,0);
       analogWrite(transistorPin_l,169);
       delay(t_90deg_right);
       analogWrite(transistorPin_r,0);
       analogWrite(transistorPin_l,0);
       delay(300);
       current_head = 0;
     }
     if(delta_x == 0 && cart_head == 180){
       analogWrite(transistorPin_r,0);
       analogWrite(transistorPin_l,169);
       delay(t_90deg_right);
       analogWrite(transistorPin_r,0);
       analogWrite(transistorPin_l,0);
       delay(300);
       current_head = 0;
     }
     if(delta_x > 0 && cart_head == 180){
       analogWrite(transistorPin_r,0);
       analogWrite(transistorPin_l,169);
       delay(t_90deg_right);
       analogWrite(transistorPin_r,0);
       analogWrite(transistorPin_l,0);
       delay(300);
       current_head = 180;
     }//head to the destination
     if(delta_x == 0){
       
       analogWrite(transistorPin_r,195);
       analogWrite(transistorPin_l,169);
       delay(t_07);
       
     }
     else if((cart_head == 0 && delta_x > 0) || (cart_head == 180 && delta_x < 0)){
       
       analogWrite(transistorPin_r,195);
       analogWrite(transistorPin_l,169);
       delay(t_07);
       
       }
     else if((cart_head == 0 && delta_x < 0) || (cart_head == 180 && delta_x > 0)){
       
       analogWrite(transistorPin_r,195);
       analogWrite(transistorPin_l,169);
       delay(t_21);
     } 
       
     }//delta_y <0
    cart_head = current_head;
    cart_x = next_x; // update current cart location
    cart_y = next_y;
    
    //sensor needed
    // front sensor needed
     digitalWrite(trigPin_f, LOW);
     delayMicroseconds(2);
     digitalWrite(trigPin_f, HIGH);
     delayMicroseconds(10); // Added this line
     digitalWrite(trigPin_f, LOW);
  
     duration_f = pulseIn(echoPin_f, HIGH);
     distance_f = (duration_f/2) / 29.1;
     Serial.print("\ndistance_f: ");
     Serial.println(distance_f);
     
     while(distance_f < 20 && distance_f > 0){
       digitalWrite(19, HIGH);
       analogWrite(transistorPin_l, 0);
       analogWrite(transistorPin_r, 0);
       delay(150);
       
       //talk to cautomer
       radio.Wakeup();
  
       radio.Send(cartID, alarm, 1, requestACK);
       delay(50); // wait for receive send back ACK
       
       if (radio.ACKReceived(cartID)){
         Serial.print("successful");
       }
    
       radio.Sleep();
  
       delay(500);
     
       digitalWrite(trigPin_f, LOW);
       delayMicroseconds(2);
       digitalWrite(trigPin_f, HIGH);
       delayMicroseconds(10); // Added this line
       digitalWrite(trigPin_f, LOW);

       duration_f = pulseIn(echoPin_f, HIGH);
       distance_f = (duration_f/2) / 29.1; //update distance front 
     } 
       digitalWrite(19, LOW);
       analogWrite(transistorPin_r,0);
       analogWrite(transistorPin_l,0);
       delay(300);
       
       //right sensor needed
       digitalWrite(trigPin_r, LOW); 
       delayMicroseconds(2); // stay low for 2ms
       digitalWrite(trigPin_r, HIGH);
       delayMicroseconds(10); // stay high for 10 ms
       digitalWrite(trigPin_r, LOW);
       duration_r = pulseIn(echoPin_r, HIGH);
       distance_r = (duration_r/2) / 29.1;
     
     if(distance_r < 20 && distance_r > 0){
       analogWrite(transistorPin_l,0);
       analogWrite(transistorPin_r,195);
       delay(t_sensor);
       Serial.print("Right sensor:");
       Serial.println(distance_r);
       Serial.print("direction modified!");
     }
    
    Serial.print("delay?");
    analogWrite(transistorPin_r, 0);
    analogWrite(transistorPin_l, 0);
    delay(3000);
    }
  }
 
