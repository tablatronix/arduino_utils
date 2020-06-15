#ifndef tft_graph_h
#define tft_graph_h

bool _DEBUG_POINT = false;
bool _debug_box = false;

int filteredId = 1; // filter graph line indexs -1 off

int FOOTERH = 40;
int HEADERH = 30;

#include "Free_Fonts.h" // Include the header file attached to this sketch
// #include "NotoSansBold15.h"
// #include "NotoSansBold36.h"

#include "InterpolationLib.h"

// #define AA_FONT_SMALL NotoSansBold15
// #define AA_FONT_LARGE NotoSansBold36

// tft graphing

// @todo refactor color customization
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
double ox_7 = -999, oy_7 = -999; // Force them to be off screen
double ox_8 = -999, oy_8 = -999; // Force them to be off screen
double ox_9 = -999, oy_9 = -999; // Force them to be off screen

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
 * @param id     trace id (hardcoded 0-6 ATM)
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
           unsigned int pcolor, int size = 0)
{

  if(_DEBUG_POINT){
    Serial.print("\n[GRAPH] ADDPOINT id: "+(String)id+ " x - y: ");
    Serial.print(x);
    Serial.print(" - ");
    Serial.println(y);
  }

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
  
  double ox = getGraphLine(id,0);
  double oy = getGraphLine(id,1);

  if(_DEBUG_POINT){
    Serial.print("\n[GRAPH] x - y: ");
    Serial.print(x);
    Serial.print(" - ");
    Serial.println(y);
    Serial.print("\n[GRAPH] ox - oy: ");
    Serial.print(ox);
    Serial.print(" - ");
    Serial.println(oy);

    Serial.print("\n[GRAPH] gx - gy: ");
    Serial.print(gx);
    Serial.print(" - ");
    Serial.println(gy);
    Serial.println("------");
    Serial.println("DrawLine");
    Serial.flush();
  }

  tft.drawLine(ox, oy, x, y, pcolor); // STILL DOUBLE!

  // drawing 2 more lines to give the graph some thickness
  if(size>1){
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

int numGraphSlots = 10;

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
  case 7:
    if(param == 0) return ox_7;
    if(param == 1) return oy_7;  
    break;
  case 8:
    if(param == 0) return ox_8;
    if(param == 1) return oy_8;  
    break;
  case 9:
    if(param == 0) return ox_9;
    if(param == 1) return oy_9;  
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
    if(param == 0) return GREENYELLOW;
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
    case 7:
    if(param == 0) return MAGENTA;
    if(param == 1) return oy_7;  
    break;
    case 8:
    if(param == 0) return CYAN;
    if(param == 1) return oy_8;  
    break;
    case 9:
    if(param == 0) return PINK;
    if(param == 1) return oy_9;  
    break;
  }
}

void setGraphLine(int id, double valueX, double valueY){
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
  case 7:
    ox_7 = valueX, oy_7 = valueY;
    break;
  case 8:
    ox_8 = valueX, oy_8 = valueY;
    break;
  case 9:
    ox_9 = valueX, oy_9 = valueY;
    break;
  }
}

void resetGraphLine(int id = 0){
  setGraphLine(id,-999,-999);
}

void resetGraphLines(){
  for(size_t i =0; i< numGraphSlots;i++){
    resetGraphLine(i);
  }
}

/**
 * [addPointSet description]
 * @param {[type]} int    id         [description]
 * @param {[type]} int    sample     [description]
 * @param {[type]} double value      [description]
 * @param {[type]} int    numSamples [description]
 * @param {Number} int    vsize      [description]
 * @param {Number} int    size       [description]
 */
