#include <MIDI.h>
#include "Controller.h"

/*************************************************************
  MIDI CONTROLLER

  by Notes and Volts
  www.notesandvolts.com

  Version 1.2 **Arduino UNO ONLY!**
 *************************************************************/

MIDI_CREATE_DEFAULT_INSTANCE();



#define PIN_A 7
#define PIN_B 6
#include "SwitchPack.h"
#include "ClickButton.h"
Encoder anEncoder(PIN_A, PIN_B);
int rotation[100];
int a;
int sense=1;
int senserotation;
int sum[100];

// the Button
const int buttonPin1 = 4;
ClickButton button1(buttonPin1, LOW, CLICKBTN_PULLUP);





//************************************************************
//***SET THE NUMBER OF CONTROLS USED**************************
//************************************************************
//---How many buttons are connected directly to pins?---------
byte NUMBER_BUTTONS = 1;
//---How many potentiometers are connected directly to pins?--
byte NUMBER_POTS = 0;
//---How many buttons are connected to a multiplexer?---------

//************************************************************


//***DEFINE DIRECTLY CONNECTED POTENTIOMETERS************************
//Pot (Pin Number, Command, CC Control, Channel Number)
//**Command parameter is for future use**

//Pot PO1(A0, 0, 1, 1);
//Pot PO2(A1, 0, 10, 1);
//Pot PO3(A2, 0, 22, 1);
//Pot PO4(A3, 0, 118, 1);
//Pot PO5(A4, 0, 30, 1);
//Pot PO6(A5, 0, 31, 1);
//*******************************************************************
//Add pots used to array below like this->  Pot *POTS[] {&PO1, &PO2, &PO3, &PO4, &PO5, &PO6};
Pot *POTS[] {};
//*******************************************************************


//***DEFINE DIRECTLY CONNECTED BUTTONS*******************************
//Button (Pin Number, Command, Note Number, Channel, Debounce Time)
//** Command parameter 0=NOTE  1=CC  2=Toggle CC **

Button BU1(4, 1, 60, 1, 5 );
//Button BU2(3, 0, 61, 1, 5 );
//Button BU3(4, 0, 62, 1, 5 );
//Button BU4(5, 0, 63, 1, 5 );
//Button BU5(6, 0, 64, 1, 5 );
//Button BU6(7, 0, 65, 1, 5 );
//Button BU7(8, 1, 64, 1, 5 );
//Button BU8(9, 2, 64, 1, 5 );
//*******************************************************************
//Add buttons used to array below like this->  Button *BUTTONS[] {&BU1, &BU2, &BU3, &BU4, &BU5, &BU6, &BU7, &BU8};
Button *BUTTONS[] {&BU1};
//*******************************************************************


void setup() {
  MIDI.begin(MIDI_CHANNEL_OFF);


  
  anEncoder.begin();

  // Setup button timers (all in milliseconds / ms)
  // (These are default if not set, but changeable for convenience)
  button1.debounceTime   = 20;   // Debounce timer in ms
  button1.multiclickTime = 250;  // Time limit for multi clicks
  button1.longClickTime  = 1000; // time until "held-down clicks" register

  
}

void loop() {
  if (NUMBER_BUTTONS != 0) updateButtons();
  if (NUMBER_POTS != 0) updatePots();


// Update button state
  button1.Update();

  // Save click codes in LEDfunction, as click codes are reset at next Update()
  
  // Simply toggle LED on single clicks
  // (Cant use LEDfunction like the others here,
  //  as it would toggle on and off all the time)
  if(button1.clicks == 1){
   //Serial.println("once");

  }

  // blink faster if double clicked
  if(button1.clicks == 2){
    a++;
    Serial.println("twice");
  }

  // blink even faster if triple clicked
  if(button1.clicks == 3){
    a--;
    Serial.println("thrice");
  }

  // slow blink (must hold down button. 1 second long blinks)
  if(button1.clicks == -1){
    Serial.println("hold");

long starttime = millis();
long endtime = starttime;
while ((endtime - starttime) <=5000) // do this loop for up to 1000mS
{

        senserotation = anEncoder.getRotation();
        if(senserotation!=0){
        sense += senserotation;
        Serial.println(sense);

int loopcount = loopcount+1;
endtime = millis();
}

    

        }
        
  }

  // slower blink (must hold down button. 2 second loong blinks)
  if(button1.clicks == -2){
    Serial.println("twicehold");
  }

  // even slower blink (must hold down button. 3 second looong blinks)
  if(button1.clicks == -3){
    Serial.println("thricehold");
  }

  
  rotation[a] = anEncoder.getRotation();
  if (rotation[a] != 0) { 
    sum[a] += (rotation[a]*sense);
    Serial.println(sum[a]);

    
  }


}


//*****************************************************************
void updateButtons() {

  // Cycle through Button array
  for (int i = 0; i < NUMBER_BUTTONS; i = i + 1) {
    byte message = BUTTONS[i]->getValue();

    //  Button is pressed
    if (message == 0) {
      switch (BUTTONS[i]->Bcommand) {
        case 0: //Note
          MIDI.sendNoteOn(a, 127, BUTTONS[i]->Bchannel);
          break;
        case 1: //CC
          MIDI.sendControlChange(a, 127, BUTTONS[i]->Bchannel);
          break;
        case 2: //Toggle
          if (BUTTONS[i]->Btoggle == 0) {
            MIDI.sendControlChange(a, 127, BUTTONS[i]->Bchannel);
            BUTTONS[i]->Btoggle = 1;
          }
          else if (BUTTONS[i]->Btoggle == 1) {
            MIDI.sendControlChange(a, 0, BUTTONS[i]->Bchannel);
            BUTTONS[i]->Btoggle = 0;
          }
          break;
      }}
    

    //  Button is not pressed
    if (message == 1) {
      switch (BUTTONS[i]->Bcommand) {
        case 0:
          MIDI.sendNoteOff(a, 0, BUTTONS[i]->Bchannel);
          break;
        case 1:
          MIDI.sendControlChange(a, 0, BUTTONS[i]->Bchannel);
          break;
      }
    }
  }
}

//***********************************************************************
void updatePots() {
  for (int i = 0; i < NUMBER_POTS; i = i + 1) {
    byte potmessage = POTS[i]->getValue();
    if (potmessage != 255) MIDI.sendControlChange(POTS[i]->Pcontrol, potmessage, POTS[i]->Pchannel);
  }
}
//***********************************************************************


