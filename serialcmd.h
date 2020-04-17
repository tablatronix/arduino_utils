#ifndef serialcmd_h
#define serialcmd_h

#include <motor.h>
#include <ssr.h>
#include <pid.h>
#include <reflow.h>

// HardwareSerial DebugOut = Serial;
// HardwareSerial DebugOut(0);
// Print Debug = &Serial;
Stream &DebugOut = Serial1;

#ifdef DEBUG
bool debug_serialcmd = true;
#else
bool debug_serialcmd = true;
#endif

bool DEBUG_SERIALCMD = debug_serialcmd;

  // SERIAL CONSOLE
#define MAX_NUM_CHARS 32 // maximum number of characters read from the Serial Monitor
char cmd[MAX_NUM_CHARS];       // char[] to store incoming serial commands
boolean cmd_complete = false;  // whether the command string is complete

void recvChar(void) {
  static byte index = 0;
  while (Serial.available() > 0 && cmd_complete == false) {
    char rc = Serial.read();
    if (rc != '\n') {
      if(index < MAX_NUM_CHARS) cmd[index++] = rc;
    } else {
      cmd[index] = '\0'; // terminate the string
      index = 0;
      cmd_complete = true;
      Serial.print("received '"); Serial.print(cmd); Serial.println("'");
    }
    delay(0);
  }
}

/*
 * Prints a usage menu.
 */
const char usageText[] PROGMEM = R"=====(
Usage:
m <n> : select mode <n>

b+    : increase brightness
b-    : decrease brightness
b <n> : set brightness to <n>

s+    : increase speed
s-    : decrease speed
s <n> : set speed to <n>

c 0x007BFF : set color to 0x007BFF

Have a nice day.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
)=====";

void printUsage() {
  Serial.println((const __FlashStringHelper *)usageText);
}

// @todo switch to h file funcs
void doMotor(int dir,int duration){
  motorDir = dir;
  motorChange = true;
}

