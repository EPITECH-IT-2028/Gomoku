#pragma once

#define SUCCESS 0
#define FAILURE -1
#define MAX_DEPTH 4

enum Player {
    NONE = 0,
    ME = 1,
    OPPONENT = 2
};

struct Move {
    int x;
    int y;
};