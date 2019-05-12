#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "game_function.h"

int main(int argc, char * argv[]) {
  int bombsfound = 0;
  int numOfbombs = 0;
  int numOfclues = 0;
//series of if statements to check for the arguments being passed in, it checks if two arguments are given and if you have the correct bombs and if you provided clues or noclues. If the conditions are not meet, then the user is given an error message and the program ends
  if (argc == 1) {

    printf("Error : Incorrect No arguments provided\n");
    printf("Information : Supply 2 arguments; number of bombs(2,3 or 4) and 'clues' or 'noclues'");

    exit(0);
  }

  if (argc == 2) {
    printf("Error : Incorrect No arguments provided\n");
    printf("Information : Supply 2 arguments; number of bombs(2,3 or 4) and 'clues' or 'noclues'");

    exit(0);
  }

  if (argc == 3) {

    if (!((!(strcmp(argv[1], "2"))) || (!(strcmp(argv[1], "3"))) || (!strcmp(argv[1], "4")))) {
      printf("Error : Invalid number of bombs, choose between 2, 3 or 4 bombs \n");
      printf("Information : Supply 2 arguments; number of bombs(2,3 or 4) and 'clues' or 'noclues'");

      exit(0);
    }

    numOfbombs = atoi(argv[1]);
    printf("%d bombs will be added to the board\n\n", numOfbombs);

    if (!(strcmp(argv[2], "clues"))) {
      printf("3 additional hints will be provided \n\n");
      numOfclues = 5;
    } else if (!(strcmp(argv[2], "noclues"))) {
      printf("No hints will be provided \n\n");

    } else {
      printf("Error: Incorrect argument, choose clues or noclues\n");
      printf("Information : Supply 2 arguments; number of bombs(2,3 or 4) and 'clues' or 'noclues'");

      exit(0);
    }

  }

  if (argc > 3) {
    printf("Error : Invalid arguments provided\n");
    printf("Information : Supply 2 arguments; number of bombs(2,3 or 4) and 'clues' or 'noclues'");

    exit(0);
  }

  //create random 2-D array of bomb locations and initialize with random numbers
  srand(time(0));
  int count = 0;

  int x;
  int y;
  int bomb_location_info[DIM][DIM];

  for (x = 0; x < DIM; x++) {
    for (y = 0; y < DIM; y++) {
      bomb_location_info[x][y] = rand() % 2 + 1;
    }
  }
  char label[] = {
    'A',
    'B',
    'C',
    'D'
  };

  int known_location_info[DIM][DIM] = {
    UNKNOWN,
    UNKNOWN,
    UNKNOWN,
    UNKNOWN,
    UNKNOWN,
    UNKNOWN,
    UNKNOWN,
    UNKNOWN,
    UNKNOWN,
    UNKNOWN,
    UNKNOWN,
    UNKNOWN,
    UNKNOWN,
    UNKNOWN,
    UNKNOWN,
    UNKNOWN
  };

  // Initialize locations which stores the location of each of the bombs ie the X,Y co-ordinate in the 2-D array
  struct locations * bombs = (struct locations * ) malloc(sizeof(struct locations) * numOfbombs);
  // Create numOfBombs randomly in bomb_location and store bomb location details in the bombs structure
  create_bombs(bombs, bomb_location_info, numOfbombs, label);

  // will only display if clues have been asked for
  if (numOfclues > 0) {

    // display current state of game showing where Bombs are found and cells opened that do not have BOMBS
    display_board(known_location_info, label, DIM, DIM);
    // Open numOfclues additional cells depending on argument passed and return number of bombs found while clues being opened

    printf("\nGranting two initial hints \n \n");

    bombsfound = bombsfound + giveHint(known_location_info, bomb_location_info, label, DIM, DIM, 2);

    numOfclues = numOfclues - 2;
  }

  display_board(known_location_info, label, DIM, DIM);

  while ((count < GAMELIMIT) && (bombsfound < numOfbombs)) {

    //Look for Bombs up to Bomb Limit
    bombsfound = bombsfound + get_user_input(known_location_info, bomb_location_info, label, DIM, numOfbombs);

    display_board(known_location_info, label, DIM, DIM);

    if ((numOfclues > 0) && (bombsfound < numOfbombs)) {
      bombsfound = bombsfound + giveHint(known_location_info, bomb_location_info, label, DIM, DIM, 1);
      display_board(known_location_info, label, DIM, DIM);
      numOfclues--;
    }

    count++;

    printf("Bombs found : %d\n", bombsfound);
    printf("Attempts left : %d\n", GAMELIMIT - count);
    if (numOfclues > 0) {
      printf("Number of hints remaining %d\n", numOfclues);
    } else {

      printf("No hints available\n");
    }

  }
  if (bombsfound >= numOfbombs) {
    printf("You have found %d bombs, You win\n", numOfbombs);
    free(bombs);
    exit(0);
  }
  if (count >= GAMELIMIT) {
    printf("\nYou've used up all attempts. Game Over\n");

    free(bombs);
  }

}

