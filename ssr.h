#ifndef ssr_h
#define ssr_h
#define RELAYPIN  3   // relay control SSR PWM

#include <number_format.h>
#include <io_utils.h>
#include <log.h>

#define DEBUG

#ifdef DEBUG
bool DEBUG_ssr = true;
#else
bool DEBUG_ssr = false;
#endif
  
bool ssrDisabled = true; // safety ON
float currentDuty = 0; // ssrpower
bool invertDuty = true; // invert duty logic vcc range, true for low side sinking, pwm is high pulse
bool invertLOW  = !invertDuty;  // Drive SSR with VCC // invert logic for 2 state relay, this is usually opposite of invertDuty

int _ssrRelayPin = -1;

void ssr_init(uint16_t pin){
  #ifdef ESP8266
  analogWriteRange(255); // esp8266 
  // analogWriteFreq(120); // min 100hz
  #elif defined(ESP32)
  // analogWriteResolution(_ssrRelayPin, 8);
  #endif

  //ssr enable

  Logger.println("[SSR] READY on pin " + (String)_ssrRelayPin);
  _ssrRelayPin = pin;
  // ssr_off();
  digitalWrite(_ssrRelayPin, invertLOW ? LOW : HIGH);
  pinMode( _ssrRelayPin, OUTPUT );
}

void ssr_init(){
  ssr_init(RELAYPIN);
}

// This is where the SSR is controlled via PWM
void SetSSRFrequency( int duty,int power =1)
{
  if(duty!=currentDuty){
  // calculate the wanted duty based on settings power override
  duty = ((float)duty * power ); // power adjust
  duty = constrain( round_f( duty ), 0, 255); // round and clamp
  duty = abs(duty); // convert to whole

  // Write the clamped duty cycle to the RELAYPIN GPIO 
  int out = invertDuty ? 255-duty : duty;

  // Logger.println("[SSR] " + (String)out);

  if(!ssrDisabled){
    #ifdef ESP8266
    analogWrite( _ssrRelayPin, out);
    #elif defined(ESP32)
    Logger.println("[SSR] - " + (String)out);
    analogWrite( _ssrRelayPin, out);
    // dacWrite(_ssrRelayPin,out);
    #endif
    // if(duty == 0)ssr_off();
    // if(duty == 255)ssr_on();
  // else ssr_off(); // ENFORCE SAFETY

    // if(DEBUG_ssr) Logger.println("[SSR] " + (String)duty);
    if(duty<1 && DEBUG_ssr) Logger.println("[SSR]: Duty OFF - " + (String)out);
    else{
      if(DEBUG_ssr) Logger.print("[SSR] Duty ON");
      if(DEBUG_ssr) Logger.println( " - duty: " + (String)duty + " " + String( ( duty / 256.0 ) * 100) + "%" +" pow:" + String( round_f( power * 100 )) + "%" );
    }
  }
  }
  currentDuty = duty;  
}

void ssr_off(){
  if(_ssrRelayPin >= 0){
    Logger.println("[SSR] OFF");
    SetSSRFrequency(0); //working
    analogWrite( _ssrRelayPin, invertDuty ? 255 : 0 ); // MUST use analogwrite if using shim lib
    // digitalWrite(_ssrRelayPin, invertLOW ? LOW : HIGH); // @todo esp32 issue? must do analogwrite first
  }
}

void ssr_on(){
  if(_ssrRelayPin >= 0) {
    Logger.println("[SSR] ON");
    SetSSRFrequency(255); // working
    analogWrite( _ssrRelayPin, invertDuty ? 0: 255);
    // digitalWrite(_ssrRelayPin, invertLOW ? HIGH : LOW);
  }
}

void setSSR(int duty){
  SetSSRFrequency(duty);
}

void setSSRFreq(int duty){
  #ifdef ESP8266
  analogWriteFreq(duty); // min 100hz
  #endif
}

float getSSRDuty(){
  return currentDuty;
}

float getSSRPower(){
  return ( currentDuty / 255.0 ) * 100;
}

void ssr_resume(){
  Logger.println("[SSR] ssr_resume");
  if(_ssrRelayPin >= 0) setSSR(currentDuty);
}

void disableSSR(bool disabled = true){
  Logger.println("[SSR] disable ssr");
  setSSR(0);
  ssr_off();
  ssrDisabled = true;
  // init safe state, lockdown
  // pinMode(_ssrRelayPin,INPUT_PULLUP);
}

void enableSSR(bool disabled = false){
  Logger.println("[SSR] enable ssr");
  setSSR(0);
  ssr_off();
  ssrDisabled = false;
  // init safe state
  // pinMode(_ssrRelayPin,OUTPUT); // PINMODE BREAKS ANALOGWRITE LEDC CHANNEL
  ssr_on();
  delay(500); // test pulse
  ssr_off();
}

void toggleSSR(){
  ssrDisabled = !ssrDisabled;
  if(!ssrDisabled) ssr_resume();
  else ssr_off();
}

void ssrTest(int speed){

  ssrDisabled = false;
  ssr_on();
  delay(1000);
  ssr_off();
  delay(1000);
  ssr_on();
  delay(1000);
  ssr_off();
  delay(2000);

  // Turn off the SSR - duty cycle of 0
  SetSSRFrequency( 255 ); // test pulse
  delay(1000);
  SetSSRFrequency( 0 );
  delay(1000);
  SetSSRFrequency( 255 ); // test pulse
  delay(1000);
  SetSSRFrequency( 0 );

  for(int i=0;i<255;i++){
    SetSSRFrequency( i );
    delay(100);
  }

  for(int i=0;i<255;i++){
    SetSSRFrequency( 255-i );
    delay(100);
  }

  ssr_off();
  ssrDisabled = true;
}

void ssrPing(int speed){
  Serial.println("[SSR] PING");

  ssrDisabled = false;
  ssr_off();
  ssr_off();

  delay(500);
  ssr_on();
  delay(speed*3);
  ssr_off();
  delay(speed*3);
  ssr_on();
  delay(speed*2);
  ssr_off();
  delay(speed*2);
  ssr_on();
  delay(speed);

  // for(int i=0;i<255;i+20){
  //   SetSSRFrequency( i );
  //   delay(100);
  // }

  // for(int i=0;i<255;i+20){
  //   SetSSRFrequency( 255-i );
  //   delay(100);
  // }

  ssr_off();
  ssrDisabled = true;
}
#endif