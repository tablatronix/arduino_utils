#ifndef tft_espi_shared_h
#define tft_espi_shared_h

// tft_spi graphics library

#include "SPI.h"
#include "TFT_eSPI.h"

// using ADA_tft = Adafruit_ST7735;
// ADA_tft tft = RM_tft(TFT_CS, TFT_DC, TFT_RST);


// TL_DATUM = Top left
// TC_DATUM = Top centre
// TR_DATUM = Top right
// ML_DATUM = Middle left
// MC_DATUM = Middle centre
// MR_DATUM = Middle right
// BL_DATUM = Bottom left
// BC_DATUM = Bottom centre
// BR_DATUM = Bottom right

/***************************************************************************************
**                         Section 6: Colour enumeration
***************************************************************************************/
// Default color definitions
// #define TFT_BLACK       0x0000      /*   0,   0,   0 */  // black
// #define TFT_NAVY        0x000F      /*   0,   0, 128 */  // navy
// #define TFT_DARKGREEN   0x03E0      /*   0, 128,   0 */  // darkgreen
// #define TFT_DARKCYAN    0x03EF      /*   0, 128, 128 */  // darkcyan
// #define TFT_MAROON      0x7800      /* 128,   0,   0 */  // maroon
// #define TFT_PURPLE      0x780F      /* 128,   0, 128 */  // purple
// #define TFT_OLIVE       0x7BE0      /* 128, 128,   0 */  // olive
// #define TFT_LIGHTGREY   0xD69A      /* 211, 211, 211 */  // lightgrey
// #define TFT_DARKGREY    0x7BEF      /* 128, 128, 128 */  // darkgray
// #define TFT_BLUE        0x001F      /*   0,   0, 255 */  // blue
// #define TFT_GREEN       0x07E0      /*   0, 255,   0 */  // green
// #define TFT_CYAN        0x07FF      /*   0, 255, 255 */  // cyan
// #define TFT_RED         0xF800      /* 255,   0,   0 */  // red
// #define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */  // magenta
// #define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */  // yellow
// #define TFT_WHITE       0xFFFF      /* 255, 255, 255 */  // white
// #define TFT_ORANGE      0xFDA0      /* 255, 180,   0 */  // orange
// #define TFT_GREENYELLOW 0xB7E0      /* 180, 255,   0 */  // greenyellow
// #define TFT_PINK        0xFE19      /* 255, 192, 203 */  // lightpink, was 0xFC9F      
// #define TFT_BROWN       0x9A60      /* 150,  75,   0 */  // pink
// #define TFT_GOLD        0xFEA0      /* 255, 215,   0 */  // brown
// #define TFT_SILVER      0xC618      /* 192, 192, 192 */  // silver
// #define TFT_SKYBLUE     0x867D      /* 135, 206, 235 */  // skyblue
// #define TFT_VIOLET      0x915C      /* 180,  46, 226 */  // violet

#define LTBLUE    0xB6DF
#define LTTEAL    0xBF5F
#define LTGREEN   0xBFF7
#define LTCYAN    0xC7FF
#define LTRED     0xFD34
#define LTMAGENTA 0xFD5F
#define LTYELLOW  0xFFF8
#define LTORANGE  0xFE73
#define LTPINK    0xFDDF
#define LTPURPLE  0xCCFF
#define LTGREY    0xE71C
#define LTGRAY    0xE71C

#define REDORANGE 	0xFAA6

#define BLUE      0x001F
#define TEAL      0x0438
#define GREEN     0x07E0
#define CYAN      0x07FF
#define RED       0xF800
#define MAGENTA   0xF81F
#define MAROON    0x7800
#define YELLOW    0xFFE0
#define ORANGE    0xFC00
#define PINK      0xF81F
#define PURPLE    0x8010
#define GREY      0xC618
#define GRAY      0xC618
#define WHITE     0xFFFF
#define BLACK     0x0000
#define SILVER    0xC618

#define GREENYELLOW 0xB7E0

#define DKBLUE    0x000D
#define DKTEAL    0x020C
#define DKGREEN   0x03E0
#define DKCYAN    0x03EF
#define DKRED     0x6000
#define DKMAGENTA 0x8008
#define DKYELLOW  0x8400
#define DKORANGE  0x8200
#define DKPINK    0x9009
#define DKPURPLE  0x4010
#define DKGREY    0x4A49
#define DKGRAY    0x4A49
#define LTBLACK   0x18C3
#define LTBLACK2  0x2945 // lighter

#define BGCOLOR   LTBLACK    // BGOVER, background overlay effect, for alert or opacity effects
#define BGOVER    LTBLACK    // BGOVER, background overlay effect, for alert or opacity effects
#define GRAPHBG   BLACK      // graph background (fill before graph, literally BG fillrect)
#define GRIDCOLOR LTBLACK2 // color of gridlines

#define HC1 LTBLACK // heading top BG
#define HC2 LTBLACK  // footer bottom BG

#define HC1_text GREY

