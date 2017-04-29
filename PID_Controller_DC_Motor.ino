//Keegan Pitre, April 29, 2017
const int motorIO = 6; // PWM output
const int RPMInput = 12; 

int PWMVal;
int RPMInd;
unsigned long newTime = 0;
unsigned long RPMtime;
int elapTime;
unsigned long timeVar;
int RPM_feedback =0;
int prevRPMInd = 1;
int ctr2 = 0;
int potPin = A1;
int val = 255;
int RPM_actual_val=0;
int invVal;

//PID Vals
float P_gain = 1;
float D_gain = .2;
float Dt = 2;
float prev_error = 0;
float D_error = 0;
float I_error = 0;
float Error = 0;
float L = 0;
float M = 0;
float Otpt = 0;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(5);
  
//Initializes Pins
  pinMode(motorIO, OUTPUT); //6
  pinMode(RPMInput, INPUT);//12
 
  digitalWrite(motorIO, 1);
  
  Serial.println("Done initialization");
}

void loop() {
//Section that checks the RPM of the motor
  timeVar = millis();
  elapTime = 100;
//Loop that counts the number of times the LED feedback changes from 1 to 0 or 0 to 1 within a certain time frame (elapTime 
  while (timeVar + elapTime > newTime){
    newTime = millis(); 
      RPMInd = digitalRead(RPMInput); //Obtains value from LED feedback
//Adds 1 to the counter every time the feedback changes value, this indicates the fan blade passing through the LED and the phototransistor     
    if (RPMInd != prevRPMInd){
      delay(1);
      ctr2 = ctr2+1;  
      prevRPMInd = RPMInd;
    }
  }
  RPMtime = (ctr2*10*60)/4; //ctr2 increases by 4 per rotation since there are two blades. RPM =  Pulse per minute / pulses per rotation
  RPM_feedback = RPMtime/24; //Converts the RPM value to a value between 0 and 255 to compare to desired RPM. Top RPM of motor is 6000. 6000/255=~24
  ctr2 = 0; //Resets the counter
  
  val = analogRead(potPin)/4; //Read the value from the potentiometer to obtain desired RPM value
  RPM_actual_val = val; //Saves the value to plot and compare later
  
  Error = abs(val) - abs(RPM_feedback); //Calculates the raw proportional error (desired - actual)
  I_error = I_error + Error; //If you want to use integral control add this to Otpt with a gain
  L = P_gain*Error; //The tuned proportional error
  M = D_gain*(Error - prev_error); //The tuned derivative error
  Otpt = P_gain*Error - D_gain*(Error - prev_error); //Total error
  prev_error = Error;// Saves previous error value for the derivative error
  val = val + Otpt; //Adds the total tuned error to the feedback signal 

  //Can't allow the output to go above 255 or below 0
  if (val>255){
    val = 255;  
  }
  else if (val<0){
    val=0;
  }

  //This how you plot two graphs using the arduino plotting tool
  Serial.print(RPM_actual_val*24); // Multiply by 24 to get back to an RPM from the 8bit value
  Serial.print(",");
  Serial.println(RPM_feedback*24); 

  invVal=255-val; //Must invert the PWM since a logic high will pull the gate of the mosfet to GND, there for cutting power to the motor
  analogWrite(motorIO, invVal); //Write inverted PWM value to the the pin


}
