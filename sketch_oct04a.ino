#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;
char buffer[32];
char buffer2[32];
void setup()
{
  //initialize the digital pin as an output.
  pinMode(led, OUTPUT);
// Open serial communications and wait for port to open: Serial.begin(38400);
while (!Serial)
{ 
; // wait for serial port to connect. Needed for Leonardo only } 
  // set the data rate for the SoftwareSerial port
  mySerial.begin(38400);
  //mySerial.println("Hello, world?");
}
} 
//==================================================================== long read_apos(int id)
long read_apos(int id)
{ 
int i, value; 
   sprintf(buffer,"cid=%d", id);
   mySerial.write(buffer);
   mySerial.write(13); //carrige return
   wait_command();
   mySerial.write("?apos");
   mySerial.write(13); //carrige return
  //Receive apos
   for(i=0; i < 16; i++)
   {
     if (mySerial.available() > 0)
     {
       value=mySerial.read();
       buffer[i]=(char)value;
       //Serial.write(value);
       if(value == 10)
break; } 
} 
   buffer[i+1]=0;
   return atol(buffer);
}
//=====================================================================     
int q_msr() { 
int value, i; 
 //LB_MSR:
  //delay(10);
   mySerial.write("?msr");
   mySerial.write(13); //carrige return
  //Receive msr
   for(i=0; i < 16; i++)
   {
     if(mySerial.available() > 0)
     {
       value=mySerial.read();
       buffer[i]=(char)value;
       //Serial.write(value);
       if(value == 10)
break; } 
} 
   buffer[i]=0;
   //Serial.println(buffer);
   value=atoi(buffer);
  //Serial.println(value, DEC);
   if(value == 8705)
     digitalWrite(led, HIGH); //Turn the LED on
   else
    digitalWrite(led, LOW); // Turn the LED off
     return value;
}
    
//====================================================================
int wait_msr(int time)
{
  int value, i;
  int t=0;
 LB_MSR:
   delay(10);
   mySerial.write("?msr");
   mySerial.write(13); //carrige return
  //Receive msr
   for(i=0; i < 16; i++)
   {
     if(mySerial.available() > 0)
     {
       value=mySerial.read();
       buffer[i]=(char)value;
       //Serial.write(value);
       if(value == 10)
break; } 
} 
   buffer[i]=0;
   //Serial.println(buffer);
   value=atoi(buffer);
  //Serial.println(value, DEC);
   if(value == 8705)
     digitalWrite(led, HIGH); //Turn the LED on
   
   else
    digitalWrite(led, LOW);
    if(t++ > time)
      return -1;
    if(value == 8193)
      goto LB_MSR;
     return value;
}
//turn the LED off

//=================================================================== int wait_command()
int wait_command()
{ 
 delay(5); //5
}
//=================================================================== 
int abs_motion(int id, long cpos, double cspd, double cacc) { 
   char abuf[32];
   sprintf(buffer,"cid=%d", id);
   mySerial.write(buffer);
   //Serial.write(buffer);
   mySerial.write(13); //carrige return
   wait_command();
   //wait_command();
  //sprint(buffer,%0d%d,temp1a)
   sprintf(buffer,"cspd=%s", ftoa(abuf, cspd,3));
   mySerial.write(buffer);
   //Serial.println(buffer);
   mySerial.write(13); //carrige return
   wait_command();
   //wait_command();
   sprintf(buffer,"cacc=%s", ftoa(abuf, cacc,3));
   mySerial.write(buffer);
   //Serial.println(buffer);
   mySerial.write(13); //carrige return
   wait_command();
   //wait_command();
   sprintf(buffer,"cpos=%ld", cpos);
   mySerial.write(buffer);
   //Serial.println(buffer);
   mySerial.write(13); //carrige return
   wait_command();
   //wait_command();
return 1; } 

    
//=================================================================== 
int upd(int id)
{
   sprintf(buffer,"cid=%d", id);
   mySerial.write(buffer);
   mySerial.write(13); //carrige return
   wait_command();
   //wait_command();
   sprintf(buffer,"upd");
   mySerial.write(buffer);
   mySerial.write(13); //carrige return
   wait_command();
   //wait_command();
return 1; } 
    
//==================================================================== 
void loop(){ // run from here – i.e. the main() loop
 int i, k,p,v1,value;
 int id;
 float cspd, cacc;
 long cpos, cnt;
 //Initiate the Servo Driver
   mySerial.write("cid=255");
   mySerial.write(13); //carrige return
   wait_command();
   mySerial.write("reset");
   mySerial.write(13); //carrige return
   wait_command();
   delay(2000);
   //Set the soft and msrw parameters
   mySerial.write("cid=255");
   mySerial.write(13); //carrige return
   wait_command();
   mySerial.write("msrw=50");
   mySerial.write(13); //carrige return
   wait_command();
   mySerial.write("soft=0.0");
   mySerial.write(13); //carrige return
   wait_command();
  //Do 10000 loops
  cpos=0; cnt=0;
for(i=0; i < 10000; i++)
{
//Perform an absolut servo motor movement abs_motion(id=255, cpos=20000, cspd=800.0, cacc=5.0); upd(id=255); //Update - i.e. start the movement wait_command(); 
//Test by polling the MSR-register if the servo motor has reached //the commanded position. 
    v1=0; p=0;
    while(v1 != 8705 || p++ < 10)
    {
v1=q_msr(); 
delay(5); }; 
    wait_command(); //delay
    //Perform another absolut motion
    
abs_motion(id=255, cpos=0, cspd=2000.0, cacc=15.0); upd(id=255); 
    v1=0; p=0;
    while(v1 != 8705 || p++ < 10)
    {
v1=q_msr(); 
delay(5); }; 
    wait_command();
    Serial.println(cnt, DEC);
    cnt++;
} } 
//===================================================================== char *ftoa(char *a, double f, int precision)
char *ftoa(char *a, double f, int precision){ 
long p[] = {0,10,100,1000,10000,100000,1000000,10000000,100000000}; 
char *ret = a;
long heiltal = (long)f;
itoa(heiltal, a, 10);
while (*a != '\0') a++;
*a++ = '.';
long desimal = abs((long)((f - heiltal) * p[precision])); itoa(desimal, a, 10);
return ret; 
} 

