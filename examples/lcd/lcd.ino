#include <menu.h>
#include <dev/encoderIn.h>//for PCINT encoder
#include <dev/keyIn.h>//for encoder button
#include <dev/chainStream.h>//for mixing input stream (encoder+button)
#include <dev/lcdOut.h>//for lcd output
#include <dev/serialOut.h>

using namespace Menu;

// LCD /////////////////////////////////////////
#define RS 8
#define RW 3
#define EN 9
LiquidCrystal lcd(RS, RW, EN, 4, 5, 6, 7);

// Encoder /////////////////////////////////////
#define encA A2
#define encB 2
//this encoder has a button here
#define encBtn A3

encoderIn encoder(encA,encB);//simple quad encoder driver
encoderInStream encStream(encoder,4);// simple quad encoder fake Stream

//a keyboard with only one key as the encoder button
keyMap encBtn_map[]={{-encBtn,options.getCmdChar(enterCmd)}};//negative pin numbers use internal pull-up, this is on when low
keyIn<1> encButton(encBtn_map);//1 is the number of keys

//input from the encoder + encoder button + serial
Stream* inputsList[]={&encStream,&encButton,&Serial};
chainStream<3> in(inputsList);//3 is the number of inputs

void action1() {Serial<<"action1!"<<endl;}

// menu structure definition //////////////////////////////////////////////
MENU(mainMenu,"Main menu",doNothing,noEvent,wrapStyle
  ,OP("Op1",action1,enterEvent)
  ,OP("Op2",doNothing,noEvent)
  ,OP("Op3",doNothing,noEvent)
  ,EXIT("<Back")
);

lcdOut outLCD(lcd,16,2);
serialOut outSerial(Serial);//the output device (just the serial port)
menuOut* outputs[]={&outLCD,&outSerial};
outputsList out(outputs,2);
NAVROOT(nav,mainMenu,2,in,out);

//#define debugPin A5

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial<<"Arduino Menu Library"<<endl;Serial.flush();
  pinMode(LED_BUILTIN,OUTPUT);
  //pinMode(debugPin,INPUT_PULLUP);
  encoder.begin();
  pinMode(encBtn,INPUT_PULLUP);
  lcd.begin(16,2);
}

void loop() {
  nav.poll();
  //digitalWrite(LEDPIN, ledCtrl);
  //digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
  delay(100);//simulate a delay as if other tasks are running
}
