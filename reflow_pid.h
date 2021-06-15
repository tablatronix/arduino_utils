#ifndef reflow_pid_h
#define reflow_pid_h

// remove all reflowoven methods, use for all SSR pid control generic
//
  
#include "InterpolationLib.h"

unsigned long stateStartMS = 0; // state start timers

// app states
int reflowState = 0; // @todo add enum state, maybe queue
int reflowGraphCounter = 0; // counter period for graphing graphInterval

// temperature vars
int hotTemp  = 70; // C burn temperature for HOT indication, 0=disable
int coolTemp = 48; // C safe temperature for HOT indication, 0=disable
int lowTemp  = 30; // C of TC warmed than typical CJ
int shutDownTemp = 210; // degrees C
int fanTemp = lowTemp+5; // cooldown complete fan off low temp

bool enableThermalCheck = false;


int reflowStepTaskID = -1;
int PIDTaskID = -1;


// cancel a task, pass ref?
// add ojbect to handle tasks and ids
int cancelTask(uint16_t taskid){
  Serial.println("[TASK] cancel task: " + (String)taskid);
  if(taskid >-1){
    taskManager.cancelTask(taskid);
    return -1;
  }
  return taskid;
}

// TIMING
void stateTimerReset(){
  stateStartMS = millis();
}

// return millis since start of state
int getStateDuration(){
  return millis()-stateStartMS;
}


// REFLOW STATES
#define RS_IDLE     0
#define RS_PREHEAT  1 // wait for reflow sync, pid or no pid
#define RS_START    2 // for setup of reflow curve and any pre reflow tasks
#define RS_REFLOW   3 // actual reflow
#define RS_PEAK     4 // run safety, logging for reflow etc,
#define RS_COOL     5 // do cooldown
#define RS_ABORT   999 // special mode for abort, stay in abort until reset!

String TITLE = ""; //  Top left
void SetTitle(String str){
  TITLE = str;
}

void setReflowState(int state){
  reflowState = state;
}

int getReflowState(){
  return reflowState;
}

void reflow(){
  // setPasteId = 0;
  // doPasteReflow();
}

void reflowAbort(){
  setReflowState(RS_ABORT);
  // abort immediate for safety
  setSSR(0);
  SetTitle("ABORTED");
}

void userAbort(){
  if(getReflowState() != RS_IDLE){
    reflowAbort();
  }
  // pressing any button during reflow , add stop buttons
  // push encoder
  // spin encoder fast
  // door open detect ( if door input present )
}

// quick dirty tc check, 
// if heating, and temp not changing enough, and 10s since start of temp call
// will have to add an actual tracking mechanism fo rrate of change expected and change since as startState is used for entire profile
bool thermalCheck(){
  if(!enableThermalCheck) return true;
  if(wantedTemp > 0 && getSSRDuty() > 3 && getTCDev()<0.10 && (millis()-stateStartMS > 10000)){
    Serial.println("THERMAL RUNAWAY");
    // setERRORSTR("THERMAL RUNAWAY");
    SetTitle("THERMAL RUNAWAY");
    return false;
  }
  return true;
}

// PID
bool pidrun = false; // flag to run pid on loop
void doPidRun(){
  pidrun = true;
}

void doPidStart(){
  if(PIDTaskID == -1){
    pidStart();
    // SSRFan(true); 
    PIDTaskID = taskManager.scheduleFixedRate(100, doPidRun); // RUN PID 10HZ 
  }
}

void doPidStop(){
  PIDTaskID = cancelTask(PIDTaskID);
  stop_PID();
}

// set static temp skip reflow curves
void setTemp(){
  // set a temperature;
  doPidStart();
  setReflowState(RS_REFLOW);
}

//preheat

// wait for preheat temp reached to start reflow, 
// this not only eases and shortens the graph
// but helps get thermal inertia and line up with curve
// this way we delay start the curve on temp instead of trying to match temp up to curve
int preheattemp = 50; // this has to match first temp, figure a better way to make compatible with all profiles

void preHeatSetup(){
  SetTitle("PREHEAT");
  // SetTitleC((String)preheattemp + "`c");
}

void preHeat(){
  if(reflowState == RS_IDLE){
    Serial.println("[START] preheat"); 
    wantedTemp = preheattemp;
    preHeatSetup();
    setReflowState(RS_PREHEAT);
    stateTimerReset();    
    doPidStart();
  }
  else if(reflowState == 1){
    if(currentTempAvg > preheattemp){
      setReflowState(RS_START);
      // @todo add timeout if sitting in state too long
      // add timeout to stabilize
    }
  }
}

