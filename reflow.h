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

#include <pid.h>

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

#include "SymbolMono18pt7b.h"
#define AA_FONT_SMALL &SymbolMono18pt7b
#define GFXFF 1

// #include "/Users/shawn/.platformio/lib/TFT_eSPI_ID1559/Fonts/GFXFF/FreeMono24pt7b.h"
// #include "FreeMono24pt7b.h"

#include "NotoSansBold15.h"
#include "NotoSansBold36.h"
#include "NotoSansMonoSCB20.h"

// The font names are arrays references, thus must NOT be in quotes ""
#define AA_FONT_MED NotoSansBold15
#define AA_FONT_LARGE NotoSansBold36
#define AA_FONT_MONO  NotoSansMonoSCB20 // NotoSansMono-SemiCondensedBold 20pt

#define GFXX18pt &FreeSans18pt7b
// #define GFXX18pt &FreeMono18pt7b


bool USEWIFI = false;

Statistics fps(10); // init stats for avg (samples)

// ICONS 
// states for fan animations, only 2 state atm
bool animStateA = true;
bool animStateB = false;
bool testIcons  = false; // set icons states randomly/Demo mode

// app states
int reflowState = 0; // @todo add enum state, maybe queue

// temperature
int hotTemp  = 30; // C burn temperature for HOT indication, 0=disable
int coolTemp = 28; // C burn temperature for HOT indication, 0=disable
int shutDownTemp = 210; // degrees C

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
String FOOTERB = ""; // footer center
String FOOTERC = ""; // opt1
String FOOTERD = ""; // opt2
String ERRORSTR = ""; // errors

bool DEBUG_POINT = false; // Debug graph point

bool DEBUG_BOX   = false; // debug show box bounds
bool DEBUG_DATUM = false; // NI debug show datum origin point

int footerBG = HC2;

// init_graph(320,240-(FOOTERH+ypad),0,HEADERH+ypad);
int SCREENWIDTH = 320;
int SCREENHEIGHT = 240;

// int GRAPHHEIGHT = SCREENHEIGHT-(FOOTERH+3); //padding
// int GRAPHWIDTH  = SCREENWIDTH;

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
  // turn on extract fan into filter to absorb or vent smoke and fumes before opening door and cooling
  // test for cooling effect to ensure reflow not affected
  fanA(50); // extract 50%
}

void coolDown(){
  doorOpen();
  fanB(100); // outake 100%
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
    lpad += tft.drawString(str,lpad,lpad,4);
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
    lpad += tft.drawString(str,120,25,2); // 22
    // Serial.println(lpad);    
    tft.setTextPadding(0);
}

void updateTitleC(){
    String str = SUBDATA;
    if(DEBUG_BOX) tft.setTextColor(HC1_text,DEBUG_HC3);
    else tft.setTextColor(HC1_text,HC1);
    tft.setTextDatum(BR_DATUM);
    int rpad = 4;
    tft.setTextPadding(60);
    // drawDatumMarker(SCREENWIDTH-rpad,29);
    rpad += tft.drawString(str,SCREENWIDTH-rpad,29,4); // 22
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
      spr.drawString("`c",lpad+5,0,4); // 22
      
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
    bool center = true;    
    String str = ERRORSTR;
    if(DEBUG_BOX) tft.setTextColor(GREY,DEBUG_HC2); 
    else tft.setTextColor(RED,HC2);

    if(center) tft.setTextDatum(BC_DATUM);
    else tft.setTextDatum(BL_DATUM);
    tft.setTextPadding(150);

    int posx = (SCREENWIDTH/3)-5;
    if(center)posx = SCREENWIDTH/2+15;
    tft.drawString(str,posx,SCREENHEIGHT-4,2);
    // tft.drawString(str,SCREENWIDTH/2,SCREENHEIGHT-4,2);
    tft.setTextPadding(0);
}

// left aligned
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

    // tft.setFreeFont(AA_FONT_SMALL);    // Must load the font first
    tft.setTextDatum(BL_DATUM);
    tft.setTextPadding(150);
    // println_footer("",HC2);
    tft.drawString(newstr,(SCREENWIDTH/2)-35,SCREENHEIGHT-4,2);
    tft.setTextPadding(0);

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

void drawFooterLine(unsigned int c){
  tft.drawFastHLine(0,SCREENHEIGHT-FOOTERH,SCREENWIDTH,c);  
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
    tft.unloadFont();
    if(!connected){
      tft.setTextColor(RED);
      tft.drawString("!",iconDX-6,iconDY+2,2);
    }
    // tft.drawString("8",SCREENWIDTH-20,SCREENHEIGHT-(FOOTERH-2),GFXFF);
}

