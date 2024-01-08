#ifndef neoindicator_h
#define neoindicator_h

#include <neopixel_helper.h>

#ifdef DEBUG
bool DEBUG_neoind = false;
#else
bool DEBUG_neoind = false;
#endif

#ifdef ESP32
bool showTwice = true;
bool noInterrupts = false; // causes crashes, sometimes ?
#else
bool showTwice = false;
bool noInterrupts = false;
#endif

// // these are const, so they don't eat RAM
// const uint32_t np_white     = ind.Color(255,255,255);
// const uint32_t np_black     = ind.Color(0  ,0,0);
// const uint32_t np_red       = ind.Color(255,0,0);
// const uint32_t np_orange    = ind.Color(255,128,0);
// const uint32_t np_yellow    = ind.Color(255,100,0); // 255,100,0
// const uint32_t np_green     = ind.Color(0  ,255,0);
// const uint32_t np_cyan      = ind.Color(0  ,255,128);
// const uint32_t np_blue      = ind.Color(0  ,0,255);
// const uint32_t np_purple    = ind.Color(128,0,255);
// const uint32_t np_turquoise = ind.Color(0,80,80);

#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel ind = Adafruit_NeoPixel();

// Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);
// Adafruit_NeoPixel strip *= ind;

	// pink	#FFC0CB	rgb(255,192,203)
 // 	lightpink	#FFB6C1	rgb(255,182,193)
 // 	hotpink	#FF69B4	rgb(255,105,180)
 // 	deeppink	#FF1493	rgb(255,20,147)
 // 	palevioletred	#DB7093	rgb(219,112,147)
 // 	mediumvioletred	#C71585	rgb(199,21,133)

uint16_t INDBRIGHTNESS = 60;
int INDNUMPIXELS = 4;
#define INDPIXELSTYPE NEO_GRB + NEO_KHZ800

bool INDPINRESET = false;
uint8_t neoIndTaskID; // timer task

uint32_t indColor; // save color

void init_indicator(uint16_t pin){
// Adafruit_NeoPixel 
  // strip = ind;
  Serial.println("[NEOIND] init pin# " + (String)pin);
  ind.setPin(pin);
  ind.setBrightness(100);
  ind.updateLength(INDNUMPIXELS);
  ind.updateType(NEO_GRB + NEO_KHZ800);
  ind.begin();
  ind.show();
  ind.show(); // on purpose, ensure its blanked for glitched resets
	delay(1);
  // init_strip();
}

/**
 * [indSetNextColor description]
 * @param c [description]
 */
void indSetNextColor(uint32_t c){
  indColor = c;
}

void debugColor(uint32_t c){
  Serial.println("Debug color");
  Serial.println("[RGB] Red: " + (String)red(c));
  Serial.println("[RGB] Green: " + (String)green(c));
  Serial.println("[RGB] Blue: " + (String)blue(c));
}

void indSetColorB(uint32_t c){
  if(DEBUG_neoind)Serial.println("[IND] set ind color:" + (String)c);
  // debugColor(c);
  ind.setPixelColor( 1, c);
  if(INDPINRESET) digitalWrite(ind.getPin(),HIGH); // reset
  #ifdef ESP32
  if(noInterrupts) portDISABLE_INTERRUPTS();
  #endif
  ind.show();
  if(showTwice) ind.show();
  #ifdef ESP32
  if(noInterrupts) portENABLE_INTERRUPTS();
  #endif
  // #endif
}

void indSetColor(uint32_t c){
  if(DEBUG_neoind)Serial.println("[IND] set ind color:" + (String)c);
  // debugColor(c);
  uint32_t color = ColorRGBA(red(c),green(c),blue(c),INDBRIGHTNESS);
  // ind.setPixelColor( 0, color );
  ind.fill(color);
  if(INDPINRESET) digitalWrite(ind.getPin(),HIGH); // reset
  #ifdef ESP32
  if(noInterrupts) portDISABLE_INTERRUPTS();
  #endif
  ind.show();
  if(showTwice) ind.show();
  #ifdef ESP32
  if(noInterrupts) portENABLE_INTERRUPTS();
  #endif
  // #endif
  indSetNextColor(ind.getPixelColor(0));
}

// alias

void setIndBrightness(uint16_t alpha = INDBRIGHTNESS){
  INDBRIGHTNESS = alpha;
  // ind.setBrightness(alpha);
}

void setIndColor(uint32_t c){
  indSetColor(c);
}

void indSetColor(uint8_t r,uint8_t g,uint8_t b){
  if(DEBUG_neoind)Serial.println("[IND] set ind color RGB:"+(String) ind.Color(r,g,b));    
  // debugColor(ind.Color(r,g,b));
  indSetColor(ind.Color(r,g,b));
}

