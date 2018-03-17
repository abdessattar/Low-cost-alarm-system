#include <OnewireKeypad.h>
#include <Password.h>
#include <NewPing.h>
#include <Servo.h>
#define buttonPin2   2
#define buttonPin4   4
#define TRIGGER_PIN1 5
#define ECHO_PIN1    6
#define TRIGGER_PIN2 7
#define ECHO_PIN2    8
#define TRIGGER_PIN3 9
#define ECHO_PIN3    10
#define GREEN_PIN    11
#define RED_PIN      12
#define ALARM        13
#define MAX_DISTANCE 200 
char KEYS[] = {
  '1', '2', '3', 'A',
  '4', '5', '6', 'B',
  '7', '8', '9', 'C',
  'D', '*', '0', '#'
};

OnewireKeypad <Print, 16 > KP(Serial, KEYS, 4, 4, A0, 4700, 1000 );
Password password = Password( "1" );
NewPing sonar1(TRIGGER_PIN1, ECHO_PIN1, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonar2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE);
NewPing sonar3(TRIGGER_PIN3, ECHO_PIN3, MAX_DISTANCE);
Servo myservo;
boolean arm = false;
boolean triggered = false;
boolean wait = true;
boolean perimet = false;
boolean b = true;

void setup (){
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin4, INPUT);
  Serial.begin(115200);
  pinMode(GREEN_PIN, OUTPUT); //The green led is set as an output.
  pinMode(RED_PIN, OUTPUT); //The red led is set as an output.
  myservo.attach(3);
  KP.SetHoldTime(1000);
  KP.ShowRange();
  KP.addEventKey(resete, 'B'); 
  KP.addEventKey(checkPassword ,'C');
  KP.addEventKey(checkperimet ,'A');
  KP.addEventKey(motor ,'#');
  KP.addEventKey(add1 ,'1');
  KP.addEventKey(add2 ,'2');
  KP.addEventKey(add3 ,'3');
  KP.addEventKey(add4 ,'4');
  KP.addEventKey(add5 ,'5');
  KP.addEventKey(add6 ,'6');
  KP.addEventKey(add7 ,'7');
  KP.addEventKey(add8 ,'8');
  KP.addEventKey(add9 ,'9');
    Serial.println("mode desarm is actif"); 
    Serial.println("enter password + C for arm mode " );
    Serial.println("enter password + A for perimetre mode " );
    Serial.println("enter B for reset password");
}

void loop()
{
      KP.ListenforEventKey();
      myservo.write(180);
      if(triggered == true &&( arm==true || perimet == true)){
      
      alarm();
      }
      
      else {
        
      calcul();
}
}

void motor(){
    if (password.evaluate() && triggered == false &&( arm==true || perimet == true))
  {
    Serial.println("open door ");
    password.reset();
    delay(10);
    myservo.write(0);
    digitalWrite(12, HIGH); //Turn the green led on.
    delay(500);
    myservo.write(180);
    delay(800); //Wait 5 seconds.
    digitalWrite(12, LOW); //Turn the green led off.
} 
else {
  Serial.println("use only in armed mode or wrong password ");
  password.reset();
      digitalWrite(12, HIGH); //Turn the green led on.
    delay(200);
    myservo.write(180);
    delay(800); //Wait 5 seconds.
    digitalWrite(12, LOW); //Turn the green led off.
}
}
void checkPassword()
{ 
  if (password.evaluate())
  {
    Serial.println("Accepted");
    password.reset();
    
wait = true;
triggered = false;
//if mode is arm it will be desarme and vice versa
if (arm ==true){
arm=false;
noTone (ALARM);
perimet = false;
 Serial.println("mode desarmed actif");
digitalWrite(11, LOW); //Turn the green led off.
}
else
{arm=true; b=true; 
Serial.println("mode armed actif");
digitalWrite(11, HIGH); //Turn the green led on.
}
//digitalWrite(3,0);
//digitalWrite(4,0);

delay(1000); //Wait 1 seconds.

  }

  else{ //If the combination inserted isn’t equal to the combination saved, the servo will not rotate.

Serial.println("Denied");
password.reset();
delay(10);
digitalWrite(12, HIGH); //Turn the green led on.
delay(300); //Wait 5 seconds.
digitalWrite(12, LOW); //Turn the green led off.
}
}
void checkperimet(){
   if (password.evaluate())
  {
    Serial.println("Accepted");
    password.reset();

wait = true;
triggered = false;


//if mode is arm it will be desarme and vice versa
if (perimet ==true){
perimet=false;
arm =false;
noTone (ALARM);
Serial.println("mode perimetre desactif");}
else{
perimet=true;
arm = false;
Serial.println("mode perimetre actif");}

digitalWrite(11, HIGH); //Turn the green led on.
delay(300); //Wait 5 seconds.
digitalWrite(11, LOW); //Turn the green led off.
  }
    else{ //If the combination inserted isn’t equal to the combination saved, the servo will not rotate.

Serial.println("Denied");
password.reset();
delay(10);
digitalWrite(12, HIGH); //Turn the green led on.
delay(300); //Wait 5 seconds.
digitalWrite(12, LOW); //Turn the green led off.
}
}
void resete ()
{password.reset();
Serial.println("reset password");
}