/**
 * PASTE
 ******************************************
 */

double pasteTime[7] = { 1, 90, 180, 210, 240, 270, 330 }; // time
double pasteTemp[7] = { 50, 90, 130, 138, 165, 138, 27 }; // value

// double pasteTime[6] = { 1,   75, 130, 180, 210, 250 }; // time
// double pasteTemp[6] = { 50, 150, 175, 210, 210, 50 }; // temps
double delta[7]     = { 0, 80, 45, 0, 70, 0,  25 }; // value 
double slope[7]     = { 0, 1.3, .75, 0, 1.4, 0,  3.9 };

  //  double xValues[7] = { 1, 90, 180, 210, 240, 270, 300 }; // time
  // double yValues[7] = { 50, 90, 130, 138, 165, 138, 25 }; // value   
const int numZones = 7;
int peakIDX = 4;
int maxTime  = pasteTime[numZones-1];
int maxTemp  = pasteTemp[peakIDX];
int peakTime = pasteTime[peakIDX];
int rampTime = pasteTime[peakIDX-1];
int extractTime = peakTime-10;
int coolTime = peakTime+20; // peak duration - 10, how early to start cooling @todo test once using door

uint16_t reflowZone = 0;
bool drawWantedCurve = true;


int getMaxTemp(){
  return maxTemp; // max temp plus padding
}

int getMaxTime(){
  return maxTime; // max temp plus padding
}

double getPasteTime(int id,int zone){
  return pasteTime[zone];
}

// temp structure
double getPasteTemp(int id,int zone){
  return pasteTemp[zone];
}

double getActualWanted(){
  return Interpolation::SmoothStep(pasteTime, pasteTemp, numZones, getStateDuration()/1000);  
}

double getZoneRate(){
  double zoneTempP = getPasteTemp(0,0);
  double zoneTimeP = getPasteTime(0,0);  
  double zoneTemp = getPasteTemp(0,1);
  double zoneTime = getPasteTime(0,1);
  double zoneSlope = zoneTemp-zoneTempP/zoneTime-zoneTimeP;
  return zoneSlope; // 1.3
}

void reflowZoneEnd(){
    Serial.println("[REFLOW] DONE");
    // continue cooling, slow graph rate to log ? show some other way
    setReflowState(RS_COOL);
    reflowStepTaskID = cancelTask(reflowStepTaskID);
    stop_PID();
    // SetTitleC("0`c"); // @todo replace with cooling rate, or estimate time left?
    reflowZone = 0;
    // cool(false);
    SetTitle("COOLING");
    // updateTitle();
}

void reflowNextZone(){
  Serial.println("ms:" + (String)millis());
  if(reflowZone == numZones-1){
    reflowZoneEnd();
    // continue cooling, slow graph rate to log ? show some other way
    stop_PID();
  }
  else Serial.println("[REFLOW] SET ZONE: " + (String)reflowZone);
  wantedTemp = pasteTemp[reflowZone];
  reflowZone++;
  // SetTitleC((String)(int)round((wantedTemp))+"`c");
}

int curveLookahead = 10; // seconds, start matching setpoint this early // check CRASH divide by 0!
int curveSamplePeriod = 5; // seconds, period to interpolate by

void reflowNextRate(){
  Serial.println("[REFLOW] ms:" + (String)getStateDuration());

  // reflow done
  if(reflowZone > (coolTime/curveSamplePeriod) || reflowState == 0){
    reflowZoneEnd();
    return;
  }
  else Serial.println("[REFLOW] SET ZONE: " + (String)reflowZone + " s:" + (String)(reflowZone*curveSamplePeriod));

  // skip first temp, no offset
  if(reflowZone==0){
    curveLookahead = 1;
  }
  // 180-10/5 = 34
  // @todo if profile does not have peak plateu, will have to ajust peak/2
  else if(reflowZone >= (rampTime)/curveSamplePeriod){
      // change pid tunings to agressive for peak
      Serial.println("[REFLOW] PEAK REFLOW");
      Serial.println("[REFLOW] *********************************");
      pid_peak(); // this gets called repeatedly, @fix only run once.
  }
  else curveLookahead = 10; // fix to not have to restore, use temp local var

  // get adjusted wanted temp, using lookahead, adjust for sampleperiod
  // if sample period is too small, or rate changed to fast, pid tunings will not work the same
  // maybe have to adjust tunings if changing curveSampleRate, which is going to be a problem..
  // but either way this always interpolates more points than profile array, 
  // and we cannot change rate faster than 20 seconds of dead time anyway, 5-10s ideal, see how we can adjust , we still have 10% power reserve etc.
  wantedTemp = Interpolation::SmoothStep(pasteTime, pasteTemp, numZones, (reflowZone*curveSamplePeriod)+curveLookahead); 
  
  Serial.println("[REFLOW] " + (String)(int)round((wantedTemp))+" / " + (String)(int)round(getActualWanted()));

  // Serial.println(" " + (String)wantedTemp);
  reflowZone++;
  // SetTitleC((String)(int)round((wantedTemp))+" / " + (String)(int)round(getActualWanted()));
}