// Wifi mananger or ap is running for config/pairing
void wifiSetupIcon(){
    String str = "!";
    tft.setFreeFont(AA_FONT_SMALL);    // Must load the font first
    tft.setTextColor(ICONACTIVECOLOR,DEBUG_BOX?DEBUG_HC3:HC2);  
    tft.setTextDatum(TR_DATUM);
    tft.setTextPadding(0);
    tft.drawString(str,iconDX,iconDY,GFXFF);
    tft.unloadFont();
    tft.setTextColor(BLUE);
    tft.drawString("*",iconDX-6,iconDY+2,2); 
}

void fanAIcon(bool enabled = false){
  String str = "\"";

  if(enabled){
    if(animStateA) str = "#";
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
}

void fanBIcon(bool enabled = false){
  String str = "\"";
  if(enabled){
    if(animStateB) str = "#";
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
  tft.drawString("&",iconCX,iconCY,GFXFF);  // W,H
}

void fanAIcon_ring(unsigned int c){
  // String str = animStateB? "\"" : "#";
  // animStateB = !animStateB;
  tft.setFreeFont(AA_FONT_SMALL);    // Must load the font first
  tft.setTextColor(c);
  tft.setTextDatum(TR_DATUM);
  tft.setTextPadding(0);
  tft.drawString("$",SCREENWIDTH-50-17,SCREENHEIGHT-30,GFXFF);  
}

void fanBIcon_ring(unsigned int c,bool full){
  // String str = animStateB? "\"" : "#";
  // animStateB = !animStateB;
  tft.setFreeFont(AA_FONT_SMALL);    // Must load the font first
  tft.setTextColor(c);
  tft.setTextDatum(TR_DATUM);
  tft.setTextPadding(0);
  tft.drawString(full ? "$" : "%",SCREENWIDTH-50-17-17,SCREENHEIGHT-30,GFXFF);  
}

void updateFooterLine(){
  if(currentTemp > hotTemp) drawFooterLine(RED);
  else if(currentTemp > coolTemp) drawFooterLine(ORANGE);
  else if(currentTemp > 25) drawFooterLine(GREEN);
  else drawFooterLine(BLACK);
}

void doAlert(int mode = 0);


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
  iconC(getSSRDuty() > 0);
}

void updateAll(){
  updateFPS(); 
  // updateDev();
  updateFooterA_C(WHITE);

  updateTitle();
  updateTitleB();
  updateTitleC();

  updateFooterLine();

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
  SetTitle("PRESET");
  SetTitleB("Sn63/Pb37 10-20um");
  SetTitleC("220`c");

  // updateheader()
  updateTitle();
  updateTitleB();
  updateTitleC();
}

void displayReflow(){
  // set icons
  // draw profile graph
  // Set title
  // Set subtitle - zone, current target, timer, percentage done
  // show power level, heater on
  // use footer and scroll for status or alerts, debugging

  // placeholders atm
  SetTitle("PREHEAT"); // assign titles for each profile zone ?
  SetTitleB("2:22 30%"); // 
  SetTitleC("50`c");

  // updateheader()
  updateTitle();
  updateTitleB();
  updateTitleC();

}

void displayPower(){
  // getSSRDuty();
  // power meter , icon or colors?
  // use smoothing and average
  // use easing on display, peak and slow decay if graph
  // graph for debugging?
}

void testPaste(){
  displayProfile(0);
  SetFooterA(currentTemp);

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

// timings
// 
// Timing for temperature readings, averaging plus sample rate, smoothing as needed
// display and graphing might need seperate smoothing algos
// update speed of scroller
// update speed of icons, use ondemand updates then periodic catch ups
// animated icons may need their own intervals based on frame rate
// Time Division for graph, 320 divisions max

// status icons
// 
// create icon 
// w,h,x,y
// state color
// add frames, map to glyps
// toggle animations, dependant of state or not


  // Escape any special HTML (unsafe) characters in a string. e.g. anti-XSS.
  // Args:
  //   unescaped: A string containing text to make HTML safe.
  // Returns:
  //   A string that is HTML safe.
  String htmlEscape(const String unescaped) {
    String result = "";
    uint16_t ulen = unescaped.length();
    result.reserve(ulen);  // The result will be at least the size of input.
    for (size_t i = 0; i < ulen; i++) {
      char c = unescaped[i];
      switch (c) {
        // ';!-"<>=&#{}() are all unsafe.
        case '\'':
          result += F("&apos;");
          break;
        case ';':
          result += F("&semi;");
          break;
        case '!':
          result += F("&excl;");
          break;
        case '-':
          result += F("&dash;");
          break;
        case '\"':
          result += F("&quot;");
          break;
        case '<':
          result += F("&lt;");
          break;
        case '>':
          result += F("&gt;");
          break;
        case '=':
          result += F("&#equals;");
          break;
        case '&':
          result += F("&amp;");
          break;
        case '#':
          result += F("&num;");
          break;
        case '{':
          result += F("&lcub;");
          break;
        case '}':
          result += F("&rcub;");
          break;
        case '(':
          result += F("&lpar;");
          break;
        case ')':
          result += F("&rpar;");
          break;
        default:
          result += c;
      }
    }
    return result;
  }

String uint64ToString(uint64_t input, uint8_t base = 10);

// Convert a uint64_t (unsigned long long) to a string.
// Arduino String/toInt/Serial.print() can't handle printing 64 bit values.
//
// Args:
//   input: The value to print
//   base:  The output base.
// Returns:
//   A string representation of the integer.
// Note: Based on Arduino's Print::printNumber()
String uint64ToString(uint64_t input, uint8_t base) {
  String result = "";
  // prevent issues if called with base <= 1
  if (base < 2) base = 10;
  // Check we have a base that we can actually print.
  // i.e. [0-9A-Z] == 36
  if (base > 36) base = 10;

  // Reserve some string space to reduce fragmentation.
  // 16 bytes should store a uint64 in hex text which is the likely worst case.
  // 64 bytes would be the worst case (base 2).
  result.reserve(16);

  do {
    char c = input % base;
    input /= base;

    if (c < 10)
      c += '0';
    else
      c += 'A' - 10;
    result = c + result;
  } while (input);
  return result;
}

#ifdef ARDUINO
// Print a uint64_t/unsigned long long to the Serial port
// Serial.print() can't handle printing long longs. (uint64_t)
//
// Args:
//   input: The value to print
//   base: The output base.
void serialPrintUint64(uint64_t input, uint8_t base) {
  Serial.print(uint64ToString(input, base));
}
#endif

#define D_CHR_TIME_SEP ":"
#define D_STR_DAY "Day"
#define D_STR_DAYS D_STR_DAY "s"
#define D_STR_HOUR "Hour"
#define D_STR_HOURS D_STR_HOUR "s"
#define D_STR_MINUTE "Minute"
#define D_STR_MINUTES D_STR_MINUTE "s"
#define D_STR_SECOND "Second"
#define D_STR_SECONDS D_STR_SECOND "s"
#define D_STR_NOW "Now"

const PROGMEM char* kTimeSep    = D_CHR_TIME_SEP;
const PROGMEM char* kDayStr     = D_STR_DAY;
const PROGMEM char* kDaysStr    = D_STR_DAYS;
const PROGMEM char* kHourStr    = D_STR_HOUR;
const PROGMEM char* kHoursStr   = D_STR_HOURS;
const PROGMEM char* kMinuteStr  = D_STR_MINUTE;
const PROGMEM char* kMinutesStr = D_STR_MINUTES;
const PROGMEM char* kSecondStr  = D_STR_SECOND;
const PROGMEM char* kSecondsStr = D_STR_SECONDS;
const PROGMEM char* kNowStr     = D_STR_NOW;

  String msToString(uint32_t const msecs) {
    uint32_t totalseconds = msecs / 1000;
    if (totalseconds == 0) return kNowStr;

    // Note: uint32_t can only hold up to 45 days, so uint8_t is safe.
    uint8_t days = totalseconds / (60 * 60 * 24);
    uint8_t hours = (totalseconds / (60 * 60)) % 24;
    uint8_t minutes = (totalseconds / 60) % 60;
    uint8_t seconds = totalseconds % 60;

    String result = "";
    if (days)
      result += uint64ToString(days) + ' ' + ((days > 1) ? "kDaysStr" : kDayStr);
    if (hours) {
      if (result.length()) result += ' ';
      result += uint64ToString(hours) + ' ' + ((hours > 1) ? kHoursStr
                                                           : kHourStr);
    }
    if (minutes) {
      if (result.length()) result += ' ';
      result += uint64ToString(minutes) + ' ' + ((minutes > 1) ? kMinutesStr
                                                               : kMinuteStr);
    }
    if (seconds) {
      if (result.length()) result += ' ';
      result += uint64ToString(seconds) + ' ' + ((seconds > 1) ? kSecondsStr
                                                               : kSecondStr);
    }
    return result;
  }

  String minsToString(const uint16_t mins) {
    String result = "";
    result.reserve(5);  // 23:59 is the typical worst case.
    if (mins / 60 < 10) result += '0';  // Zero pad the hours
    result += uint64ToString(mins / 60) + kTimeSep;
    if (mins % 60 < 10) result += '0';  // Zero pad the minutes.
    result += uint64ToString(mins % 60);
    return result;
  }


#endif