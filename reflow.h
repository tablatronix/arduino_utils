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

int filteredId = 1; // -1 off

int setFilterId(int id){
  filteredId = id;
}

void SetTitle(String str){
  TITLE = str;
}

// DOOR
int doorAbortTime = 50000;
uint8_t doorAbortTaskID;

void doorCancelAbort(){
	if(doorAbortTaskID >0) taskManager.cancelTask(doorAbortTaskID);
	doorAbortTaskID = 0;
}

void doorAbort(){
  //@todo set state, so stall can cancel scheduled aborts
  Serial.println("[TASK] Door Abort");
  motor(0,0);
}

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