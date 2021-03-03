#ifndef buzzer_h
#define buzzer_h

#ifdef ESP8266

#include <ESP8266_Tones.h>
#include <Tone_Pitches.h>
ESP8266_Tones BUZZER_TONE(BUZ_PIN);
char szBuf[128]; // serial buffer seems to be only 128 bytes on ESP, only 64 on ATmega

#include <MmlMusicPWM.h> // requires #include <MmlBUZZER_MUSIC.h>
// define the pin used and initialize a MusicEngine object
MmlMusicPWM BUZZER_MUSIC(BUZ_PIN);
int BUZZ_VOLUME = 100;

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

bool init_buzzer(){
  pinMode(BUZ_PIN,OUTPUT);
}

bool chime(){
  analogWrite(BUZ_PIN,500);
  delay(100);
  analogWrite(BUZ_PIN,500);
  delay(100);
  analogWrite(BUZ_PIN,0);
}

bool soundalarm(){
  analogWrite(BUZ_PIN,500);
  delay(100);
  analogWrite(BUZ_PIN,500);
  delay(100);
  analogWrite(BUZ_PIN,0);
}

#endif

#endif
