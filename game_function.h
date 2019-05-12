#ifndef GAME_FUNCTION_H_INCLUDED
#define GAME_FUNCTION_H_INCLUDED
//#define DEBUG

#include <stdbool.h>
#include <string.h>
#define DIM 4
#define UNKNOWN -1
#define BOMB 66
#define GAMELIMIT 4

struct locations{
int x;
int y;
bool found;
};


#endif // GAME_FUNCTION_H_INCLUDED