void addPointSet(int id, int sample, double value, int numSamples,int vsize = 100,int size = 1){
    // if(id != filteredId && filteredId >= 0) return;
    if(_DEBUG_POINT){
      Serial.println("addPointSet");
      Serial.println(id);
      Serial.println(sample);
      Serial.println(value);
    }
    int i = sample;
    // int vsize = 100;
    // if(sample=0) addPoint(tft,id, 0, 0 , 0, numSamples, 0, vsize, getLineColor(0,0)); // @FIXME does not show up , must add 0 point
    int yindexstart = 0; // start Y at non 0 value
    unsigned int c = WHITE;
    if(size==0){
      size = 2;
      c = GRAY;
    }
    else c = getLineColor(id,0);
    addPoint(tft,id, i, value , 0, numSamples, yindexstart, vsize, c,size);

    // addPoint(tft,id, i, ((abs(vsize/numSamples))*(i)) , 0, numSamples, 0, vsize, getLineColor(i-1,0));    
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

void drawBorder(unsigned int c = LTBLACK){
  // Draw border left bottom
  tft.drawFastVLine(0,0,tft.height(),c);
  tft.drawFastHLine(0,tft.height()-1,tft.width()-1,c);

  tft.drawFastVLine(tft.width()-1,0,tft.height()-1,c);
  tft.drawFastHLine(0,0,tft.width()-1,c);  
}

void drawBorderB(unsigned int c = LTBLACK){
  // Draw border left bottom
  tft.drawFastVLine(1,1,tft.height(),c);
  tft.drawFastHLine(1,tft.height()-2,tft.width()-2,c);

  tft.drawFastVLine(tft.width()-2,1,tft.height()-2,c);
  tft.drawFastHLine(1,1,tft.width()-2,c);  
}


// candidate for class
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
  Serial.println("[GRAPH] init w:" + (String)width + " h: " + (String)height);
  // int xPos = 0;
  // int yPos = 0;
  
  // @todo add better clamping for display size
  // if xpos + width > tft.width
  // if ypos + height > tft.height
  width  -= xPos+1; // reduce width kludge
  height -= yPos+1;

  Serial.println("[GRAPH] init x:" + (String)width + " y: " + (String)height);

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

  // @todo make grid value based not pixel, stupid
  double xlo         = 0;          // xlo = lower bound of x axis - x scale min
  double xhi         = 100;        // xhi = upper bound of x axis - x scale max
  double xinc        = 5;          // xinc = division of x axis (pixels not count)

  int xgridpx = (h/(xhi-xlo))*xinc;

  double ylo         = 0;         // ylo = lower bound of y axis - y scale min
  double yhi         = 100;       // yhi = upper bound of y axis  - y scale max
  double yinc        = 5;        // yinc = division of y axis (pixels not count)
  
  int ygridpx = (w/(yhi-ylo))*yinc;

	char *title        = "";    // title = title of graph
	char *xlabel       = "x";        // xlabel = x asis label
	char *ylabel       = "y";        // ylabel = y asis label
	boolean &redraw    = display1;   // &redraw = flag to redraw graph on first call only
	
  unsigned int color = PURPLE;     // color = plotted trace colour
  unsigned int bgcolor = bcolor;

  Serial.println("[GRAPH] grid x px: " + (String)xgridpx);
  Serial.println("[GRAPH] grid y px: " + (String)ygridpx);

  // draw background to overwrite previous traces
  int padding = 3; // we add bg padding so you can see traces at 0 and max
  tft.fillRect((int16_t)xPos,(int16_t)yPos-padding,(int16_t)w,(int16_t)h+(padding*2),bgcolor);
  // drawBorder(RED);
	Graph(tft, _x, _y, dp, gx, gy,    w,          h, xlo, xhi, xinc, ylo, yhi, yinc, title, xlabel, ylabel,redraw, color);
	// Graph(tft, x, y, 2,  30, 135-8, 240-30, 135-8, 0, 6.5, 1, -1, 1, .25, "x", "y", "", display1, YELLOW);
}



// test stuff an old 

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
    // addPoint(RM_tft &tft, int id, double x,  double y,double xlo, double xhi,double ylo, double yhi,unsigned int pcolor)    
    // addPoint(RM_tft &tft, id, x,  y, xlo, xhi ylo, yhi, color)    
    addPoint(tft, 0,x, x, 0, 100, 0, 100, c);
    delay(100);
  }
}

// add a bunch of traces for testing
void testTraces(int sample){
  return;
  int ssize = 240;
  int vsize = 100;
  int numTraces = 4;
  for(int i=0;i<numTraces;i++){
    addPoint(tft,i/* id **/,sample/* X */, ((abs(vsize/numTraces))*(i))+random(10)/* Y */, 0 /*XLOW*/, ssize /* XHIGH */, 0 /* YLOW */, vsize-random(vsize/6)/* YHIGH */, getLineColor(i,0) /* color */);    
    delay(10);
  }
}

