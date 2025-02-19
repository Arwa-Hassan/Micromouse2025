//Define sensor pins
#define IR_left A1
#define IR_center A2
#define IR_right A0
#define PushButton 12
#define led 10

//Define pin numbers for encoders
#define encoder1PinA 2
#define encoder2PinA 3
#define encoder1PinB 4
#define encoder2PinB 5

//Variables for motor control
const int maxSpeed2 = 50;
const int maxSpeed = 50;
const int lowSpeed = 100;
const int dir1 = 6;  // Left motors direction
const int pwm1 = 3;  // Left motors speed control
const int dir2 = 8;  // Right motors direction
const int pwm2 = 9;  // Right motors speed control
bool is_on = 0;

//Variables for encoder counts
volatile long encoder1Count = 0;
volatile long encoder2Count = 0;

//Variables for PID Control
int target = maxSpeed;
float u1;
float u2;
float kp = 0.0;
float kd = 0.0;
float ki = 0.0;
long previousTime = 0;
float ePrevious = 0;
float eIntegral = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(PushButton, INPUT_PULLUP);
  pinMode(IR_left, INPUT);
  pinMode(IR_center, INPUT);
  pinMode(IR_right, INPUT);
  pinMode(led, OUTPUT);

  pinMode(dir1, OUTPUT);
  pinMode(pwm1, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(pwm2, OUTPUT);
  

  pinMode(encoder1PinA, INPUT);
  pinMode(encoder1PinB, INPUT);
  pinMode(encoder2PinA, INPUT);
  pinMode(encoder2PinB, INPUT);

  attachInterrupt(digitalPinToInterrupt(encoder1PinA), handleEncoder1, RISING);
  attachInterrupt(digitalPinToInterrupt(encoder2PinA), handleEncoder2, RISING);
}

void loop() {
  // put your main code here, to run repeatedly:
  int L, C, R;

  L = analogRead(IR_left);
  C = analogRead(IR_center);
  R = analogRead(IR_right);

  //what the heck does this mean?!
  if (digitalRead(PushButton) == 0) {
    while (digitalRead(PushButton) == 0) {}
    is_on = !is_on;
  }

  if (is_on == 1) {
    digitalWrite(led, 1);
    Serial.print("C= ");
    Serial.print(C);
    Serial.print(" L= ");
    Serial.print(L);
    Serial.print(" R= ");
    Serial.println(R);

    u1= pidController(target, kp, kd, ki, encoder1Count);
    u2= pidController(target, kp, kd, ki, encoder2Count);

    if (C > 900) {
        accelerateForward();
        Serial.print("Forward ");
        Serial.print("C= ");
        Serial.println(C);
    } else if (L > 500) {
        accelerateTurnLeft();
        Serial.print("Left ");
        Serial.print("L= ");
        Serial.println(L);
    } else if (R > 500) {
        accelerateTurnRight();
        Serial.print("Right ");
        Serial.print("R= ");
        Serial.println(R);
    } else {
      accelerateback();
      Serial.println("back");
    }
  } else {
    digitalWrite(led, 0);
    stop_mymotor();
  }
  Serial.println(is_on);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void accelerateForward() {
  digitalWrite(dir1, HIGH);
  digitalWrite(dir2, HIGH);
  analogWrite(pwm1, fabs(u1));
  analogWrite(pwm2, fabs(u2));
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void accelerateback() {
  digitalWrite(dir1, LOW);
  digitalWrite(dir2, LOW);
  analogWrite(pwm1, fabs(u1));
  analogWrite(pwm2, fabs(u2));
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void accelerateTurnLeft() {
  digitalWrite(dir1, LOW);
  digitalWrite(dir2, HIGH);
  analogWrite(pwm1, fabs(u1));
  analogWrite(pwm2, fabs(u2));
  delay (1200);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void accelerateTurnRight() {
  digitalWrite(dir1, HIGH);
  digitalWrite(dir2, LOW);
  analogWrite(pwm1, fabs(u1));
  analogWrite(pwm2, fabs(u2));
  delay (1200);

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void stop_mymotor() {
  digitalWrite(dir1, LOW);
  digitalWrite(dir2, LOW);
  analogWrite(pwm1, 0);
  analogWrite(pwm2, 0);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void handleEncoder1() {
  if (digitalRead(encoder1PinA) > digitalRead(encoder1PinB)){
    encoder1Count++;
  }
  else{
    encoder1Count--;
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void handleEncoder2() {
  if (digitalRead(encoder2PinA) > digitalRead(encoder2PinB)){
    encoder2Count++;
  }
  else{
    encoder2Count--;
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float pidController(int target, float kp, float kd, float ki, volatile long encoderCount) {
  long currentTime = micros();
  float deltaT = ((float)(currentTime - previousTime)) / 1.0e6;

  int e = encoderCount - target;
  float eDerivative = (e - ePrevious) / deltaT;
  eIntegral = eIntegral + e * deltaT;

  float u = (kp * e) + (kd * eDerivative) + (ki * eIntegral);

  previousTime = currentTime;
  ePrevious = e;

  return u;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*void stopMovingGradually() {
  analogWrite(pwm1, 0);
  analogWrite(pwm2, 0);
}*/