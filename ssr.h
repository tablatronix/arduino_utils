#ifndef ssr_h
#define ssr_h
#define RELAYPIN  16   // relay control SSR PWM

float currentDuty = 0;

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
  currentDuty = ((float)duty * power );
  currentDuty = constrain( round_f( currentDuty ), 0, 255);
  currentDuty = 255-currentDuty; // invert
  currentDuty = abs(currentDuty);

  // Write the clamped duty cycle to the RELAYPIN GPIO    
  analogWrite( RELAYPIN, currentDuty );
  
  // #ifdef DEBUG
  if(duty!=currentDuty){
    Serial.println("\n[SetSSRFrequency] " + (String)currentDuty);
    if(duty<1) Serial.println("[SSR]: OFF");
    // #ifdef DEBUG
    Serial.print("[SSR] Duty Cycle: ");
    Serial.println( "aw: " + (String)currentDuty + " " + String( ( currentDuty / 256.0 ) * 100) + "%" +" P: " + String( round_f( power * 100 )) + "%" );
    // #endif
    }
}

void SetRelayFrequency(int duty){
  SetSSRFrequency(duty);
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