void doPasteGraph(){
  Serial.println("[START] doPasteReflow");
  // reflow_graph();

 int maxtime = getMaxTime(); // max time frame for reflow, or make sure last point is not drawn huh ?
 int maxtemp = getMaxTemp(); // max value
 int ssize = maxtime;

 maxtemp+=10; // top margin for overshoot and padding

  double y;
  for(int i=0;i<maxtime;i++){
    y = Interpolation::SmoothStep(pasteTime, pasteTemp, numZones, i);
    // Serial.println("y: " + (String)y);
    // addPointSet(5,i,y,ssize,maxtemp,0);
  }

  if(drawWantedCurve){
  // draw actual wanted curve, lead adjust
  
  for(int i=0;i<maxtime/curveSamplePeriod;i+=curveSamplePeriod){
    // line up first temp, no offset
    if(i==0){
      curveLookahead = 1;
    }
    else if(i > (rampTime)/curveSamplePeriod){
      // Serial.println("[REFLOW] PEAK REFLOW");
      // curveSamplePeriod = 2;
      // curveLookahead = 5;
    }
    else curveLookahead = 10;

    y = Interpolation::SmoothStep(pasteTime, pasteTemp, numZones, (i*curveSamplePeriod)+curveLookahead);   
    // addPointSet(1,i*curveSamplePeriod,y,ssize,maxtemp,1);
    }
  }
}

void doPasteReflow(){
  // resetGraphLines();
  doPasteGraph();
  SetTitle("REFLOW");
  setReflowState(RS_REFLOW);
  // min sample rate
  // for(int i = 0; i < numZones; i++){
  //   // Serial.print(Interpolation::CatmullSpline(xValues,yValues, numZones, i));
  //   // addPointSet(5,xValues[i],yValues[i],ssize,maxtemp);
  //   Serial.println("x2 timer " + (String)((int)round(xValues[i])*1000));
  //   taskManager.scheduleOnce(xValues[i], reflowNextZone,TIME_SECONDS);
  // }
  reflowStepTaskID = taskManager.scheduleFixedRate(curveSamplePeriod*1000, reflowNextRate);
  Serial.println("[REFLOW] step task created: " + (String)reflowStepTaskID);
  reflowGraphCounter = 0;
  // (int)getStateDuration()/1000;  // reset graph counter for main loop
  // unsigned long preheatendtime = getStateDuration(); calculate forward in slope where temp is at
  stateTimerReset();

  // reflowZone = 6;
  // reflowNextZone();
  reflowNextRate();
  doPidStart();
}

// reflowstates
// 0 - off
// 1 - preheat/warmup
// 2 - reflow begin/setup
// 3 - reflow running
// 
// entrypoints
// preheat()
// doPasteReflow()
// doPidStart() - set temp, skip reflow curves

void process_reflow(){  
  if(reflowState == RS_IDLE){
    return;
  }
  if(reflowState == RS_ABORT){
    // stop everything!
    doPidStop();
    reflowStepTaskID = cancelTask(reflowStepTaskID); 
    SetTitle("ABORTED");
    // if(coolOnAbort) cool(false); // cooldown on abort
    delay(1000);
    setReflowState(RS_IDLE); // auto switch back to idle, @todo add acknowlege UI first
    return;
  }

  if(reflowState == RS_PREHEAT){
    preHeat();
  }
  if(reflowState == RS_START){
    doPasteReflow();
  }

  if(reflowState > RS_IDLE && reflowState < RS_COOL){
    if(pidrun){
      pidrun = false;
      if(!thermalCheck()){
        doPidStop();
        // abort_reflow();
      } else run_PID();
    }

  }
}

void startReflow(){
  // fansOff();
  preHeat();
}


#endif