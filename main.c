#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "board.h"

#define MAX_CHARNAME        200
#define N_PLAYER            3
#define MAX_DIE             6

#define PLAYERSTATUS_LIVE   0
#define PLAYERSTATUS_DIE    1
#define PLAYERSTATUS_END    2


int player_position[N_PLAYER];
char player_name[N_PLAYER][MAX_CHARNAME];
int player_coin[N_PLAYER];
int player_status[N_PLAYER];
char player_statusString[3][MAX_CHARNAME] = {"LIVE","DIE","END"};


void opening(void)
{
     printf("================================================================\n");
     printf("=======================SHARK ISLAND GAME========================\n");
     printf("================================================================\n");
     
}

int rolldie(void)
{
    return rand()%MAX_DIE+1;
}

void printPlayerPosition(int player)
{
     int i;
     for(i=0;i<N_BOARD;i++)
     {
              printf("|");   
              if(i == player_position[player])
                 printf("%c", player_name[player][0]);
              else
              {
                  if(board_getBoardStatus(i) == BOARDSTATUS_NOK)
                    printf("X");
                  else
                    printf(" ");
              }
     }
     printf("|\n");
} 

void printPlayerStatus(void)
{
     int i;
     printf("player status ---\n");
     for(i=0;i<N_PLAYER;i++)
     {
                            printf("%s : pos %i, coin: %i, status: %s\n", player_name[i], player_position[i],player_coin[i], player_statusString[player_status[i]]);
     }
     printf("---------------\n");
}

void checkDie(void)
{
     int i;
     for(i=0;i<N_PLAYER;i++)
        if(board_getBoardStatus(player_position[i]) == BOARDSTATUS_NOK)
          player_status[i] = PLAYERSTATUS_DIE;
}

int game_end(void);
int getALivePlayer(void);
int getWinner(void);

int main(int argc, char *argv[])
{
    int pos=0;
    int i;
    int turn = 0;
    
    srand((unsigned)time(NULL));
    
    //0. opening
    opening();
    
    //1. 초기화 & 플레이어 이름 결정
    board_initboard();
    //1-1.
    for (i=0;i<N_PLAYER;i++)
    {
        player_position[i] = 0;
        player_coin[i] = 0;
        player_status[i] = PLAYERSTATUS_LIVE;
        //player_name
        printf("Player %i's name: ",i);
        scanf("%s", player_name[i]);
    }
    
    //2. 반복문 (플레이어 턴)
    do {
        int step = rolldie();
        int coinResult;
        char c;
        
        if(player_status[turn] != PLAYERSTATUS_LIVE)
        {
            turn = (turn +1)%N_PLAYER;
            continue;
        }
        
        
    //2-1. 보드& 플레이어의 상태 출력
        board_printBoardStatus();      
        for (i=0;i<N_PLAYER;i++)
             printPlayerPosition(i);
             printPlayerStatus();
        

      //2-2. 주사위 던지기 
      printf("%s turn!!", player_name[turn]);
      printf("press any key to roll a die!");
      scanf("%d", &c);
      fflush(stdin);
      step = rolldie();
      
      board_printBoardStatus();
      
      
      //2-3. 이동
      player_position[turn] += step;
      if(player_position[turn]>= N_BOARD)
      {
        player_position[turn] = N_BOARD-1;
      }
      if(player_position[turn]>=N_BOARD)
        player_status[turn] = PLAYERSTATUS_END;
        
      printf("%s moved %d step\n", player_name[turn],step);
      
        

      //2-4. 동전 줍기
      coinResult =board_getBoardCoin(pos);
      player_coin[turn] += coinResult;
      if (coinResult > 0) 
      {
         printf("%s found %d coins!\n", player_name[turn], coinResult);
      } 
      else 
      {
         printf("No coins found at position %d.\n", player_position[turn]);
      }

      
      
      
      
      //2-5. 다음턴
      turn = (turn + 1)%N_PLAYER;
      //2-6. if 상어 동작(조건: 모든 플레이어가 한번씩 턴을 돔)
      if(turn == 0)
      {
         int shark_pos = board_stepShark();  
         printf("Shark moved to %d\n", shark_pos); 
         checkDie();  
      }
}while(!game_end()); 

//3. 정리 (승자 계산, 출력)
    printf("Game Over!\n");
        int alivePlayers = getAlivePlayer();
        printf("Number of alive players: %d\n", alivePlayers);
        if (alivePlayers == 1) 
        {
            int winner = getWinner();
            printf("Player %s wins with %d coins!\n", player_name[winner], player_coin[winner]);
        } 
        else 
        {
            printf("No clear winner!\n");
        }

  
  return 0;
}

int game_end(void)
{
    int i;
    int flag_end = 1;
    
    for (i = 0; i < N_PLAYER; i++) 
    {
        if (player_status[i] == PLAYERSTATUS_LIVE) 
        {
            flag_end = 0;
            break; // 아직 게임이 끝나지 않음
        }
    }
    return flag_end; // 모든 플레이어가 죽음
}

// getAlivePlayer 함수 정의
int getAlivePlayer(void) 
{
    int alive = 0;
    int i;
    for (i = 0; i < N_PLAYER; i++) 
    {
        if (player_status[i] == PLAYERSTATUS_LIVE) 
        {
            alive++;
        }
    }
    return alive;
}

// getWinner 함수 정의
int getWinner(void) 
{
    int max_coin = -1;
    int winner = 0;
    int i;
    
    for (i = 0; i < N_PLAYER; i++) 
    {
        if (player_coin[i] > max_coin) 
        {
            max_coin = player_coin[i];
            winner = i;
        }
    }
    return winner;


  system("PAUSE");	
  return 0;
}

