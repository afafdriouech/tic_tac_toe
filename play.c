#include "./game.h"
#include "./play.h"
#include <stdlib.h>
#include <time.h>


void change_player(game_t *game)
{
    if (game->player == PLAYER_X) {
        game->player = PLAYER_O;
    } else if (game->player == PLAYER_O) {
        game->player = PLAYER_X;
    }
}


int check_winner(game_t *game, int player)
{
    int nb_row = 0;
    int nb_column = 0;
    int nb_diag1 = 0;
    int nb_diag2 = 0;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (game->board[i * N + j] == player) {
                nb_row++;
            }

            if (game->board[j * N + i] == player) {
                nb_column++;
            }
        }

        if (nb_row >= N || nb_column >= N) {
            return 1;
        }

        nb_row = 0;
        nb_column = 0;

        if (game->board[i * N + i] == player) {
            nb_diag1++;
        }

        if (game->board[i * N + N - i - 1] == player) {
            nb_diag2++;
        }
    }

    return nb_diag1 >= N || nb_diag2 >= N;
}





int empty_cells(const int *board, int cell)
{
    int cmp = 0;

    for (int i = 0; i < N * N; ++i) {
        if (board[i] == cell) {
            cmp++;
        }
    }

    return cmp;
}



int game_over(game_t *game)
{
    if (check_winner(game, PLAYER_X)) {
        game->state = X_WIN_STATE;
        return 1;
    } else if (check_winner(game, PLAYER_O)) {
        game->state = O_WIN_STATE;
    } else if (empty_cells(game->board, EMPTY) == 0) {
        game->state = DRAWN_STATE;
    }
  return 0;
}



void replay_game(game_t *game)
{
    game->player = PLAYER_X;
    game->state = PLAYING_STATE;
    for (int i = 0; i < N * N; ++i) {
        game->board[i] = EMPTY;
    }
}


/*****************************Computer position with minimax algorithm*********************************/
/* https://thecodingtrain.com/CodingChallenges/154-tic-tac-toe-minimax.html*/
int minimax(game_t *game,int depth,int isMaximizing) {
  if (check_winner(game, PLAYER_X)) {
        return -1;
    } else if (check_winner(game, PLAYER_O)) {
        return 1;
    } else if (empty_cells(game->board, EMPTY) == 0) {
        return 0;
    }

  if (isMaximizing) {
    int bestScore = -1;
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        // Is the spot available?
        if (game->board[i*N+j] == EMPTY) {
          game->board[i*N+j] = PLAYER_O;
          int score = minimax(game, depth + 1, 0);
          game->board[i*N+j] = EMPTY;
          if(score > bestScore)
          bestScore = score;
        }
      }
    }
    return bestScore;
  } else {
    int bestScore = 1;
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        // Is the spot available?
        if (game->board[i*N+j] == EMPTY) {
          game->board[i*N+j] = PLAYER_X;
          int score = minimax(game, depth + 1, 1);
          game->board[i*N+j] = EMPTY;
          if(score < bestScore)
          bestScore = score;
        }
      }
    }
    return bestScore;
  }
}


void BestMove(game_t *game) {
    int move = -1;
    int BestScore = -2;
    int i,j;
    for(i = 0; i < N; ++i) {
       for(j = 0; j < N; ++j) {
        if(game->board[i * N + j] == EMPTY) {
            game->board[i * N + j] = PLAYER_O;
            int Score = minimax(game,0,0);
            game->board[i * N + j] = EMPTY;
            if(Score > BestScore) {
               BestScore = Score;
                move = i * N + j;
            }
        }
    }
  }
    //returns a score based on minimax tree at a given node.
    game->board[move] = PLAYER_O;
}

void turn(game_t *game, int row, int column)
{
    if(game->player == PLAYER_X)
         {   if (game->board[row * N + column] == EMPTY)
             {
               game->board[row * N + column] = PLAYER_X ;
               change_player(game);
               game_over(game);
               if(game_over(game)!=1)
                {
                     BestMove(game);
                     change_player(game);
                     game_over(game);
                     }
                }
  }
}

/*****************************Random Computer positions *********************************/

/*void shuffle(int *board, size_t n)
{
    if (n > 1) {
        size_t i;
	for (i = 0; i < n - 1; i++) {
	  size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
	  int t = board[j];
	  board[j] = board[i];
	  board[i] = t;
	}
    }
}

void turn(game_t *game, int row, int column)
{
    int *array;
    array=(int*)malloc(sizeof(int)*9);
    int n=0;
    if(game->player == PLAYER_X)
         {   if (game->board[row * N + column] == EMPTY)
             {
               game->board[row * N + column] = PLAYER_X ;
               change_player(game);
               game_over(game);
               if(game_over(game)!=1)
                { for (int i = 0; i <9 ; i++)
                 array[i]=i;
                      srand((unsigned)time(NULL));
                      shuffle(array,N);
                     while (game->board[array[0]] != EMPTY && n<8)
                         {  shuffle(array,9);
                            n++; }
                     game->board[array[0]] = PLAYER_O ;
                     change_player(game);
                     game_over(game);
                     }
                }
  }
}*/

void click(game_t *game, int row, int column)
{
       if (game->state == PLAYING_STATE)
       turn(game, row, column);
}