void testTraceScaleOLD(){
  int id = 5;
  int ssize = 240;
  int vsize = 100;
  int numSamples = 5;

  // setGraphLine(id,0,0);
  // addPoint(tft,id, 0, 0 ,  0, 5, 0, vsize, getLineColor(0,0)); // @FIXME does not show up , must add 0 point
  // addPoint(tft,id, 0, 0 ,  0, 5, 0, vsize, getLineColor(0,0));    
  // addPoint(tft,id, 1, 10 , 0, 5, 0, vsize, getLineColor(0,0));    
  // addPoint(tft,id, 2, 20 , 0, 5, 0, vsize, getLineColor(1,0));
  // return;
  
  // addPoint(tft,id, 0, 0 ,  0, 5, 0, vsize, getLineColor(0,0)); // @FIXME does not show up , must add 0 point
  // setGraphLine(id,0,0); ?
  for(int i=0;i<numSamples+2;i++){
     // addPoint(RM_tft &tft, id, x,  y, xlo, xhi ylo, yhi, color)    
    addPoint(tft,id, i, ((abs(vsize/numSamples))*(i)) , 0, 5, 0, vsize, getLineColor(i-1,0));    
    delay(100);
  }
}

// testing prescaling, you can already pass in ylo yhi and let graph point scale, this scales actual values
void testTraceScale(){
  int id = 5;
  int ssize = 240;
  int vsize = 100;
  int numSamples = 5;

  // setGraphLine(id,0,0);
  // addPoint(tft,id, 0, 0 ,  0, 5, 0, vsize, getLineColor(0,0)); // @FIXME does not show up , must add 0 point
  // addPoint(tft,id, 0, 0 ,  0, 5, 0, vsize, getLineColor(0,0));    
  // addPoint(tft,id, 1, 10 , 0, 5, 0, vsize, getLineColor(0,0));    
  // addPoint(tft,id, 2, 20 , 0, 5, 0, vsize, getLineColor(1,0));
  // return;
  
  // addPoint(tft,id, 0, 0 ,  0, 5, 0, vsize, getLineColor(0,0)); // @FIXME does not show up , must add 0 point
  // setGraphLine(id,0,0);
  for(int i=0;i<numSamples+2;i++){
    addPointSet(id,i,random(vsize),numSamples);
 // addPoint(RM_tft &tft, id, x,  y, xlo, xhi ylo, yhi, color)    
    // addPoint(tft,id, i, ((abs(vsize/numSamples))*(i)) , 0, 5, 0, vsize, getLineColor(i-1,0));    
    delay(100);
  }
}

