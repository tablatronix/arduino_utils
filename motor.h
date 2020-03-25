#ifndef motor_h
#define motor_h

bool motorChange = false;
int motorDir     = 0; // 0=off, 1=Forward, 2=reverse
int motorpinA    = 3;
int motorpinB    = 4;
int motorInt     = 7;
int stallcnt = 0;

int motor_lastmicros = 0; 

void motor(int dir,int dur){
  Serial.print("[MOTOR]: ");
  if(dir==0){
    Serial.println("OFF");
    ioDeviceDigitalWriteS(switches.getIoAbstraction(), motorpinB, HIGH);
    ioDeviceDigitalWriteS(switches.getIoAbstraction(), motorpinA, HIGH);
  }
  else if(dir==1){
    Serial.println("F");
    ioDeviceDigitalWriteS(switches.getIoAbstraction(), motorpinB, LOW);
    ioDeviceDigitalWriteS(switches.getIoAbstraction(), motorpinA, HIGH);
    if(dur!=0){
      delay(dur);
      motor(0,0);
    } 
  }
  else if(dir==2){
    Serial.println("R");
    ioDeviceDigitalWriteS(switches.getIoAbstraction(), motorpinB, HIGH);
    ioDeviceDigitalWriteS(switches.getIoAbstraction(), motorpinA, LOW);
    if(dur){
      delay(dur);
      motor(0,0);
    }  
  }
}

void motorPulse(int dir,int count,int dur,int delayms){
  for(int i=0;i<count;i++){
    motor(dir,dur);
    delay(delayms);
  }
}

void onFeedback(uint8_t pin, bool heldDown) {
  // first time in a while reset stallcount
  if(micros()-motor_lastmicros > 500000 ){
    stallcnt = 1; // reset stall check, timeout
  }

  Serial.println("[MOTOR]: OVERCURRENT DETECTED: " + (String)stallcnt + " " + (String)((micros()-motor_lastmicros)/10000) + "ms" );
  // Serial.println(heldDown ? "STALLED" : " ignoring");

bool stalldetect    = true;
int stalllimit      = 5;
bool autoreverse    = true;
int autoreversepause = 200;
int autoreversetime = 1000;

  if(stalldetect && (stallcnt >= stalllimit) && motorDir>0){
    Serial.println("[MOTOR]: STALLED");    
    stallcnt = 0;
    motor(0,0); // motor off
    if(autoreverse){
      delay(autoreversepause);
      motor(motorDir == 1?2:1,autoreversetime); // motor Back
      motor(0,0);
    }
    motorDir = 0;
  } else stallcnt++;

  motor_lastmicros = micros();
}

void motorTest(){
  int dur = 1000;
  motor(0,0); 
  motor(1,dur);
  delay(500);
  motor(2,dur);
  motor(0,0);
}

#endif