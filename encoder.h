/**
 * Encoder Library
 * // https://github.com/soligen2010/encoder
 */
#include <ClickEncoder.h>

#define ENCODER_PINA     2  // not working
#define ENCODER_PINB     16 // working
#define ENCODER_BTN      -1 // using analog, could maybe use rx/3?

int encoder_value;
int encoder_numsteps =2; // Type 1 
// int encoder_numsteps = 4; // Type 2
#define ENCODER_STEPS_PER_NOTCH    1   // Change this depending on which encoder is used
ClickEncoder encoder = ClickEncoder(ENCODER_PINA,ENCODER_PINB,ENCODER_BTN,ENCODER_STEPS_PER_NOTCH);

void test_encoder(){
	bool pinclk  = digitalRead(ENCODER_PINA) == LOW;
	bool pindata = digitalRead(ENCODER_PINB) == LOW;

	if(pinclk && !pindata){
		Serial.println("pinclk");
	}

	if(!pinclk && pindata){
		Serial.println("pindata");
	}

	if(pinclk && pindata){
		Serial.println("both");
	}	
}

bool process_encoder(){
	// test_encoder();return;
  static uint32_t lastService = 0;
  static int16_t encoder_prev;
  // encoder_value; // running total
 
  // // throttle
  // if (micros() - lastService <= 200) {
  // 	return;
  // }

  encoder.service();

  encoder_value += encoder.getValue();
  // if(encoder_value!=0) Serial.print(encoder_value);
  // workaround for 
  if (abs(encoder_value - encoder_prev) >= encoder_numsteps) {
  // if (encoder_value != encoder_prev) {
    Serial.print("Encoder Value: ");
    Serial.print(encoder_value);
    Serial.print(" Dir: ");
    Serial.println((encoder_value > encoder_prev) ? "10" : "20"); // L:R ints for plotting
    Serial.print("\t");
    Serial.print(" Steps: ");
    Serial.print((encoder_value-encoder_prev)/encoder_numsteps);
    Serial.print("\t");
    encoder_prev = encoder_value;
  	return true;
  }
  return false;
}

uint16_t getEncoder(){
	return encoder_value;
}

void init_encoder(){

}