#define HC3 GREEN    // alert OK
#define HC4 RED      // alert BAD
#define HC5 ORANGE   // alert WARN

#define DEBUG_HC1 RED
#define DEBUG_HC2 BLUE
#define DEBUG_HC3 DKGREEN

using RM_tft = TFT_eSPI;
RM_tft tft = RM_tft();

TFT_eSprite img = TFT_eSprite(&tft); 

// #define TFT_CS   -1  // Chip select control pin D8
// #define TFT_DC   0  // Data Command control pin
// #define TFT_RST  -1  // Reset pin (could connect to NodeMCU RST, see next line)

char* string2char(String command)
{
    if(command.length()!=0)
    {
        char *p = const_cast<char*>(command.c_str());
        return p;
    }
    return 0;
}

void charBounds(char c, int16_t *x, int16_t *y,
	int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy) {

	if (!true) {	//If non-default font is used not usable in my quick "hack"

		//if (c == '\n') { // Newline?
		//	*x = 0;    // Reset x to zero, advance y by one line
		//	*y += textsize * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
		//}
		//else if (c != '\r') { // Not a carriage return; is normal char
		//	uint8_t first = pgm_read_byte(&gfxFont->first),
		//		last = pgm_read_byte(&gfxFont->last);
		//	if ((c >= first) && (c <= last)) { // Char present in this font?
		//		GFXglyph *glyph = &(((GFXglyph *)pgm_read_pointer(
		//			&gfxFont->glyph))[c - first]);
		//		uint8_t gw = pgm_read_byte(&glyph->width),
		//			gh = pgm_read_byte(&glyph->height),
		//			xa = pgm_read_byte(&glyph->xAdvance);
		//		int8_t  xo = pgm_read_byte(&glyph->xOffset),
		//			yo = pgm_read_byte(&glyph->yOffset);
		//		if (wrap && ((*x + (((int16_t)xo + gw)*textsize)) > _width)) {
		//			*x = 0; // Reset x to zero, advance y by one line
		//			*y += textsize * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
		//		}
		//		int16_t ts = (int16_t)textsize,
		//			x1 = *x + xo * ts,
		//			y1 = *y + yo * ts,
		//			x2 = x1 + gw * ts - 1,
		//			y2 = y1 + gh * ts - 1;
		//		if (x1 < *minx) *minx = x1;
		//		if (y1 < *miny) *miny = y1;
		//		if (x2 > *maxx) *maxx = x2;
		//		if (y2 > *maxy) *maxy = y2;
		//		*x += xa * ts;
		//	}
		//}

	}
	else { // Default font

		if (c == '\n') {                     // Newline?
			*x = 0;                        // Reset x to zero,
			*y += tft.textsize * 8;             // advance y one line
			// min/max x/y unchaged -- that waits for next 'normal' character
		}
		else if (c != '\r') {  // Normal char; ignore carriage returns
			if (/*wrap*/ false && ((*x + tft.textsize * 6) > tft.width())) { // Off right?
				*x = 0;                    // Reset x to zero,
				*y += tft.textsize * 8;         // advance y one line
			}
			int x2 = *x + tft.textsize * 6 - 1, // Lower-right pixel of char
				y2 = *y + tft.textsize * 8 - 1;
			if (x2 > *maxx) *maxx = x2;      // Track max x, y
			if (y2 > *maxy) *maxy = y2;
			if (*x < *minx) *minx = *x;      // Track min x, y
			if (*y < *miny) *miny = *y;
			*x += tft.textsize * 6;             // Advance x one char
		}
	}
}

void getTextBounds(const char *str, int16_t x, int16_t y,
	int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h) {
	uint8_t c; // Current character

	*x1 = x;
	*y1 = y;
	*w = *h = 0;

	int16_t minx = tft.width(), miny = tft.width(), maxx = -1, maxy = -1;

	while ((c = *str++)){
		charBounds(c, &x, &y, &minx, &miny, &maxx, &maxy);
		delay(0);
	}

	if (maxx >= minx) {
		*x1 = minx;
		*w = maxx - minx + 1;
	}
	if (maxy >= miny) {
		*y1 = miny;
		*h = maxy - miny + 1;
	}
}

void drawOverlay(unsigned int c = BLACK){
  // Draw border left bottom
  tft.drawFastVLine(0,0,tft.height(),c);
  tft.drawFastHLine(0,tft.height()-1,tft.width()-1,c);

  tft.drawFastVLine(tft.width()-1,0,tft.height()-1,c);
  tft.drawFastHLine(0,0,tft.width()-1,c);  
}

void setAlertMode(int mode = 0){
	unsigned int color = BGCOLOR;
	unsigned int colorB = BGCOLOR;
	if(mode == 0){
		color = LTBLACK;
		colorB = LTBLACK2;
	}
	if(mode == 1){
		color = MAROON;
		colorB = RED;
	}
	if(mode == 2){
		color = DKGREEN;
		colorB = GREEN;
	}
	// println_header("HEADER",color);
	// tft.drawFastHLine(0, 24, tft.width(), colorB);  
	// println_footer("FOOTER",color);
	tft.drawFastHLine(0, tft.height()-24, tft.width(), colorB);
	// drawOverlay(colorB);
}

