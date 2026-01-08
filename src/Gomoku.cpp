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

std::vector<Move> Gomoku::generateMoves() {

  struct ScoredMove {
      Move move;
      int score;
  };

  std::vector<ScoredMove> scoredMoves;

  for (int x = 0; x < _width; x++) {
    for (int y = 0; y < _height; y++) {
      if (_board[x][y] != NONE) continue;

      int score = 0;
      bool hasNeighbor = false;

      for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
          if (dx == 0 && dy == 0) continue;
          
          if (isOnBoard({x + dx, y + dy})) {
              Player p = _board[x + dx][y + dy];
              if (p != NONE) {
                  hasNeighbor = true;

                  score += 10; 

                  if (isOnBoard({x - dx, y - dy}) && _board[x - dx][y - dy] == p) {
                      score += 50;
                  }
              }
          }
        }
      }

      if (hasNeighbor) {
        scoredMoves.push_back({{x, y}, score});
      }
    }
  }

  for (size_t i = 0; i < scoredMoves.size(); ++i) {
      for (size_t j = 0; j < scoredMoves.size() - 1 - i; ++j) {
          if (scoredMoves[j].score < scoredMoves[j + 1].score) {
              std::swap(scoredMoves[j], scoredMoves[j + 1]);
          }
      }
  }

  std::vector<Move> moves;

  for (const auto& sm : scoredMoves) {
      moves.push_back(sm.move);
  }

  return moves;
}

long long Gomoku::negamax(int depth, long long alpha, long long beta, Player player) {
  if (depth == 0) {
    long long score = evaluate();
    return (player == ME) ? score : -score;
  }

  std::vector<Move> moves = generateMoves();
  if (moves.empty())
    return (player == ME) ? evaluate() : -evaluate();

  long long bestValue = std::numeric_limits<long long>::min();

  for (const auto &move : moves) {
    _board[move.x][move.y] = player;

    long long val =
        -negamax(depth - 1, -beta, -alpha, (player == ME) ? OPPONENT : ME);

    _board[move.x][move.y] = NONE;

    if (val > 900000000) val -= 1;
    else if (val < -900000000) val += 1;

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

long long Gomoku::evaluate() {
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
        int holes = 0;
        int currX = x;
        int currY = y;
        
        while (isOnBoard({currX, currY})) {
            if (_board[currX][currY] == currentPlayer) {
                count++;
            } else if (_board[currX][currY] == NONE) {
                if (holes == 0) {
                    int nextX = currX + dx[dir];
                    int nextY = currY + dy[dir];
                    if (isOnBoard({nextX, nextY}) && _board[nextX][nextY] == currentPlayer) {
                        holes++;
                    } else {
                        break;
                    }
                } else {
                    break;
                }
            } else {
                break;
            }
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
             if (openEnds >= 1) {
                  if (currentPlayer == ME) currentScore = 200000000;
                  else currentScore = 950000000;
             }
        } else if (count == 3) {
             if (openEnds >= 1) {
                  if (currentPlayer == ME) currentScore = 50000000;
                  else currentScore = 500000000; 
             }
        } else if (count == 2) {
             if (openEnds == 2) {
                  currentScore = 600000;
                  if (currentPlayer != ME) currentScore = 400000; 
             }
             else if (openEnds == 1) currentScore = 1000;
        }

        if (currentPlayer == ME) myScore += currentScore;
        else opponentScore += currentScore;
      }
    }
  }

  return (myScore - (opponentScore * 1.5));
}

