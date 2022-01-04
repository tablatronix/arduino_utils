/**
 * Encoder Library
 * // https://github.com/soligen2010/encoder
 */
#include <ClickEncoder.h>
// : error: 'ClickEncoder::buttonHeldEnabled' will be initialized after [-Werror=reorder]

#define ENCODER_PINA     2  // not working
#define ENCODER_PINB     16 // working
#define ENCODER_BTN      0 // using analog, could maybe use rx/3?

int encoder_value;
// int encoder_numsteps =2; // Type 1 
int encoder_numsteps = 4; // Type 2
#define ENCODER_STEPS_PER_NOTCH    1   // Change this depending on which encoder is used
// ClickEncoder encoder = ClickEncoder(ENCODER_PINA,ENCODER_PINB,ENCODER_BTN,ENCODER_STEPS_PER_NOTCH);
ClickEncoder *encoder;

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

int encoder_prev;
int8_t encoder_change;

bool process_encoder(){
	// test_encoder();return;
  static uint32_t lastService = 0;
  // encoder_value; // running total
 
  // // throttle
  // if (micros() - lastService <= 200) {
  // 	return;
  // }

  encoder->service();

  encoder_value += encoder->getValue();
  // workaround for 
  
  if (abs(encoder_value - encoder_prev) >= encoder_numsteps) {
  // if (encoder_value != encoder_prev) {
    Serial.print("Encoder Value: ");
    Serial.print(encoder_value);
    Serial.print(" Dir: ");
    Serial.print((encoder_value > encoder_prev) ? "10" : "20"); // L:R ints for plotting
    Serial.print("\t");
    Serial.print(" Steps: ");
    Serial.print((encoder_value-encoder_prev)/encoder_numsteps);
    Serial.print("\n");
    encoder_change = (encoder_value-encoder_prev)/encoder_numsteps;
    encoder_prev = encoder_value;
  	return true;
  }
  return false;
}

uint8_t getButtonState(){
  ClickEncoder::Button b = encoder->getButton();
  if (b != ClickEncoder::Open) {
    Serial.print("Button: ");
    #define VERBOSECASE(label) case label: Serial.println(#label); break;
    switch (b) {
      VERBOSECASE(ClickEncoder::Pressed);
      VERBOSECASE(ClickEncoder::Held)
      VERBOSECASE(ClickEncoder::Released)
      VERBOSECASE(ClickEncoder::Clicked)
      VERBOSECASE(ClickEncoder::DoubleClicked)
      VERBOSECASE(ClickEncoder::Open)
      VERBOSECASE(ClickEncoder::Closed)
    }
  }
  return b;
}

int8_t getEncoder(){
	return encoder_change;
}

void init_encoder(int8_t A, int8_t B, int8_t BTN, uint8_t stepsPerNotch){
  // int8_t A, int8_t B, int8_t BTN, uint8_t stepsPerNotch, bool active
  encoder = new ClickEncoder(A, B, BTN, stepsPerNotch);
  if(BTN == 0)encoder->setButtonOnPinZeroEnabled(true);
  encoder->setDoubleClickEnabled(false); // add delay for dblclick checking
  encoder->setButtonHeldEnabled(true);
}