#ifndef grapher_h
#define grapher_h


class grapher{
	public:

	grapher();
	grapher(int width = 0, int height = 0, int xPos = 0, int yPos = 0){

	int _id; // graph instance id

	int _x; // x position
	int _y; // y position
	int _w; // width
	int _h; // height

	int _yLower; // h min value
	int _yUpper; // h max value
	int _xLower; // v min value
	int _yUpper; // v max value

	int _xInt; // h grid interval units
	int _yInt; // v grid interval units
	// setter for set by pixels
	
	// options
	bool _showBaseline_x; // show x baseline
	bool _showBaseline_y; // show y baseline 

	bool _showAxis_x; // show x axis labels
	bool _showAxis_y; // show y axis labels

	bool refresh; // flag to redraw graph 


	// axis'
  	// axis padding
  	bool xaxis = false; // show xaxis padding
  	bool yaxis = false; // show yaxis padding
  	int xaxispad = 20; // set padding for graph when xaxis shown
  	int yaxispad = 16;	

	char *title        = "";    // title = title of graph
	char *xlabel       = "x";        // xlabel = x asis label
	char *ylabel       = "y";        // ylabel = y asis label


	// @todo refactor color customization
	unsigned int gcolor    = GRIDCOLOR; // gridline color
	unsigned int gxcolor   = DKGREY; // gridline x axis color @NI
	unsigned int gmincolor = DKGREY; // gridline minor color @NI
	unsigned int acolor    = RED;    // axis line color
	unsigned int lcolor    = GREY;    // axis label color
	unsigned int tcolor    = WHITE;  // text color
	unsigned int bcolor    = LTBLACK;  // bg color

}



#endif