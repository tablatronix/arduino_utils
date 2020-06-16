#ifndef ind_accent_h
#define ind_accent_h

#include <Adafruit_NeoPixel.h>
#include <neoindicator.h>
#include <neopixel_helper.h>

bool DEBUG_neoaccentind = false;

uint16_t INDACCENTBRIGHTNESS = 180;
int INDACCENTNUMPIXELS  = 2;

uint32_t accentColor = np_turquoise; // turquoise

void init_ind_accent(){
  ind.updateLength(INDNUMPIXELS+INDACCENTNUMPIXELS);
}

void indAccentSetColor(uint32_t c){
  if(DEBUG_neoaccentind)Serial.println("[ACCENT] set ind color:" + (String)c);  
  for(size_t i=(INDNUMPIXELS); i<INDACCENTNUMPIXELS+INDNUMPIXELS; i++) {
    ind.setPixelColor(i,c);
    accentColor = c;
  }
  ind.show();
  if(INDPINRESET) digitalWrite(ind.getPin(),HIGH); // reset 
}

void indAccentSetColor(uint8_t r,uint8_t g,uint8_t b){
  indAccentSetColor(ind.Color(r,g,b));
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbowAccent(int wait) {
  for(long firstPixelHue = 0; firstPixelHue < 3*65536; firstPixelHue += 256) {
    indAccentSetColor(ind.gamma32(ind.ColorHSV(firstPixelHue)));
    ind.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

void indAccentTest(){
    bool quicktest = true;
    int wait = quicktest ? 5 : 50;
    ind.setBrightness(255); // full bright

    for(size_t i=0;i<20;i++){
        indAccentSetColor(indWheel(random(255)));
        delay(wait);
    }
    // indBlink(Wheel(random(255)),10,50);
    rainbowAccent(wait);

    indAccentSetColor(0,0,0); // set black
    ind.setBrightness(INDBRIGHTNESS); // set normal brightness
    if(DEBUG_neoaccentind)Serial.println("[ACCENT] ind accent test complete");
}

#endif