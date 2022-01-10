//--------libraries----------------
#include "Adafruit_GFX.h"
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>

//-------define color------------------------
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
//-------define pins-----------------------
uint8_t YP = A1;
uint8_t XM = A2;
uint8_t YM = 7;
uint8_t XP = 6;
uint8_t SwapXY = 1;

#define cs   10
#define dc   9
#define rst  8

//--------define images-------------------
extern const uint8_t bottle1[];
extern const uint8_t straw1[];
extern const uint8_t bag1[];
extern const uint8_t fish[];

//---------define touch screen--------------------

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
TSPoint tp;

#define MINPRESSURE 20
#define MAXPRESSURE 1000

int switchPin = 10;
int buzzerPin = 11;
int controllerPin = A5;
#define SWAP(a, b) {uint16_t tmp = a; a = b; b = tmp;}

uint16_t g_identifier;
//--------------Scene control-----------------------------------------
#define MENU 1
#define GAME 2
#define RANK 3
#define GUIDE 4
#define RESULT 5

int displayScene = 1;
//------------player Control--------------------------
int paddleX = 120;
int oldPaddleX = 120;
boolean shoot = 0;
int bulletX;
int bulletY = 425;
int posX = 10;
int posY = 30;
//-----------ENEMIES Control----------------------------
#define ROW 7
#define COL 3
int ENEMY_WIDTH = 32;
int ENEMY_HEIGHT = 50;
int INITIAL_X = 10;
int INITIAL_Y = 30;
int distance = 5;
int down = 50;
boolean moveDown = 0;
int enemyBulletY;
int enemyBulletX;
//------------gameControl----------------------
int score = 0;
int count = 0;
int num = 0;
boolean pause = 0;
//--------------------------------------
int oldScore = 0;
int latestScore = 0;
int first, second, third;

//---------------------------------------
struct Enemy {
  
  int posX, posY;
  int health;

  void move(int x, int y) {
    posX += x;
    posY += y;
  }

  void collide(int bulletX, int bulletY, int col) {
    if (health == 0) { // enemy already dead, ignore
      return;
    }

    if (bulletX >= posX && bulletX <= posX + ENEMY_WIDTH
        && bulletY >= posY && bulletY <= posY + ENEMY_HEIGHT && shoot == 1) {
      // enemy got hit, health reduced
      health--;
      switch(col){
        case 0: score += 40;
        case 1: score += 20;
        case 2: score += 10;
        
        }
      num += 1;
      tone( buzzerPin, 800, 100);
      tft.fillCircle( bulletX, bulletY + 40, 5, BLACK);
      shoot = 0;
    }
  }
};

Enemy enemies[ROW][COL];