void alarm(){
  if (wait ==true){
    int i = 0;
   while(i<100 && (arm == true || perimet == true)) {///delais d'attente
      digitalWrite(11, HIGH);
      digitalWrite(12, HIGH);
      delay(50);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      delay(50);
      KP.ListenforEventKey();
      i++;
      }
      }
      wait =false;
   digitalWrite(12, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(40);             
  digitalWrite(12, LOW);    // turn the LED off by making the voltage LOW
  delay(40);             
  digitalWrite(11, LOW); //Turn the green led off.
   for (int x=0; x<180; x++) {
    // convert degrees to radians then obtain sin value
 float  sinVal = (sin(x*(3.1412/180)));
    // generate a frequency from the sin value
   int  toneVal = 2000+(int(sinVal*1000));
   if (arm ==true)
    tone(ALARM,toneVal);
  }
}
int Distance1 (){
      delay(50);// Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
      unsigned int uS1 = sonar1.ping(); // Send ping, get ping time in microseconds (uS).
      unsigned int distance1 = uS1 / US_ROUNDTRIP_CM;
      return distance1;
}
int Distance2 (){

      unsigned int uS2 = sonar2.ping(); // Send ping, get ping time in microseconds (uS).
      unsigned int distance2 = uS2 / US_ROUNDTRIP_CM;
      return distance2;
}
int Distance3 (){

      unsigned int uS3 = sonar3.ping(); // Send ping, get ping time in microseconds (uS).
      unsigned int distance3 = uS3 / US_ROUNDTRIP_CM;
      return distance3;      
}
 void add1 ()
{ 
      Serial.println("Password number entered: 1");
      password.append('1');
}
  void add2 ()
{ 
      Serial.println("Password number entered: 2");
      password.append('2');
}
  void add3 ()
{ 
      Serial.println("Password number entered: 3");
      password.append('3');
}
  void add4 ()
{ 
      Serial.println("Password number entered:  4");
      password.append('4');
}
  void add5 ()
{ 
      Serial.println("Password number entered: 5");
      password.append('5');
}
  void add6 ()
{ 
      Serial.println("Password number entered: 6");
      password.append('6');
}
  void add7 ()
{ 
      Serial.println("Password number entered: 7");
      password.append('7');
}
  void add8 ()
{ 
      Serial.println("Passwordnumber entered: 8");
      password.append('8');
}
  void add9 ()
{ 
      Serial.println("Password number entered: 9");
      password.append('9');
}
void calcul(){
      int reading2a = digitalRead(buttonPin2);delay(10);
      int reading2b = digitalRead(buttonPin2);delay(12);
      int reading2c = digitalRead(buttonPin2);delay(10);
      int reading2d = digitalRead(buttonPin2);delay(11);
      int reading4a = digitalRead(buttonPin4);delay(10);
      int reading4b = digitalRead(buttonPin4);delay(10);
      int reading4c = digitalRead(buttonPin4);delay(10);
      int reading4d = digitalRead(buttonPin4);delay(10);
      int distance1 = Distance1();
      int distance2 = Distance2();
      int distance3 = Distance3();
         
         if (arm==true){
          if( distance1 < 12 ){
          triggered = true;
         Serial.println("intrusion in room 1");}
         if( distance2 < 20 ){
          triggered = true;
         Serial.println("intrusion in room 2");}
          if( distance3 < 20 ){
          triggered = true;
         Serial.println("intrusion in room 3");}
         }
         if (arm==true || perimet == true){
         if (reading2a == 1  && reading2b == 1   &&  reading2c == 1 &&  reading2d == 1 ){
          if (b==true){
         Serial.println("Batterie est déchargé");
         digitalWrite(11, HIGH);
         delay(500);
         digitalWrite(11, LOW);
         b=false;}
         }
         if (reading4a == 1  && reading4b == 1   &&  reading4c == 1 &&  reading4d == 1 ){
         triggered = true;
         Serial.println(" intrusion in wind 4");}
         }
}
  
