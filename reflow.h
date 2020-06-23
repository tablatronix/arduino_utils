#ifndef reflow_h
#define reflow_h

// #include <config.h>

#include <log.h>
#include <buttons.h>
#include <i2c_fans.h>

// #include <serialcmd.h>
// #include <telnet_cmd.h>
#include <ota.h>
#include <wifi_funcs.h>
#include <motor.h>
#include <ssr.h>
#include <max31855.h>

// #include <adafruit_tft.h>
#include <tft_spi_shared.h>

#include <tft_graph.h>
#include <io_utils.h>
#include <time_funcs.h>
#include <pid.h>

// ota bargraph
#include <BarGraph.h>
#define SCALECOLOR 0xFFFF
BarGraph bar0;

#define TFTROT 3

#define USENEOIND // use neopixel indicator
#ifdef USENEOIND
  #define NEOINDPIN 2
  #include <neoindicator.h>
  #include <neo_ind_accent.h>
#endif

// #define USENTC // use thermistor
#ifdef USENTC
  #include <ntc.h>
#endif

// symbol glyph font for icons
#include "SymbolMono18pt7b.h"
#define AA_FONT_SMALL &SymbolMono18pt7b
#define GFXFF 1

// #include "/Users/shawn/.platformio/lib/TFT_eSPI_ID1559/Fonts/GFXFF/FreeMono24pt7b.h"
// #include "FreeMono24pt7b.h"

#include "Free_Fonts.h"

#include "NotoSansBold15.h"
#include "NotoSansBold36.h"
#include "NotoSansMonoSCB20.h"

// The font names are arrays references, thus must NOT be in quotes ""
#define AA_FONT_MED NotoSansBold15
#define AA_FONT_LARGE NotoSansBold36
#define AA_FONT_MONO  NotoSansMonoSCB20 // NotoSansMono-SemiCondensedBold 20pt

#define GFXX18pt &FreeSans18pt7b
// #define GFXX18pt &FreeMono18pt7b

bool USEWIFI = true; // enabled wifi

Average<float> fps(10); // init stats for avg (samples)

// ICONS 
// states for fan animations, only 2 state atm
bool animStateA = true;
bool animStateB = false;
bool testIcons  = false; // set icons states randomly/Demo mode

unsigned long stateStartMS = 0; // state start timers

// app states
int reflowState = 0; // @todo add enum state, maybe queue
int reflowGraphCounter = 0; // counter period for graphing graphInterval

// temperature vars
int hotTemp  = 70; // C burn temperature for HOT indication, 0=disable
int coolTemp = 50; // C safe temperature for HOT indication, 0=disable
int lowTemp  = 30; // C of TC warmed than typical CJ
int shutDownTemp = 210; // degrees C
int fanTemp = lowTemp+5; // cooldown complete fan off low temp

bool enableThermalCheck = false;


// strings
// String TITLE = "TITLE";
String TITLE = ""; //  Top left
String OPT1  = ""; // BR A
String OPT2  = ""; // BR B
String STATUSLINE  = ""; // footer center
bool   SCROLL = false; // center is scroll
String DETAILS = ""; // top center
String SUBDATA = ""; // top right

String FOOTERA = ""; // bottom left
float  FOOTERA_FLOAT = 0; // bottom left float num
String FOOTERCENTER = ""; // footer center
String FOOTERCENTERL1 = ""; // footer center
String FOOTERCENTERL2 = ""; // footer center
String FOOTERB = ""; // footer center
String FOOTERC = ""; // opt1
String FOOTERD = ""; // opt2
String ERRORSTR = ""; // errors


// DEBUGS
bool DEBUG_POINT = false; // Debug graph point

bool DEBUG_BOX   = false; // debug show box bounds
bool DEBUG_DATUM = false; // NI debug show datum origin point

int footerBG = HC2;

// init_graph(320,240-(FOOTERH+ypad),0,HEADERH+ypad);
int SCREENWIDTH = 320; // TFT_HEIGHT
int SCREENHEIGHT = 240; // TFT_WIDTH

int GRAPHHEIGHT = SCREENHEIGHT-(FOOTERH); //padding 200px
int GRAPHWIDTH  = SCREENWIDTH;

int graphInterval = 1000; // graph update rate ms

TFT_eSprite spr = TFT_eSprite(&tft); 

unsigned int ICONACTIVECOLOR = SILVER;

// FANS
bool coolOnAbort = true; // cool down on abort
bool coolOnBoot  = true; // cool down on boot

// DOOR
int doorAbortTime = 50000; // time we expect door full operations to take, will disable door motor after this time


