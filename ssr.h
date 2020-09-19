#ifndef ssr_h
#define ssr_h
#define RELAYPIN  16   // relay control SSR PWM

#include <number_format.h>

#ifdef DEBUG
bool DEBUG_ssr = true;
#else
bool DEBUG_ssr = false;
#endif

float currentDuty = 0; // ssrpower
bool invertDuty = true; // invert logic vcc range

int _ssrRelayPin = -1;

void ssr_init(int pin){
  _ssrRelayPin = pin;
  pinMode( _ssrRelayPin, OUTPUT );
  digitalWrite(_ssrRelayPin,HIGH);

  #ifdef ESP8266
  analogWriteRange(255); // esp8266 
  // analogWriteFreq(240); // min 100hz
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
  #ifdef ESP8266P
  analogWrite( _ssrRelayPin, out);
  #endif
  // if(duty == 0)digitalWrite(RELAYPIN,invertDuty ? LOW:HIGH);
  // if(duty == 255)digitalWrite(RELAYPIN,!invertDuty ? LOW:HIGH);

  if(duty!=currentDuty){
    // if(DEBUG_ssr) Serial.println("[SSR] " + (String)duty);
    if(duty<1 && DEBUG_ssr) Serial.println("[SSR]: OFF");
    else{
      if(DEBUG_ssr) Serial.print("[SSR] ON");
      if(DEBUG_ssr) Serial.println( " - duty: " + (String)duty + " " + String( ( duty / 256.0 ) * 100) + "%" +" pow:" + String( round_f( power * 100 )) + "%" );
    }
  }
  currentDuty = duty;  
}

void setSSR(int duty){
  SetSSRFrequency(duty);
}

float getSSRDuty(){
  return currentDuty;
}

float getSSRPower(){
  return ( currentDuty / 255.0 ) * 100;
}

void ssrTest(){
  // Turn off the SSR - duty cycle of 0
  SetSSRFrequency( 255 ); // test pulse
  delay(1000);
  SetSSRFrequency( 0 );
  delay(1000);
  SetSSRFrequency( 255 ); // test pulse
  delay(1000);
  SetSSRFrequency( 0 );
}

#endif