#ifndef motor_h
#define motor_h

bool DEBUG_motor = true;

bool motorChange = false;
int motorDir     = 0; // 0=off, 1=Forward, 2=reverse
int motorpinA    = 3; // PCF PIN
int motorpinB    = 4; // PCF PIN
int motorInt     = 7; // PCF PIN

int stallcnt         = 0; // counter for stall
int stallReset       = 500000; // reset stall count duration micros
bool stalldetect     = true; // use interupt to detect stalls
int stalllimit       = 5;    // stall limit, num interrupts for stall detect
bool autoreverse     = true; // autoreverse on stall
int autoreversepause = 200; // autoreverse delay
int autoreversetime  = 1000; // autoreverse duratiton

int motor_lastmicros = 0; 

#define MOTORDIR_OFF 0
#define MOTORDIR_F 1
#define MOTORDIR_R 2

void motor(int dir,int dur){
  if(DEBUG_motor)Serial.print("[MOTOR]: ");
  if(dir==MOTORDIR_OFF){
    if(DEBUG_motor)Serial.println("OFF");
    ioDeviceDigitalWriteS(switches.getIoAbstraction(), motorpinB, HIGH);
    ioDeviceDigitalWriteS(switches.getIoAbstraction(), motorpinA, HIGH);
  }
  else if(dir==MOTORDIR_F){
    if(DEBUG_motor)Serial.println("F");
    ioDeviceDigitalWriteS(switches.getIoAbstraction(), motorpinB, LOW);
    ioDeviceDigitalWriteS(switches.getIoAbstraction(), motorpinA, HIGH);
    if(dur!=0){
      delay(dur);
      motor(0,0);
    } 
  }
  else if(dir==MOTORDIR_R){
    if(DEBUG_motor)Serial.println("R");
    ioDeviceDigitalWriteS(switches.getIoAbstraction(), motorpinB, HIGH);
    ioDeviceDigitalWriteS(switches.getIoAbstraction(), motorpinA, LOW);
    if(dur){
      delay(dur);
      motor(0,0);
    }  
  }
}

void motorOff(){
  motor(MOTORDIR_OFF,0);
}

void motorPulse(int dir,int count,int dur,int delayms){
  for(int i=0;i<count;i++){
    motor(dir,dur);
    delay(delayms);
  }
}

void ICACHE_RAM_ATTR onFeedback(uint8_t pin, bool heldDown) {
  // first time in a while reset stallcount
  if(micros()-motor_lastmicros > stallReset ){
    stallcnt = 1; // reset stall check, timeout
  }

  // if(DEBUG_motor)
  Serial.println("[MOTOR]: OVERCURRENT DETECTED: " + (String)stallcnt + " " + (String)((micros()-motor_lastmicros)/10000) + "ms" );
  // if(DEBUG_motor)Serial.println(heldDown ? "STALLED" : " ignoring");

  if(stalldetect && (stallcnt >= stalllimit) && motorDir>MOTORDIR_OFF){
    if(DEBUG_motor)Serial.println("[MOTOR]: STALLED");    
    stallcnt = 0;
    motor(MOTORDIR_OFF,0); // motor off
    if(autoreverse){
      delay(autoreversepause);
      motor(motorDir == MOTORDIR_F?MOTORDIR_R:MOTORDIR_F,autoreversetime); // motor Back
      motor(MOTORDIR_OFF,0);
    }
    motorDir = MOTORDIR_OFF;
  } else stallcnt++;

  motor_lastmicros = micros();
}

void motorTest(){
  int dur = 1000;
  motorOff();
  motor(MOTORDIR_F,dur);
  delay(500);
  motor(MOTORDIR_R,dur);
  motorOff();
}

#endif