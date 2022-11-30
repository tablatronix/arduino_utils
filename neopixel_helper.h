/**
 * All helpers need to be refactored to use neopixel objected passed in
 * kludge reassign strip to your obj etc and use as a shadow
 */
#ifndef NEOPIXEL_HELPER_H
#define  NEOPIXEL_HELPER_H

#ifdef DEBUG
bool DEBUG_neohelp = true;
#else
bool DEBUG_neohelp = false;
#endif

uint16_t NEOBRIGHTNESS = 100;
int NEONUMPIXELS = 5;
#define NEOPIXELSTYPE NEO_GRB + NEO_KHZ800

#include <Adafruit_NeoPixel.h>
// Adafruit_NeoPixel strip = Adafruit_NeoPixel();
// Adafruit_NeoPixel strip = Adafruit_NeoPixel(4, -1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip = Adafruit_NeoPixel();

// these are const, so they don't eat RAM
const uint32_t np_white     = strip.Color(255, 255,  255);
const uint32_t np_black     = strip.Color(  0,   0,   0);
const uint32_t np_red       = strip.Color(255,   0,   0);
const uint32_t np_orange    = strip.Color(255, 128,   0);
const uint32_t np_yellow    = strip.Color(255, 100,   0); // 255,100,0
const uint32_t np_green     = strip.Color(  0, 255,   0);
const uint32_t np_cyan      = strip.Color(  0, 255, 128);
const uint32_t np_blue      = strip.Color(  0,   0, 255);
const uint32_t np_purple    = strip.Color(128,   0, 255);
const uint32_t np_pink      = strip.Color(128,   0, 255);
const uint32_t np_turquoise = strip.Color(  0,    80,  80);

  // pink #FFC0CB rgb(255,192,203)
 //   lightpink #FFB6C1 rgb(255,182,193)
 //   hotpink #FF69B4 rgb(255,105,180)
 //   deeppink  #FF1493 rgb(255,20,147)
 //   palevioletred #DB7093 rgb(219,112,147)
 //   mediumvioletred #C71585 rgb(199,21,133)
 //   
/**
 * collection of helper functions
 */

void init_strip(int pin, Adafruit_NeoPixel& ind){
  strip = ind; return;
}

void init_strip(int pin){
  strip.setPin(pin);
  strip.setBrightness(NEOBRIGHTNESS);
  strip.updateLength(NEONUMPIXELS);
  strip.updateType(NEO_GRB + NEO_KHZ800);
  strip.begin();
  strip.show();
  strip.show(); // on purpose, ensure its blanked for glitched resets
  delay(1);
}

int fadeDuration = 150; // ms fade animation duration

const float R = (255 * log10(2))/(log10(255)); // gamma scaler

uint8_t red(uint32_t c) {
  return (c >> 16);
}
uint8_t green(uint32_t c) {
  return (c >> 8);
}
uint8_t blue(uint32_t c) {
  return (c);
}
uint8_t white(uint32_t c) {
  return (c >> 24);
}

// local alias
void show(){
	strip.show();
}

uint32_t color(uint8_t R, uint8_t G, uint8_t B){
	return strip.Color(R,G,B);
}