void process_command(){
  if (strncmp(cmd,"f ",2) == 0) {
    uint32_t arg = (uint32_t)atoi(cmd + 2);
    if(DEBUG_SERIALCMD) DebugOut.print(F("Set value to: ") );
    if(DEBUG_SERIALCMD) DebugOut.println(arg);
    fanAV((int)arg);
    fanBV((int)arg);
  }

  if (strncmp(cmd,"f1 ",3) == 0) {
    uint32_t arg = (uint32_t)atoi(cmd + 2);
    if(DEBUG_SERIALCMD) DebugOut.print(F("Set voltage to: ") );
    if(DEBUG_SERIALCMD) DebugOut.println(arg);
    if((int)arg == 1) arg = 500;
    fanAVolts((int)arg);
  }

  if (strncmp(cmd,"f2 ",3) == 0) {
    uint32_t arg = (uint32_t)atoi(cmd + 2);
    if(DEBUG_SERIALCMD) DebugOut.print(F("Set B voltage to: ") );
    if(DEBUG_SERIALCMD) DebugOut.println(arg);
    if((int)arg == 1) arg = 500;    
    fanBVolts((int)arg);
  }

  if (strncmp(cmd,"f3 ",3) == 0) {
    uint32_t arg = (uint32_t)atoi(cmd + 2);
    if(DEBUG_SERIALCMD) DebugOut.print(F("Set C eanble to: ") );
    if(DEBUG_SERIALCMD) DebugOut.println(arg);
    fanCEnable((int)arg == 1);
  }

  if (strncmp(cmd,"f4 ",3) == 0) {
    uint32_t arg = (uint32_t)atoi(cmd + 2);
    if(DEBUG_SERIALCMD) DebugOut.print(F("Set D eanble to: ") );
    if(DEBUG_SERIALCMD) DebugOut.println(arg);
    fanDEnable((int)arg == 1);
  }

  if (strncmp(cmd,"mo",2) == 0) {
    if(DEBUG_SERIALCMD) DebugOut.print(F("Motor OFF: ") );
  }

  if (strncmp(cmd,"mf",2) == 0) {
    uint32_t arg = (uint32_t)atoi(cmd + 2);
    if(DEBUG_SERIALCMD) DebugOut.print(F("Motor ON F: ") );
    if(DEBUG_SERIALCMD) DebugOut.println(arg);
    doMotor(1,(int)arg);
  }

  if (strncmp(cmd,"mr",2) == 0) {
    uint32_t arg = (uint32_t)atoi(cmd + 2);
    if(DEBUG_SERIALCMD) DebugOut.print(F("Motor ON R: ") );
    if(DEBUG_SERIALCMD) DebugOut.println(arg);
    doMotor(2,(int)arg);
  }

  if (strncmp(cmd,"b ",2) == 0) {
    uint32_t arg = (uint32_t)atoi(cmd + 2);
    if(DEBUG_SERIALCMD) DebugOut.print(F("Set freq to: ") );
    if(DEBUG_SERIALCMD) DebugOut.println(arg);
    // analogWriteFreq(arg); // confirm ?
  }

  if (strncmp(cmd,"A",1) == 0) {
    uint8_t arg = (uint8_t)atoi(cmd + 2);
    if(DEBUG_SERIALCMD) DebugOut.print(F("Abort") );
    if(DEBUG_SERIALCMD) DebugOut.println(arg);
  }

  if (strncmp(cmd,"R",1) == 0) {
    uint8_t arg = (uint8_t)atoi(cmd + 2);
    if(DEBUG_SERIALCMD) DebugOut.print(F("REFLOW") );
    if(DEBUG_SERIALCMD) DebugOut.println(arg);
  }

  if (strncmp(cmd,"C",1) == 0) {
    uint8_t arg = (uint8_t)atoi(cmd + 2);
    if(DEBUG_SERIALCMD) DebugOut.print(F("COOLDOWN") );
    coolDown();
  }

  if (strncmp(cmd,"S",1) == 0) {
    uint8_t arg = (uint8_t)atoi(cmd + 2);
    if(DEBUG_SERIALCMD) DebugOut.print(F("COOLDOWN") );
    sleep();
  }

  if (strncmp(cmd,"B ",2) == 0) {
    uint8_t arg = (uint8_t)atoi(cmd + 2);
    if(DEBUG_SERIALCMD) DebugOut.print(F("BUTTON") );
    if(DEBUG_SERIALCMD) DebugOut.println(arg);
  }

  if (strncmp(cmd,"X",1) == 0) {
    uint8_t arg = (uint8_t)atoi(cmd + 2);
    if(DEBUG_SERIALCMD) DebugOut.print(F("REBOOT") );
    if(DEBUG_SERIALCMD) DebugOut.println(arg);
    ESP.restart();
  }

  if (strncmp(cmd,"ssr ",4) == 0) {
    uint32_t arg = (uint32_t)atoi(cmd + 4);
    if(DEBUG_SERIALCMD) DebugOut.print(F("[CMD] [SSR] duty:") );
    if(DEBUG_SERIALCMD) DebugOut.println(arg);
    SetSSRFrequency((int)arg);
  }

  if (strncmp(cmd,"temp ",5) == 0) {
    uint32_t arg = (uint32_t)atoi(cmd + 5);
    if(DEBUG_SERIALCMD) DebugOut.print(F("[CMD] [TEMP] set:") );
    if(DEBUG_SERIALCMD) DebugOut.println(arg);
    wantedTemp = (int)arg;
    Serial.println("[ERROR] START PID : " + (String)wantedTemp);
    MatchTemp();
  }

  if (strncmp(cmd,"a0 ",3) == 0) {
    uint32_t arg = (uint32_t)atoi(cmd + 3);
    if(DEBUG_SERIALCMD) DebugOut.print(F("[CMD] [ANALOG] duty:") );
    if(DEBUG_SERIALCMD) DebugOut.println((int)arg);
    analogWrite(16,(int)(arg));
  }

  if (strncmp(cmd,"debug",5) == 0) {
    debug_serialcmd = !debug_serialcmd;
    if(DEBUG_SERIALCMD) DebugOut.print("[CMD] [DEBUG]:" + (String)(debug_serialcmd?"ON":"OFF"));
  }

  if (strncmp(cmd,"pidtune ",8) == 0) {
    uint32_t arg = (uint32_t)atoi(cmd + 8); 
    targetInputValue = (float)(int)arg;
    if(DEBUG_SERIALCMD) DebugOut.print("[DEBUG]: AUTOTUNING PID " + (String)arg);
    init_pidtune();
  }


  cmd[0] = '\0';         // reset the commandstring
  cmd_complete = false;  // reset command complete 
}

