#ifndef buzzer_h
#define buzzer_h

int BUZZ_VOLUME = 100;
int buzzer_pin = -1;

#ifdef ESP8266

#include <ESP8266_Tones.h> // https://github.com/Mottramlabs/ESP8266-Tone-Generator
#include <Tone_Pitches.h>
ESP8266_Tones BUZZER_TONE(BUZ_PIN);
char szBuf[128]; // serial buffer seems to be only 128 bytes on ESP, only 64 on ATmega

#include <MmlMusicPWM.h> // requires #include <MmlBUZZER_MUSIC.h>
// define the pin used and initialize a MusicEngine object
MmlMusicPWM BUZZER_MUSIC(BUZ_PIN);

bool init_buzzer(){
	pinMode(BUZ_PIN,OUTPUT);
}

void soundSiren(int nTimes=10)
{
  for(int nLoop=0; nLoop<nTimes; nLoop++)
  {
    for(int nFreq=100; nFreq<2000; nFreq+=10)
    {
      BUZZER_MUSIC.tone(nFreq);
      delay(1);
    }
    for(int nFreq=2000; nFreq>100; nFreq-=10)
    {
      BUZZER_MUSIC.tone(nFreq);
      delay(1);
    }
  }
}

void soundNoise(int nLength=30)
{
  srand(analogRead(A0));
  for(int nLoop=0; nLoop<nLength; nLoop++)
  {
    for(int nTimes=1; nTimes<max(400-nLoop*5,0); nTimes++)
    {
      BUZZER_MUSIC.tone(random(100, 2000));
      delay(1);
    }
    BUZZER_MUSIC.noTone();
    delay(max(200-nLoop*5,0));
  }
}

void soundNoise2(int nLength=20)
{
  srand(analogRead(A0));
  for(int nLoop=0; nLoop<nLength; nLoop++)
  {
    for(int nTimes=1; nTimes<100; nTimes++)
    {
      BUZZER_MUSIC.tone(random(100, 2000));
      delay(1);
    }
    delay(100);
  }
  BUZZER_MUSIC.noTone();
}

void soundNoise3(int nLength=5)
{
  srand(analogRead(A0));
  for(int nLoop=0; nLoop<nLength; nLoop++)
  {
    for(int nTimes=1; nTimes<200; nTimes++)
    {
      BUZZER_MUSIC.tone(random(100, 2000));
      delay(10);
    }
    delay(20);
  }
  BUZZER_MUSIC.noTone();
}

void testsounds() {
  // put your setup code here, to run once:
  BUZZER_MUSIC.play("T240 L16 O8 r1 CDEDC"); // give a short blurp
  // soundSiren(5);
  BUZZER_MUSIC.play("T240 L32 O8 r1 GDECBADGECDEDCr2GDECBADGECDEDCr1GDECBADGECDEDC"); // play a different noise
  // soundNoise();
  // delay(500);
  // soundNoise2();
  // delay(500);
  // soundNoise3();
  BUZZER_MUSIC.play("T240 L2 O2 r1 CDCECBrCDCDCD O1 r CDCDCDrCDCECB O0 r CDCDCDrCDCECB"); // play a different noise
}

void blurp() {
  BUZZER_MUSIC.play("V10 T240 L16 O8 CDEDC"); // give a short blurp
}

void blurp2() {
  BUZZER_MUSIC.play("T240 L16 O8 r1 CDEDC"); // give a short blurp
}

void blurp3(){
    BUZZER_MUSIC.play_P(PSTR("T240 O4 L64 AB>C, T240 O4 L32 CBA V0 A \0\0")); // give a short beep
}

void blurp4(){
    BUZZER_MUSIC.play("T240 O8 E64"); // give a short beep
}

void blurp5(){
    BUZZER_MUSIC.play("V20 T240 O8 G64\0\0"); // give a short beep
    // BUZZER_MUSIC.play("T240 O8 G64\0\0"); // give a short beep
}

void blurp6(){
    BUZZER_MUSIC.play("V5 T250 L64 O7 B");
}

void playBlurpA(){
    blurp5();
    delay(50);      
    blurp5();
    delay(50);      
    blurp5();
    delay(50);
}

void chime(){
  blurp4();
}

void soundalarm(){
  for(int i=0; i<6; i++){
    blurp6();
    delay(50);
  }
}

#else

#include <Tone32.h>
#define BUZ_CHAN 0

void Chirp() {
  tone(BUZ_PIN, NOTE_A7, 10); // F6
  tone(BUZ_PIN, NOTE_C8, 20); // F5
  noTone(BUZ_PIN, BUZ_CHAN);  
} // End of beep

void Bleep() {
  tone(BUZ_PIN, NOTE_C7, 50, BUZ_CHAN); // F6
  tone(BUZ_PIN, NOTE_C8, 100, BUZ_CHAN); // F5
} // End of beep

void Tone_Down() {
  tone(BUZ_PIN, NOTE_F6, 200, BUZ_CHAN); // F6
  tone(BUZ_PIN, NOTE_F5, 200, BUZ_CHAN); // F5
  tone(BUZ_PIN, NOTE_C6, 200, BUZ_CHAN); // C6
} // End of beep

void Tone_Up() {
  tone(BUZ_PIN, NOTE_C6, 200, BUZ_CHAN); // C6
  tone(BUZ_PIN, NOTE_F5, 200, BUZ_CHAN); // F5
  tone(BUZ_PIN, NOTE_F6, 200, BUZ_CHAN); // F6
} // End of beep

void buzzer_test() {
  Chirp();
  delay(500);
  Bleep();
  delay(500);
  Tone_Down();
  delay(500);
  Tone_Up();
  delay(500);

  tone(BUZ_PIN, NOTE_C4, 500, BUZ_CHAN);
  noTone(BUZ_PIN, BUZ_CHAN);
  tone(BUZ_PIN, NOTE_D4, 500, BUZ_CHAN);
  noTone(BUZ_PIN, BUZ_CHAN);
  tone(BUZ_PIN, NOTE_E4, 500, BUZ_CHAN);
  noTone(BUZ_PIN, BUZ_CHAN);
  tone(BUZ_PIN, NOTE_F4, 500, BUZ_CHAN);
  noTone(BUZ_PIN, BUZ_CHAN);
  tone(BUZ_PIN, NOTE_G4, 500, BUZ_CHAN);
  noTone(BUZ_PIN, BUZ_CHAN);
  tone(BUZ_PIN, NOTE_A4, 500, BUZ_CHAN);
  noTone(BUZ_PIN, BUZ_CHAN);
  tone(BUZ_PIN, NOTE_B4, 500, BUZ_CHAN);
  noTone(BUZ_PIN, BUZ_CHAN);
}

#include <analogWrite.h>

bool init_buzzer(int pin){
  buzzer_pin = pin;
  pinMode(buzzer_pin,OUTPUT);
}

bool init_buzzer(){
  init_buzzer(BUZ_PIN);
}

bool chime(){
  analogWrite(buzzer_pin,500);
  delay(100);
  analogWrite(buzzer_pin,500);
  delay(100);
  analogWrite(buzzer_pin,0);
}

bool soundalarm(){
  analogWrite(buzzer_pin,500);
  delay(100);
  analogWrite(buzzer_pin,500);
  delay(100);
  analogWrite(buzzer_pin,0);
}

#endif

#endif
