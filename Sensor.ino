#define trigPin 9
#define echoPin_f 8
#define echoPin_l 7
#define echoPin_r 6

int alarm_f, alarm_l, alarm_r; // 0 if no objects within 20 cm
 
void setup() {
   Serial.begin (9600);
   pinMode(trigPin, OUTPUT);
   pinMode(echoPin_f, INPUT);
   pinMode(echoPin_l, INPUT);
   pinMode(echoPin_r, INPUT);
 }
 
void loop() {
   long duration_f, distance_f;
   long duration_l, distance_l;
   long duration_r, distance_r;
   
   digitalWrite(trigPin, LOW); 
   delayMicroseconds(2); // stay low for 2ms
   digitalWrite(trigPin, HIGH);
   delayMicroseconds(10); // stay high for 10 ms
   digitalWrite(trigPin, LOW);
   
   duration_f = pulseIn(echoPin_f, HIGH);
   distance_f = (duration_f/2) / 29.1;
   duration_l = pulseIn(echoPin_l, HIGH);
   distance_l = (duration_l/2) / 29.1;
   duration_r = pulseIn(echoPin_r, HIGH);
   distance_r = (duration_r/2) / 29.1;
   
   if (distance_f > 20) {  
     alarm_f = 0; // no objects in front
   }
   else {
     alarm_f = 1; // objects detected in front
   }
   if (distance_l > 20) {  
     alarm_l = 0; // no objects in front
   }
   else {
     alarm_l = 1; // objects detected in front
   }
   if (distance_r > 20) {  
     alarm_r = 0; // no objects in front
   }
   else {
     alarm_r = 1; // objects detected in front
   }
   
   if (alarm_f == 1 || alarm_l ==1){
     // turn right by 15 degree
   }
   if (alarm_r == 1){
     // turn left by 15 degree
   }
   
   delay(1000);
 }
