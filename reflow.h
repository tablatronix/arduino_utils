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
#define AA_FONT_MED &FreeSans18pt7b

// states for fan animations, only 2 state atm
bool animStateA = true;
bool animStateB = false;


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
int SCREENHEIGHT = 240

int GRAPHHEIGHT = SCREENHEIGHT-(FOOTERH+3); //padding
int GRAPHWIDTH  = SCREENWIDTH;

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


void testPaste(){
  SetTitle("PRESET");
  SetTitleB("Sn63/Pb37 10-20um");
  SetTitleC("220`c");
  SetFooterA(currentTemp);

  String str = "Ms: "+(String)(millis())+" RSSI: "+(String)getRSSIasQuality(); // 19 characters 
  SetFooterBSCROLL(str);

  SetFooterC("FAN 1");
  SetFooterD("FAN 2");
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
    lpad += tft.drawString(str,lpad,lpad,4); // 22
    // Serial.println(lpad);
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
}

void updateTitleC(){
    String str = SUBDATA;
    if(DEBUG_BOX) tft.setTextColor(HC1_text,DEBUG_HC3);
    else tft.setTextColor(HC1_text,HC1);
    tft.setTextDatum(BR_DATUM);
    int rpad = 4;
    tft.setTextPadding(60);
    // drawDatumMarker(TFT_HEIGHT-rpad,29);
    rpad += tft.drawString(str,TFT_HEIGHT-rpad,29,4); // 22
    // Serial.println(lpad);
}

// temperature C
void updateFooterA_C(unsigned int c){
    float str = FOOTERA_FLOAT;
    // Serial.println("footer val1: " + str);
    if(DEBUG_BOX) tft.setTextColor(WHITE,DEBUG_HC1);  
    else tft.setTextColor(c,HC2);
    tft.setTextDatum(BL_DATUM);
    int lpad = 5;
    // tft.setTextPadding(90);
    tft.setTextPadding(115); // 18pt font
    tft.setFreeFont(AA_FONT_MED);    // Must load the font first    
    lpad = tft.drawFloat((float)str,1,lpad,TFT_WIDTH,GFXFF); // 63
    tft.setTextPadding(0);
    lpad += tft.drawString("`c",lpad+6,TFT_WIDTH,4); // 22
    // Serial.println(lpad);
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
    tft.drawString(str,(TFT_HEIGHT/3)+10,TFT_WIDTH-4,2);
}

void updateFooterB_ERROR(){
    bool center = true;    
    String str = ERRORSTR;
    if(DEBUG_BOX) tft.setTextColor(GREY,DEBUG_HC2); 
    else tft.setTextColor(RED,HC2);

    if(center) tft.setTextDatum(BC_DATUM);
    else tft.setTextDatum(BL_DATUM);
    tft.setTextPadding(150);

    int posx = (TFT_HEIGHT/3)-5;
    if(center)posx = TFT_HEIGHT/2+15;
    tft.drawString(str,posx,TFT_WIDTH-4,2);
    // tft.drawString(str,TFT_HEIGHT/2,TFT_WIDTH-4,2);
}

// left aligned
void updateFooterB_L(){
    String str = FOOTERCENTER;
    // str = "ABCDEFGHIJKLMNOPQR"; // 19 characters
    // str = "!\"#$%&'()*+,-./0123456789:;<=>?@";
    // str = 0x11;
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
    tft.drawString(newstr,(TFT_HEIGHT/2)-35,TFT_WIDTH-4,2);
}

void UpdateScroll(){
  if(!SCROLL) return;
  String str = STATUSLINE;
  // str = "!\"#$%&'()*+,-./01234";
  // str = "/01";
  str = "!#$%&'()*+,-./012345678"; // 9:;<=>?@";
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
    tft.drawString(str,TFT_HEIGHT-2,TFT_WIDTH-30,2);    
    // tft.setTextDatum(BR_DATUM);
    // tft.drawString("FAN 2",TFT_HEIGHT-2,TFT_WIDTH,2);  
}

void updateFooterD(bool enable){
    String str = FOOTERD;
    tft.setTextSize(1);
    tft.setTextColor(enable?GREEN:DKGREY,DEBUG_BOX?DEBUG_HC3:HC2);  
    // tft.setTextDatum(TR_DATUM);
    // tft.drawString("FAN 1",TFT_HEIGHT-2,TFT_WIDTH-30,2);    
    tft.setTextPadding(0);
    tft.setTextDatum(BR_DATUM);
    tft.drawString(str,TFT_HEIGHT-2,TFT_WIDTH,2);    
}

void updateFPS(){
    tft.setTextSize(2);
    tft.setTextColor(WHITE,HC2);
    tft.setCursor(TFT_WIDTH,TFT_HEIGHT-100);
    // frames / sec = 1 / (sec / frame)
    // tft.println((String)(1000/((micros()-fpsmicros))));
    // tft.println((String)(1000000/((micros()-fpsmicros))));
    // tft.print("     ");
    sprintf (buffer, "%03u",(int)(1000/((millis()-fpsmicros)))); 
    // Serial.println((String)(1000/((millis()-fpsmicros))));
    // tft.println((String)(1000/(millis()-fpsmicros)));
    tft.println(buffer);
    // Serial.println(buffer);
    // println(" FPS");
    fpsmicros = millis();
}

