#ifndef pid_h
#define pid_h

#include <max31855.h>
#include <pidtune.h>
#include <FastPID.h>

// [PIDTUNE] COMPLETE
// P 97.403
// I 3.142
// D 754.980

float wantedTemp = -1;
float currentDelta = 0;
bool isCuttoff = false;
bool isFanOn = false;
float lastWantedTemp = -1;

bool DEBUG_pid = false;

// FastPID myPID;
  float Kp=0.01, Ki=0.5, Kd=0, Hz=10;
  int output_bits = 1;
  bool output_signed = false;
  FastPID myPID(Kp, Ki, Kd, Hz, output_bits, output_signed); 
  bool res = myPID.setOutputRange(0, 255);

void init_PID(){
  // float Kp=97.403, Ki=3.142, Kd=754.9, Hz=10;
  Serial.println("[PID] init");
  if(!res) Serial.println("[ERROR] init FAILED (outputrange)");
  if(myPID.err()) Serial.println("[ERROR] init FAILED (construct)");
}

void run_PID(){
  uint16_t setpoint   = (uint16_t)wantedTemp;
  uint16_t feedback   = (uint16_t)currentTempAvg;
  int16_t output = myPID.step(setpoint, feedback);
  Serial.print("-");
  Serial.print(output);
  SetRelayFrequency(output);
}

// void MatchTemp_init(int temp){
//   init_pid();
//   wantedTemp = temp;
// }

void MatchTemp()
{
  Serial.print(".");
  run_PID();
  return;
  float duty = 0;
  // float wantedTemp = 0;
  float wantedDiff = 0;
  float tempDiff = 0;
  float perc = 0;
  
    if(wantedTemp < 0) return;

    wantedDiff = (wantedTemp - lastWantedTemp );
    lastWantedTemp = wantedTemp;
    
    tempDiff = abs( currentTemp - lastTemp );
    lastTemp = currentTemp;
    
    // prevent perc less than 1 and negative
    perc = wantedDiff - tempDiff; // @todo fails if tempdiff is negative!
    // perc = constrain(wantedDiff - tempDiff,0,100);
    if(perc < 1) perc = 0;
    
    isCuttoff = false;  
    currentDelta = (wantedTemp - currentTemp);
    DEBUG_pid = currentDelta > 0;

if(DEBUG_pid){
    Serial.print( "[PID]" );
    
    // Serial.print( "T: " );
    // Serial.print( timeX );
    
    Serial.print( "  Current: " );
    Serial.print( currentTemp );
    
    Serial.print( "  Wanted: " );
    Serial.print( wantedTemp );
    
    Serial.print( "  T Diff: " );
    Serial.print( tempDiff  );
    
    Serial.print( "  W Diff: " );
    Serial.print( wantedDiff );
    
    Serial.print( "  Perc: " );
    Serial.print( perc );
  
    Serial.print( "  Delta: " );
    Serial.print( currentDelta );
}
  
  float base = 128;
  
  if ( currentDelta >= 0 )
  {
      base = 128 + ( currentDelta * 5 ); // 128? swing?
  }
  else if ( currentDelta < 0 )
  {
      base = 32 + ( currentDelta * 15 ); // 32? swing?
  }

  base = constrain( base, 0, 256 );

if(DEBUG_pid){
  Serial.print("  Base: ");
  Serial.print( base );
  // Serial.print( " -> " );
}
  
  duty = base + ( 172 * perc ); // 172?
if(DEBUG_pid){
  Serial.print("  Duty: ");
  Serial.print( duty );
  Serial.print( " -> " );
  Serial.println("");
}

  // if(duty<0)duty = 0;
  duty = constrain( duty, 0, 256 );

  // override for full blast at start
  // Serial.println("startFullBlast");
  // Serial.println(timeX);
  // Serial.println(CurrentGraph().reflowGraphX[1]);
  // if ( set.startFullBlast && (timeX < CurrentGraph().reflowGraphX[1]) ) duty = 256;
  // if ( set.startFullBlast && timeX < CurrentGraph().reflowGraphX[1] && currentTemp < wantedTemp ) duty = 256;
  SetRelayFrequency( duty );
}

#endif