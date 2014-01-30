int readPin = 3; //analog pin3
int sum = 0; // add up the first 5 sets of data and take average
int i = 0;
int val=0;
int val2=0;
void setup()
{
 pinMode(readPin,INPUT);
 Serial.begin(9600);
}

void loop()
{
   while(i < 5){
     val = analogRead(readPin);
     sum = sum + val;
     i ++;
     delay(100);
   }
   val2 = sum/5;
   
   if(val2 < 80)
   Serial.print("Nothing\n");
   
   else if(80 < val2 && val2 < 150){
     Serial.print("Grocery\n");
   }
   if(150 < val2 && val2< 250){
     Serial.print("Kids\n");
   }
   else if(250 < val2 && val2 < 350){
     Serial.print("Books\n");
   }
   else if(350 < val2 && val2 < 450){
     Serial.print("Office\n");
   }  
   else if(450 < val2 && val2 < 550){
     Serial.print("Health\n");
   }   
   else if(550 < val2 && val2 < 650){
     Serial.print("Beauty\n");
   }  
   else if(650 < val2 && val2 < 750){
     Serial.print("Sports\n");
   }   
   else if(750 < val2 && val2 < 850){
     Serial.print("Electronics\n");  
   }
   else if(850 < val2 && val2 < 950){
     Serial.print("Apparel\n"); 
   }
   else if(950 < val2 && val2 < 1050){
     Serial.print("Home\n");  
   } 
   
   sum = 0;
   i = 0;
   delay(1000);
}
