#include <Adafruit_NeoPixel.h>
#define PIN 1
#define PINBTS 0
#define PINBTU 2
#define PINBTD 3
#define NUMPIXELS 11
#define BRIGHTNESS 15
#define GAMERS 8

struct Gamer {
    int level;
    uint32_t color;
};

Gamer gamer[GAMERS] = {};
int gamerId[GAMERS] = {0,1,2,3,4,5,6,7};
int cGamer = 0;
int count = 0;
int shift = 0;
int selMode=0;
int rnd=0;
int msDelay=10;
int userChDelay=1500;
bool checkGamers = false;
bool showRnd = false;
bool selBtnState,upBtnState,downBtnState;
bool lastSelBtnState = LOW,lastUpBtnState = LOW,lastDownBtnState = LOW;
unsigned long lastDebounceTimeSel = 0,lastDebounceTimeUp = 0,lastDebounceTimeDown = 0;
unsigned long debounceDelay = 50;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pinMode (PINBTS, INPUT_PULLUP);
  pinMode (PINBTU, INPUT_PULLUP);
  pinMode (PINBTD, INPUT_PULLUP);  
  gamer[0].color=strip.Color(255,0,0);
  gamer[1].color=strip.Color(0,255,0);
  gamer[2].color=strip.Color(0,0,255);
  gamer[3].color=strip.Color(255,255,0);
  gamer[4].color=strip.Color(80,0,255);
  gamer[5].color=strip.Color(255,255,255);
  gamer[6].color=strip.Color(255,0,80);
  gamer[7].color=strip.Color(0,255,255);
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show();
}

int checkGamersCount(){
  int countG=0;
  for(int i=0; i<sizeof(gamer)/sizeof(gamer[0]);i++ ){
    if(gamer[i].level > 0){
      gamerId[countG] = i;
      countG++;
    }
  }
  return GAMERS-countG;
}


void showGamer(){
  if(selMode==0){
    userChDelay=1500;
    if(gamer[gamerId[cGamer]].level>0){
      strip.clear();
      strip.setPixelColor(10,gamer[gamerId[cGamer]].color);
      strip.fill(gamer[gamerId[cGamer]].color,0,gamer[gamerId[cGamer]].level);  
      strip.setBrightness(BRIGHTNESS);
      strip.show();
    }
  }else if(selMode>0 && selMode <= GAMERS-shift){
    strip.clear();
    strip.setPixelColor(10,strip.Color(0,255,50));
    strip.fill(gamer[gamerId[selMode-1]].color,0,gamer[gamerId[selMode-1]].level);
    strip.setBrightness(BRIGHTNESS);
    strip.show();    
  }else if(selMode>GAMERS-shift){
    if(checkGamers == true){
      strip.clear();
      strip.fill(strip.Color(0,0,255),0,5);
    }else{
      if(showRnd == true){
        showRandom();
      }else{
        strip.clear();
        strip.setPixelColor(10,strip.Color(0,0,255));
        strip.show();
      } 
    }  
    strip.setBrightness(BRIGHTNESS);
    strip.show();    
  }
}


void showConfirm(){
  strip.clear();
  strip.setPixelColor(10,strip.Color(0,0,255));
  for(int i=0; i<NUMPIXELS; i++){
    strip.setPixelColor(i,strip.Color(random(255),random(255),random(255)));  
    strip.setBrightness(BRIGHTNESS);
    strip.show();
    delay(100);
  }
  delay(200);
}

void showRandom(){
  strip.clear();
  strip.setPixelColor(10,strip.Color(255,255,255));
  for(int i=0; i<rnd; i++){
    strip.setPixelColor(i,strip.Color(255,255,255));
  }
  strip.setBrightness(BRIGHTNESS-5);
  strip.show();
}


void loop() {
  
  if (checkGamers == true){
    shift=checkGamersCount();
    checkGamers = false;
    showGamer();
  }
  bool selBtn = digitalRead(PINBTS);
  if (selBtn != lastSelBtnState) {
    lastDebounceTimeSel = millis();
  }
  
  if ((millis() - lastDebounceTimeSel) > debounceDelay) {
    if (selBtn != selBtnState) {
      selBtnState = selBtn;      
      if (selBtnState == HIGH) {
        selMode++;
        showRnd = false;
        if(selMode > GAMERS-shift+1) selMode=0;
        showGamer();
      }
    }
  }
  lastSelBtnState = selBtn;

  bool upBtn = digitalRead(PINBTU);
  if (upBtn != lastUpBtnState) {
    lastDebounceTimeUp = millis();
  }
  
  if ((millis() - lastDebounceTimeUp) > debounceDelay) {
    if (upBtn != upBtnState) {
      upBtnState = upBtn;      
      if (upBtnState == HIGH) {
        if(selMode>0 && (selMode-1)<GAMERS){
          gamer[gamerId[selMode-1]].level++;
        }else if(selMode>GAMERS){
          checkGamers = true;
          showConfirm();
        }
        showGamer();
      }
    }
  }
  lastUpBtnState = upBtn;

  bool downBtn = digitalRead(PINBTD);
  if (downBtn != lastDownBtnState) {
    lastDebounceTimeDown = millis();
  }
  
  if ((millis() - lastDebounceTimeDown) > debounceDelay) {
    if (downBtn != downBtnState) {
      downBtnState = downBtn;      
      if (downBtnState == HIGH) {
        if(selMode>0 && (selMode-1)<GAMERS){
          gamer[gamerId[selMode-1]].level--;
        }
        if(selMode==(GAMERS-shift+1)){
          showRnd = true;
          rnd=(int)random(NUMPIXELS-1)+1;
          showRandom();
        }
        showGamer();
      }
    }
  }
  lastDownBtnState = downBtn;

  if(count < userChDelay/msDelay){
    count++;
  }else{
    count=0;
    if(cGamer < GAMERS-shift-1){
      cGamer++;
    }else{
      cGamer=0;
    }
    showGamer();
  }
  delay(10);
}
