const int Contact_PIN = 46; // the number of the pushbutton pin
const int Activate_PIN = 37;
int lastState = HIGH; // the previous state from the input pin
int currentState;    // the current reading from the input pin

void setup() {
  Serial.begin(9600);
  // the pull-up input pin will be HIGH when the switch is open and LOW when the switch is closed.
  pinMode(Contact_PIN, INPUT_PULLUP);
  pinMode(Activate_PIN, OUTPUT);
}

void loop() {
  digitalWrite(Activate_PIN, HIGH);
  check();
  delay(1000);
  digitalWrite(Activate_PIN, LOW);
  check();
  delay(1000);
}
void check(){
  currentState = digitalRead(Contact_PIN);
  
  if(lastState == LOW && currentState == HIGH)
    Serial.println("The state changed from LOW to HIGH");

  else if(lastState == HIGH && currentState == LOW)
    Serial.println("The state changed from HIGH to LOW");
  // save the last state
  else Serial.println("Nothing");
  lastState = currentState;
}