// void setPixelBrightness(uint32_t n, uint32_t a){
// }

// void indSetBrightness(uint8_t a){
//   uint32_t c = strip.getPixelColor(0);
//   // if(DEBUG_neoind)Serial.println("[IND] set ind color brightness:"+(String) ind.Color(r,g,b));    
//   setPixelColorAlpha(n,c,a);
//   ind.setPixelBrightness
// }


void setIndColor(uint8_t r,uint8_t g,uint8_t b){
  indSetColor(r,g,b);
}

void stop_indicator(){
  if(DEBUG_neoind)Serial.println("[IND] stop");    
  for(size_t i=0; i<INDNUMPIXELS; i++) {
    ind.setPixelColor(i,0,0,0);
  }  
	ind.show();
  // @todo unset object
}

// non blocking stepped color setter
void updateIndColor(){
  if(indColor != ind.getPixelColor(0)){
    ind.setPixelColor(0,indColor);
    ind.show();
   }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbowInd(int wait) {
  // Hue of first pixel runs 3 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 3*65536. Adding 256 to firstPixelHue each time
  // means we'll make 3*65536/256 = 768 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 3*65536; firstPixelHue += 256) {
    // for(int i=0; i<ind.numPixels(); i++) { // For each pixel in ind...
    //   // Offset pixel hue by an amount to make one full revolution of the
    //   // color wheel (range of 65536) along the length of the strip
    //   // (ind.numPixels() steps):
    //   int pixelHue = firstPixelHue + (i * 65536L / ind.numPixels());
    //   // ind.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
    //   // optionally add saturation and value (brightness) (each 0 to 255).
    //   // Here we're using just the single-argument hue variant. The result
    //   // is passed through ind.gamma32() to provide 'truer' colors
    //   // before assigning to each pixel:
    //   ind.setPixelColor(i, ind.gamma32(ind.ColorHSV(pixelHue)));
    // }
    ind.fill(ind.gamma32(ind.ColorHSV(firstPixelHue)));

    ind.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t indWheel(byte WheelPos) {
  // Serial.print("indWheel: ");
  // Serial.print(WheelPos,BIN);
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return ind.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return ind.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return ind.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void indBlink(uint32_t c,int times, int wait){
    for(size_t i=0;i<times;i++){
        indSetColor(c);
        delay(wait);
        indSetColor(ind.Color(0,0,0));
        delay(wait);
    }
}

// void rainbowInd(uint8_t wait) {
//   uint16_t i, j;

//   for(j=0; j<256; j++) {
//     for(i=0; i<ind.numPixels(); i++) {
//       ind.setPixelColor(i, indWheel((j) & 255));
//     }
//     ind.show();
//     delay(wait);
//   }
// }

void indClear(){
   if(DEBUG_neoind)Serial.println("[IND] clear ind");   
    for(size_t i=0;i<10;i++){
        indSetColor(0,0,0);
        delay(100);
    }  
}

void indTest(bool quicktest = true){
    // bool quicktest = true;
    int wait = quicktest ? 1 : 100;
    ind.setBrightness(255); // full bright

    indSetColor(255,0,0);
    delay(wait*100);
    indSetColor(0,255,0);
    delay(wait*100);
    indSetColor(0,0,255);
    delay(wait*100);
    indSetColor(0,0,0);
    delay(200);

    for(size_t i=0;i<10;i++){
        indSetColor(indWheel(random(255)));
        delay(wait);
    }
    // indBlink(Wheel(random(255)),10,50);
    rainbowInd(wait);

    indSetColor(0,0,0); // set black
    // ind.setBrightness(INDBRIGHTNESS); // set normal brightness
    Serial.println("[IND] indtest complete");
}

void indTest2(){
    ind.setBrightness(255); // full bright

    indSetColor(255,0,0);
    delay(500);
}

void accentSetColor(uint32_t c){
  if(DEBUG_neoind)Serial.println("[IND] set accent color");   
    for(size_t i=1; i<ind.numPixels(); i++) {
      ind.setPixelColor(i, c);
    }
    ind.show();
}


unsigned long IND_lastUpdate = 0 ; // for millis() when last update occoured
uint32_t IND_ANIMDELAY = 30;

void IND_nb_rainbow() { // modified from Adafruit example to make it a state machine
  static uint16_t ind_j=0;
    for(int i=0; i<ind.numPixels(); i++) {
      ind.setPixelColor(i, Wheel((i+ind_j) & 255));
      delay(0);
    }
    ind.show();
     ind_j++;
  if(ind_j >= 256) ind_j=0;
  IND_lastUpdate = millis(); // time for next change to the display
}

void IND_nb_animate(){
  if(millis() - IND_lastUpdate > IND_ANIMDELAY) IND_nb_rainbow();
}


void processNeoInd(){
  updateIndColor();
}

#endif