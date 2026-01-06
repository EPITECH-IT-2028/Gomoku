#include "Gomoku.hpp"
#include <limits>
#include <vector>
#include "Macro.h"
#include <algorithm>

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
    updateBoard({_width / 2, _height / 2}, ME);
    return {_width / 2, _height / 2};
  }

  moves = generateMoves();

  for (const auto& move : moves) {
    _board[move.x][move.y] = ME;
    
    if (evaluate() >= 900000000) { 
        _board[move.x][move.y] = NONE;
        updateBoard(move, ME);
        return move;
    }
    _board[move.x][move.y] = NONE;
  }

  for (const auto& move : moves) {
    _board[move.x][move.y] = OPPONENT;

    if (evaluate() < -800000000) { 
        _board[move.x][move.y] = NONE;
        updateBoard(move, ME);
        return move;
      }
    _board[move.x][move.y] = NONE;
  }

  for (const auto& move : moves) {
      _board[move.x][move.y] = ME;

      if (evaluate() >= 400000000) { 
          _board[move.x][move.y] = NONE;
          updateBoard(move, ME);
          return move;
      }
      _board[move.x][move.y] = NONE;
  }

  for (const auto& move : moves) {
      _board[move.x][move.y] = ME;

      if (evaluate() >= 100000000) { 
          _board[move.x][move.y] = NONE;
          updateBoard(move, ME);
          return move;
      }
      _board[move.x][move.y] = NONE;
  }
  
  if (moves.empty()) {
    for (int x = 0; x < _width; ++x) {
        for (int y = 0; y < _height; ++y) {
            if (_board[x][y] == NONE) {
                Move fallback = {x, y};
                updateBoard(fallback, ME);
                return fallback;
            }
        }
    }
    return {-1, -1};
  }

  Move bestMove = moves[0];
  int bestValue = std::numeric_limits<int>::min();
  int alpha = std::numeric_limits<int>::min();
  int beta = std::numeric_limits<int>::max();

  for (const auto &move : moves) {
    _board[move.x][move.y] = ME;

    int val = -negamax(MAX_DEPTH - 1, -beta, -alpha, OPPONENT);

    _board[move.x][move.y] = NONE;

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
      
      if (_board[x][y] != NONE) continue;

      bool hasNeighbor = false;
      
      for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
          if (dx == 0 && dy == 0) continue;

          if (isOnBoard({x + dx, y + dy}) && _board[x + dx][y + dy] != NONE) {
            hasNeighbor = true;
            break;
          }
        }
        if (hasNeighbor) break;
      }

      if (hasNeighbor) {
        moves.push_back({x, y});
      }
    }
  }

  Move center = {_width / 2, _height / 2};
  std::sort(moves.begin(), moves.end(), [center](const Move& a, const Move& b) {
      int distA = (a.x - center.x)*(a.x - center.x) + (a.y - center.y)*(a.y - center.y);
      int distB = (b.x - center.x)*(b.x - center.x) + (b.y - center.y)*(b.y - center.y);
      return distA < distB;
  });

  return moves;
}

int Gomoku::negamax(int depth, int alpha, int beta, Player player) {
  if (depth == 0) {
    int score = evaluate();
    return (player == ME) ? score : -score;
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

    if (val >= bestValue) {
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
  long long myScore = 0;
  long long opponentScore = 0;

  const int dx[] = {1, 0, 1, 1};
  const int dy[] = {0, 1, 1, -1};

  for (int x = 0; x < _width; x++) {
    for (int y = 0; y < _height; y++) {
      if (_board[x][y] == NONE) continue;

      Player currentPlayer = _board[x][y];

      for (int dir = 0; dir < 4; dir++) {
        int prevX = x - dx[dir];
        int prevY = y - dy[dir];

        if (isOnBoard({prevX, prevY}) && _board[prevX][prevY] == currentPlayer) continue;

        int count = 0;
        int currX = x;
        int currY = y;

        while (isOnBoard({currX, currY}) && _board[currX][currY] == currentPlayer) {
          count++;
          currX += dx[dir];
          currY += dy[dir];
        }

        int openEnds = 0;
        if (isOnBoard({prevX, prevY}) && _board[prevX][prevY] == NONE) openEnds++;
        if (isOnBoard({currX, currY}) && _board[currX][currY] == NONE) openEnds++;

        long long currentScore = 0;

        if (count >= 5) {
          currentScore = 1000000000;
        } else if (count == 4) {
          if (openEnds == 2) currentScore = 500000000;
          else if (openEnds == 1) {
              if (currentPlayer == ME) currentScore = 150000000;
              else currentScore = 100000000;
          }
        } else if (count == 3) {
          if (openEnds == 2) {
              if (currentPlayer == ME) currentScore = 200000000;
              else currentScore = 80000000;
          }
          else if (openEnds == 1) currentScore = 10000;
        } else if (count == 2) {
          if (openEnds == 2) currentScore = 500;
          else if (openEnds == 1) currentScore = 50;
        }

        if (currentPlayer == ME) myScore += currentScore;
        else opponentScore += currentScore;
      }
    }
  }
  
  return (int)(myScore - (opponentScore * 2));
}