void setup() {

  Serial.begin(9600);
  pinMode(switchPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  //---------------------
  static uint16_t identifier;
  uint8_t Orientation = 0;
  g_identifier = tft.readID();
  if (g_identifier == 0x00D3 || g_identifier == 0xD3D3) g_identifier = 0x9481;
  if (g_identifier == 0xFFFF) g_identifier = 0x9341;
  ts = TouchScreen(XP, YP, XM, YM, 300);
  tft.begin(g_identifier);
  // show_Serial();
  tft.fillScreen(BLACK);

  //------------------
  initializeEnemies();
 
}
void loop() {
  if( displayScene == MENU){
        menu();
  }
 
  if(displayScene == GAME){
        game();
        
        }

  if( displayScene == RANK){
        rank();      
    } 
 if( displayScene == GUIDE){
        guide();  
           } 

 if( displayScene == RESULT){
    gameResult();
  
  }
     if ( shoot == 1) {

    tone( buzzerPin, 264, 100);
     //digitalWrite(buzzerPin, HIGH);
     //delay(200);
     //digitalWrite(buzzerPin, LOW);

    tft.fillCircle(bulletX, bulletY + 40, 5, BLACK);

    tft.fillCircle(bulletX, bulletY, 5, CYAN);
    bulletY = bulletY - 40;

  }

 
}
void menu() {

  tft.fillScreen(BLACK);
  tft.drawCircle(50, 150, 15, CYAN);
  tft.drawCircle(58, 145, 4, CYAN);
  tft.drawCircle(280, 100, 15, CYAN);

  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.setCursor((tft.width() * 3 / 16), (tft.height() * 4 / 16));
  tft.println("SEA INVADERS");
  tft.setTextSize(3);
  tft.setTextColor(CYAN);
  tft.setCursor((tft.width() / 2 - 45), (tft.height() / 2));
  tft.println("START");
  tft.drawRect((tft.width() - 280), (tft.height() / 2 - 5), 250, 30, WHITE);
  tft.setTextSize(3);
  tft.setTextColor(CYAN);
  tft.setCursor((tft.width() / 2 - 35), (tft.height() / 2 + 45));
  tft.println("RANK");
  tft.drawRect((tft.width() - 280), (tft.height() / 2 + 40), 250, 30, WHITE);
  tft.setTextSize(3);
  tft.setTextColor(CYAN);
  tft.setCursor((tft.width() / 2 - 85), (tft.height() / 2 + 90));
  tft.println("HOW TO PLAY");
  tft.drawRect((tft.width() - 280), (tft.height() / 2 + 85), 250, 30, WHITE);

  while (1) {

    tp = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    pinMode(XP, OUTPUT);
    pinMode(YM, OUTPUT);

    if (tp.z < MINPRESSURE && tp.z > MAXPRESSURE) continue;
    
    if ( tp.x > 260 && tp.x < 847 && tp.y > 510 && tp.y < 570) {
      tone( buzzerPin, 400, 100);
      tft.fillScreen(BLACK);
      displayScene = GAME;
      break;
      }

    if ( tp.x > 260 && tp.x < 847 && tp.y > 440 && tp.y < 480){
      //break;
       tone( buzzerPin, 400, 100);
      tft.fillScreen(BLACK);
      displayScene = RANK;
     break;
      }

     if( tp. x > 260 && tp.x < 847 &&  tp.y >370 && tp.y < 400){
       tone( buzzerPin, 400, 100);
      tft.fillScreen(BLACK);
     displayScene = GUIDE;
      break;
      }
  }
}
void game() {

  bullets();
  player();
  gameLoop();
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  tft.setCursor((tft.width() / 2 - 30), 10);
  tft.println("SEA INVADERS");
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor((tft.width() - 65), 10);
  tft.print("Score: " + String(score));
  tft.setCursor(5, 10);
  tft.print("Destroyed: " + String(num));
  tft.drawFastHLine( 0, 20, (tft.width()), WHITE);
  
    tp = ts.getPoint(); 
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    pinMode(XP, OUTPUT);
    pinMode(YM, OUTPUT);
    
    if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE) {
      shoot=0;
      tone( buzzerPin, 400, 100);
      displayScene = RESULT;
      
      }
  if(( count >= 6)||(num == 21)) {
    shoot = 0;
    displayScene = RESULT;
  }
 
}
void player() {

  paddleX = map( analogRead(controllerPin), 0, 1023, (tft.width() - 32), 0 );

  if (oldPaddleX != paddleX) {

    tft.fillRect(oldPaddleX - 25, 425,  100, 50 , BLACK);
  }

  ShowImage( paddleX - 25, 425, 50, 50, fish);
  oldPaddleX = paddleX;

}

void bullets() {

  int switchState = digitalRead(switchPin);
  if (( switchState != 1) && (shoot == 0)) {

    bulletX = paddleX;
    bulletY = 425;
    shoot = 1;
  }

  if ( (shoot == 1) && (bulletY <= -20)) {
    shoot = 0;
  }

}

void ShowImage(int x, int y, int w, int h, const uint8_t img[]) {
  tft.setAddrWindow(x, y, x + w - 1, y + h - 1);
  tft.pushColors(img, w * h, 1);
}

