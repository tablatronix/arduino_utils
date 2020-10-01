#include "grapher.h"


grapher::~grapher(int width = 0, int height = 0, int xPos = 0, int yPos = 0){
  _x = xPos;
  _y = yPos;
  _w = width;
  _h = height;

  init_graph();
}

// candidate for class
grapher::init_graph(){

  // class vars
  xPos = _x;
  yPos = _y;
  width = _w;
  height = _h;

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
  // height -= yPos+1;

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

  syncGraphInstance(graphID);

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

  setGraphInstance(graphID);

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


/**
 * [addPointSet description]
 * @param {[type]} int    id         [description]
 * @param {[type]} int    sample     [description]
 * @param {[type]} double value      [description]
 * @param {[type]} int    numSamples [description]
 * @param {Number} int    vsize      [description]
 * @param {Number} int    size       [description]
 */
void addPointSet(int id, int sample, double value, int numSamples,int vsize = 100,int size = 1,int yindexstart = 0){
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
    // int yindexstart = 65; // start Y at non 0 value
    unsigned int c = WHITE;
    if(size==0){
      size = 2;
      c = GRAY;
    }
    else c = getLineColor(id,0);
    addPoint(tft,id, i, value , 0, numSamples, yindexstart, vsize, c,size);

    // addPoint(tft,id, i, ((abs(vsize/numSamples))*(i)) , 0, numSamples, 0, vsize, getLineColor(i-1,0));    
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