void graphPaste(){
  int id = 6;
  int ssize = 320;
  int vsize = 230;
  int numSamples = 7;

  // id = 0;
  // numSamples = 7; 
  // resetGraphLine(id);
  // float baseGraphX[7] = { 1, 90, 180, 210, 240, 270, 300 }; // time
  // float baseGraphY[7] = { 27, 90, 130, 138, 165, 138, 27 }; // value
  // for(int i=0;i<numSamples;i++){
  //   addPointSet(id,baseGraphX[i],baseGraphY[i],300,vsize);
  // }

  // solderPaste[0] = ReflowGraph( "CHIPQUIK", "No-Clean Sn42/Bi57.6/Ag0.4", 138, baseGraphX, baseGraphY, ELEMENTS(baseGraphX), 240, 270 );
 
  // id = 1;
  // numSamples = 7;  
  // resetGraphLine(id);  
  // float baseGraphX2[7] = { 1, 90, 180, 225, 240, 270, 300 }; // time
  // float baseGraphY2[7] = { 25, 150, 175, 190, 210, 125, 50 }; // value

  // // solderPaste[1] = ReflowGraph( "CHEMTOOLS L", "No Clean 63CR218 Sn63/Pb37", 183, baseGraphX2, baseGraphY2, ELEMENTS(baseGraphX2), 240, 270 );
  // for(int i=0;i<numSamples;i++){
  //   addPointSet(id,baseGraphX2[i],baseGraphY2[i],ssize,vsize);
  // }

  // id = 2;
  // numSamples = 6;
  // resetGraphLine(id);   
  // float baseGraphX3[6] = { 1, 75, 130, 180, 210, 250 }; // time
  // float baseGraphY3[6] = { 25, 150, 175, 210, 150, 50 }; // value

  // // solderPaste[2] = ReflowGraph( "CHEMTOOLS S", "No Clean 63CR218 Sn63/Pb37", 183, baseGraphX3, baseGraphY3, ELEMENTS(baseGraphX3), 180, 210 );
  // for(int i=0;i<numSamples;i++){
  //   addPointSet(id,baseGraphX3[i],baseGraphY3[i],ssize,vsize);
  // }

  // tmin = 
  // tmax = 

  // id = 2;
  // numSamples = 7;
  // resetGraphLine(id);    
  // float baseGraphX4[7] = { 1, 60-20, 120, 160, 210, 260, 310 }; // time
  // float baseGraphY4[7] = { 25, 105, 150, 150, 220, 150, 20 }; // value

  // // solderPaste[3] = ReflowGraph( "DOC SOLDER", "No Clean Sn63/Pb36/Ag2", 187, baseGraphX4, baseGraphY4, ELEMENTS(baseGraphX4), 210, 260 );
  // for(int i=0;i<numSamples;i++){
  //   addPointSet(id,baseGraphX4[i],baseGraphY4[i],ssize,vsize);
  // }

  id = 3;
  numSamples = 7;
  resetGraphLine(id);    
  double baseGraphX5[7] = {  1,  60, 120, 160, 210, 260,  310  }; // time
  double baseGraphY5[7] = { 25, 105, 150, 150, 220, 220,  25 }; // value

  // solderPaste[3] = ReflowGraph( "DOC SOLDER", "No Clean Sn63/Pb36/Ag2", 187, baseGraphX4, baseGraphY4, ELEMENTS(baseGraphX4), 210, 260 );
  
  // Serial.print(Interpolation::CatmullSpline(xValues, yValues, numValues, xValue));

 int maxtime = 310; // max time frame for reflow, or make sure last point is not drawn
 int maxtemp = 220+20; // max value + top padding

  for(int i=0;i<maxtime;i++){
    double x;

    if(filteredId == 0){
    x = Interpolation::CatmullSpline(baseGraphX5, baseGraphY5, numSamples, i);
    addPointSet(0,i,x,ssize,maxtemp);
    }

    if(filteredId == 1){    
      x = Interpolation::SmoothStep(baseGraphX5, baseGraphY5, numSamples, i);
      addPointSet(1,i,x,ssize,maxtemp,2);
    }

    if(filteredId == 2){    
     x = Interpolation::Linear(baseGraphX5, baseGraphY5, numSamples, i,false);
     addPointSet(2,i,x,ssize,maxtemp);
    }

    if(filteredId == 3){
      x = Interpolation::Linear(baseGraphX5, baseGraphY5, numSamples, i,true);
      addPointSet(3,i,x,ssize,maxtemp);
    }

    if(filteredId == 4){
      x = Interpolation::ConstrainedSpline(baseGraphX5, baseGraphY5, numSamples, i);
      addPointSet(4,i,x,ssize,maxtemp);
    }

    if(filteredId == 5){
      x = Interpolation::Step(baseGraphX5, baseGraphY5, numSamples, i, 0.0);
      addPointSet(5,i,x,ssize,maxtemp);
    }

    if(filteredId == 6){
      x = Interpolation::Step(baseGraphX5, baseGraphY5, numSamples, i, 0.5);
      addPointSet(6,i,x,ssize,maxtemp);
    }

    if(filteredId == 7){
      x = Interpolation::Step(baseGraphX5, baseGraphY5, numSamples, i, 0.2);
      addPointSet(7,i,x,ssize,maxtemp);
    }
  }

  // id = 4;
  // numSamples = 8; 
  // resetGraphLine(id);    
  // float baseGraphX6[8] = { 1, 20, 90, 120, 160, 210, 260, 310 }; // time
  // float baseGraphY6[8] = { 0, 0, 105, 150, 150, 220, 150, 20 }; // value

  // solderPaste[3] = ReflowGraph( "DOC SOLDER", "No Clean Sn63/Pb36/Ag2", 187, baseGraphX4, baseGraphY4, ELEMENTS(baseGraphX4), 210, 260 );
  // for(int i=0;i<numSamples;i++){
  //   addPointSet(id,baseGraphX6[i],baseGraphY6[i],ssize,vsize);
  // }

  // id = 4;
  // numSamples = 6;
  // resetGraphLine(id);
  // float baseGraphX5[6] = { 1,   90,  100,  160, 170, 200  }; // time
  // float baseGraphY5[6] = { 25, 100, 110, 110, 100, 25 }; // value
  // solderPaste[4] = ReflowGraph( "PLA", "Anneal 110C/30min", 110, baseGraphX5, baseGraphY5, ELEMENTS(baseGraphX5), 160, 170 );
  // for(int i=0;i<numSamples;i++){
  //   addPointSet(id,baseGraphX4[i],baseGraphY4[i],ssize,vsize);
  // }

  return;

  // addPoint(tft,id, 0, 0 ,  0, 5, 0, vsize, getLineColor(0,0)); // @FIXME does not show up , must add 0 point  
  // setGraphLine(id,0,0);
  for(int i=0;i<numSamples;i++){
    // addPointSet(id,baseGraphX[i],baseGraph Y[i],ssize,vsize);
  }
}

