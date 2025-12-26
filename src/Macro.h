#pragma once

#define SUCCESS 0
#define FAILURE -1

enum Player {
    NONE = 0,
    ME = 1,
    OPPONENT = 2
};

struct Move {
    int x;
    int y;
};