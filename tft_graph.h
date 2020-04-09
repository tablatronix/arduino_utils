#ifndef tft_graph_h
#define tft_graph_h

#include "Free_Fonts.h" // Include the header file attached to this sketch
// #include "NotoSansBold15.h"
// #include "NotoSansBold36.h"

// // The font names are arrays references, thus must NOT be in quotes ""
// #define AA_FONT_SMALL NotoSansBold15
// #define AA_FONT_LARGE NotoSansBold36

// tft graphing

  unsigned int gcolor    = GRIDCOLOR; // gridline color
  unsigned int gxcolor   = DKGREY; // gridline x axis color @NI
  unsigned int gmincolor = DKGREY; // gridline minor color @NI
  unsigned int acolor    = RED;    // axis line color
  unsigned int lcolor    = GREY;    // axis label color
  unsigned int tcolor    = WHITE;  // text color
  unsigned int bcolor    = LTBLACK;  // bg color

// these are the only external variables used by the graph function
// it's a flag to draw the coordinate system only on the first call to the Graph() function
// and will mimize flicker
// also create some variables to store the old x and y, if you draw 2 graphs on the same display
// you will need to store ox and oy per each display
boolean display1 = true;
boolean update1 = true;

// 6 trace instances for now, will move to obj class
double ox   = -999, oy   = -999; // Force them to be off screen
double ox_1 = -999, oy_1 = -999; // Force them to be off screen
double ox_2 = -999, oy_2 = -999; // Force them to be off screen
double ox_3 = -999, oy_3 = -999; // Force them to be off screen
double ox_4 = -999, oy_4 = -999; // Force them to be off screen
double ox_5 = -999, oy_5 = -999; // Force them to be off screen
double ox_6 = -999, oy_6 = -999; // Force them to be off screen

double _w,_h,_gx,_gy;


void setGraphLine(int id, double valueX,double valueY);
double getGraphLine(int id,int param);

/*

  function to draw a cartesian coordinate system and plot whatever data you want
  just pass x and y and the graph will be drawn

  huge arguement list
  &d name of your display object
  x = x data point
  y = y datapont
  dp = decimal points, precision 
  gx = x graph location (lower left) Should be TL
  gy = y graph location (lower left) Should be TL
  w = width of graph
  h = height of graph
  xlo = lower bound of x axis
  xhi = upper bound of x asis
  xinc = division of x axis (distance not count)
  ylo = lower bound of y axis
  yhi = upper bound of y asis
  yinc = division of y axis (distance not count)
  title = title of graph
  xlabel = x axis label
  ylabel = y axis label
  &redraw = flag to redraw graph on first call only
  color = plotted trace colour
*/


// options
// 
// show axis x
// show axis y
// axis x scale
// axis y scale
// axis x minor color
// axis y minor color
// save graph into memory so it can be xored with buffer to refresh redraw parts of it only for scrolled graphing ?
// 
// 
// wrappers
// add dashed line generator
// add double line generator, above below or center 2/3 pixels thick
// 


