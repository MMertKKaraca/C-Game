#include<math.h>
#include<string.h>
#include<conio.h>
#include<windows.h>
#include<time.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


#define SAG 77
#define SOL 75
#define SPACE 32
#define mapHeight 40 // 
#define mapWidth 50// 
#define cursorHeight 3
#define cursorWidth 6
#define barrierCount 2
#define barrierLenght 5 // tek sayı
#define pointStuffCount 1



char cursor[cursorHeight][cursorWidth]={{' ',' ','/','\\',' ',' '},
                                        {' ','/','-','-','\\',' '},
                                        {'/','-','-','-','-','\\'}}; 
char map[mapHeight][mapWidth];


void clearScreen(){	
      COORD cursorPosition;cursorPosition.X = 0;cursorPosition.Y = 0;
      SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}


void emptyMap(){
      for(int mapRow = 0;mapRow<mapHeight;mapRow++){
            for(int mapColumn = 0;mapColumn<mapWidth;mapColumn++){
                  if(mapColumn==0 || mapColumn==mapWidth-1){
                        map[mapRow][mapColumn]='|';
                  }
                  else{
                        map[mapRow][mapColumn]=' ';
                  }
            }
      };
}

void printMap(){
      for(int mapRow = 0;mapRow<mapHeight;mapRow++){
            for(int mapColumn = 0;mapColumn<mapWidth;mapColumn++){     
                  printf("%c",map[mapRow][mapColumn]);
                  if(mapColumn==mapWidth-1){
                        printf("\n");
                  }
            }
      };
}


int bulletPosition,bulletLeft,bulletRight;
int bulletQueueRow = 0,bulletQueueColumn = 3;
int **bulletQueue;

void popBullet(){
      
      if(bulletQueueRow!=0){
             for(int row=0;row<bulletQueueRow;row++){
                  if(bulletQueue[row][0]==-1){
                        bulletQueueRow--;
                        free(bulletQueue[row]);
                        bulletQueue = realloc(bulletQueue,sizeof(int *) * bulletQueueRow+1);
                        break;
                  }
            }
      }
}

void pushLastBullet(){

      bulletQueueRow++;
      bulletQueue = realloc(bulletQueue,sizeof(int *) * bulletQueueRow);
      bulletQueue[bulletQueueRow-1] = malloc(sizeof(int)*bulletQueueColumn);

      for(int column = 0;column<bulletQueueColumn;column++){
            switch(column){
                  case 0:{
                        bulletQueue[bulletQueueRow-1][column]=bulletPosition;
                        break;
                  }
                  case 1:{
                        bulletQueue[bulletQueueRow-1][column]=bulletLeft;
                        break;
                  }
                  case 2:{
                        bulletQueue[bulletQueueRow-1][column]=bulletRight;
                        break;
                  }
            }
      }

   

}

char bulletForm = '|';
void gun(){
      for(int brow=0;brow<bulletQueueRow;brow++){
            for(int mrow=0;mrow<mapHeight-cursorHeight;mrow++){
                  if(bulletQueue[brow][0]==mrow){
                        for(int bcolumn=1;bcolumn<bulletQueueColumn;bcolumn++){
                              switch(bcolumn){
                                    case 1:{
                                          int bulletL = bulletQueue[brow][bcolumn];
                                          map[mrow][bulletL]=bulletForm;
                                          break;
                                    }
                                    case 2:{
                                          int bulletR = bulletQueue[brow][bcolumn];
                                          map[mrow][bulletR]=bulletForm;
                                          break;
                                    }
                              }
                        }
                  }
            }
      }
      for(int row=0;row<bulletQueueRow;row++){
            if(bulletQueue[row][0]>-1){
                  bulletQueue[row][0]-=1;
            } 
      }
}

int input = (mapWidth/2)-(cursorWidth-3);
void inputs(){
      while(_kbhit()){
             switch(_getch()){
                  case SOL:{
                        if(input!=1){
                              input--;
                              break;
                        }
                        break;
                  }
                  case SAG:{
                        if(input!=mapWidth-cursorWidth-1){
                              input++;
                              break;
                        }
                        break;
                  }
                  case SPACE:{
                        bulletLeft = input + (cursorWidth/2)-1;
                        bulletRight = input + cursorWidth/2;
                        bulletPosition = mapHeight-cursorHeight-1;
                        pushLastBullet();
                        break; 
                  }
            }
      }
}

