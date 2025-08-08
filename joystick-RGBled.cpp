int ledPin = 11; 
#define BLUE 3
#define GREEN 5
#define RED 6
int colorState = 0;
const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = A0; // analog pin connected to X output
const int Y_pin = A1; // analog pin connected to Y output
int lastButtonState = HIGH;
int z = 255;

void setup() {
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  Serial.begin(9600);
  
}


void setLED(int r,int g,int b){

  analogWrite(RED,r);
  analogWrite(GREEN,g);
  analogWrite(BLUE,b);
}


void loop() {
  int x = (analogRead(X_pin) / 1023.0) * 255;
  int y = (analogRead(Y_pin)/1023.0) * 255;
  int button = digitalRead(SW_pin);

  //print X and Y axes to the serial monitor
  Serial.print("X-value: ");
  Serial.println(x);
  Serial.print("Y-value: ");
  Serial.println(y);
  Serial.print("Switch: ");
  Serial.println(button);
  Serial.print("Last Switch State: ");
  Serial.println(lastButtonState);
  delay(50);
  
  if (!button and lastButtonState == HIGH){
    if (z==255){
      z=0;
    }
    else{
      z=255;
    }
    delay(50);
  }
 
  lastButtonState = button;

  setLED(x,y,z);



    
}