void Graph(RM_tft &tft, double x, double y, byte dp,
                           double gx, double gy, double w, double h,
                           double xlo, double xhi, double xinc,
                           double ylo, double yhi, double yinc,
                           char *title, char *xlabel, char *ylabel,
                           boolean &redraw, unsigned int pcolor) {

  double ydiv, xdiv;
  double i;
  double temp;
  int rot, newrot;
  _h = h;
  _w = w;
  _gx = gx;
  _gy = gy;

  bool drawXbaseline = false;
  bool drawYbaseline = false;

  if (redraw == true) {

    bool showaxis_x = true; // @NI
    bool showaxis_y = true; // @NI

    // redraw = false;
    // initialize old x and old y in order to draw the first point of the graph
    // but save the transformed value
    // note my transform funcition is the same as the map function, except the map uses long and we need doubles
    //ox = (x - xlo) * ( w) / (xhi - xlo) + gx;
    //oy = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;

    // tft.setTextDatum(MR_DATUM);

    // draw y scale
    for ( i = ylo; i <= yhi; i += yinc){
      // compute the transform
      temp =  (i - ylo) * (gy - h - gy) / (yhi - ylo) + gy; //posy

      if (i == 0 && drawYbaseline) {
        tft.drawLine(gx, temp, gx + w, temp, acolor); // Y a line
        tft.setTextColor(lcolor, bcolor);
        // tft.drawString(xlabel, (int)(gx + w -q 8) , (int)temp, 2); // x axis label
      }
      else {
        tft.drawLine(gx, temp, gx + w, temp, gcolor);
      }

      // draw the y axis values
      int yvalpad  = 4; // will not go negative ( might depend on driver and buffer though )
      int yvalsize = 1;
      // tft.setTextColor(tcolor, bcolor);
      tft.setTextColor(tcolor);
      // precision is default Arduino--this could really use some format control
      // if(dp>0) tft.drawFloat(i, dp, gx - yvalpad, temp, yvalsize); // y axis value labels w padding
      // else tft.drawString((String)(int)round(i),(int)(gx - yvalpad), (int)temp, yvalsize);
    }

    // draw x scale
    for (i = xlo; i <= xhi; i += xinc) {

      // compute the transform
      temp =  (i - xlo) * ( w) / (xhi - xlo) + gx; // posx
      if (i == 0 && drawXbaseline ) {
        tft.drawLine(temp, gy, temp, gy - h, acolor); // Y a-line @todo draws on top of y values
        tft.setTextColor(lcolor, bcolor);
        // tft.setTextDatum(BC_DATUM);
        // tft.drawString(ylabel, (int)temp, (int)(gy - h - 8) , 2); // y axis label BT-height w padding, @todo rotate
      }
      else {
        tft.drawLine(temp, gy, temp, gy - h, gcolor);
      }

      // Serial.println(i);
      // Serial.println(xhi);
      // Serial.println(xinc);
      // Serial.println((String)abs(xhi/xinc));
      // Close graph grid, @todo add actual border options
      // if(i == abs(xhi/xinc)){
        if(i!= xhi && i == abs(xhi/xinc)){
          tft.drawLine(gx + w, gy, gx + w, gy - h, gcolor); // Y end line (w-1?)
        }

      // draw the x axis values
      int xvalpad  = 7;
      int xvalsize = 1;
      // tft.setTextColor(tcolor, bcolor);
      tft.setTextColor(tcolor);
      // tft.setTextDatum(TC_DATUM);
      // precision is default Arduino--this could really use some format control
      // drawFloat(float floatNumber, uint8_t dp, int32_t poX, int32_t poY, [puint8_t font])
      // if(dp>0)tft.drawFloat(i, dp, temp, gy + xvalpad, xvalsize); // x axis value labels w padding
      // else tft.drawString((String)(int)round(i),(int)temp,(int)(gy + xvalpad), xvalsize); // v pad +7, fontsize 1
    }

    //draw graph title
    tft.setTextColor(tcolor, bcolor);
    // tft.drawString(title, (int)(gx + w / 2) , (int)(gy - h - 30), 4); // x axis label
  }

  // the coordinates are now drawn, plot the data
  // the entire plotting code are these few lines...
  // recall that ox and oy are initialized above
  //x =  (x - xlo) * ( w) / (xhi - xlo) + gx;
  //y =  (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
  //tft.drawLine(ox, oy, x, y, pcolor);
  // it's up to you but drawing 2 more lines to give the graph some thickness
  //tft.drawLine(ox, oy + 1, x, y + 1, pcolor);
  //tft.drawLine(ox, oy - 1, x, y - 1, pcolor);
  // ox = x;
  // oy = y;
}