// TASKMANAGER
int doorAbortTaskID = -1; // timer task
int reflowStepTaskID = -1;
int PIDTaskID = -1;
int coolAbortTaskID = -1; // -1 empty


// REFLOW STATES
#define RS_IDLE     0
#define RS_PREHEAT  1 // wait for reflow sync, pid or no pid
#define RS_START    2 // for setup of reflow curve and any pre reflow tasks
#define RS_REFLOW   3 // actual reflow
#define RS_PEAK     4 // run safety, logging for reflow etc,
#define RS_COOL     5 // do cooldown
#define RS_ABORT   999 // special mode for abort, stay in abort until reset!


// TIMING
void stateTimerReset(){
  stateStartMS = millis();
}

// return millis since start of state
int getStateDuration(){
  return millis()-stateStartMS;
}

void setgraphInterval(int intv){
  graphInterval = intv;
}

void reflow_graph(){
  int ypad = 3; // padding
  // int FOOTERH = 40;
  // int HEADERH = 30;
  init_graph(GRAPHWIDTH,240-(FOOTERH+ypad),0,HEADERH+ypad);
  // graphPaste();
}

// int filteredId = -1; // filter graph line indexs -1 off

// fan aliases
// fanA 12v fume exhaust
// fanB 5v  cool vent

// set filter id, filters graph only filter line will display
int SetFilterId(int id){
  filteredId = id;
}

void SetTitle(String str){
  TITLE = str;
}

void SetTitleB(String str){
  DETAILS = str;
}

void SetTitleC(String str){
  SUBDATA = str;
}

void SetFooterA(String str){
  FOOTERA = str;
}

void SetFooterA(float str){
  FOOTERA_FLOAT = str;
}

void SetFooterB(String str){
  FOOTERCENTER = str;
}

void setFooterBL1(String str){
  FOOTERCENTERL1 = str;
}

void SetFooterBSCROLL(String str){
  SCROLL = true;
  STATUSLINE = str;
}

void SetFooterC(String str){
  OPT1 = FOOTERC = str;
}

void SetFooterD(String str){
  OPT2 = FOOTERD = str;
}

void setERRORSTR(String str){
  ERRORSTR = str;
}

// alias
void setTempDisp(){
  // NI, not using colored temps, using colored footer line instead
  // color code temperature
  // if(currentTemp > hotTemp) tft.setTextColor( RED, BLACK );
  // else if(currentTemp < coolTemp) tft.setTextColor( GREEN, BLACK );
  // else tft.setTextColor( YELLOW, BLACK ); 
  // SetFooterA();
}


void doorCancelAbort(){
	if(doorAbortTaskID >-1) taskManager.cancelTask(doorAbortTaskID);
	doorAbortTaskID = -1;
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
  doorCancelAbort();
  doorAbortTaskID = taskManager.scheduleOnce(doorAbortTime, doorAbort);  
}

void doorClose(){
  motorDir = 2;
  motorChange = true;
  doorCancelAbort();
  doorAbortTaskID = taskManager.scheduleOnce(doorAbortTime, doorAbort);  
}

// MACROS

void extract(){
  fanA(50); // extract 50%
}

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

void coolCancel(){
  if(coolAbortTaskID >-1){
    coolAbortTaskID = cancelTask(coolAbortTaskID);
  }
}

void coolCompleteWatcher(){
  if(currentTempAvg < fanTemp){
    Serial.println("[COOL] Cooldown Complete");
    coolCancel();
    Serial.println("[COOL] Closing Door, shutoff fans");
    doorClose();
    fanB(0);
    SSRFan(false);
  }
}

void cool(bool dooropen){
  Serial.println("[COOL] cooldown");
  coolCancel(); // make sure we do not create multiples timers
  if(currentTempAvg < fanTemp){
    Serial.println("[COOL] cooldown not needed, skipping");
    return;
  }
  if(dooropen) doorOpen();
  fanB(100); // outake 100%
  Serial.println("[COOL] setup cooldown timer");
  coolAbortTaskID = taskManager.scheduleFixedRate(1000, coolCompleteWatcher);
}

void coolDown(){
  cool(true);
}

void standby(){
  doorClose();
  fanB(0);
  fanA(0);
  SSRFan(true); // ssr fan full
}

