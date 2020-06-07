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

// profiling open
// pulse 100% @ 23C
// start  0s 23C
// dead  20s 24C
// 63%   46s 67C
// end   63s 107C
// DPV = 84
// slope 1.3C/s from start
// slope 1.9C/s from dead
// Peak/OS 105s 133C (26C rise/42s) 1.6C/s coasting


// This stealth tuning applies to dead time processes. If the process has a lag, then this tuning still applies if the lag is 1/5 or less than the size of the dead time. The stealth tuning uses the process dead time and the process gain:
// controller gain = 0.3/(process gain)
// integral time = .42 * (process dead time)

// P = (A / PG) * (DT/TC)b
// I = TC / (A * (DC/TC)b ??
// D = TC * A(DT/TC)b

// P 1.22
// I 28.27
  
// P 1.76
// I 31.73
// D 13.15

// 1.57
// 37.35
// 17.05

// double Kp = 2;
// double Ki = 0;
// double Kd = 0;

// P = 300, I = 30 and D = 0.

// double Kp = 1;
// double Ki = .01;
// double Kd = 0;

// of Kp=1:Kd=0.5:Ki=0.025.


//Specify the links and initial tuning parameters
// PID HEATING Parallel independant
// double Kp=15, Ki=0, Kd=0; // P+1
// 0.01 for Kp and 0.0001 for Ki

double Kp=3, Ki=0, Kd=.4; // GOOD FOR NOW, slight over/undershoots depending on range
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
// PID myPID(&currentTempAvg, &pidduty, &wantedTemp, Kp, Ki, Kd, DIRECT);

void init_PID(){
  // float Kp=97.403, Ki=3.142, Kd=754.9, Hz=10;
  Serial.println("[PID] init");
  Serial.println("[PID] PID : " + (String)Kp + " " + (String)Ki + " " + (String)Kd);
  myPID.SetMode(AUTOMATIC);
  // myPid.SetOutputLimits(low,high); // depnding on tghe ssr freq, low value do nothing < 5%
  // myPID.SetSampleTime(120);
  // if(!res) Serial.println("[ERROR] init FAILED (outputrange)");
  // if(myPID.err()) Serial.println("[ERROR] init FAILED (construct)");
}

void run_PID(){
  // updateTemps(); // this is too close to the PID freq
  // make sure temps are updated before calling run pid, but do not run close to the same frequency
  // this will cause oscillations due to noise induced inthe TC for example
  Setpoint = (double)wantedTemp;
  Input = (double)currentTempAvg;
  myPID.Compute();
  // Serial.print("-");
  // Serial.print(Output);
  SetRelayFrequency(Output);
}

// void MatchTemp_init(int temp){
//   init_pid();
//   wantedTemp = temp;
// }

// ensure we reset everything and stop pid
void stop_PID(){
  wantedTemp = 0;
  SetRelayFrequency(0);
}

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