void interpolationGraph(){

  const int numValues = 10;
  double xValues[10] = {   5,  12,  30,  50,  60,  70,  74,  84,  92, 100 };
  double yValues[10] = { 150, 200, 200, 200, 180, 100, 100, 150, 220, 320 };

  for(int i = 0; i < numValues; i++){
    Serial.print(Interpolation::CatmullSpline(xValues,yValues, numValues, i));
    addPointSet(6,xValues[i],yValues[i],230,320);
  }

  return;

  for (float xValue = 0; xValue <= 110; xValue += .25)
  {
    
    Serial.print(Interpolation::Step(xValues, yValues, numValues, xValue, 0.0));
    Serial.print(',');
    Serial.print(Interpolation::Step(xValues, yValues, numValues, xValue, 0.5));
    Serial.print(',');
    Serial.print(Interpolation::Step(xValues, yValues, numValues, xValue, 1.0));
    Serial.print(',');
    Serial.print(Interpolation::SmoothStep(xValues, yValues, numValues, xValue));
    Serial.print(',');
    Serial.print(Interpolation::Linear(xValues, yValues, numValues, xValue, false));
    Serial.print(',');
    Serial.print(Interpolation::Linear(xValues, yValues, numValues, xValue, true));
    Serial.print(',');
    Serial.print(Interpolation::CatmullSpline(xValues, yValues, numValues, xValue));
    Serial.print(',');
    Serial.println(Interpolation::ConstrainedSpline(xValues, yValues, numValues, xValue));
  }
}

void setupGraph(){
    int ypad = 3;
    // TFT+TFT_WIDTH,TFT_HEIGHT - footerH,
    init_graph(320,240-(FOOTERH+ypad),0,HEADERH+ypad); // x,y offsets handled automatically and adjusted to TL 
    // testTraceScale();
    graphPaste();
    // interpolationGraph();
}

void tft_set_header_val1(String str){
    // Serial.println("footer val1: " + str);
    if(_debug_box) tft.setTextColor(WHITE,RED);
    else tft.setTextColor(WHITE,HC1);
    tft.setTextDatum(TL_DATUM);
    int lpad = 2;
    tft.setTextPadding(115);
    // drawDatumMarker(2,2);
    lpad += tft.drawString(str,lpad,lpad,4); // 22
    // Serial.println(lpad);
}

void tft_set_header_val2(String str){
    if(_debug_box) tft.setTextColor(HC1_text,BLUE);
    else tft.setTextColor(HC1_text,HC1);
    tft.setTextDatum(BL_DATUM);
    int lpad = 2;
    tft.setTextPadding(120);
    // drawDatumMarker(120,25);
    lpad += tft.drawString(str,120,25,2); // 22
    // Serial.println(lpad);
}