uint32_t color(uint8_t R, uint8_t G, uint8_t B, uint8_t W){
	return strip.Color(R,G,B,W);
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// @todo imported, rewrite all

// apha is brightness
// maybe replace with colorHSV 
uint8_t getRGBA(uint8_t rgb, uint16_t A){
  if(A > 255 || rgb == 0) return rgb;
  if(A < 255) return floor(A*rgb/255);
  return rgb;
}

// RGB to binary with MAXBRIGHTNESS brightness scaling
uint32_t ColorRGBA(uint8_t R, uint8_t G, uint8_t B,uint16_t A){
  if(A < 255){
    if(R>0) R = getRGBA(R,A);
    if(G>0) G = getRGBA(G,A);
    if(B>0) B = getRGBA(B,A);
  }
  // Serial.println("RGB:" + (String)R + ',' + (String)G + ',' + (String)B);
  return color(R,G,B);
}

uint32_t ColorRGBA(uint32_t C, uint16_t A){
	return ColorRGBA(red(C),green(C),blue(C),A);
}

void setPixelColor(uint32_t n, uint32_t c){
	strip.setPixelColor(n,c);
}

void setPixelColorAlpha(uint32_t n,uint32_t c,uint16_t a){
	uint32_t ca = ColorRGBA(c,a);
	setPixelColor(n,ca);
}

void setPixelBrightness(uint32_t n, uint32_t a){
	uint32_t c = strip.getPixelColor(n);
	setPixelColorAlpha(n,c,a);
}

uint32_t getPixelColor(uint32_t n){
	return strip.getPixelColor(n);
}

void setAllPixels(uint32_t c){
	strip.fill(c);
	strip.show();
}

/**
 * get random brightness, range centered on value
 * @param  uint16_t a        center brightness
 * @param  uint8_t  range    +/- range
 * @return uint16_t	         brightness
 */
uint16_t randomBrightness(uint16_t a,uint8_t range){
	// range = range/2;
	uint16_t b = a + (range-(random(range)*2));
	// Serial.println(b);
	if(b>255) return 255;
	return b;
}

// does not allow offsets 0-255
uint16_t getBrightnessStep(uint8_t step, uint16_t numsteps){
  uint16_t brightness;
//  brightness = 255 - (step * (255/numsteps)); // linear
  brightness = (pow (2, (((numsteps+2)-step) / R)) - 1);
// uint16_t brightness = pow (2, ((step+stepshift) / R)) - 1;  
  return brightness;
}


// brightness gamme corrections curves
// simple gamma curve
uint16_t getBrightnessGamma(uint16_t value,float gamma){
//  float gamma   = 2.0; // Correction factor
  int   max_in  = 255; // Top end of INPUT range
  int   max_out = 255; // Top end of OUTPUT range  
  // return round((pow (2, (value / R)) - 1));
  return (int)(pow((float)value / (float)max_in, gamma) * max_out + 0.5);
}

// brightness gradient
void gradient(int gamma){
  for (int i=0; i<strip.numPixels(); i++) {
    int step = (255/strip.numPixels() * (i+1));
    uint16_t bx = step;
    if(gamma > 0) bx = getBrightnessGamma(step,2.5);
    Serial.print((String)bx + " ");
    setPixelColor(i,ColorRGBA(0,255,0,bx));
  }
  show();
  Serial.print("\n");
  delay(1000);
}

// cie 1931 brightness correction
// if (L > 7.9996248) 
// {
//   Y = Math.Pow((L+16),3)/116;
// }
// else
// {
//   Y = L/(903.3);
// }
uint16_t cie(uint16_t n){
  uint16_t pwmValue;
  uint16_t brightness=n/2.56;
  if (brightness > 8){
      pwmValue= pow( ((brightness + 16) / 116) , 3  );
  } else {
      pwmValue = brightness / 903.3;      
  }
  pwmValue*=256;
  return pwmValue;
}

// simple power curve
uint16_t power(uint16_t n){
  // use simple power
  uint16_t pwmValue=pow(255,(n/256.0))-1;        
  pwmValue=int(pwmValue*256);
  return pwmValue;
}

uint32_t getSinStep(uint32_t numSteps,uint32_t step){
	// real sinewave
	  // for (int j = 0; j < 360; j++) {
	  //  Serial.println(sin(j * (PI / 180)));
	  // }
  return step < numSteps ? step : numSteps+(numSteps-step);
}

void breathe(uint32_t wait){
    uint32_t numSteps = 256;
    // 255 level sinewave
    for (int j=1; j<(numSteps*2); j++) {
      	
      	uint32_t value = getSinStep(numSteps,j);
       	
       	// gamma curves
       	int b  = getBrightnessGamma(value,3.0);
       	int bx = getBrightnessGamma(value,1.5);
	   	
	   	int bright = (int)bx;
	   	// Serial.println("gamma step " + (String)(int)value + " " + (String)bright);
		
		for (int i=0; i<strip.numPixels(); i++) {
			setPixelColor(i,ColorRGBA(255,0,255,(uint16_t)bright));
			// setPixelColor(i,color(255,0,255));
			// strip.setPixelColor(i,strip.Color(255,0,255));
			
			// if(i<2) bits[i] = RGBtoBin(255,0,0,(int)value);  // linear
			// else if(i>5) bits[i] = RGBtoBin(0,255,0,(int)b); // gamma 3
			// else bits[i] = RGBtoBin(0,0,255,(int)bx);        // gamma 1.5
		}
		
		show();
       	delay(wait);
    }
}

void fade(uint32_t startColor, uint32_t endColor,uint16_t duration) {
//    Serial.println("from:\t" + (String)red(startColor) + "\t" + (String)green(startColor) + "\t" + (String)blue(startColor));
//    Serial.println("to:\t" + (String)red(endColor) + "\t" + (String)green(endColor) + "\t" + (String)blue(endColor));    
	int16_t redDiff   = red(endColor)   - red(startColor);
	int16_t greenDiff = green(endColor) - green(startColor);
	int16_t blueDiff  = blue(endColor)  - blue(startColor);
	int16_t whiteDiff = white(endColor) - white(startColor);

	int16_t wait   = 20; // refresh rate 30fps - overhead = 20ms
	int16_t factor = 1; // adjust refresh rate/steps
	int16_t steps  = duration / (wait*factor);

    int16_t redValue, greenValue, blueValue, whiteValue;

    for (int16_t i = 1 ; i < steps ; ++i) {
		redValue   = (int16_t)red(startColor)   + (redDiff   * i / steps);
		greenValue = (int16_t)green(startColor) + (greenDiff * i / steps);
		blueValue  = (int16_t)blue(startColor)  + (blueDiff  * i / steps);
		whiteValue = (int16_t)white(startColor) + (whiteDiff * i / steps);

        for(uint16_t j=0; j< strip.numPixels(); j++) {
          setPixelColor(j,color(redValue, greenValue, blueValue,whiteValue));
        }
        show();
        delay(wait);
    }
      for(uint16_t j=0; j< strip.numPixels(); j++) {
        setPixelColor(j,endColor);
      }
      show();
      delay(wait);    
}

void fade(uint32_t startColor, uint32_t endColor) {
  	fade(startColor, endColor, fadeDuration);
}

void fadeTo(uint32_t c){
  fade(getPixelColor(0),c,fadeDuration);
}

void fadeTo(uint32_t c,uint16_t duration){
  fade(getPixelColor(0),c,duration);
}

// stepping non blocking fader
uint32_t getFadeStep(uint32_t startColor, uint32_t endColor, uint16_t steps, uint16_t step){
	int16_t redDiff   = red(endColor)   - red(startColor);
	int16_t greenDiff = green(endColor) - green(startColor);
	int16_t blueDiff  = blue(endColor)  - blue(startColor);
	int16_t whiteDiff = white(endColor) - white(startColor);

    int16_t redValue, greenValue, blueValue, whiteValue;
	redValue   = (int16_t)red(startColor)   + (redDiff   * step / steps);
	greenValue = (int16_t)green(startColor) + (greenDiff * step / steps);
	blueValue  = (int16_t)blue(startColor)  + (blueDiff  * step / steps);
	whiteValue = (int16_t)white(startColor) + (whiteDiff * step / steps);

	// return color(redValue, greenValue, blueValue,whiteValue);
  return color(redValue, greenValue, blueValue);
}

void flasher(uint32_t colorA, uint32_t colorB,int waitA, int waitB){
	setAllPixels(colorA);
	delay(waitA);
	setAllPixels(colorB);
	delay(waitB);
}

// void flasherSmooth(uint32_t colorA, uint32_t colorB){
// 	flasherSmooth(colorA, colorB,150,150);
// }

void flasherSmooth(uint32_t colorA, uint32_t colorB,int waitA, int waitB){
	fadeTo(fadeDuration,colorA);
	delay(waitA);
	fadeTo(fadeDuration,colorB);
	delay(waitB);
}

unsigned long NEO_lastUpdate = 0 ; // for millis() when last update occoured
uint32_t NEO_ANIMDELAY = 10;

void NEO_nb_rainbow() { // modified from Adafruit example to make it a state machine
  static uint16_t neo_j=128;
    for(int i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+neo_j) & 255));
      delay(0);
    }
    strip.show();
     neo_j++;
  if(neo_j >= 256) neo_j=0;
  NEO_lastUpdate = millis(); // time for next change to the display
}

