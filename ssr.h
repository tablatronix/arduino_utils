#ifndef ssr_h
#define ssr_h
#define RELAYPIN  16   // relay control SSR PWM

#ifdef DEBUG
bool DEBUG_ssr = true;
#else
bool DEBUG_ssr = false;
#endif

float currentDuty = 0;
bool invertDuty = true; // invert logic vcc range

int round_f(float x){
  return (int)round(x);
}

float round_f_2(float x){
  return round(x);
}

void ssr_init(){
  pinMode( RELAYPIN, OUTPUT );
  digitalWrite(RELAYPIN,HIGH);

  #ifdef ESP8266
  analogWriteRange(255); // esp8266 
  // analogWriteFreq(120); // min 100hz
  #elif defined(ESP32)
  #endif 
}

// This is where the SSR is controlled via PWM
void SetSSRFrequency( int duty,int power =1)
{
  // calculate the wanted duty based on settings power override
  duty = ((float)duty * power ); // power adjust
  duty = constrain( round_f( duty ), 0, 255); // round and clamp
  duty = abs(duty); // convert to whole

  // Write the clamped duty cycle to the RELAYPIN GPIO    
  analogWrite( RELAYPIN, invertDuty ? 255-duty : duty );
  
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

void SetRelayFrequency(int duty){
  SetSSRFrequency(duty);
}

float getSSRDuty(){
  return currentDuty;
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