void tft_set_header_val3(String str){
    if(_debug_box) tft.setTextColor(HC1_text,GREEN);
    else tft.setTextColor(HC1_text,HC1);
    tft.setTextDatum(BR_DATUM);
    int rpad = 4;
    tft.setTextPadding(60);
    // drawDatumMarker(TFT_HEIGHT-rpad,29);
    rpad += tft.drawString(str,TFT_HEIGHT-rpad,29,4); // 22
    // Serial.println(lpad);
}

void tft_set_footer_val1(float str = 0){
    // Serial.println("footer val1: " + str);
    if(_debug_box) tft.setTextColor(WHITE,RED);  
    else tft.setTextColor(WHITE,HC2);  
    tft.setTextDatum(BL_DATUM);
    int lpad = 5;
    tft.setTextPadding(90);
    lpad = tft.drawFloat((float)str,1,lpad,TFT_WIDTH,4); // 63
    tft.setTextPadding(0);
    lpad += tft.drawString("`c",lpad+6,TFT_WIDTH,4); // 22
    // Serial.println(lpad);
}

void tft_set_footer_val2(String str = "Status"){
    // str = "ABCDEFGHIJKLMNOPQR"; // 19 characters
    // str = "Ms: "+(String)(millis())+" RSSI: "+(String)getRSSIasQuality(); // 19 characters
    if(_debug_box) tft.setTextColor(GREY,BLUE);
    else tft.setTextColor(GREY,HC2);
    tft.setTextDatum(BL_DATUM);
    tft.setTextPadding(145);
    // println_footer("",HC2);
    tft.drawString(str,(TFT_HEIGHT/3)+10,TFT_WIDTH-4,2);
}

void tft_set_footer_val2_left(String str = "Status"){
    // str = "ABCDEFGHIJKLMNOPQR"; // 19 characters
    // str = "Ms: "+(String)(millis())+" RSSI: "+(String)getRSSIasQuality(); // 19 characters
    if(_debug_box) tft.setTextColor(GREY,BLUE);
    else tft.setTextColor(GREY,HC2);

    tft.setTextDatum(BL_DATUM);
    tft.setTextPadding(150);
    // println_footer("",HC2);
    tft.drawString(str,((TFT_HEIGHT/3)-5),TFT_WIDTH-4,2);
}

void scrollStatus(String str){
  static int charindex = 0;
  int chars = 20;
  int len = str.length()+20;
  String pad;
  for(int i=0;i<chars;i++){
    pad += " ";
  }
  str = pad+str;
  String newstr = str.substring(charindex, charindex+chars < len ? charindex+chars : len );
  tft_set_footer_val2_left(newstr);
  charindex++;
  if(charindex>len) charindex = 0;
}

void tft_set_footer_val2_error(String str = "Status"){
    tft.setTextColor(RED,HC2);  
    tft.setTextDatum(BC_DATUM);
    tft.setTextPadding(15);
    // println_footer("",HC2);
    tft.drawString("str",TFT_HEIGHT/2,TFT_WIDTH-4,2);
}

void tft_set_footer_val3(bool enable){
    tft.setTextSize(1);
    tft.setTextColor(enable?GREEN:DKGREY,HC2);  
    tft.setTextDatum(TR_DATUM);
    tft.setTextPadding(0);
    tft.drawString("FAN 1",TFT_HEIGHT-2,TFT_WIDTH-30,2);    
    // tft.setTextDatum(BR_DATUM);
    // tft.drawString("FAN 2",TFT_HEIGHT-2,TFT_WIDTH,2);  
}

void tft_set_footer_val4(bool enable){
    tft.setTextSize(1);
    tft.setTextColor(enable?GREEN:DKGREY,HC2);  
    // tft.setTextDatum(TR_DATUM);
    // tft.drawString("FAN 1",TFT_HEIGHT-2,TFT_WIDTH-30,2);    
    tft.setTextPadding(0);
    tft.setTextDatum(BR_DATUM);
    tft.drawString("FAN 2",TFT_HEIGHT-2,TFT_WIDTH,2);    
}

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

int fpsmicros = millis();
char buffer [32];

void displayFPS(){
    // tft_footer_val1();
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

// what is this note? lol
// float smoothing = 0.9; // larger=more smoothing
// measurement = (measurement * smoothing) + (current * (1.0-smoothing))


#endif