void serialprocess(){
  recvChar(); // read serial comm
  if(cmd_complete) {
    process_command();
  }
}

String inputString;         // a string to hold incoming data
bool stringComplete = false; // whether the string is complete
bool echo = false;

char inData[32]; // Allocate some space for the string, cmd buffer
char inChar= 0; // Where to store the character read
byte inidx = 0; // Index into array; where to store the character

void emptyBuffer(){
    // clear buffer
    inidx = 0;
    inData[0] = (char)0;
    inputString = "";
}


void parseSerial(Stream & consolePort,char *cmdStr){
    DebugOut.println("---------------------------");
    DebugOut.println("Start Parsing");
        // Serial.println(Serial.peek());
     // inputString = consolePort.readStringUntil('\n');
     DebugOut.println(cmdStr);
     consolePort.println(cmdStr);
     // cmd = cmdStr;
     strncpy(cmd, cmdStr, 32);
     process_command();
}

// FF FD 3 FF FD 1 mode raw character
// FF FE 3 FF FE 1 mode line
// 
void readSerial(Stream & consolePort){
    while (consolePort.available() > 0) // Don't read unless
                                   // there you know there is data
    {
        DebugOut.println("Bytes avail: " + (String)consolePort.available());
        inChar = consolePort.read(); // Read a character
        if(inChar == 0x00) DebugOut.println("Char: NULL");
        if(inidx < 32 && inChar != 0x00) // buffer not full and not null
        {
            inputString += inChar;
            inData[inidx] = inChar; // Store it
            inidx++; // Increment where to write next
            
            // debugging
            if(inChar == 0x0A) DebugOut.print("Char: LF ");
            else if(inChar == 0x0D) DebugOut.print("Char: CR ");
            else DebugOut.print("Char: " + (String)inChar + " ");
            DebugOut.print(inChar,HEX);
            DebugOut.print(" ");
            DebugOut.print(inChar,OCT);
            DebugOut.println("");
    
            if(echo && (inChar > 0x1F)) consolePort.print(inChar);

            // newline detected
            if (inChar == 0x0D || inChar == 0x0A){
                // ignore LF following CR
                if(consolePort.available() > 0 && consolePort.peek() == 0x0A){
                    DebugOut.println("Ignoring LF");
                    // consolePort.flush(); // @todo I think this works on incoming? not sure, NOPE
                    consolePort.read(); // eat LF from input buffer
                }
                inData[inidx++] = '\0'; // Null terminate the string
                DebugOut.println("LINE RECEIVED");
                // DebugOut.println(inputString);
                // DebugOut.flush();
                // if(echo) consolePort.println("");
                // handle string
                parseSerial(consolePort,inData);
                emptyBuffer();
            }
        }
        else if(inChar != 0x00){
            consolePort.println("ERROR: CMD BUFFER FULL");
            emptyBuffer();    
        }
    }
}


// class DualWriter : public Print{
//     public:
//       DualWriter( Print &p_Out1, Print &p_Out2 ) : OutputA( p_Out1 ), OutputB( p_Out2 ){}
       
//       size_t write( uint8_t u_Data )
//         {
//           OutputA.write( u_Data );
//           OutputB.write( u_Data );
//           return 0x01;
//         }
//     protected:
//       Print &OutputA;
//       Print &OutputB;
//   }; 

#endif