#include <util/atomic.h>

// Pins
#define E1 5     //M1 Speed Control
#define M1 4     //M1 Direction Control
#define encA 2
#define encB 3

// globals
long prevT = 0;
int posPrev = 0;
// Use the "volatile" directive for variables
// used in an interrupt
volatile int pos_i = 0;
volatile float velocity_i = 0;
volatile long prevT_i = 0;

float eintegral = 0;

int rowCount=9000;
int rowNumber=0;

void setup() {
  Serial.begin(115200);

  pinMode(encA, INPUT);
  pinMode(encB, INPUT);
  pinMode(E1, OUTPUT);
  pinMode(M1, OUTPUT);
  //declarem interrupcio per quan es detecti encoderA
  attachInterrupt(digitalPinToInterrupt(encA), readEncoder, RISING);
}

void loop() {

  // read the position in an atomic block
  // to avoid potential misreads
  int pos = 0;
  float velocity2 = 0;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
    pos = pos_i;
    velocity2 = velocity_i;
  }

  // Convert count/s to RPM
  float v2 = velocity2/696.5*60.0;

  // Set a target
  float vt = 100*(sin(currT/1e6)>0); //genera senyal sinus quadrat

  // Compute the control signal u
  float kp = 0.2;
  float ki = 5;
  float e = vt-v2; //error entre consigna i dada mesurada
  eintegral = eintegral + e*deltaT;
  
  float u = kp*e + ki*eintegral; //senyal de control del PID

  // Set the motor speed and direction
  int dir = 1;
  if (u<0){
    dir = -1;
  }
  int pwr = (int) fabs(u);
  if(pwr > 255){
    pwr = 255;
  }
  setMotor(dir, pwr, E1, M1);
  writeSerial(vt,v2);//funcio per a exportar al ArduSheet-> per a poder passar dades a excel
  Serial.println();
  delay(1);
}
void writeSerial(float a, float b){
  if(rowNumber<rowCount){
    rowNumber++;
    Serial.print(a);
    Serial.print('\t');
    Serial.print(b);
    Serial.print('\t');
  }
}

void setMotor(int dir, int pwmVal, int pwmPin, int enPin){
  analogWrite(pwmPin,pwmVal); // Motor speed
  if(dir==1) digitalWrite(enPin, HIGH);
  else if (dir==-1) digitalWrite(enPin, LOW);
  else digitalWrite(enPin, LOW);
}

void readEncoder(){
  // Read encoder B when ENCA rises
  int b = digitalRead(encB);
  int increment = 0;
  if(b>0){
    // If B is high, increment forward
    increment = 1;
  }
  else{
    // Otherwise, increment backward
    increment = -1;
  }
  pos_i = pos_i + increment;

  // Compute velocity with method 2
  long currT = micros();
  float deltaT = ((float) (currT - prevT_i))/1.0e6;
  velocity_i = increment/deltaT;
  prevT_i = currT;
}
