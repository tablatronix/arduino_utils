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
bool invertDuty = true; // invert duty logic vcc range
bool invertLOW  = false;  // Drive SSR with VCC

int _ssrRelayPin = -1;

void ssr_off(){
  if(_ssrRelayPin >= 0){
    Logger.println("[SSR] off");
    digitalWrite(_ssrRelayPin, invertLOW ? LOW : HIGH); // @todo esp32 issue? must do analogwrite first
    // analogWrite( _ssrRelayPin, 255);
  }
}

void ssr_on(){
  Logger.println("[SSR] on");
  if(_ssrRelayPin > 0) digitalWrite(_ssrRelayPin, invertLOW ? HIGH : LOW);
}

void ssr_init(uint16_t pin){
  Logger.println("[SSR] ready on pin " + (String)_ssrRelayPin);
  _ssrRelayPin = pin;
  pinMode( _ssrRelayPin, OUTPUT );
  delay(600);
  ssr_off();

  #ifdef ESP8266
  analogWriteRange(255); // esp8266 
  // analogWriteFreq(120); // min 100hz
  #elif defined(ESP32)
  #endif 
}

void ssr_init(){
  ssr_init(RELAYPIN);
}

// This is where the SSR is controlled via PWM
void SetSSRFrequency( int duty,int power =1)
{
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
    analogWrite( _ssrRelayPin, out);
    // dacWrite(_ssrRelayPin,out);
    #endif
    // if(duty == 0)ssr_off();
    // if(duty == 255)ssr_on();
  }
  else ssr_off(); // ENFORCE SAFETY

  if(duty!=currentDuty){
    // if(DEBUG_ssr) Logger.println("[SSR] " + (String)duty);
    if(duty<1 && DEBUG_ssr) Logger.println("[SSR]: duty OFF - " + (String)out);
    else{
      if(DEBUG_ssr) Logger.print("[SSR] ON");
      if(DEBUG_ssr) Logger.println( " - duty: " + (String)duty + " " + String( ( duty / 256.0 ) * 100) + "%" +" pow:" + String( round_f( power * 100 )) + "%" );
    }
  }
  currentDuty = duty;  
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
  if(_ssrRelayPin > 0) setSSR(currentDuty);
}

void disableSSR(bool disabled = true){
  ssrDisabled = disabled;
}

void toggleSSR(){
  ssrDisabled = !ssrDisabled;
  if(!ssrDisabled) ssr_resume();
  else ssr_off();
}

void ssrTest(int speed){

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
}

void ssrPing(int speed){
  
  ssr_on();
  delay(1000);
  ssr_off();

  for(int i=0;i<255;i+20){
    SetSSRFrequency( i );
    delay(100);
  }

  for(int i=0;i<255;i+20){
    SetSSRFrequency( 255-i );
    delay(100);
  }

  ssr_off();
}
#endif