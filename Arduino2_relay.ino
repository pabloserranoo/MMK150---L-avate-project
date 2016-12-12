int relay = 2;
int prox1 = A0; //Proximity sensor 1
int val1 = 0; //value from proximity sensor 1
int prox2 = A1; //Proximity sensor 2
int val2 = 0; //value from proximity sensor 2
int mindistance = 16; //this value is the one taken from the sensors when the basket is in the top
int mindiff = 3; //the margin error between the two sensors, less than this triggers the relay
int timedelay = 0; //the time between the reading of the sensors and when we need to stop the lift
int sbutton = A2; //the pin for the start button, so the arduino can lower the basket after its finished
int obutton = A3;
void setup() {                
  pinMode(relay, OUTPUT); 
  Serial.begin(9600); // Serial output, need to comment this when final upload    
}

void loop() {
  val1 = analogRead(prox1)/0.79;
  val2 = analogRead(prox2)/0.79;
  if(abs(val1-val2)<mindiff && val1<mindistance ){ //might need to calibrate this difference
    delay(timedelay);
    digitalWrite(relay, HIGH); //relay is NC, this opens the circuit
  }
  int rbutton = analogRead(A2);
  delay(5);
  int rbutton2 = analogRead(A2);
  while (abs(rbutton-rbutton2) > 50){ //need to calibrate this number once
    rbutton = analogRead(A2);
    delay(5);
    int rbutton2 = analogRead(A2);
    delay(1);
  }
  digitalWrite(relay,LOW); //we give power to the motor once more so it can lower the basket
  for(int i=0; i<20;i++) analogWrite(obutton,rbutton);
  delay(10);  //safety delay               
}
