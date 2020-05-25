#ifndef pid_h
#define pid_h

// testing
// fastpid https://github.com/mike-matera/FastPID
// autopid https://github.com/r-downing/AutoPID/blob/master/examples/BasicTempControl/BasicTempControl.ino
// arduino pid library pid_v1 https://github.com/br3ttb/Arduino-PID-Library
// pidautotuner https://github.com/jackw01/arduino-pid-autotuner

#include <max31855.h>
#include <pidtune.h>
// #include <FastPID.h> 
#include <PID_v1.h> // https://github.com/br3ttb/Arduino-PID-Library

// [PIDTUNE] COMPLETE
// P 97.403
// I 3.142
// D 754.980

// 11.6979
// 0.0234
// 1468.5591
// da fuq?

float wantedTemp     = -1;
float currentDelta   = 0;
bool isCuttoff       = false;
bool isFanOn         = false;
float lastWantedTemp = -1;

bool DEBUG_pid       = false;

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
double Kp=3, Ki=0, Kd=1; // P+1
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
// PID myPID(&currentTempAvg, &pidduty, &wantedTemp, Kp, Ki, Kd, DIRECT);

void init_PID(){
  // float Kp=97.403, Ki=3.142, Kd=754.9, Hz=10;
  Serial.println("[PID] init");
  myPID.SetMode(AUTOMATIC);
  // myPid.SetOutputLimits();
  // if(!res) Serial.println("[ERROR] init FAILED (outputrange)");
  // if(myPID.err()) Serial.println("[ERROR] init FAILED (construct)");
}

void run_PID(){
  updateTemps();
  myPID.Compute();
  Setpoint = (double)wantedTemp;
  Input = (double)currentTempAvg;
  // Serial.print("-");
  // Serial.print(Output);
  SetRelayFrequency(Output);
}

// void MatchTemp_init(int temp){
//   init_pid();
//   wantedTemp = temp;
// }

void MatchTemp()
{
  // Serial.print(".");
  if(wantedTemp > 0)  run_PID();
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