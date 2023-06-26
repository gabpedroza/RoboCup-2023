#include "SoftwareSerial.h"
// RX: Arduino pin 47, XBee pin DOUT.  TX:  Arduino pin 49, XBee pin DIN
SoftwareSerial XBee(47, 49);
#include <Adafruit_NeoPixel.h>
#include <Servo.h>
#define K 0x000000//BLACK
#define B 0x0048ff//BLUE
#define G 0x008000//GREEN
#define P 0x9d00d2//PURPLE
#define R         0xFF0000//RED
#define O         0xFFA500//ORANGE
#define S 0xFFC0CB//ROSE
#define Y 0xFf6400//YELLOW
#define W 0xFFFFFF//WHITE
#define PIN 12// Pin of controle
#define LED_COUNT 79// Number of leds
//Cria uma instância da Classe Adafruit NeoPixel chamado "leds"
#define innerLeftShoulder 7
#define outerLeftShoulder 8
#define leftElbow 9
#define innerRightShoulder 2
#define outerRightShoulder 3
#define rightElbow 5//CAUTION! verify ports and change if needed
class articulation{ 
  public:
    articulation(Servo mmotor, int ini, int(*ffunc)(int x)){ //initializes an articulation
      _motor = mmotor; //motor
      _func = ffunc; //function to adjust angles
      _ang = _func(ini); //angle in which the articulation is
      move(ini); //sets the initial position
    }
    void move(int x, int del){
      x = _func(x); //adjusts angle
      if(x > _ang){ //if it needs to increase angle
        for(; _ang <= x; ++_ang){
          _motor.write(_ang);
          delay(del); //moves slowly
          //Serial.println("subindo" + String(_ang));
        }
        _ang=x; //sets the end angle to desired
      }else{
        for(; _ang >= x; --_ang){
          _motor.write(_ang);
          delay(del);//moves slowly
          //Serial.println("descendo" + String(_ang));
        }
        _ang=x; //sets the end angle to desired
      }
    }
    void move(int x){
      move(x, 15); //quick move, with standard speed
    }
    void brute(int x){
      _motor.write(x); //write directly to motor
    }
  private:
    int _ang;
    Servo _motor;
    int(*_func)(int x);
};
Servo mirs, mors, mre, mils, mols, mle; //acronyms for above definitions
articulation *irs, *ors, *re, *ils, *ols, *le; //articulations
//functions for adjusting angle 
int adjustIRS(int x){ return x;}
int adjustORS(int x){ return 180-x;}
int adjustRE(int x){ return (x>155)?180-155:180-x;}
int adjustILS(int x){ return x - 5;}
int adjustOLS(int x){ return 180-x;}
int adjustLE(int x){ return (x>155)?180-155:180-x;}
void greet();
void selfDestroy();
void calculate();
void track();
void receipt();
void angrier();
void normal();
void understand();
void translate();
Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

unsigned long time;//Will count the time
unsigned long timeout;//timeout
int brilho[3];//Receives the values of the serial
int i=0;//amount of characters
int posicao=0;
int flag=0;//Informs that a information by the serial was received
char temp,cor;//Storages data received by the serial
void setup() {
  // put your setup code here, to run once:
  leds.begin();
  leds.show();
  mirs.attach(innerRightShoulder);
  mors.attach(outerRightShoulder);
  mre.attach(rightElbow);
  mils.attach(innerLeftShoulder);
  mols.attach(outerLeftShoulder);
  mle.attach(leftElbow);
  delay(5000);
  irs = new articulation(mirs, 0, adjustIRS);
  delay(2000);
  ors = new articulation(mors, 0, adjustORS);
  delay(2000);
  re = new articulation(mre, 0, adjustRE);
  delay(2000);
  ils = new articulation(mils, 0, adjustILS);
  delay(2000);
  ols = new articulation(mols, 0, adjustOLS);
  delay(2000);
  le = new articulation(mle, 0, adjustLE);
  delay(12000);//initializing motors. plenty of delay to let capacitors stabilize
  Serial.begin(115200);
  Serial.setTimeout(5); //usual set-up
  XBee.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available())
  {
    char i = Serial.read();
    if(i == 'D'){
      //makes her angry and sends the little kart
      selfDestroy();
    }else if(i == 'G'){
      //waves goodbye and turns on the leds
      greet();
    }else if(i == 'T'){ //tracking
      //recognize
      understand();
      tracking();
      normal();
    }else if(i == 'O' || i == 'C'){ //ocr or calculate
      //recognize
      understand();
      normal();
    }else if(i == 'R'){ //receipt
      //recognize kinda angry
      receipt();
    }else if(i == 'A'){
      //recognize even angrier
      angrier();
    }
  }
}