void display_board(int known_location_info[DIM][DIM], char label[], int numberOfRows, int numberOfColumns) {

  int row = 0;
  int column = 0;
  printf("    MineSweeper  Game\n");
  printf("      \n     ");

  for (column = 0; column < numberOfColumns; column++) {
    printf(" %c     ", label[column]);

  }
  printf("\n\n");

  for (row = 0; row < numberOfRows; row++) {
    printf("%c :", label[row]);

    for (column = 0; column < numberOfColumns; column++) {

      if (known_location_info[row][column] == UNKNOWN) {

        printf("   *   ");

      } else if (known_location_info[row][column] == BOMB) {
        printf("   B   ");
      } else {
        printf("   %d   ", known_location_info[row][column]);
      }

    }
    printf("\n");
  }

  printf("\n====================================================\n");

}
int giveHint(int known_location_info[][DIM], int bomb_location_info[][DIM], char label[], int numberOfRows, int numberOfColumns, int noOfHints) {

    int bombfound = 0;
    int row;
    int col;
    int i = 0;

    while (i < noOfHints) {
      row = (rand() % DIM);
      col = (rand() % DIM);
      // check that the Hint is only for a cell that we have NOT previously opened

    #  ifdef DEBUG {
        printf("\nChecking for Hint at %c %c\n", label[row], label[col]);
       # endif // DEBUG

        if (known_location_info[row][col] == UNKNOWN) {

          if (bomb_location_info[row][col] == BOMB) {
            bombfound++;
           # ifdef DEBUG {
              printf("\nFound Bomb at %c %c\n", label[row], label[col]);
             # endif // DEBUG
            }

            printf("\nHint granted at  %c %c\n\n", label[row], label[col]);

            known_location_info[row][col] = bomb_location_info[row][col];
            i++;
          }

        }

        return bombfound;
      }


void create_bombs(struct locations * bombs, int ** bomb_location_info[][DIM], int noOfMines, char label[]) {

          srand(time(0));
          int rowrandom;
          int colrandom;
          int i = 0;
          while (i < noOfMines) {

            rowrandom = rand() % (DIM);
            colrandom = rand() % (DIM);
            if (bomb_location_info[rowrandom][colrandom] != 'B') {


              #ifdef DEBUG {
                printf("Bomb Location %d : ", i + 1);
                printf(" %c, %c\n ", label[rowrandom], label[colrandom]);
                printf("\n");
              #  endif // DEBUG

                bombs[i].x = rowrandom;
                bombs[i].y = colrandom;
                bombs[i].found = false;
                bomb_location_info[rowrandom][colrandom] = 'B';
                i++;
              }

            }

    }

int get_user_input(int known_location_info[][DIM], int bomb_location_info[][DIM], char label[], int size, int numOfbombs) {
              char rselect;
              char cselect;
              char newline;
              int row = 0;
              int col = 0;
              int bombsFound = 0;

              do {
                printf("\nMake a selection  \n");
                printf("Row--> ");
                scanf("%c", & rselect);
                scanf("%c", & newline);
                row = rselect - 'A';
                if ((row < 0) || (row > DIM - 1)) {

                  printf("Invalid row selection, try again\n");
                  fflush(stdin);
                }

                printf("Col--> ");
                scanf("%c", & cselect);
                scanf("%c", & newline);

                col = cselect - 'A';
                if ((col < 0) || (col > DIM - 1)) {

                  printf("Invalid column selection, try again\n");
                  fflush(stdin);
                }

              } while ((((row < 0) || (row > DIM - 1))) || (((col < 0) || (col > DIM - 1))));

              //Only open cells that are UNKNOWN
              if ((bomb_location_info[row][col] == BOMB) && (!(known_location_info[row][col] == BOMB))) {

                #ifdef DEBUG {
                  printf("\nFound Bomb at %c %c\n", label[row], label[col]);
                  #endif // DEBUG

                  bombsFound++;
                  //looks for more potential bombs adjacent to an already revealed bomb, inputted by the user
                  bombsFound = bombsFound + checkForAdjacentBombs(known_location_info, bomb_location_info, row, col, numOfbombs);

                }
                known_location_info[row][col] = bomb_location_info[row][col];
                return bombsFound;

              }