int Gomoku::getLineStrength(int x, int y, Player p) {
  int maxStrength = 0;
  const int dx[] = {1, 0, 1, 1};
  const int dy[] = {0, 1, 1, -1};

  for (int dir = 0; dir < 4; dir++) {
    int count = 1;
    int holes = 0; 

    int cx = x + dx[dir];
    int cy = y + dy[dir];
    while (cx >= 0 && cx < _width && cy >= 0 && cy < _height) {
        if (_board[cx][cy] == p)
        count++;
        else if (_board[cx][cy] == NONE) {
            if (holes == 0) {
                int nx = cx + dx[dir];
                int ny = cy + dy[dir];
                if (nx >= 0 && nx < _width && ny >= 0 && ny < _height && _board[nx][ny] == p)
                    holes++;
                else
                    break;
            } else
                break;
        } else
            break;
        cx += dx[dir];
        cy += dy[dir];
    }

    int bx = x - dx[dir];
    int by = y - dy[dir];
    while (bx >= 0 && bx < _width && by >= 0 && by < _height) {
        if (_board[bx][by] == p) count++;
        else if (_board[bx][by] == NONE) {
            if (holes == 0) {
                int nx = bx - dx[dir];
                int ny = by - dy[dir];
                if (nx >= 0 && nx < _width && ny >= 0 && ny < _height && _board[nx][ny] == p)
                    holes++;
                else
                    break;
            } else
                break;
        } else
            break;
        bx -= dx[dir];
        by -= dy[dir];
    }
    
    int openEnds = 0;
    if (cx >= 0 && cx < _width && cy >= 0 && cy < _height && _board[cx][cy] == NONE) openEnds++;
    if (bx >= 0 && bx < _width && by >= 0 && by < _height && _board[bx][by] == NONE) openEnds++;

    if (count >= 5) return 100;
    
    if (count == 4) {
      if (openEnds >= 2) { 
         if (maxStrength < 95) maxStrength = 95; 
      } else if (openEnds >= 1) { 
         if (maxStrength < 90) maxStrength = 90; 
      } else {
         if (maxStrength < 10) maxStrength = 10;
      }
    }
  }
  return maxStrength;
}

Move Gomoku::getBestMove() {
  std::vector<Move> moves;

  if (_begin) {
    _begin = false;
    if (isValid({_width / 2, _height / 2})) {
        updateBoard({_width / 2, _height / 2}, ME);
        return {_width / 2, _height / 2};
    }
  }

  moves = generateMoves();
  if (moves.empty()) return {-1, -1};

  for (const auto& move : moves) {
    _board[move.x][move.y] = ME;
    if (getLineStrength(move.x, move.y, ME) >= 100) {
        _board[move.x][move.y] = NONE;
        updateBoard(move, ME);
        return move;
    }
    _board[move.x][move.y] = NONE;
  }

  for (const auto& move : moves) {
    _board[move.x][move.y] = OPPONENT;
    if (getLineStrength(move.x, move.y, OPPONENT) >= 100) {
        _board[move.x][move.y] = NONE;
        updateBoard(move, ME);
        return move;
    }
    _board[move.x][move.y] = NONE;
  }

  for (const auto& move : moves) {
    _board[move.x][move.y] = ME;
    if (getLineStrength(move.x, move.y, ME) >= 95) {
        _board[move.x][move.y] = NONE;
        updateBoard(move, ME);
        return move;
    }
    _board[move.x][move.y] = NONE;
  }

  for (const auto& move : moves) {
    _board[move.x][move.y] = OPPONENT;
    if (getLineStrength(move.x, move.y, OPPONENT) >= 95) {
        _board[move.x][move.y] = NONE;
        updateBoard(move, ME);
        return move;
    }
    _board[move.x][move.y] = NONE;
  }

  for (const auto& move : moves) {
    _board[move.x][move.y] = ME;
    if (getLineStrength(move.x, move.y, ME) >= 90) {
        _board[move.x][move.y] = NONE;
        updateBoard(move, ME);
        return move;
    }
    _board[move.x][move.y] = NONE;
  }

  for (const auto& move : moves) {
    _board[move.x][move.y] = OPPONENT;
    if (getLineStrength(move.x, move.y, OPPONENT) >= 90) {
        _board[move.x][move.y] = NONE;
        updateBoard(move, ME);
        return move;
    }
    _board[move.x][move.y] = NONE;
  }
  
  Move bestMove = moves[0];
  long long bestValue = std::numeric_limits<long long>::min();
  long long alpha = std::numeric_limits<long long>::min();
  long long beta = std::numeric_limits<long long>::max();

  for (const auto &move : moves) {
    _board[move.x][move.y] = ME;
    long long val = -negamax(MAX_DEPTH - 1, -beta, -alpha, OPPONENT);
    _board[move.x][move.y] = NONE;

    if (val > 900000000) val -= 1;
    else if (val < -900000000) val += 1;

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