void NEO_nb_animate(){
  if(millis() - NEO_lastUpdate > NEO_ANIMDELAY) NEO_nb_rainbow();
}

void rainbow(int wait) {
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
    strip.fill(strip.gamma32(strip.ColorHSV(firstPixelHue)));

    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbowHue(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

void clear(){
    setAllPixels(color(0,0,0));
    delay(200);  
}

void demo(){
  int wait = 5000;
  int blank = 200;

  Serial.println("demo BEGIN");
  // testStrip();
  Serial.println("rainbow");
  // rainbow(20); // cycle all pixels through rainbow
  Serial.println("rainbowCycleSmooth");
  // rainbowCycleSmooth(10); // smooth chase through rainbow  (can adjust single shot counts inside function)
  //if(NUMPIXELS > 1) for(int i=0;i<15;i++) 
  Serial.println("rainbowCycleFast");
  // rainbowCycleFast(60); // chase through rainbow (can adjust single shot counts inside function) for small strips

  uint8_t count = 20;

  for(int i=0;i<count;i++)breathe(2);
  clear();
  delay(blank);
  for(int i=0;i<count;i++) fadeTo(Wheel(random(255)),500);
  clear();
  delay(blank);
  for(int i=0;i<count;i++) fadeTo((i % 2) ? color(255,0,255) : color(0,255,255),500);
  clear();
  delay(blank);
  for(int i=0;i<count;i++) flasher(color(255,0,255),color(0,255,255),100,100);
  clear();
  delay(blank);
  for(int i=0;i<count;i++) flasherSmooth(color(255,0,255),color(0,255,255),100,100);
  Serial.println("demo DONE");

}

void stripTest(bool quicktest = true){
    // bool quicktest = true;
    int wait = quicktest ? 1 : 10;
    strip.setBrightness(255); // full bright

    setAllPixels(color(255,0,0));
    delay(wait*100);
    setAllPixels(color(0,255,0));
    delay(wait*100);
    setAllPixels(color(0,0,255));
    delay(wait*100);
    setAllPixels(color(0,0,0));
    delay(200);

    for(size_t i=0;i<10;i++){
        setAllPixels(Wheel(random(255)));
        delay(wait);
    }
    // indBlink(Wheel(random(255)),10,50);
    rainbow(wait+5);

    setAllPixels(color(0,0,0)); // set black
    strip.setBrightness(NEOBRIGHTNESS); // set normal brightness
    Serial.println("[STRIP] striptest complete");
}

#endif