void initializeEnemies() {
  distance = 5;
  for (int i = 0; i < ROW; i++) {
    int rowX = i * ENEMY_WIDTH;
    for (int j = 0; j < COL; j++) {
      enemies[i][j].posX = INITIAL_X + rowX;
      enemies[i][j].posY = INITIAL_Y + j * ENEMY_HEIGHT;
      enemies[i][j].health = 1;
    }
  }
}

void drawEnemies() {

  for (int i = 0; i < ROW; i++) {
    for (int j = 0; j < COL; j++) {
      if (enemies[i][j].health > 0) {

        switch (j) {

          case (0):
            tft.setAddrWindow( enemies[i][j].posX, enemies[i][j].posY, enemies[i][j].posX + 32 - 1, enemies[i][j].posY + 50 - 1);
            tft.pushColors( bottle1, 32 * 50, 1);
            break;

          case (1):
            tft.setAddrWindow( enemies[i][j].posX, enemies[i][j].posY, enemies[i][j].posX + 32 - 1, enemies[i][j].posY + 50 - 1);
            tft.pushColors( straw1, 32 * 50, 1);
            break;

          case (2):
            tft.setAddrWindow( enemies[i][j].posX, enemies[i][j].posY, enemies[i][j].posX + 32 - 1, enemies[i][j].posY + 50 - 1);
            tft.pushColors( bag1, 32 * 50, 1);
            break;
        }
      }

      if (enemies[i][j].health <= 0) {
        tft.fillRect( enemies[i][j].posX, enemies[i][j].posY, 32, 50, BLACK);
      }
    }


  }
}

void checkCollision(int bulletX, int bulletY) {
  
  for (int i = 0; i < ROW; i++) {
    for (int j = 0; j < COL; j++) {
      
      enemies[i][j].collide(bulletX, bulletY, j);
         
    }
  }
}

void moveEnemies() {

  if (distance > 0) {
    if (enemies[0][COL - 1].posX + distance > tft.width() - 225) {
      distance *= -1;
      moveDown = 1;

    }
  } else if (distance < 0) {
    if (enemies[0][0].posX - distance < 0) {
      distance *= -1;

    }
  }

  for (int i = 0; i < ROW; i++) {
    for (int j = 0; j < COL; j++) {

      enemies[i][j].posX += distance;

      if (moveDown == 1) {
        enemies[i][j].posY += down;
             }   
     
    }

  }
  //clear previous position
  if ( moveDown == 1) {
    tft.fillRect(enemies[0][0].posX, enemies[0][0].posY - down , 224, 150 + down, BLACK);
     count = count + 1;
  }
  moveDown = 0;

}

void clearEnemy() {
  tft.fillRect(enemies[0][0].posX - 5, enemies[0][0].posY, 10, 150, BLACK);
  tft.fillRect(enemies[6][0].posX + 30, enemies[6][0].posY, 10, 150, BLACK);

}

   
  
  
void gameLoop() {
  clearEnemy();
  moveEnemies();
  checkCollision(bulletX, bulletY);
  drawEnemies();
}


void rank(){

    tft.fillScreen(BLACK);
    tft.setTextSize(4);
    tft.setTextColor(WHITE);
    tft.setCursor( 110, 120);
    tft.println("RANK");

    tft.setTextColor(YELLOW);
    tft.setCursor( 100, 190);
    tft.println("1. " + String(first));

    tft.setCursor( 100, 240);
    tft.println("2. " + String(second));
       
    tft.setCursor(100, 290);
    tft.println("3. " + String(third));

    tft.drawCircle( 160, 240, 150, CYAN);
      while (1){
            tp = ts.getPoint();
            pinMode(XM, OUTPUT);
            pinMode(YP, OUTPUT);
            pinMode(XP, OUTPUT);
            pinMode(YM, OUTPUT);
            
            if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE){
              tone( buzzerPin, 400, 100);
             displayScene = MENU;
             break;                 
              }
            
          } 
  
  
  }

