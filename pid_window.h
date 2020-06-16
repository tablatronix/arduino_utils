#ifndef pid_window_h
#define pid_window_h

#include <max31855.h>
#include <pidtune.h>
// #include <FastPID.h> 
#include <PID_v1.h> // https://github.com/br3ttb/Arduino-PID-Library

uint16_t fullPowerPeriod = 15000; // full power startup pulse period
bool fullPowerStartup = true; // enable full power period

int long pidTimerStart = 0;
bool pidRunning        = false; // flag is pid running, used for init start etc.

float wantedTemp     = -1;
float currentDelta   = 0;
bool isCuttoff       = false;
bool isFanOn         = false;
float lastWantedTemp = -1;

bool DEBUG_pid       = false;

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

void pid_reset_I(){
  myPID.SetTunings(Kp, 0, Kd);
}

void pid_preset_I(){
  myPID.SetTunings(Kp, Ki, Kd);
}

void init_PID(){
  // float Kp=97.403, Ki=3.142, Kd=754.9, Hz=10;
  Serial.println("[PID] init");
  Serial.println("[PID] PID : " + (String)Kp + " " + (String)Ki + " " + (String)Kd);
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(0,250); // depending on the ssr freq, low values do nothing < 5%, test this, even 1% will slowly add thermal over time
  // myPID.SetSampleTime(120);
  // if(!res) Serial.println("[ERROR] init FAILED (outputrange)");
  // if(myPID.err()) Serial.println("[ERROR] init FAILED (construct)");
}

void pidStart(){
  pidTimerStart = millis();
  if(fullPowerStartup){
    myPID.SetMode(MANUAL);
    Output = 250; // output never returns to normal !!!!
  }
  pidRunning = true;
}


void run_PID(){
  if(!pidRunning) pidStart();
  // updateTemps(); // this is too close to the PID freq
  // make sure temps are updated before calling run pid, but do not run close to the same frequency
  // this will cause oscillations due to noise induced inthe TC for example
  Setpoint = (double)wantedTemp;
  Input = (double)currentTempAvg;

  // reset integrator at 50%
  if(Setpoint > (wantedTemp*0.50)){
    pid_reset_I();
  }
  else pid_preset_I();

  myPID.Compute();

  // why is PID doing effort after setpoint
  if(Input > Setpoint && (Output > 1)){
    Serial.println("[PID] WTF PID");
  }
  // Serial.print("-");
  // Serial.print(Output);
  if(fullPowerStartup){
    if(millis()-pidTimerStart < fullPowerPeriod){
    }
    else if(myPID.GetMode() == MANUAL){
      Output = 0;
      myPID.SetMode(AUTOMATIC);
    }
  }

  setSSR(Output);
}

void stop_PID(){
  wantedTemp = 0;
  setSSR(0);
  pidRunning = false;
}

void MatchTemp()
{
  // Serial.print(".");
  if(wantedTemp > 0)  run_PID();
  return;
}  

#endif