void gameStartPosition(){
      int mapRow= mapHeight-3;
      int mapColumn = input;
      for(int cursorRow = 0;cursorRow<cursorHeight;cursorRow++){
            for(int cursorColumn = 0;cursorColumn<cursorWidth;cursorColumn++){    
                  map[mapRow][mapColumn]=cursor[cursorRow][cursorColumn];
                  mapColumn++;
            }
            mapColumn = input;
            mapRow++;
      };
}


bool gameOver=true;
int ranNumber;
int barrierQueueRow = 0;
int barrierQueueColumn = barrierCount+1;
int **barrierQueue;

char barrierForm ='_';
int count = 1;
void parkour(){
      if(count == 15){
            barrierQueueRow++;
            barrierQueue = realloc(barrierQueue,sizeof(int *) * barrierQueueRow);
            barrierQueue[barrierQueueRow-1] = malloc(sizeof(int)*barrierQueueColumn);

            ranNumber = (rand()%(mapWidth-(barrierLenght+1)))+(barrierLenght/2)+1;

            barrierQueue[barrierQueueRow-1][0]=0;
            barrierQueue[barrierQueueRow-1][1]=ranNumber;

            bool status;
            for(int brrC = 2;brrC<barrierQueueColumn;brrC++){

                  status=true;
                  jump:{
                        while(status){
                              ranNumber = (rand()%(mapWidth-(barrierLenght+1)))+(barrierLenght/2)+1;
                              for(int col = 1; col<barrierQueueColumn;col++){
                                    for(int brrL = -(barrierLenght/2); brrL<=(barrierLenght/2);brrL++){
                                          if(ranNumber == barrierQueue[barrierQueueRow-1][col]+brrL){
                                                goto jump;
                                          }
                                          else{
                                                if(col == barrierQueueColumn-1){
                                                      barrierQueue[barrierQueueRow-1][brrC]=ranNumber;
                                                      status = false;
                                                }
                                          }
                                    }
                              }
                        }  
                  }
            }
           
            count = 1;
      }
      else{
            count++;
      }

      
      for(int brrow=0;brrow<barrierQueueRow;brrow++){
            for(int mrow=0;mrow<mapHeight;mrow++){
                  if(barrierQueue[brrow][0]==mrow){
                        for(int brrcolumn=1;brrcolumn<barrierQueueColumn;brrcolumn++){
                              int barrierC = barrierQueue[brrow][brrcolumn];
                              for(int brrL = -(barrierLenght/2); brrL<=(barrierLenght/2);brrL++){
                                    map[mrow][barrierC+brrL]=barrierForm;
                              }
                        }
                  }
            }
      }

      for(int row=0;row<barrierQueueRow;row++){
            if(barrierQueue[row][0]<mapHeight+1){
                  barrierQueue[row][0]+=1;
            } 
      }
}

int scoreStuffQueueRow = 0;
int scoreStuffQueueColumn = pointStuffCount+1;
int **scoreStuffQueue;
 
char scoreStuffForm ='+';
int countTwo = 1;
void scoreStuff(){
      if(countTwo==5){
            scoreStuffQueueRow++;
            scoreStuffQueue = realloc(scoreStuffQueue,sizeof(int *) * scoreStuffQueueRow);
            scoreStuffQueue[scoreStuffQueueRow-1] = malloc(sizeof(int)*scoreStuffQueueColumn);

            scoreStuffQueue[scoreStuffQueueRow-1][0]=0;
            for(int sSColumn=1;sSColumn<scoreStuffQueueColumn;sSColumn++){
                  ranNumber = (rand()%(mapWidth-cursorWidth-1))+cursorWidth/2;
                  scoreStuffQueue[scoreStuffQueueRow-1][sSColumn]=ranNumber;
            }
            countTwo = 1;
      }
      else{
            countTwo++;
      }
      for(int sSRow=0;sSRow<scoreStuffQueueRow;sSRow++){
            for(int mrow=0;mrow<mapHeight;mrow++){
                  if(scoreStuffQueue[sSRow][0]==mrow){
                        for(int sSColumn=1;sSColumn<scoreStuffQueueColumn;sSColumn++){
                              int sSC = scoreStuffQueue[sSRow][sSColumn];
                              map[mrow][sSC]=scoreStuffForm;
                        }
                  }
            }
      }

      for(int row=0;row<scoreStuffQueueRow;row++){
            if(scoreStuffQueue[row][0]<mapHeight){
                  scoreStuffQueue[row][0]+=1;
            }

      }
      
}

