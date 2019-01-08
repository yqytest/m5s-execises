#include <M5Stack.h>
#define TFT_GREY 0x5AEB
#define TFT_BROWN 0XBC40
#define TFT_BRRED 0XFC07
#define TFT_DARKBLUE 0X01CF
#define TFT_LIGHTBLUE 0X7D7C
#define TFT_GRAYBLUE 0X5458
#define TFT_LIGHTGREEN 0X841F
#define TFT_LGRAY 0XC618
#define TFT_LGRAYBLUE 0XA651
#define TFT_LBBLUE 0X2B12
#define TFT_MAGENTA 0xF81F
#define TFT_GRED 0XFFE0
#define TFT_GBLUE  0X07FF

//GLOBAL
unsigned long pre_millis_ = 0;
uint8_t refresh_millis_ = 1000;
uint8_t isGameOver = 0;
//layout
uint8_t scX=87, scY=0, scW=135, scH=240;
uint8_t rn=20, cn=11;
//grids and blocks
uint8_t gX0=89, gY0=0, gW=12, gH=12, bW=11, bH=11;
uint8_t rL[2][3] = {{1,0,0},{1,1,1}};
uint8_t fixedBlocks[20][11];
uint8_t activedBlock[4][2];

// the setup routine runs once when M5Stack starts up
void setup() {
  // initialize the M5Stack object
  M5.begin();
  
  M5.Lcd.fillScreen(BLACK);
  delay(500);
  pre_millis_ = millis();

  // draw graphic
  delay(1000);
  M5.Lcd.fillRect(scX, scY, scW, scH, TFT_BROWN);
  
  init();
  createItem(0, 4);
}

void init(){
  for(int i = 0; i<rn; i++) {
    for(int j = 0; j<cn; j++) {
      fixedBlocks[i][j] = 0;
      M5.Lcd.fillRect(gX0+(j*gW), i*gH, bW, bH, TFT_GREY);
    }
  }
  for(int i = 0; i<4; i++){
    activedBlock[i][0] = 0;
    activedBlock[i][1] = 0;
  }
  //next item
  createText(10, 10, TFT_GRAYBLUE, 1,"NEXT");
  M5.Lcd.drawRoundRect(10, 20, 68, 50, 5, TFT_GRAYBLUE);
  //levels
  createText(234, 10, TFT_LGRAYBLUE, 1, "LEVELS");
  M5.Lcd.drawRoundRect(234, 20, 74, 50, 5, TFT_LGRAYBLUE);
  createText(250, 36, TFT_LGRAYBLUE, 3, "1");
  //scores
  createText(234, 170, TFT_MAGENTA, 1, "SCORES");
  M5.Lcd.drawRoundRect(234, 180, 74, 50, 5, TFT_MAGENTA);
  createText(244, 200, TFT_MAGENTA, 2, "0");
  //logo
  createText(8, 220, YELLOW, 2, "TETRIS");
}

//layout
void resetLayout(){
  for(int i = 0; i<rn; i++) {
    for(int j = 0; j<cn; j++) {
      drawBlock(i,j, TFT_GREY);
    }
  }
}

void createText(uint8_t locX, uint8_t locY, uint16_t color, uint8_t fSize,  char* str){
  M5.Lcd.setCursor(locX, locY);
  M5.Lcd.setTextColor(color);
  M5.Lcd.setTextSize(fSize);
  M5.Lcd.printf(str);
}

//draw a block, row and col in layout
void drawBlock(uint8_t rIdx, uint8_t cIdx, uint16_t color){
  M5.Lcd.fillRect(gX0+(cIdx*gW), rIdx*gH, bW, bH, color);
}

void drawFixedBlocks(){
  for(int i = 0; i<rn; i++) {
    for(int j = 0; j<cn; j++) {
      if(fixedBlocks[i][j]==1){
        drawBlock(i, j, RED);
      }
    }
  }
}

void createItem(uint8_t startRIdx, uint8_t startCIdx){
  uint8_t k = 0;
  for(int i = 0; i<2; i++) {
    for(int j = 0; j<3; j++) {
      if(rL[i][j]==1){
        activedBlock[k][0] = startRIdx+i;
        activedBlock[k][1] = startCIdx+j;
        k++;
      }
    }
  }
}

void moveActivedItem(int8_t dx, int8_t dy){
  for(int i = 0; i<4; i++) {
    activedBlock[i][0] = activedBlock[i][0] + dx;
    activedBlock[i][1] = activedBlock[i][1] + dy;
  }
}

void drawActivedItem(){
  for(int i = 0; i<4; i++) {
    drawBlock(activedBlock[i][0], activedBlock[i][1], GREEN);
  }
}

uint8_t checkActivedItem(){
  for(int i=0; i<4; i++){
    if(activedBlock[i][0]==rn-1 || fixedBlocks[activedBlock[i][0]+1][activedBlock[i][1]]==1){
      return 0;
    }
  }
  return 1;
}

// the loop routine runs over and over again forever
void loop(){
  //buttons
  if(M5.BtnA.wasPressed()){
    moveActivedItem(0, -1);
  }
  if(M5.BtnB.wasPressed()){
    moveActivedItem(0, 1);
  }
  if(M5.BtnC.wasPressed()){
    moveActivedItem(1, 0);
  }

  resetLayout();
  drawFixedBlocks();

  if(isGameOver==1){
      createText(56, 100, WHITE, 4, "GAME OVER");
      delay(5000);
  }else{
      if(checkActivedItem()==1){
        //drawBlock(activedBlock[0], activedBlock[1], GREEN);
        drawActivedItem();
        unsigned long millis_ = millis();
        if(millis_>=pre_millis_+ refresh_millis_){
          moveActivedItem(1, 0);
          pre_millis_ = millis_;
        }
        delay(200);
      }else{
        for(int i=0; i<4; i++){
          fixedBlocks[activedBlock[i][0]][activedBlock[i][1]]=1;
          if(activedBlock[i][0]==0){
            isGameOver = 1;
          }
        }
        drawFixedBlocks();
        createItem(0, 4);
        delay(500);
      }
  }

  M5.update();
}
