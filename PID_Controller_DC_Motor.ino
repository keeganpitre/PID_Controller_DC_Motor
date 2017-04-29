//Keegan Pitre, April 25, 2017
#define INPUT_SIZE 30

const int motorIO = 6; 
const int RPMInput = 12; 
int PWMVal;

//Other Necessary Variables
int RPMInd;
unsigned long newTime = 0;
unsigned long RPMtime;
int elapTime;
unsigned long timeVar;
int RPM_feedback =0;
int prevRPMInd = 1;
int ctr = 0;
int ctr2 = 0;
int cap_voltage2;
char ind;
char on_off_ind;
String serString;
int serInt;
int potPin = A1;
int val = 255;
int RPM_actual_val=0;
int invVal;

//PID Vals
float P_gain = 1
;
float I_gain = 0;
float D_gain = .2;
float Dt = 2;
float prev_error = 0;
float D_error = 0;
float I_error = 0;
float Error = 0;
float H = 0;
float I = 0;
float J = 0;
float K = 0;
float L = 0;
float M = 0;
float Otpt = 0;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(5);
  
//Initializes Pins
  pinMode(motorIO, OUTPUT); //$ 6
  pinMode(RPMInput, INPUT);// 12
 
  digitalWrite(motorIO, 1);
  
  Serial.println("Done initialization");
}

void loop() {
//  delay(1);
  //Section that checks the RPM of the motor
  timeVar = millis();
  elapTime = 100;
  //Serial.println(timeVar);
  while (timeVar + elapTime > newTime){
    newTime = millis();
      RPMInd = digitalRead(RPMInput);
      
    if (RPMInd != prevRPMInd){
      delay(1);
      ctr2 = ctr2+1;  
      prevRPMInd = RPMInd;
    }
  }
  RPMtime = (ctr2*10*60)/4;
  RPM_feedback = RPMtime/24;
  ctr = 0;
  ctr2 = 0;
  
  val = analogRead(potPin)/4;    // read the value from the sensor
  RPM_actual_val = val;
  
  Error = abs(val) - abs(RPM_feedback);
  I_error = I_error + Error;
  L = P_gain*Error;
  M = D_gain*(Error - prev_error);
  Otpt = P_gain*Error - D_gain*(Error - prev_error);
  prev_error = Error;
  val = val + Otpt;
  
  if (val>255){
    val = 255;  
  }
  else if (val<0){
    val=0;
  }
  Serial.print(RPM_actual_val*24);
  Serial.print(",");
  Serial.println(RPM_feedback*24);

  invVal=255-val;
  analogWrite(motorIO, invVal);


}