int score = 0;
void logic(){
      //barrier-cursor logic
      int crow = 0;
      int ccolumn=0;
      for(int mrow = mapHeight-cursorHeight;mrow<mapHeight;mrow++){
            for(int mcolumn = input;mcolumn<input+cursorWidth;mcolumn++){
                  if(cursor[crow][ccolumn]!=' ' && map[mrow][mcolumn]==barrierForm){
                        gameOver=false;
                  }
                  ccolumn++;
            }
            ccolumn=0;
            crow++;
      }

      // barrier-bullet logic
      
      for(int bllRow = 0; bllRow<bulletQueueRow;bllRow++){
            for(int brrRow=0;brrRow<barrierQueueRow;brrRow++){

                  if(bulletQueue[bllRow][0]==barrierQueue[brrRow][0] || bulletQueue[bllRow][0]+1==barrierQueue[brrRow][0]){

                        for(int bllColumn=1;bllColumn<bulletQueueColumn;bllColumn++){
                              for(int brrColumn=1;brrColumn<barrierQueueColumn;brrColumn++){
                                    for(int brrL = -(barrierLenght/2); brrL<=(barrierLenght/2);brrL++){

                                          if(bulletQueue[bllRow][bllColumn]==barrierQueue[brrRow][brrColumn]+brrL){
                                                bulletQueue[bllRow][0] = -1;
                                          }
                                    }
                              }
                        }
                  }
            }
      }

      //bullet-score stuff logic

      for(int bllRow = 0; bllRow<bulletQueueRow;bllRow++){
            for(int sSRow=0;sSRow<scoreStuffQueueRow;sSRow++){

                  if(bulletQueue[bllRow][0]==scoreStuffQueue[sSRow][0] || bulletQueue[bllRow][0]+1==scoreStuffQueue[sSRow][0]){

                        for(int bllColumn=1;bllColumn<bulletQueueColumn;bllColumn++){
                              for(int sSColumn=1;sSColumn<scoreStuffQueueColumn;sSColumn++){
                                    if(bulletQueue[bllRow][bllColumn]==scoreStuffQueue[sSRow][sSColumn]){
                                          scoreStuffQueue[sSRow][0] = mapHeight+10;
                                          score +=10;
                                    }
                              }
                        }
                  }
            }
      }
        
}


void hidecursor(){
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}


void game(){
       while(gameOver){
            inputs();
            clearScreen();
            emptyMap();
            scoreStuff();
            gameStartPosition();
            gun();
            parkour();
            printMap();
            logic();
            printf("Score: %d",score);

      }
}


int main(){

      srand((unsigned int)time(NULL));
      bulletQueue= malloc(sizeof(int *) * 1);
      for(int i = 0; i <1;i++){
            bulletQueue[i] = malloc(sizeof(int) *bulletQueueColumn);
      };

      barrierQueue= malloc(sizeof(int *) * 1);
      for(int i = 0; i <1;i++){
            barrierQueue[i] = malloc(sizeof(int) *barrierQueueColumn);
      };

      scoreStuffQueue= malloc(sizeof(int *) * 1);
      for(int i = 0; i <1;i++){
            scoreStuffQueue[i] = malloc(sizeof(int) *scoreStuffQueueColumn);
      };

      hidecursor();
      printf("Oyun Nasıl Oynanır?\n");
      printf("SOL ve SAG ok tusları ile hareket edip engellerden kacılır. \n");
      printf("SPACE tusu ile ates edip puan toplanılır.\n");
      printf("Oyna Baslamak icin herhangi bir tusa basınız.\n");
      getch();
      system("CLS");

      game();

      printf("\n\n\n\n");
      printf("\n");
      
      bool status = true;
      printf("Oyundan Cıkmak icin K tusuna basınız.");
      while(status){
            if(getch()==107){
                  status = false;
            }
      }

      for(int i = 0; i <bulletQueueRow;i++){
            free(bulletQueue[i]);
      };
      free(bulletQueue);

      for(int i = 0; i <barrierQueueRow;i++){
            free(barrierQueue[i]);
      };
      free(barrierQueue);

      for(int i = 0; i <scoreStuffQueueRow;i++){
            free(scoreStuffQueue[i]);
      };
      free(scoreStuffQueue);

      system("CLS");
      
      return 0;
}





