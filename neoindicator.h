#ifndef neoindicator_h
#define neoindicator_h

#ifdef DEBUG
bool DEBUG_neoind = true;
#else
bool DEBUG_neoind = false;
#endif

#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel ind = Adafruit_NeoPixel();
// Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);

	// pink	#FFC0CB	rgb(255,192,203)
 // 	lightpink	#FFB6C1	rgb(255,182,193)
 // 	hotpink	#FF69B4	rgb(255,105,180)
 // 	deeppink	#FF1493	rgb(255,20,147)
 // 	palevioletred	#DB7093	rgb(219,112,147)
 // 	mediumvioletred	#C71585	rgb(199,21,133)

#define INDBRIGHTNESS 180

bool INDPINRESET = false;

void init_indicator(uint16_t pin){
  ind.setPin(pin);
  ind.setBrightness(INDBRIGHTNESS);
  ind.updateLength(3);
  ind.updateType(NEO_GRB + NEO_KHZ800);
  ind.begin();
  ind.show();
  ind.show();
	delay(1);
	if(INDPINRESET) digitalWrite(ind.getPin(),HIGH); // reset
}

void indSetColor(uint32_t c){
  if(DEBUG_neoind)Serial.println("[IND] set ind color:" + (String)c);  
  ind.setPixelColor( 0, c );
  ind.show();
  if(INDPINRESET) digitalWrite(ind.getPin(),HIGH); // reset 
}

// alias
void setIndColor(uint32_t c){
  indSetColor(c);
}

void indSetColor(uint8_t r,uint8_t g,uint8_t b){
  if(DEBUG_neoind)Serial.println("[IND] set ind color:");    
  ind.setPixelColor(0,r,g,b);
	ind.show();
  delay(1);
  if(INDPINRESET) digitalWrite(ind.getPin(),HIGH); // reset
}

void setIndColor(uint8_t r,uint8_t g,uint8_t b){
  indSetColor(r,g,b);
}

void stop_indicator(){
  if(DEBUG_neoind)Serial.println("[IND] stop");    
  for(size_t i=0; i<ind.numPixels(); i++) {
    ind.setPixelColor(i,0,0,0);
  }  
	ind.show();
  // @todo unset object
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t indWheel(byte WheelPos) {
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

void rainbowInd(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<ind.numPixels(); i++) {
      ind.setPixelColor(i, indWheel((j) & 255));
    }
    ind.show();
    delay(wait);
  }
}

void indClear(){
   if(DEBUG_neoind)Serial.println("[IND] clear ind");   
    for(size_t i=0;i<100;i++){
        indSetColor(0,0,0);
        delay(100);
    }  
}

void indTest(){
    ind.setBrightness(255); // full bright

    for(size_t i=0;i<20;i++){
        indSetColor(indWheel(random(255)));
        delay(5);
    }
    // indBlink(Wheel(random(255)),10,50);
    rainbowInd(5);

    indSetColor(0,0,0); // set black
    ind.setBrightness(INDBRIGHTNESS); // set normal brightness
}

void accentSetColor(uint32_t c){
  if(DEBUG_neoind)Serial.println("[IND] set accent color");   
    for(size_t i=1; i<ind.numPixels(); i++) {
      ind.setPixelColor(i, c);
    }
    ind.show();
}

#endif