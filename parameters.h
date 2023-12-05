#pragma once

#define TORUS_SIZE 10001

#define U_LIST_LEN 200
#define U_LIST_MAX 10

#define MEMORY 7
// #define VIKI_MEMORY 1002001
#define VIKI_MEMORY 5

#define BLANK 0
#define MINE 77

#define RIGHT 0
#define UP 1
#define ZUP 2
#define LEFT 3
#define DOWN 4
#define ZDOWN 5

#define VIKI 0
#define RANDOM_WALK 1
#define GREEDY_BIASED 2
#define GREEDY_UNBIASED 3
#define GREEDY_BIASED_XY 4
#define GREEDY_BIASED_YZ 5
#define GREEDY_BIASED_ZX 6
#define VIKI_COLORBLIND 7
#define RANDOM_WALK_NB 8

const int dirx[] = {1, 0, 0, -1, 0, 0};
const int diry[] = {0, 1, 0, 0, -1, 0};
const int dirz[] = {0, 0, 1, 0, 0, -1};