int checkForAdjacentBombs(int known_location_info[][DIM], int bomb_location_info[][DIM], int bombxcord, int bombycord, int mineNo) {

int mineFound = 0;
        if (mineFound < mineNo) {
                  //check for bombs in current row from left to right
                  if ((bombxcord >= 0) && (bombycord > 0)) {
                    //  printf("CHECK FOR BOMBS IN CURRENT ROW\n");

                    if ((bomb_location_info[bombxcord][bombycord - 1] == BOMB) && (!(known_location_info[bombxcord][bombycord - 1] == BOMB))) {
                      mineFound++;
                    }
                    known_location_info[bombxcord][bombycord - 1] = bomb_location_info[bombxcord][bombycord - 1];
                  }

                  if ((bombycord >= 0) && (bombycord < DIM - 1)) {
                    //  printf("CHECK FOR BOMBS IN CURRENT ROW\n");
                    if ((bomb_location_info[bombxcord][bombycord + 1] == BOMB) && (!(known_location_info[bombxcord][bombycord + 1] == BOMB))) {

                      mineFound++;
                    }
                    known_location_info[bombxcord][bombycord + 1] = bomb_location_info[bombxcord][bombycord + 1];
                  }

                  //check for boms in row above from left to right

                  if ((bombxcord > 0) && (bombycord >= 0)) {
                    //    printf("CHECK FOR BOMBS IN ABOVE ROW\n");
                    if ((bomb_location_info[bombxcord - 1][bombycord] == BOMB) && (!(known_location_info[bombxcord - 1][bombycord] == BOMB))) {

                      mineFound++;
                    }
                    known_location_info[bombxcord - 1][bombycord] = bomb_location_info[bombxcord - 1][bombycord];
                  }
                  if ((bombxcord > 0) && (bombycord - 1 >= 0)) {
                    //       printf("CHECK FOR BOMBS IN ABOVE ROW\n");
                    if ((bomb_location_info[bombxcord - 1][bombycord - 1] == BOMB) && (!(known_location_info[bombxcord - 1][bombycord - 1] == BOMB))) {

                      mineFound++;
                    }
                    known_location_info[bombxcord - 1][bombycord - 1] = bomb_location_info[bombxcord - 1][bombycord - 1];
                  }
                  if ((bombxcord > 0) && (bombycord < DIM - 1)) {
                    //     printf("CHECK FOR BOMBS IN ABOVE ROW\n");
                    if ((bomb_location_info[bombxcord - 1][bombycord + 1] == BOMB) && (!(known_location_info[bombxcord - 1][bombycord + 1] == BOMB))) {

                      mineFound++;
                    }
                    known_location_info[bombxcord - 1][bombycord + 1] = bomb_location_info[bombxcord - 1][bombycord + 1];
                  }

                  //check right below from left to right

                  if ((bombxcord + 1 < DIM) && (bombycord > 0)) {
                    //   printf("CHECK FOR BOMBS BLEOW IN  ROW\n");
                    if ((bomb_location_info[bombxcord + 1][bombycord - 1] == BOMB) && (!(known_location_info[bombxcord + 1][bombycord - 1] == BOMB))) {
                      mineFound++;
                    }
                    known_location_info[bombxcord + 1][bombycord - 1] = bomb_location_info[bombxcord + 1][bombycord - 1];
                  }
                  if ((bombxcord + 1 < DIM) && (bombycord < DIM)) {
                    //   printf("CHECK FOR BOMBS BLEOW IN  ROW\n");
                    if ((bomb_location_info[bombxcord + 1][bombycord] == BOMB) && (!(known_location_info[bombxcord + 1][bombycord] == BOMB))) {

                      mineFound++;
                    }
                    known_location_info[bombxcord + 1][bombycord] = bomb_location_info[bombxcord + 1][bombycord];
                  }
                  if ((bombxcord + 1 < DIM) && (bombycord + 1 < DIM)) {
                    //    printf("CHECK FOR BOMBS BLEOW IN  ROW\n");
                    if ((bomb_location_info[bombxcord + 1][bombycord + 1] == BOMB) && (!(known_location_info[bombxcord + 1][bombycord + 1] == BOMB))) {
                      mineFound++;
                    }
                    known_location_info[bombxcord + 1][bombycord + 1] = bomb_location_info[bombxcord + 1][bombycord + 1];
                  }

                }
                printf("%d", mineFound);
                return mineFound;

              }
