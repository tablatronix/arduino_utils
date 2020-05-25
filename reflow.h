#ifndef reflow_h
#define reflow_h

// strings
// String TITLE = "TITLE";
String OPT1  = "OPT 1";
String OPT2  = "OPT 2";
String STATUSLINE  = "Status Line";
String DETAILS = "Subheading";
String SUBDATA = "888";

String TITLE = "PRESET";
// String OPT1 = "FAN 1";
// String OPT2 = "FAN 2";

int filteredId = -1; // -1 off

// fan aliases
// fanA 12v fume exhaust
// fanB 5v  cool vent

/**
 * TITLE TITLEB TITLEC
 * BODY/GRAPH
 * FOOTERA FOOTERB FOOTERC FOOTERD
 */

// set filter id, filters graph only filter line will display
int setFilterId(int id){
  filteredId = id;
}

void SetTitle(String str){
  TITLE = str;
}

void SetTitleB(String str){
  // DETAILS = str;
}

void SetTitleC(String str){
  // SUBDATA = str;
}

void SetFooterA(String str){
  // FOOTERA = str;
}

void SetFooterB(String str){
  // FOOTERCENTER = str;
}

void SetFooterBSCROLL(String str){
  // SCROLL = true;
  // STATUSLINE = str;
}

void SetFooterC(String str){
  OPT1 = str;
}

void SetFooterD(String str){
  OPT2 = str;
}

// DOOR
int doorAbortTime = 50000; // time we expect door full operations to take, will disable door motor after this time
uint8_t doorAbortTaskID; // timer task

void doorCancelAbort(){
	if(doorAbortTaskID >0) taskManager.cancelTask(doorAbortTaskID);
	doorAbortTaskID = 0;
}

void doorAbort(){
  //@todo set state, so stall can cancel scheduled aborts
  Serial.println("[TASK] Door Abort");
  motor(0,0);
}

/**
 * open door, add failsafe stop timer
 */
void doorOpen(){
  motorDir = 1;
  motorChange = true;
  doorAbortTaskID = taskManager.scheduleOnce(doorAbortTime, doorAbort);  
}

void doorClose(){
  motorDir = 2;
  motorChange = true;
  doorAbortTaskID = taskManager.scheduleOnce(doorAbortTime, doorAbort);  
}

// MACROS

void extract(){

}

void coolDown(){
  doorOpen();
  fanB(100); // outake 100%
}

void standby(){
  doorClose();
  fanB(0); // outake 100%
  fanA(0); // outake 100%
  SSRFan(true); // ssr fan full
}

void sleep(){
  doorClose();
  fanB(0); // outake 100%
  fanA(0); // outake 100%
  SSRFan(false); // ssr fan full
}

void reflow(){

}

void reflowabort(){

}

void reboot(){

}

void cancel(){

}

void preheat(){

}

void hold(){

}

void reset(){

}

void save(){

}

void load(){

}


#endif