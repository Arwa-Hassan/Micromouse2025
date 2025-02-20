//Define sensor pins
// #define IR_left A1
// #define IR_center A2
// #define IR_right A0
// #define PushButton 12
// #define led 10

//Define pin numbers for encoders
#define encoder1PinA 2 //inturrupt pin
#define encoder2PinA 3 //inturrupt pin
#define encoder1PinB 4
#define encoder2PinB 5

//Variables for motor control
const int maxSpeed2 = 50;
const int maxSpeed = 10;
const int lowSpeed = 100;
const int dir1 = 6;  // Left motors direction
const int pwm1 = 10;  // Left motors speed control //pwm pin
const int dir2 = 8;  // Right motors direction
const int pwm2 = 9;  // Right motors speed control //pwm pin
bool is_on = 0;

//Variables for encoder counts
volatile long encoder1Count = 0;
volatile long encoder2Count = 0;

//Variables for PID Control
int target_ticks = 0;
float u1; //1st motor gain
float u2; //2nd motor gain 
float kp1 = 2.0; //1st motor constants
float kd1 = 1.0;
float ki1 = 1.0;
float kp2 = 2.0; //2nd motor constants
float kd2 = 1.0;
float ki2 = 1.0;
long previousTime = 0;
float ePrevious = 0;
float eIntegral = 0;
bool dircetion1 = 1;
bool dircetion2 = 1;

//maze and robot constants
const int cell_distance= 180; //mm
const int ticks_per_rev= 360;
const int wheel_rad= 66; //mm  //assuming the yellow wheel
const int wheel_base= 300; //mm
const int speed_factor= 1;
const int gear_ratio= 10; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // pinMode(PushButton, INPUT_PULLUP);
  // pinMode(IR_left, INPUT);
  // pinMode(IR_center, INPUT);
  // pinMode(IR_right, INPUT);
  // pinMode(led, OUTPUT);

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

    //moving forward 18cm in a straight line
    calculate_target('f');
    while(abs(target_ticks-encoder1Count)>1 && abs(target_ticks-encoder2Count)>1){
      u1= pidController(target_ticks, kp1, kd1, ki1, encoder1Count);
      u2= pidController(u1, kp2, kd2, ki2, encoder2Count);
      dircetion1=(u1>0)? 1 : 0 ;
      dircetion2=(u2>0)? 1 : 0 ;
      accelerateForward();
    }
    delay(1000);
    stop_mymotor();
    delay(1000);

    //turning left 90 degress
    calculate_target('l');
    while(abs(target_ticks-encoder1Count)>1 && abs(target_ticks-encoder2Count)>1){
      u1= pidController(target_ticks, kp1, kd1, ki1, encoder1Count);
      u2= pidController(u1*-1, kp2, kd2, ki2, encoder2Count);
      dircetion1=(u1>0)? 1 : 0 ;
      dircetion2=(u2>0)? 1 : 0 ;
      accelerateTurnLeft();
    }
    delay(1000);
    stop_mymotor();
    delay(1000);
    

//turning right 90 degress
    calculate_target('r');
    while(abs(target_ticks-encoder1Count)>1 && abs(target_ticks-encoder2Count)>1){
      u1= pidController(target_ticks*-1, kp1, kd1, ki1, encoder1Count);
      u2= pidController(u1, kp2, kd2, ki2, encoder2Count);
      dircetion1=(u1>0)? 1 : 0 ;
      dircetion2=(u2>0)? 1 : 0 ;
      accelerateTurnRight();
    }
    delay(1000);
    stop_mymotor();
    delay(1000);

  exit(0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void accelerateForward() {
  digitalWrite(dir1, dircetion1);
  digitalWrite(dir2, dircetion2);
  analogWrite(pwm1, fabs(u1)*speed_factor);
  analogWrite(pwm2, fabs(u2)*speed_factor);
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
  digitalWrite(dir1, dircetion1);
  digitalWrite(dir2, dircetion2);
  analogWrite(pwm1, fabs(u1)*speed_factor);
  analogWrite(pwm2, fabs(u2)*speed_factor);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void accelerateTurnRight() {
  digitalWrite(dir1, dircetion1);
  digitalWrite(dir2, dircetion2);
  analogWrite(pwm1, fabs(u1)*speed_factor);
  analogWrite(pwm2, fabs(u2)*speed_factor);
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
void calculate_target(char dir) {
  if (dir == 'f') target_ticks= (cell_distance * ticks_per_rev)/(2 * 3.14 * wheel_rad);
  else target_ticks= (wheel_base * ticks_per_rev)/(8 * wheel_rad);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*void stopMovingGradually() {
  analogWrite(pwm1, 0);
  analogWrite(pwm2, 0);
}*/