void guide(){
  
    tft.fillScreen(BLACK);
    tft.setTextSize(4);
    tft.setTextColor(CYAN);
    tft.setCursor( 30, 15);
    tft.println("HOW TO PLAY");
    tft.drawFastHLine( 0, 50, (tft.width()), WHITE);

    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.setCursor( 10, 60);
    tft.println( "1. Use potentiometer to");
    tft.setCursor( 10, 80);
    tft.println( "   move the fish");
    tft.setCursor( 10, 120);
    tft.println("2. Press switch button");
    tft.setCursor( 10, 140);
    tft.println("   to shoot");
    tft.setCursor( 10, 180);
    tft.println("3. Touch the screen");
    tft.setCursor( 10, 200);
    tft.println("   to end game");
    tft.setCursor( 10, 240);
    tft.println("4. Scoring: ");

    ShowImage(50, 270, 32, 50, bottle1);
    tft.setTextColor(YELLOW);
    tft.setCursor( 52, 330);
    tft.println("40");
    tft.setCursor( 32, 350);
    tft.println("Points");
    
    ShowImage(140, 270, 32, 50, straw1);
    tft.setCursor( 152, 330);
    tft.print("20");
    tft.setCursor( 132, 350);
    tft.println("Points");

    ShowImage(250, 270, 32, 50, bag1);
    tft.setCursor( 252, 330);
    tft.print("10");
    tft.setCursor( 232, 350);
    tft.print("Points");
    
     while (1){
            tp = ts.getPoint();
            pinMode(XM, OUTPUT);
            pinMode(YP, OUTPUT);
            pinMode(XP, OUTPUT);
            pinMode(YM, OUTPUT);
            
            if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE){
              tone( buzzerPin, 400, 100);
             displayScene = MENU;
             break;                 
              }
            
          } 
  
  }  

void gameResult(){
  
    tft.fillScreen(BLACK);
    tft.drawCircle( 160, 240, 150, CYAN);
    tft.setTextSize(4);
    tft.setTextColor(WHITE);
    tft.setCursor( 50, 150);
    tft.println("Game Over");
    
    tft.setTextSize(2);
    tft.setCursor(50,200);
    tft.println("You have destroyed");

    tft.setTextSize(2);
    tft.setCursor(80, 300);
    tft.println("Marine Litters");

    tft.setCursor(100, 330);
    tft.setTextColor(YELLOW);
    tft.println("Score: " + String(score));

    tft.setTextSize(3);
    tft.setTextColor(YELLOW);
    tft.setCursor(150, 250);
    tft.println(num);

    

    latestScore = score;
    ranking();
    
    ShowImage( 30, 370, 50, 50, fish);

     while (1){
            tp = ts.getPoint();
            pinMode(XM, OUTPUT);
            pinMode(YP, OUTPUT);
            pinMode(XP, OUTPUT);
            pinMode(YM, OUTPUT);
            
            if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE){
               tone( buzzerPin, 400, 100);
              displayScene = MENU;
              gameReset();
             break;                 
              }
          
          } 
      }  

void gameReset(){
  count = 0;
  score = 0;
  num = 0;
  INITIAL_X = 10;
  INITIAL_Y = 30;
   for (int i = 0; i < ROW; i++) {
    int rowX = i * ENEMY_WIDTH;
    for (int j = 0; j < COL; j++) {
      enemies[i][j].posX = INITIAL_X + rowX;
      enemies[i][j].posY = INITIAL_Y + j * ENEMY_HEIGHT;
      enemies[i][j].health = 1;
    }
  }
  }  
 
void ranking(){
  
  if( latestScore > first){

    third = second;
    second = first;
    first = latestScore;
      
        }
   if( latestScore > second && latestScore < first){
    third = second;
    second = latestScore;
    
    }
   if( latestScore > third && latestScore < second){
    third = latestScore;
        }
    }
 