void drawFooterBorder(unsigned int c = RED){
  // Draw border left bottom
  tft.drawFastVLine(0,TFT_WIDTH-FOOTERH,FOOTERH,c);
  tft.drawFastVLine(TFT_HEIGHT-1,TFT_WIDTH-FOOTERH,FOOTERH,c);
  tft.drawFastHLine(0,TFT_WIDTH-1,TFT_HEIGHT,c);

  // tft.drawFastVLine(tft.width()-1,0,tft.height()-1,c);
  // tft.drawFastHLine(0,0,tft.width()-1,c);
}

void drawFooterLine(unsigned int c){
  tft.drawFastHLine(0,TFT_WIDTH-FOOTERH,TFT_HEIGHT,c);  
}

void wifiIcon(){
    String str = "!";
    // tft.setTextSize(1);
    tft.setFreeFont(AA_FONT_SMALL);    // Must load the font first
    tft.setTextColor(WHITE,DEBUG_BOX?DEBUG_HC3:HC2);  
    tft.setTextDatum(TR_DATUM);
    tft.setTextPadding(0);
    tft.drawString(str,TFT_HEIGHT-20,TFT_WIDTH-(FOOTERH-2),GFXFF);  
}

void fanAIcon(){
  String str = animStateA ? "\"" : "#";
  animStateA = !animStateA;
  tft.setFreeFont(AA_FONT_SMALL);    // Must load the font first
  tft.setTextColor(WHITE,DEBUG_BOX?DEBUG_HC3:HC2);  
  tft.setTextDatum(TR_DATUM);
  tft.setTextPadding(0);
  tft.drawString(str,TFT_HEIGHT,TFT_WIDTH-(FOOTERH-1),GFXFF);  
}

void fanBIcon(){
  String str = animStateB? "\"" : "#";
  animStateB = !animStateB;
  tft.setFreeFont(AA_FONT_SMALL);    // Must load the font first
  tft.setTextColor(WHITE,DEBUG_BOX?DEBUG_HC3:HC2);  
  tft.setTextDatum(BR_DATUM);
  tft.setTextPadding(0);
  tft.drawString(str,TFT_HEIGHT,TFT_WIDTH,GFXFF);  // W,H
}

void fanCIcon(){
  tft.setFreeFont(AA_FONT_SMALL);    // Must load the font first
  tft.setTextColor(WHITE,DEBUG_BOX?DEBUG_HC3:HC2);  
  tft.setTextDatum(BR_DATUM);
  tft.setTextPadding(0);
  tft.drawString("&",TFT_HEIGHT-20,TFT_WIDTH,GFXFF);  // W,H
}

void fanAIcon_ring(unsigned int c){
  // String str = animStateB? "\"" : "#";
  // animStateB = !animStateB;
  tft.setFreeFont(AA_FONT_SMALL);    // Must load the font first
  tft.setTextColor(c);
  tft.setTextDatum(TR_DATUM);
  tft.setTextPadding(0);
  tft.drawString("$",TFT_HEIGHT-50-17,TFT_WIDTH-30,GFXFF);  
}

void fanBIcon_ring(unsigned int c,bool full){
  // String str = animStateB? "\"" : "#";
  // animStateB = !animStateB;
  tft.setFreeFont(AA_FONT_SMALL);    // Must load the font first
  tft.setTextColor(c);
  tft.setTextDatum(TR_DATUM);
  tft.setTextPadding(0);
  tft.drawString(full ? "$" : "%",TFT_HEIGHT-50-17-17,TFT_WIDTH-30,GFXFF);  
}

void doAlert(int mode = 0);

void updateAll(){
  updateTitle();
  updateTitleB();
  updateTitleC();

  updateFooterA_C(WHITE);

  if(currentTemp > hotTemp) drawFooterLine(RED);
  else if(currentTemp > coolTemp) drawFooterLine(ORANGE);
  else if(currentTemp > 25) drawFooterLine(GREEN);
  else drawFooterLine(BLACK);

  if(ERRORSTR !="") updateFooterB_ERROR();
  else if(SCROLL) UpdateScroll();
  // else updateFooterB();
  // updateFooterC(getFanStatus(1)>0);
  // updateFooterD(getFanStatus(1)>0);
  // doAlert(0);
  // drawFooterLine();
  wifiIcon();
  fanAIcon();
  // fanAIcon_ring(DKGRAY);
  fanBIcon();
  // fanBIcon_ring(LTBLACK,true);
  // fanBIcon_ring(TFT_SILVER,false);
  fanCIcon();
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

void tft_setupDraw(){
  tft.fillScreen(GRAPHBG);
  println_header("",LTBLACK);
  println_footer("",LTBLACK);
  tft.drawFastHLine(0, HEADERH, tft.width(), BGCOLOR);  
  tft.drawFastHLine(0, tft.height()-FOOTERH, tft.width(), BGCOLOR);  
}

void tft_init(){
  tft.init();
  tft.setRotation(3); 
}

#endif