void Trace(RM_tft &tft, int id, double x,  double y,  byte dp,
           double gx, double gy,
           double w, double h,
           double xlo, double xhi, double xinc,
           double ylo, double yhi, double yinc,
           char *title, char *xlabel, char *ylabel,
           boolean &update1, unsigned int pcolor)
{
  double ydiv, xdiv;
  double i;
  double temp;
  int rot, newrot;

  // initialize old x and old y in order to draw the first point of the graph
  // but save the transformed value
  // note my transform funcition is the same as the map function, except the map uses long and we need doubles
  if (update1) {
    update1 = false;
    
    ox = (x - xlo) * ( w) / (xhi - xlo) + gx;
    oy = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;

    setGraphLine(id,ox,oy);

    if ((ox < gx) || (ox > gx+w)) {update1 = true; return;}
    if ((oy < gy-h) || (oy > gy)) {update1 = true; return;}
    

    // tft.setTextDatum(MR_DATUM);

    // draw y scale
    for ( i = ylo; i <= yhi; i += yinc) {
      // compute the transform
      temp =  (i - ylo) * (gy - h - gy) / (yhi - ylo) + gy;

      if (i == 0) {
        tft.setTextColor(acolor, bcolor);
        // tft.drawString(xlabel, (int)(gx + w) , (int)temp, 2);
      }
      // draw the axis labels
      tft.setTextColor(tcolor, bcolor);
      // precision is default Arduino--this could really use some format control
      // tft.drawFloat(i, dp, gx - 4, temp, 1);
    }

    // draw x scale
    for (i = xlo; i <= xhi; i += xinc) {

      // compute the transform
      temp =  (i - xlo) * ( w) / (xhi - xlo) + gx;
      if (i == 0) {
        tft.setTextColor(acolor, bcolor);
        // tft.setTextDatum(BC_DATUM);
        // tft.drawString(ylabel, (int)temp, (int)(gy - h - 8) , 2);
      }

      // draw the axis labels
      tft.setTextColor(tcolor, bcolor);
      // tft.setTextDatum(TC_DATUM);
      // precision is default Arduino--this could really use some format control
      // tft.drawFloat(i, dp, temp, gy + 7, 1);
    }

    //now draw the graph labels
    tft.setTextColor(tcolor, bcolor);
    // tft.drawString(title, (int)(gx + w / 2) , (int)(gy - h - 30), 4);
  }

  // the coordinates are now drawn, plot the data
  // the entire plotting code are these few lines...
  // recall that ox and oy are initialized above
  x =  (x - xlo) * ( w) / (xhi - xlo) + gx;
  y =  (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;

  if ((x < gx) || (x > gx+w)) {update1 = true; return;}
  if ((y < gy-h) || (y > gy)) {update1 = true; return;}
    
  
  tft.drawLine(ox, oy, x, y, pcolor);
  // it's up to you but drawing 2 more lines to give the graph some thickness
  //tft.drawLine(ox, oy + 1, x, y + 1, pcolor);
  //tft.drawLine(ox, oy - 1, x, y - 1, pcolor);
  ox = x;
  oy = y;

}

/**
 * [addPoint description]
 * @param tft    tft instance
 * @param x      x coord
 * @param y      y coord
 * @param xlo    x lo
 * @param xhi    x high
 * @param ylo    y lo
 * @param yhi    y high
 * @param pcolor point color
 */
void addPoint(RM_tft &tft, int id, double x,  double y,
           double xlo, double xhi,
           double ylo, double yhi,
           unsigned int pcolor)
{

  double w = _w;
  double h = _h;
  double gx = _gx;
  double gy = _gy;
  bool doubleLine = false;

  // the coordinates are now drawn, plot the data
  // the entire plotting code are these few lines...
  // recall that ox and oy are initialized above
  x =  (x - xlo) * ( w) / (xhi - xlo) + gx;
  y =  (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;

  if ((x < gx) || (x > gx+w)) {return;}
  if ((y < gy-h) || (y > gy)) {return;}

  
  // Serial.print("\n[GRAPH] x - y ");
  // Serial.print(x);
  // Serial.print(" - ");
  // Serial.println(y);
  // Serial.print("\n[GRAPH] ox - oy ");
  // Serial.print(ox);
  // Serial.print(" - ");
  // Serial.println(oy);

  // Serial.print(x);
  // Serial.print(" - ");
  // Serial.println(y);
  // Serial.println("------");
  double ox = getGraphLine(id,0);
  double oy = getGraphLine(id,1);

  tft.drawLine(ox, oy, x, y, pcolor); // STILL DOUBLE!

  // drawing 2 more lines to give the graph some thickness
  if(doubleLine){
    tft.drawLine(ox, oy + 1, x, y + 1, pcolor);
    tft.drawLine(ox, oy - 1, x, y - 1, pcolor);
  }

  // store last point
  setGraphLine(id,x,y);
  // ox = x;
  // oy = y;
}

/*

  End of graphing function

*/

double getGraphLine(int id,int param){
  switch(id){
  case 0:
    if(param == 0) return ox;
    if(param == 1) return oy;
    // ox = valueX, oy = valueY;
    break;
  case 1:
    if(param == 0) return ox_1;
    if(param == 1) return oy_1;  
    break;
  case 2:
    if(param == 0) return ox_2;
    if(param == 1) return oy_2;  
    break;
  case 3:
    if(param == 0) return ox_3;
    if(param == 1) return oy_3;  
    break;
  case 4:
    if(param == 0) return ox_4;
    if(param == 1) return oy_4;  
    break;
  case 5:
    if(param == 0) return ox_5;
    if(param == 1) return oy_5;  
    break;
  case 6:
    if(param == 0) return ox_6;
    if(param == 1) return oy_6;  
    break;
  }
}


unsigned int getLineColor(int id,int param){
  switch(id){
  case 0:
    if(param == 0) return BLUE;
    if(param == 1) return oy;
    // ox = valueX, oy = valueY;
    break;
  case 1:
    if(param == 0) return RED;
    if(param == 1) return oy_1;  
    break;
  case 2:
    if(param == 0) return GREEN;
    if(param == 1) return oy_2;  
    break;
  case 3:
    if(param == 0) return YELLOW;
    if(param == 1) return oy_3;  
    break;
  case 4:
    if(param == 0) return WHITE;
    if(param == 1) return oy_4;  
    break;
  case 5:
    if(param == 0) return ORANGE;
    if(param == 1) return oy_5;  
    break;
  case 6:
    if(param == 0) return PURPLE;
    if(param == 1) return oy_6;  
    break;
  }
}

void setGraphLine(int id = 0, double valueX = -999,double valueY = -999){
  switch(id){
  case 0:
    ox = valueX, oy = valueY;
    break;
  case 1:
    ox_1 = valueX, oy_1 = valueY;
    break;
  case 2:
    ox_2 = valueX, oy_2 = valueY;
    break;
  case 3:
    ox_3 = valueX, oy_3 = valueY;
    break;
  case 4:
    ox_4 = valueX, oy_4 = valueY;
    break;
  case 5:
    ox_5 = valueX, oy_5 = valueY;
    break;
  case 6:
    ox_6 = valueX, oy_6 = valueY;
    break;
  }
}

void resetGraphLine(int id = 0, double value = -999){
  setGraphLine(id);
}

void resetGraphLines(){
  for(size_t i =0; i< 7;i++){
    resetGraphLine(i);
  }
}

void init_graph_tft(){
  // if (TFT_BL > 0) {
  //     pinMode(TFT_BL, OUTPUT);
  //     digitalWrite(TFT_BL, HIGH);
  // }

  tft.begin();
  tft.fillScreen(HC1);
  tft.setRotation(3);
}

void drawBorder(unsigned int c = BLACK){
  // Draw border left bottom
  tft.drawFastVLine(0,0,tft.height(),c);
  tft.drawFastHLine(0,tft.height()-1,tft.width()-1,c);

  tft.drawFastVLine(tft.width()-1,0,tft.height()-1,c);
  tft.drawFastHLine(0,0,tft.width()-1,c);  
}

void init_graph(int width = 0, int height = 0, int xPos = 0, int yPos = 0){
  double x, y;

  // use full size
  // setup basic inputs, x,y and W,h, add padding automatically if showing axis, and auto compensate for graph function y offset bottom left!
  
  // int height;
  // int width;
  bool invertWH = false;

  if(width == 0 && height == 0){
  	width  = TFT_WIDTH; //240;
  	height = TFT_HEIGHT; // 320;
    invertWH = true;
  }

  // invert width height (tft_eSPI uses shortend wide for some reason)
  if(invertWH){
    int _height = height;
    height = width;
    width  = _height;
  }

  // init defaults
  Serial.println("init w:" + (String)width + " h: " + (String)height);
  // int xPos = 0;
  // int yPos = 0;
  
  // @todo add better clamping for display size
  // if xpos + width > tft.width
  // if ypos + height > tft.height
  width  -= xPos+1; // reduce width kludge
  height -= yPos+1;

  Serial.println("init x:" + (String)width + " y: " + (String)height);

  // axis padding
  bool xaxis = false; // show xaxis padding
  bool yaxis = false; // show yaxis padding
  int xaxispad = 20; // set padding for graph when xaxis shown
  int yaxispad = 16;
 
  int16_t graph_w = width; // temp vars, edit in place
  int16_t graph_h = height;

  int graphOff_x  = xPos;
  int graphOff_y  = yPos+height; // translate y position from TL to BL relative

  if(xaxis){
    graph_w     += -xaxispad;
    graphOff_x  += xaxispad;
  }
  if(yaxis){
    graph_h    += -yaxispad;
    graphOff_y += -yaxispad;
  }

  	// data set
	double _x          = 0;          // x = x data point
	double _y          = 0;          // y = y datapont
	byte   dp          = 0;          // dp = decimal place precision
	double gx          = graphOff_x; // gx = x graph location (top left)
	double gy          = graphOff_y; // gy = y graph location (bottom left)
	double w           = graph_w;    //  w = width of graph
	double h           = graph_h;    //  h = height of graph

  double xlo         = 0;          // xlo = lower bound of x axis - x scale min
  double xhi         = 100;        // xhi = upper bound of x axis - x scale max
  double xinc        = 5;          // xinc = division of x axis (distance not count)

  double ylo         = 0;         // ylo = lower bound of y axis - y scale min
  double yhi         = 100;          // yhi = upper bound of y axis  - y scale max
  double yinc        = 5;        // yinc = division of y axis (distance not count)

	char *title        = "";    // title = title of graph
	char *xlabel       = "x";        // xlabel = x asis label
	char *ylabel       = "y";        // ylabel = y asis label
	boolean &redraw    = display1;   // &redraw = flag to redraw graph on first call only
	unsigned int color = PURPLE;     // color = plotted trace colour


  // tft.fillRoundRect((int16_t)xPos-3,(int16_t)yPos-5,(int16_t)w+8,(int16_t)h+10,5,BGOVER);
  tft.fillRect((int16_t)xPos,(int16_t)yPos-6,(int16_t)w,(int16_t)h+11,BGCOLOR);
  drawBorder();
	Graph(tft, _x, _y, dp, gx, gy,    w,          h, xlo, xhi, xinc, ylo, yhi, yinc, title, xlabel, ylabel,redraw, color);
	// Graph(tft, x, y, 2,  30, 135-8, 240-30, 135-8, 0, 6.5, 1, -1, 1, .25, "x", "y", "", display1, YELLOW);
}

void setupGraph(){
    init_graph(320,240-35,0,30); // x,y offsets handled automatically  
}

void test_trace_sin(){
  double x, y; 
  for (x = 0; x <= 6.3; x += .1) {
    y = sin(x);
    // Trace(tft, x, y, 1, 60, 290, 390, 260, 0, 6.5, 1, -1, 1, .25, "Sin(x)", "x", "fn(x)", update1, YELLOW);
    addPoint(tft, 0,x, y, 0, 6.5, -1, 1, YELLOW);
    // Graph(tft, x, y, dp, gx, gy,    w,          h, xlo, xhi, xinc, ylo, yhi, yinc, title, xlabel, ylabel,redraw, color);
    delay(100);
  }  
}

void test_trace(uint32_t c){
  double x, y; 
  for (x = 0; x <= 100; x += 10) {
    addPoint(tft, 0,x, x, 0, 100, 0, 100, c);
    delay(100);
  }
}

void testTraces(int sample){
  int ssize = 240;
  int vsize = 100;
  for(size_t i=0;i<7;i++){
    addPoint(tft,i,sample, ((abs(vsize/7))*(i))+random(10), 0, ssize, 0, vsize-random(vsize/6), getLineColor(i,0));    
    delay(10);
  }
}


void tft_set_footer_val1(){

}

void tft_set_footer_val2(String str = "Status"){
    tft.setTextColor(GREY,HC2);  
    tft.setTextDatum(BC_DATUM);
    tft.setTextPadding(10);
    // println_footer("",HC2);
    tft.drawString("Status Message",TFT_HEIGHT/2,TFT_WIDTH-4,2);
}

void tft_set_footer_val3(){
  
}

void tft_set_footer_val4(){
  
}

int fpsmicros = millis();
char buffer [32];

void tft_footer_val1(){
    // tft.loadFont(AA_FONT_LARGE);    // Must load the font first

    tft.setTextSize(1);
    tft.setTextColor(DKGREY,HC2);  
    tft.setTextDatum(TR_DATUM);
    tft.drawString("FAN 1",TFT_HEIGHT-2,TFT_WIDTH-30,2);    
    tft.setTextDatum(BR_DATUM);
    tft.drawString("FAN 2",TFT_HEIGHT-2,TFT_WIDTH,2);

    tft.setTextColor(WHITE,HC2);  
    tft.setTextDatum(BL_DATUM);
    tft.drawString("220`c",5,TFT_WIDTH,4);

    tft_set_footer_val2();    
}


void displayFPS(){
    tft_footer_val1();
    return;
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

// float smoothing = 0.9; // larger=more smoothing
// measurement = (measurement * smoothing) + (current * (1.0-smoothing))

#endif