void tft_clear(){
  tft.fillScreen(GRAPHBG);
  // println_header("",HC1);
  tft.drawFastHLine(0, 30, tft.width(), BGCOLOR);  
  // println_footer("",HC2);
  tft.drawFastHLine(0, tft.height()-30, tft.width(), BGCOLOR);  
}

// void tft_init(){
//   Serial.println("[TFT] Starting tft"); 
//   tft.init();
//   tft.setRotation(3);
// }

void tftDispCenter(){

}

void println_Center( RM_tft &d, String heading, int centerX, int centerY )
{
    int x = 0;
    int y = 0;
    int16_t  x1, y1;
    uint16_t ww, hh;

    getTextBounds( string2char(heading), x, y, &x1, &y1, &ww, &hh );
    // Serial.println("println_center");
    // Serial.println(heading);
    // Serial.println(centerX);
    // Serial.println(centerY);
    // Serial.println(x1);
    // Serial.println(y1);
    // Serial.println(ww);
    // Serial.println(hh);

    // Serial.println( centerX - ww/2 + 2);
    // Serial.println( centerY - hh / 2);

    d.setCursor( centerX - ww/2 + 2, centerY - hh / 2);
    d.println( heading );
    // d.setCursor( 160 - 216/2 + 2, 160 - 16 / 2);
    // Serial.println(160 - 216/2 + 2);
    // Serial.println(160 - 16 / 2);
    
    // d.setCursor( 54,152 );
    // d.println( "Settings Stomped!!" );
    
    // d.setCursor( 45,216 );
    // d.println( "1234567890" );
    // d.println( "Reflow Master - PCB v2018-2, Code v1.03" );
}

void setFault(RM_tft &d){
  d.setCursor(45,216);
}

// void println_Right( RM_tft &d, String heading, int centerX, int centerY )
// {
//     int x = 0;
//     int y = 0;
//     int16_t  x1, y1;
//     uint16_t ww, hh;

//     // Serial.println(centerY);
//     // Serial.println(menuSelY);
//     if(menuSel > 0 && menuSelY == centerY) heading = (String)char(0x10)+(String)" "+(String)heading;
//     else heading = "  " + heading; // hack space
//     // Serial.println(heading);
//     getTextBounds( string2char(heading), x, y, &x1, &y1, &ww, &hh );
//     d.setCursor( centerX + ( 18 - ww ), centerY - hh / 2);
//     d.println( heading );
// }

void tft_test(){
	tft.setCursor(0, 0);
	tft.setTextColor(TFT_WHITE);
	tft.setTextSize(2);

 	tft.println(F("   TFT_eSPI test"));
    println_Center( tft, "  "+String( ( "Starting..." ) ),tft.width() / 2 - 20, ( tft.height() / 2 ) ); 	
}

void drawDatumMarker(int x, int y)
{
  tft.drawLine(x - 5, y, x + 5, y, TFT_GREEN);
  tft.drawLine(x, y - 5, x, y + 5, TFT_GREEN);
}

void build_scroll(String msg, int xpos){

  int IWIDTH = 240;
  int IHEIGHT = 30;

  int h = IHEIGHT;
  // We could just use fillSprite(color) but lets be a bit more creative...

  // Fill with rainbow stripes
  // while (h--) img.drawFastHLine(0, h, IWIDTH, rainbow(h * 4));

  // Draw some graphics, the text will apear to scroll over these
  // img.fillRect  (IWIDTH / 2 - 20, IHEIGHT / 2 - 10, 40, 20, TFT_YELLOW);
  // img.fillCircle(IWIDTH / 2, IHEIGHT / 2, 10, TFT_ORANGE);

  // Now print text on top of the graphics
  img.setTextSize(1);           // Font size scaling is x1
  img.setTextFont(4);           // Font 4 selected
  img.setTextColor(TFT_BLACK);  // Black text, no background colour
  img.setTextWrap(false);       // Turn of wrap so we can print past end of sprite

  // Need to print twice so text appears to wrap around at left and right edges
  img.setCursor(xpos, 2);  // Print text at xpos
  img.print(msg);

  img.setCursor(xpos - IWIDTH, 2); // Print text at xpos - sprite width
  img.print(msg);
}

void do_scroll(){
  int IWIDTH = 240;
  int IHEIGHT = 30;
  img.setColorDepth(8);
  // Create the sprite and clear background to black
  img.createSprite(IWIDTH, IHEIGHT);

  for (int pos = IWIDTH; pos > 0; pos--){
    build_scroll("Hello World", pos);
    img.pushSprite(0, 0); 
    
    build_scroll("TFT_eSPI sprite" , pos);
    img.pushSprite(0, 50);

    // Delete sprite to free up the memory
    img.deleteSprite();
  }
}

#endif // END tft_espi_shared_h
