#include "Gomoku.hpp"
#include <limits>
#include <vector>
#include "Macro.h"

int Gomoku::start(int size) {
  if (size <= 0) {
    return FAILURE;
  }

  _width = size;
  _height = size;
  initializeBoard();
  return SUCCESS;
}

int Gomoku::rectStart(int width, int height) {
  if (width <= 0 || height <= 0) {
    return FAILURE;
  }

  _width = width;
  _height = height;
  initializeBoard();
  return SUCCESS;
}

void Gomoku::restart() {
  _board.clear();
  initializeBoard();
}

void Gomoku::initializeBoard() {
  _board.assign(_width, std::vector<Player>(_height, NONE));
  _initialized = true;
}

bool Gomoku::isValid(const Move &move) const {
  return move.x >= 0 && move.x < _width && move.y >= 0 && move.y < _height &&
         _board[move.x][move.y] == NONE;
}

bool Gomoku::isOnBoard(const Move &move) const {
  return move.x >= 0 && move.x < _width && move.y >= 0 && move.y < _height;
}

void Gomoku::updateBoard(const Move &move, Player player) {
  if (isValid(move)) {
    _board[move.x][move.y] = player;
  }
}

Move Gomoku::getBestMove() {
  std::vector<Move> moves;

  if (_begin) {
    _begin = false;
    return {_width / 2, _height / 2};
  }

  moves = generateMoves();

  Move bestMove = moves[0];
  int bestValue = std::numeric_limits<int>::min();
  int alpha = std::numeric_limits<int>::min();
  int beta = std::numeric_limits<int>::max();
  int maxDepth = 3;

  for (const auto &move : moves) {
    _board[move.x][move.y] = ME;

    int val = -negamax(maxDepth - 1, -beta, -alpha, OPPONENT);

    if (val > bestValue) {
      bestValue = val;
      bestMove = move;
    }
    if (val > alpha) {
      alpha = val;
    }
  }

  updateBoard(bestMove, ME);
  return bestMove;
}

std::vector<Move> Gomoku::generateMoves() {
  std::vector<Move> moves;

  for (int x = 0; x < _width; x++) {
    for (int y = 0; y < _height; y++) {
      if (_board[x][y] == NONE)
        continue;

      bool hasNeighbor = false;
      for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
          if (dx == 0 && dy == 0)
            continue;
          if (isOnBoard({x + dx, y + dy}) && _board[y + dy][x + dx] != NONE) {
            hasNeighbor = true;
            break;
          }
        }
        if (hasNeighbor)
          break;
      }
      if (hasNeighbor) {
        moves.push_back({x, y});
      }
    }
  }

  return moves;
}

int Gomoku::negamax(int depth, int alpha, int beta, Player player) {
  if (depth == 0) {
    return (player == ME) ? evaluate() : -evaluate();
  }

  std::vector<Move> moves = generateMoves();
  if (moves.empty())
    return (player == ME) ? evaluate() : -evaluate();

  int bestValue = std::numeric_limits<int>::min();

  for (const auto &move : moves) {
    _board[move.x][move.y] = player;

    int val =
        -negamax(depth - 1, -beta, -alpha, (player == ME) ? OPPONENT : ME);

    _board[move.x][move.y] = NONE;

    if (val > bestValue) {
      bestValue = val;
    }
    if (val > alpha) {
      alpha = val;
    }
    if (alpha >= beta) {
      break;
    }
  }

  return bestValue;
}

int Gomoku::evaluate() {
  int score = 0;

  const int dx[] = {1, 0, 1, 1};
  const int dy[] = {0, 1, 1, -1};

  for (int x = 0; x < _width; x++) {
    for (int y = 0; y < _height; y++) {
      if (_board[x][y] == NONE)
        continue;

      Player currentPlayer = _board[x][y];

      int multiplicator = (currentPlayer == ME) ? 1 : -1;

      for (int dir = 0; dir < 4; dir++) {
        int prevX = x - dx[dir];
        int prevY = y - dy[dir];

        if (isOnBoard({prevX, prevY}) &&
            _board[prevX][prevY] == currentPlayer) {
          continue;
        }

        int count = 0;
        int currX = x;
        int currY = y;

        while (isOnBoard({currX, currY}) &&
               _board[currX][currY] == currentPlayer) {
          count++;
          currX += dx[dir];
          currY += dy[dir];
        }

        if (count >= 5) {
          score += 10000 * multiplicator;
        } else if (count == 4) {
          score += 1000 * multiplicator;
        } else if (count == 3) {
          score += 100 * multiplicator;
        } else if (count == 2) {
          score += 10 * multiplicator;
        }
      }
    }
  }

  return score;
}
