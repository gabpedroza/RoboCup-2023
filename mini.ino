/*
  Software serial multiple serial test

 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.

 The circuit:
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)

 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts,
 so only the following can be used for RX:
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

 Not all pins on the Leonardo and Micro support change interrupts,
 so only the following can be used for RX:
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

 created back in the mists of time
 modified 25 May 2012
 by Tom Igoe
 based on Mikal Hart's example

 This example code is in the public domain.

 */
#include <SoftwareSerial.h>
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


#define PIN 9// Pin of controle
#define LED_COUNT 79// Number of leds
//Cria uma instância da Classe Adafruit NeoPixel chamado "leds"
Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);
SoftwareSerial mySerial(3, 4); // RX, TX
Servo le, ri;
void garrincha_frente()
{
  //digitalWrite(pino_um,HIGH);//MOTOR A
  PORTD = PORTD|(1<<pino_um)|(1<<pino_quatro);//the command of type a = a|(1<<i) turns on the i-th bit in a
  //digitalWrite(pino_dois,LOW);//MOTOR A
  PORTD = ~((~PORTD)|(1<<pino_dois)|(1<<pino_tres));//the command of type a = ~((~a)|(1<<i)) turns off the i-th bit in a
  //digitalWrite(pino_tres,LOW);//MOTOR B
  //digitalWrite(pino_quatro,HIGH);//MOTOR B

}
void garrincha_tras()
{
  //digitalWrite(pino_um,LOW);//MOTOR A
  PORTD = PORTD|(1<<pino_dois)|(1<<pino_tres);
  //digitalWrite(pino_dois,HIGH);//MOTOR A
  PORTD = ~((~PORTD)|(1<<pino_um)|(1<<pino_quatro));
  //digitalWrite(pino_tres,HIGH);//MOTOR B
  //digitalWrite(pino_quatro,LOW);//MOTOR B
}
void parar_garrincha()
{
  PORTD = ~((~PORTD)|(1<<pino_um)|(1<<pino_dois));
  PORTD = ~((~PORTD)|(1<<pino_tres)|(1<<pino_quatro));
  /*digitalWrite(pino_um,LOW);//MOTOR A
  digitalWrite(pino_dois,LOW);//MOTOR A
  digitalWrite(pino_tres,LOW);//MOTOR B
  digitalWrite(pino_quatro,LOW);//MOTOR B*/
}
void setup() {
  // Open serial communications and wait for port to open:
  leds.begin(); // initiates the LED strip
  leds.show(); // updates  the state of the LEDS
  for(int i = 0; i <= 100; ++i) leds.setPixelColor(i, B);
  leds.show();
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  le.attach(10);
  ri.attach(11);
  le.write(5);
  ri.write(175);
  delay(1000);
  analogWrite(5, 215);
  analogWrite(6, 255); //minha direita
  digitalWrite(13, 1);
  digitalWrite(12, 0);
  digitalWrite(7, 1);
  digitalWrite(8, 0);
  delay(4000);
  digitalWrite(13, 1);
  digitalWrite(12, 1);
  digitalWrite(7, 1);
  digitalWrite(8, 1);
  delay(2000);
  for(int i = 0; i <= 100; ++i) leds.setPixelColor(i, R);
  leds.show();
  le.write(105);
  ri.write(85);
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() { // run over and over

}