void sleep(){
  doorClose();
  fanB(0);
  fanA(0);
  SSRFan(false); // ssr fan off
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

void reboot(){
  tft.fillScreen(BLACK);
  tft.setTextSize(2);
  tft.setTextFont(2);
  Serial.println("REBOOTING");
  delay(1000);
  ESP.restart();
}


// some placeholders for menu actions etc
void cancel(){

}

void hold(){

}

void reset(){

}

void save(){

}

void load(){

}

// sanity/safety

// what to do if tc fails
// main tc, alarm , abort
void TCSafe(){
  // reflowAbort();
}

void tempIsSafe(){
  // if(currentTemp > shutDownTemp){

  // }
  // tc is above max limit
  // fan failing, no way to detect other than cooling state slope not moving or negative..
  // ssr above temp (NTC)
  // cold junction temp above temp
}

void restartDetector(){
  // check for spurious restarts
  // not sure if this is needed other than burn in testing
  // if reset reason was bad, and was in a reflow, use flash to save state ?
  // resume or warn or just alarm and leave to user.. Allow reflow resume, skip preheat
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

// set fan status display
void SetFanInd(){
  int fan1 = getFanStatus(1);
  int fan2 = getFanStatus(2);
  SetFooterC("FAN 1");
  SetFooterD("FAN 2");  
  // tft_set_footer_val3(fan1>0);
  // tft_set_footer_val4(fan2>0);
}


/**
 * ALIGN <L >R <C>
 * --------------------------------------
 * | <TITLE    <DETAILS        SUBDATA> |
 * |------------------------------------|
 * |                                    |
 * |               GRAPH                |
 * |                                    |
 * |------------------------------------|
 * | <FOOTERA  <FOOTERCENTER>  FOOTERC> |
 * |            <STATUSLINE>   FOOTERD> |
 * --------------------------------------
 */

uint8_t TITLETOPPAD = 4; // main title top padding

// add getter for colors?

void updateTitle(){
    String str = TITLE;
    // Serial.println("footer val1: " + str);
    if(DEBUG_BOX) tft.setTextColor(WHITE,DEBUG_HC1);
    else tft.setTextColor(WHITE,HC1);
    tft.setTextDatum(TL_DATUM);
    int lpad = 2;
    tft.setTextPadding(115);
    // drawDatumMarker(2,2);
    // tft.setFreeFont(AA_FONT_MED);    // Must load the font first    
    lpad += tft.drawString(str,lpad,TITLETOPPAD,4);
    // Serial.println(lpad);
    tft.setTextPadding(0);
}

void updateTimer(){
    String str = DETAILS;
    if(DEBUG_BOX) tft.setTextColor(HC1_text,DEBUG_HC2);
    else tft.setTextColor(HC1_text,HC1);
    tft.setTextDatum(BL_DATUM);
    int lpad = 2;
    tft.setTextPadding(120);
    // drawDatumMarker(120,25);
    lpad += tft.drawString(str,120,30,4); // 22
    // Serial.println(lpad);    
    tft.setTextPadding(0);
}

void updateTitleB(){
    String str = DETAILS;
    if(DEBUG_BOX) tft.setTextColor(HC1_text,DEBUG_HC2);
    else tft.setTextColor(HC1_text,HC1);
    tft.setTextDatum(BL_DATUM);
    int lpad = 2;
    tft.setTextPadding(120);
    // drawDatumMarker(120,25);
    lpad += tft.drawString(str,130,30,4); // 22
    // Serial.println(lpad);
    tft.setTextPadding(0);
}

void updateTitleC(){
    String str = SUBDATA;
    if(DEBUG_BOX) tft.setTextColor(HC1_text,DEBUG_HC3);
    else tft.setTextColor(HC1_text,HC1);
    tft.setTextDatum(BR_DATUM);
    int rpad = 4;
    tft.setTextPadding(80);
    // drawDatumMarker(SCREENWIDTH-rpad,29);
    rpad += tft.drawString(str,SCREENWIDTH-rpad,30,4); // 22
    // Serial.println(lpad);
    tft.setTextPadding(0);
}

// temperature C
// sprite 55fps/67fps varies by font
// gfx 72fps freefont()
// block font 88fps loadFont()
void updateFooterA_C(unsigned int c){

    float str = FOOTERA_FLOAT;
    #define USESPRITE
    #ifdef USESPRITE
      spr.setColorDepth(16);
      spr.createSprite(115, 35);
      spr.fillSprite(HC2);
      if(DEBUG_BOX) spr.setTextColor(WHITE,DEBUG_HC1);  
      else spr.setTextColor(c,HC2);
      spr.setTextDatum(TL_DATUM);
      int lpad = 5;
      spr.setTextPadding(115); // 18pt font
      spr.setFreeFont(GFXX18pt);
      // spr.loadFont(AA_FONT_LARGE);
      lpad = spr.drawFloat((float)str,1,0,GFXFF); // 63
      // if(DEBUG_BOX) spr.setTextColor(WHITE,MAROON);  
      spr.unloadFont();
      spr.drawString("`c",lpad+5,2,4); // 22
      
      spr.pushSprite(5, SCREENHEIGHT-35,BLACK);
      spr.deleteSprite();
    #else
    // Serial.println("footer val1: " + str);
    if(DEBUG_BOX) tft.setTextColor(WHITE,DEBUG_HC1);  
    else tft.setTextColor(c,HC2);
    tft.setTextDatum(BL_DATUM);
    int lpad = 5;
    // tft.setTextPadding(90);
    tft.setTextPadding(115); // 18pt font
    // tft.setFreeFont(GFXX18pt);    // Must load the font first   
    lpad = tft.drawFloat((float)str,1,lpad,SCREENHEIGHT,4); // 63
    if(DEBUG_BOX) tft.setTextColor(WHITE,MAROON);  
    tft.setTextPadding(0);
    lpad += tft.drawString("`c",lpad+6,SCREENHEIGHT,4); // 22
    // tft.setTextPadding(0);
    // Serial.println(lpad);
    #endif
}

// user footerb_l
void updateFooterB(){
    String str = FOOTERCENTER;
    // str = "ABCDEFGHIJKLMNOPQR"; // 19 characters
    // str = "Ms: "+(String)(millis())+" RSSI: "+(String)getRSSIasQuality(); // 19 characters
    if(DEBUG_BOX) tft.setTextColor(GREY,DEBUG_HC2);
    else tft.setTextColor(GREY,HC2);
    
    tft.setTextDatum(BL_DATUM);

    tft.setTextPadding(145);
    // println_footer("",HC2);
    tft.drawString(str,(SCREENWIDTH/3)+10,SCREENHEIGHT-4,2);
    tft.setTextPadding(0);
}

void updateFooterB_ERROR(){
    bool center = false;    
    String str = ERRORSTR;
    if(DEBUG_BOX) tft.setTextColor(GREY,DEBUG_HC2); 
    else tft.setTextColor(RED,HC2);

    if(center) tft.setTextDatum(BC_DATUM);
    else tft.setTextDatum(BL_DATUM);
    tft.setTextPadding(150);

    int posx = (SCREENWIDTH/2)-35;
    if(center)posx = (SCREENWIDTH/2)+23;
    tft.drawString(str,posx,SCREENHEIGHT-4,2);
    // tft.drawString(str,SCREENWIDTH/2,SCREENHEIGHT-4,2);
    tft.setTextPadding(0);
}

// left aligned
// rework all of this, since we can do 2 lines now!!
void updateFooterB_L(){
    String str = FOOTERCENTER;
    // str = "ABCDEFGHIJKLMNOPQR"; // 19 characters
    // str = "Ms: "+(String)(millis())+" RSSI: "+(String)getRSSIasQuality(); // 19 characters
    if(DEBUG_BOX) tft.setTextColor(GREY,DEBUG_HC2);
    else tft.setTextColor(GREY,HC2);

    // text clipping
    int maxchars = 0;
    String newstr;
    if(maxchars > 0) newstr = str.substring(0,maxchars);
    else newstr = str;

    tft.setTextPadding(150);
    tft.setTextDatum(BL_DATUM);
    // println_footer("",HC2);
    tft.drawString(newstr,(SCREENWIDTH/2)-35,SCREENHEIGHT-4,2);
}

void updatefooterBL1(){
    String str = FOOTERCENTERL1;
    // str = "ABCDEFGHIJKLMNOPQR"; // 19 characters
    // str = "Ms: "+(String)(millis())+" RSSI: "+(String)getRSSIasQuality(); // 19 characters
    if(DEBUG_BOX) tft.setTextColor(GREY,DEBUG_HC2);
    else tft.setTextColor(GREY,HC2);

    // text clipping
    int maxchars = 0;
    String newstr;
    if(maxchars > 0) newstr = str.substring(0,maxchars);
    else newstr = str;

    tft.setTextDatum(BL_DATUM);
    tft.setTextPadding(150);
    tft.drawString(str,(SCREENWIDTH/2)-35,SCREENHEIGHT-20,2);
}

void updatefooterBL2(){
 
}

void UpdateScroll(){
  if(!SCROLL) return;
  String str = STATUSLINE;
  // str = "!\"#$%&'()*+,-./01234";
  // str = "/01";
  // str = "!\"#$%&'()*+,-./012345678"; // 9:;<=>?@"; // ascii
  static int charindex = 0;
  int charbuffer = 20;
  int chars = charbuffer;
  int len = str.length()+charbuffer;
  String pad;
  for(int i=0;i<chars;i++){
    pad += " ";
  }
  str = pad+str; // prefix spaces so it scrolls in, can remove this after first scroll, or add transitions here

  String newstr = str.substring(charindex, charindex+chars < len ? charindex+chars : len );
  SetFooterB(newstr);
  updateFooterB_L(); // footerB_left
  charindex++;
  if(charindex>len) charindex = 0;
}

void updateFooterC(bool enable){
    String str = FOOTERC;
    tft.setTextSize(1);
    tft.setTextColor(enable?GREEN:DKGREY,DEBUG_BOX?DEBUG_HC3:HC2);  
    tft.setTextDatum(TR_DATUM);
    tft.setTextPadding(0);
    tft.drawString(str,SCREENWIDTH-2,SCREENHEIGHT-30,2);    
    // tft.setTextDatum(BR_DATUM);
    // tft.drawString("FAN 2",SCREENWIDTH-2,SCREENHEIGHT,2);  
}

void updateFooterD(bool enable){
    String str = FOOTERD;
    tft.setTextSize(1);
    tft.setTextColor(enable?GREEN:DKGREY,DEBUG_BOX?DEBUG_HC3:HC2);  
    // tft.setTextDatum(TR_DATUM);
    // tft.drawString("FAN 1",SCREENWIDTH-2,SCREENHEIGHT-30,2);    
    tft.setTextPadding(0);
    tft.setTextDatum(BR_DATUM);
    tft.drawString(str,SCREENWIDTH-2,SCREENHEIGHT,2);    
}

void updateFPS(){
    fps.push((1000/(millis()-fpsmicros)));
    fpsmicros = millis();
    SetFooterB((String)fps.mean() + " FPS");
}

void updateDev(){
    SetFooterB("Dev: " +(String)getTCDev());
}

void drawFooterBorder(unsigned int c = RED){
  // Draw border left bottom
  tft.drawFastVLine(0,SCREENHEIGHT-FOOTERH,FOOTERH,c);
  tft.drawFastVLine(SCREENWIDTH-1,SCREENHEIGHT-FOOTERH,FOOTERH,c);
  tft.drawFastHLine(0,SCREENHEIGHT-1,SCREENWIDTH,c);

  // tft.drawFastVLine(tft.width()-1,0,tft.height()-1,c);
  // tft.drawFastHLine(0,0,tft.width()-1,c);
}


void drawFooterLineBottom(unsigned int c){
  tft.drawFastHLine(0,0,SCREENWIDTH,c);  
  tft.drawFastHLine(0,1,SCREENWIDTH,c);  
}

void drawFooterLine(unsigned int c){
  tft.drawFastHLine(0,SCREENHEIGHT-FOOTERH,SCREENWIDTH,c);  
  tft.drawFastHLine(0,SCREENHEIGHT-FOOTERH-1,SCREENWIDTH,c);  
}


// @NOTE If Y is off screen is snaps to negative

// fan a
int iconAX = SCREENWIDTH-20;
int iconAY = SCREENHEIGHT-(FOOTERH-2);
// fan b
int iconBX = SCREENWIDTH;
int iconBY = SCREENHEIGHT-(FOOTERH-2);
// heater
int iconCX = SCREENWIDTH-20;
int iconCY = SCREENHEIGHT-(FOOTERH-20);
// wifi
int iconDX = SCREENWIDTH;
int iconDY = SCREENHEIGHT-(FOOTERH-20);

void wifiIcon(bool enabled = true,bool connected = false){
    String str = "!";
    // tft.setTextSize(1);
    tft.setFreeFont(AA_FONT_SMALL);    // Must load the font first
    if(enabled && connected){
      tft.setTextColor(ICONACTIVECOLOR,DEBUG_BOX?DEBUG_HC3:HC2);  
    }
    else {
      tft.setTextColor(DKGREY,DEBUG_BOX?DEBUG_HC3:HC2);  
    }
    tft.setTextDatum(TR_DATUM);
    tft.setTextPadding(0);
    tft.drawString(str,iconDX,iconDY,GFXFF);
    if(!connected){
      tft.setTextColor(RED);
      tft.drawString("!",iconDX-6,iconDY+2,2);
    }
    // tft.drawString("8",SCREENWIDTH-20,SCREENHEIGHT-(FOOTERH-2),GFXFF);
}

// fonts from custom glyph
// using symbolMono18pt7b test for now, needs cleanup
#define ICON_CHR_WIFI "!"
#define ICON_CHR_WIFI_OVERA "!"
#define ICON_CHR_WIFI_OVERB "*"
#define ICON_CHR_FANA "\""
#define ICON_CHR_FANB "#"
#define ICON_CHR_FANOVERA"$"
#define ICON_CHR_FANOVERB "%"
#define ICON_CHR_HEAT "&"

// NI Wifi mananger or ap is running for config/pairing
void wifiSetupIcon(){
    String str = ICON_CHR_WIFI;
    tft.setFreeFont(AA_FONT_SMALL);    // Must load the font first
    tft.setTextColor(ICONACTIVECOLOR,DEBUG_BOX?DEBUG_HC3:HC2);  
    tft.setTextDatum(TR_DATUM);
    tft.setTextPadding(0);
    tft.drawString(str,iconDX,iconDY,GFXFF);
    tft.setTextColor(BLUE);
    tft.drawString(ICON_CHR_WIFI_OVERB,iconDX-6,iconDY+2,2); 
    tft.unloadFont();
}

void fanAIcon(bool enabled = false){
  String str = ICON_CHR_FANA;

  if(enabled){
    if(animStateA) str = ICON_CHR_FANB;
    animStateA = !animStateA;
    tft.setTextColor(ICONACTIVECOLOR,DEBUG_BOX?DEBUG_HC3:HC2);  
  }
  else {
    tft.setTextColor(DKGREY,DEBUG_BOX?DEBUG_HC3:HC2);  
  }
  tft.setFreeFont(AA_FONT_SMALL);    // Must load the font first
  tft.setTextDatum(TR_DATUM);
  tft.setTextPadding(0);
  tft.drawString(str,iconAX,iconAY,GFXFF);
  tft.unloadFont();
}

void fanBIcon(bool enabled = false){
  String str = ICON_CHR_FANA;
  if(enabled){
    if(animStateB) str = ICON_CHR_FANB;
    animStateB = !animStateB;
    tft.setTextColor(ICONACTIVECOLOR,DEBUG_BOX?DEBUG_HC3:HC2);  
  }
  else {
    tft.setTextColor(DKGREY,DEBUG_BOX?DEBUG_HC3:HC2);  
  }
  tft.setTextDatum(TR_DATUM);
  tft.setTextPadding(0);
  tft.drawString(str,iconBX,iconBY,GFXFF);  // W,H
}

void iconC(bool enabled = false){
  // heater
  if(enabled){
    tft.setTextColor(REDORANGE,DEBUG_BOX?DEBUG_HC3:HC2);  
  }
  else {
    tft.setTextColor(DKGREY,DEBUG_BOX?DEBUG_HC3:HC2);  
  }  
  tft.setFreeFont(AA_FONT_SMALL);    // Must load the font first
  tft.setTextDatum(TR_DATUM);
  tft.setTextPadding(0);
  tft.drawString(ICON_CHR_HEAT,iconCX,iconCY,GFXFF);  // W,H
  tft.unloadFont();
}

void fanAIcon_ring(unsigned int c){
  // String str = animStateB? "\"" : "#";
  // animStateB = !animStateB;
  tft.setFreeFont(AA_FONT_SMALL);    // Must load the font first
  tft.setTextColor(c);
  tft.setTextDatum(TR_DATUM);
  tft.setTextPadding(0);
  tft.drawString(ICON_CHR_FANOVERA,SCREENWIDTH-50-17,SCREENHEIGHT-30,GFXFF);  
  tft.unloadFont();  
}

void fanBIcon_ring(unsigned int c,bool full){
  // String str = animStateB? "\"" : "#";
  // animStateB = !animStateB;
  tft.setFreeFont(AA_FONT_SMALL);    // Must load the font first
  tft.setTextColor(c);
  tft.setTextDatum(TR_DATUM);
  tft.setTextPadding(0);
  tft.drawString(full ? ICON_CHR_FANOVERA : ICON_CHR_FANOVERB,SCREENWIDTH-50-17-17,SCREENHEIGHT-30,GFXFF);  
  tft.unloadFont();    
}

// Red DANGER burns
// Orange HOT use caution
// Green Above ambient
// Blue, cooldown remove PCB ? NI
void updateFooterLine(){
  if(currentTempAvg > hotTemp) drawFooterLine(RED);
  else if(currentTempAvg > coolTemp) drawFooterLine(ORANGE);
  else if(currentTempAvg > lowTemp) drawFooterLine(GREEN);
  else drawFooterLine(BLACK);
}

void updateAccentColor(){
  if(currentTempAvg > hotTemp) indAccentSetColor(255,0,0);
  else if(currentTempAvg > coolTemp) indAccentSetColor(255,128,0);
  else if(currentTempAvg > lowTemp) indAccentSetColor(0,255,0);
  else indAccentSetColor(accentColor);
}

void doAlert(int mode = 0);

void updateSSRIcon(){
  iconC(getSSRDuty() > 0);
}
    
// random indication icon test
void testIconsUpdate(){
  wifiIcon(random(2)-1,random(2)-1);
  fanAIcon(true);
  // fanAIcon_ring(DKGRAY);
  fanBIcon(random(2)-1);
  // fanBIcon_ring(LTBLACK,true);
  // fanBIcon_ring(TFT_SILVER,false);
  iconC(random(2)-1);
}

void updateIcons(){
  if(testIcons){
    return testIconsUpdate();
  }

  // wifiSetupIcon();
  wifiIcon(USEWIFI && wifiIsAutoConnect(),wifiIsConnected());
  fanAIcon(getFanStatus(1)>0);
  // fanAIcon_ring(DKGRAY);
  fanBIcon(getFanStatus(2)>0);
  // fanBIcon_ring(LTBLACK,true);
  // fanBIcon_ring(TFT_SILVER,false);
  updateSSRIcon();
}


void dispReflowStats(){
  String str = "";
  str = "NTC: "+(String)(int)(round(get_ntc()/10));
  str += " CJ: " + (String)(int)(round(internalTemp));
  setFooterBL1(str);
  str = "";
  str += "P: " + (String)(int)getSSRPower() + "%";
  str += " o: " + (String)getTCDev();
  str += " E: " + (String)getTCErrorCount();
  SetFooterB(str);
}

void updateAll(){
  // updateFPS(); 
  // updateDev();
  dispReflowStats();
  updateFooterA_C(WHITE);

  updateTitle();
  updateTitleB();
  updateTitleC();

  updateFooterLine();
  updateAccentColor();  

  updatefooterBL1();
  if(ERRORSTR !="") updateFooterB_ERROR();
  else if(SCROLL) UpdateScroll();
  else updateFooterB_L();

  // updateIcons();
  // doAlert(0);
  // drawFooterLine();
}

void doAlert(int mode){
  if(mode == 0){
    drawBorder(RED);
    // drawBorderB(DKRED);
    // drawFooterBorder(RED);
  }
  else if(mode == 1){
    drawBorder(GREEN);
    // drawBorderB(DKGREEN);
    // drawFooterBorder(DKGREEN);
  }
}

// graphics
// @todo rename
void println_header(const char *string, uint16_t color)
{
  int thickness = HEADERH;
  int vpad = 3;
  int fontsize = 4;
  int rad = 0; // 5;

  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
  if(rad > 0) tft.fillRoundRect(0, 0, 320, thickness,rad, color);
  else tft.fillRect(0, 0, 320, thickness, color);
  // void TFT_eSPI::fillRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, uint32_t color)
  // tft.setTextDatum(TC_DATUM);
  // tft.drawString(string, 160, 2, fontsize); // Font 4 for fast drawing with background
  // tft.drawString(string,tft.width()/2,vpad,fontsize);  
}

void println_footer(const char *string, uint16_t color){
  // void TFT_eSPI::fillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color)
  int thickness = FOOTERH;
  int vpad = 3;
  int fontsize = 4;
  int rad = 0;// 5;

  tft.setTextSize(1);
    if(rad > 0) tft.fillRoundRect(0, tft.height()-thickness , tft.width(), thickness, rad,color);
    else tft.fillRect(0, tft.height()-thickness , tft.width(), thickness, color);
    tft.setTextDatum(TC_DATUM);
    tft.setTextColor(TFT_WHITE);
    tft.drawString(string,tft.width()/2,tft.height()-(thickness-vpad),fontsize);
}

void displayProfile(uint16_t = 0){
    // get profile ID
  // show title
  // show dubtitle from profile
  // show titleC target/max temperature
  
  // placeholders atm
  // 
  SetTitle("PRESET");
  SetTitleB("Sn63/Pb37 10-20um");
  SetTitleC("220`c");

  // updateheader()
  updateTitle();
  updateTitleB();
  updateTitleC();
}

// reflow header!
void displayReflow(){
  // set icons
  // draw profile graph
  // Set title
  // Set subtitle - zone, current target, timer, percentage done
  // show power level, heater on
  // use footer and scroll for status or alerts, debugging

  // placeholders atm
  // SetTitle("REFLOW"); // assign titles for each profile zone ?
  
  // timer disp
  // real time, change to use stepping from reflow instead
  if(graphInterval < 1000) SetTitleB("   " + (String)getTimerMS(millis()-stateStartMS)); // 
  else SetTitleB("   " + (String)getTimer(millis()-stateStartMS)); // 
  
  // SetTitleC("50`c");

  // updateheader()
  updateTitle();
  updateTitleB();
  // updateTimer();
  updateTitleC();

  updateAll();
}

void displayPower(){
  // getSSRDuty();
  // power meter , icon or colors?
  // use smoothing and average
  // use easing on display, peak and slow decay if graph
  // graph for debugging?
}

void testPaste(){
  // displayProfile(0);
  displayReflow();
  SetFooterA(currentTempAvg);

  // String str = "Ms: "+(String)(millis())+" RSSI: "+(String)getRSSIasQuality(); // 19 characters 
  // SetFooterBSCROLL(str);
}

void tft_setupDraw(){
  tft.fillScreen(GRAPHBG);
  println_header("",LTBLACK);
  println_footer("",LTBLACK);
  tft.drawFastHLine(0, HEADERH, tft.width(), BGCOLOR);  
  tft.drawFastHLine(0, tft.height()-FOOTERH, tft.width(), BGCOLOR);  
}

void tft_init(){
  tft.init();
  tft.setRotation(TFTROT);
}

// quick dirty tc check, 
// if heating, and temp not changing enough, and 10s since start of temp call
// will have to add an actual tracking mechanism fo rrate of change expected and change since as startState is used for entire profile
bool thermalCheck(){
  if(!enableThermalCheck) return true;
  if(wantedTemp > 0 && getSSRDuty() > 3 && getTCDev()<0.10 && (millis()-stateStartMS > 10000)){
    Serial.println("THERMAL RUNAWAY");
    setERRORSTR("THERMAL RUNAWAY");
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
    SSRFan(true);  
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
  SetTitleC((String)preheattemp + "`c");
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
    SetTitleC("0`c"); // @todo replace with cooling rate, or estimate time left?
    reflowZone = 0;
    cool(false);
    SetTitle("COOLING");
    updateTitle();
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
  SetTitleC((String)(int)round((wantedTemp))+"`c");
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
  SetTitleC((String)(int)round((wantedTemp))+" / " + (String)(int)round(getActualWanted()));
}


void doPasteGraph(){
  Serial.println("[START] doPasteReflow");
  reflow_graph();

 int maxtime = getMaxTime(); // max time frame for reflow, or make sure last point is not drawn huh ?
 int maxtemp = getMaxTemp(); // max value
 int ssize = maxtime;

 maxtemp+=10; // top margin for overshoot and padding

  double y;
  for(int i=0;i<maxtime;i++){
    y = Interpolation::SmoothStep(pasteTime, pasteTemp, numZones, i);
    // Serial.println("y: " + (String)y);
    addPointSet(5,i,y,ssize,maxtemp,0);
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
    addPointSet(1,i*curveSamplePeriod,y,ssize,maxtemp,1);
    }
  }
}

void doPasteReflow(){
  resetGraphLines();
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
    if(coolOnAbort) cool(false); // cooldown on abort
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
  fansOff();
  preHeat();
}

// BARGRAPH
// 
void update_bargraph(int perc){
  bar0.drawBarHorizontal(perc);  
}

void init_bargraph(){
    bar0.begin( 0,  300-20,  20,  50, &tft);  
    bar0.setColor(WHITE, BLACK);
    tft.fillScreen(BLACK);

    tft.unloadFont();
    // tft.setFreeFont(AA_FONT_SMALL); 
    // tft.setFreeFont(FONT2);
    tft.setTextFont(FONT2);
    // tft.setFreeFont(FM12);
    tft.setCursor(20,20);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK); 
    tft.println("OTA Update in Progress...");
}

void testBargraph(){
  Serial.println("initbargraph");
  init_bargraph();

  // tft.fillRect(20, 50 + 14, 280, 15,  RED);
  for(int i=0;i<100;i++){
      update_bargraph(i);
      delay(50);
  }
  bar0.setColor(GREEN, BLACK);
  update_bargraph(100);  
  delay(1000);

  tft.setCursor(10,10);
  tft.println("Rebooting...                    ");

  for(int i=0;i<100;i++){
      update_bargraph(100-i);
      delay(50);
  }
  bar0.setColor(WHITE, RED);
  update_bargraph(1);
  update_bargraph(0);
  delay(1000);
}


void setupOtaCB(){
  ArduinoOTA.onStart([]() {
    tft.fillScreen(RED);
        // init_bargraph();
         // Serial.println("\nOTA onStart");
  });

  // ArduinoOTA.onEnd([]() {
  //       otastarted = false;
  //       bar0.setColor(GREEN, BLACK);
  //       update_bargraph(100);
  //       tft.setTextSize(1);
  //       // Serial.println("\nOTA onEnd");
  // });

  // ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
  //   Serial.printf("OTA Progress: %u%%\n", (progress / (total / 100)));
  //   update_bargraph((int)(progress / (total / 100)));
  // });
}

#endif