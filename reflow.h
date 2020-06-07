#ifndef reflow_h
#define reflow_h

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

Statistics fps(10); // init stats for avg (samples)

// ICONS 
// states for fan animations, only 2 state atm
bool animStateA = true;
bool animStateB = false;
bool testIcons  = false; // set icons states randomly/Demo mode

unsigned long stateStartMS = 0; // state start timers

// app states
int reflowState = 0; // @todo add enum state, maybe queue

// temperature
int hotTemp  = 70; // C burn temperature for HOT indication, 0=disable
int coolTemp = 50; // C safe temperature for HOT indication, 0=disable
int lowTemp  = 30; // C of TC warmed than typical CJ
int shutDownTemp = 210; // degrees C
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

// FANS
uint8_t coolAbortTaskID;

// DOOR
int doorAbortTime = 50000; // time we expect door full operations to take, will disable door motor after this time
uint8_t doorAbortTaskID; // timer task

void stateTimerReset(){
  stateStartMS = millis();
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
  // color code temperature
  // if(currentTemp > hotTemp) tft.setTextColor( RED, BLACK );
  // else if(currentTemp < coolTemp) tft.setTextColor( GREEN, BLACK );
  // else tft.setTextColor( YELLOW, BLACK ); 
  // SetFooterA();
}


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
  fanA(50); // extract 50%
}

// cancel a task, pass ref?
// add ojbect to handle tasks and ids
uint16_t cancelTask(uint16_t taskid){
  if(taskid >0) taskManager.cancelTask(taskid);
  return 0;
}

void coolComplete(){
  if(currentTempAvg < lowTemp){
    if(coolAbortTaskID >0) taskManager.cancelTask(coolAbortTaskID);
    coolAbortTaskID = 0;
    doorClose();
    fanB(0);
  }
}

void coolDown(){
  doorOpen();
  fanB(100); // outake 100%
  coolAbortTaskID = taskManager.scheduleFixedRate(1000, coolComplete);  
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
  SSRFan(false); // ssr fan full
}

void reflow(){

}

void reflowabort(){

}

void reboot(){
  ESP.restart();
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
  // fan failing
  // ssr above temp (NTC)
  // cold junction temp above temp
}

void restartDetector(){
  // check for spurious restarts
}

void userAbort(){
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

uint8_t TITLETOPPAD = 4;

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
    lpad += tft.drawString(str,120,30,4); // 22
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

TFT_eSprite spr = TFT_eSprite(&tft); 

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

unsigned int ICONACTIVECOLOR = SILVER;

void updateFPS(){
    fps.addData((1000/(millis()-fpsmicros)));
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

void setReflowState(int state){
  int ReflowState = state;
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
  SetTitle("REFLOW"); // assign titles for each profile zone ?
  
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

uint16_t reflowZone = 0;

void reflowNextZone(){
  Serial.println("ms:" + (String)millis());
  const int numSamples = 7;
  if(reflowZone == numSamples-1){
    Serial.println("[REFLOW] DONE");
    // continue cooling, slow graph rate to log ? show some other way
    stop_PID();
  }
  else Serial.println("[REFLOW] SET ZONE: " + (String)reflowZone);
  double xValues[7] = {  1,  60, 120, 160, 210, 260,  310  }; // time
  double yValues[7] = { 25, 105, 150, 150, 220, 220,  25 }; // value  
  wantedTemp = yValues[reflowZone];
  reflowZone++;
  SetTitleC((String)(int)round((wantedTemp))+"`c");
}

void doPasteReflow(){

  const int numSamples = 7;
  double xValues[7] = {  1,  60, 120, 160, 210, 260,  310  }; // time
  double yValues[7] = { 25, 105, 150, 150, 220, 220,  25 }; // value

 int maxtime = 310; // max time frame for reflow, or make sure last point is not drawn
 int maxtemp = 220+20; // max value + top padding
 int ssize = 320;

  for(int i=0;i<maxtime;i++){
    double y;
    y = Interpolation::SmoothStep(xValues, yValues, numSamples, i);
    Serial.println("y: " + (String)y);
    addPointSet(5,i,y,ssize,maxtemp,0);
  }

  for(int i = 0; i < numSamples; i++){
    // Serial.print(Interpolation::CatmullSpline(xValues,yValues, numSamples, i));
    // addPointSet(5,xValues[i],yValues[i],ssize,maxtemp);
    Serial.println("x2 timer " + (String)((int)round(xValues[i])*1000));
    taskManager.scheduleOnce(xValues[i], reflowNextZone,TIME_SECONDS);
    stateTimerReset();
  }
  reflowNextZone(); 
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
  tft.println("Rebooting...                     ");

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
      otastarted = true;
      // init_bargraph();
       // Serial.println("\nOTA onStart");
  });

  ArduinoOTA.onEnd([]() {
        otastarted = false;
        bar0.setColor(GREEN, BLACK);
        update_bargraph(100);
        tft.setTextSize(1);
        // Serial.println("\nOTA onEnd");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("OTA Progress: %u%%\n", (progress / (total / 100)));
    update_bargraph((int)(progress / (total / 100)));
  });
}

// [graphing]
// - [ ] fix start graph -999 to >0 line , fix to not draw first line if adding first point
// - [ ] add dotted lines
// - [ ] add marker lines
// - [ ] add indicator labels, side of screen lock to trace or move 
// - [ ] add power meter
// - [ ] add percentage and timer
// - [ ] draw buffer around desired trend line for profile
// - [ ] rework the grid increments
// - [ ] Temperature trend indicator <o> up down holding 
// 
// Fan control
// - [ ] Start FanA extract on fume temp, peak reflow, make sure it does not preturb temperature
// - [ ] Test slow speed FanB for heat distribution during warmup, convection probably not useful since its IR direct heating
// - [ ] NTC auto run internal fans, SSR kick 100% when reflow starts, ensure this does not pull additional heat into chamber, add cool intake on bottom of enclosure
// - [ ]turn on extract fan into filter to absorb or vent smoke and fumes before opening door and cooling
// - [ ] test for cooling effect to ensure reflow not affected
// 
// Runtime
// - [ ] Time to temp, startimer, record time to reach temp, all states, cool etc create logging array for debug output later or transmission stats
// - [ ] Record Max temp reached and time, detect overshoots!


// [PID]
// - [ ] PID KickAdd Reflow Peak kick if PID is not getting there fast enough, special PID overrides for plateus.
// - [ ] Custom power per stage, use power for kick, normalize to 100% , 125% kick on ramp start etc..
// - [ ] Record time when PID stops pushing, detect early rise, rise too fast or slow
// - [ ] Calculate slope for all temp ranges/zones record linearity across ranges
// - [ ] Ideal PID for HEAT stages, cooldown, temperatures above 100C may take longer to reach, 
// - [ ] account for thermal masses and board colors can be inferred here, power might also help here
// - [x] Thermal runway (quick dirty)

// [timings]
// 
// Timing for temperature readings, averaging plus sample rate, smoothing as needed
// display and graphing might need seperate smoothing algos
// update speed of scroller
// update speed of icons, use ondemand updates then periodic catch ups
// animated icons may need their own intervals based on frame rate
// Time Division for graph, 320 divisions max
// Use timer for profile and each stage or PV change, used for thermal runaway and rate of change insufficiant reflow failure detciton or bail.

// status icons
// 
// create icon 
// w,h,x,y
// state color
// add frames, map to glyps
// toggle animations, dependant of state or not
// - [x] toggle heater faster than the rest
// - [ ] add TC warn icon
// 
#endif