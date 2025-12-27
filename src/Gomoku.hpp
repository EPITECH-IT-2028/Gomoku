#pragma once

#include "Macro.h"
#include <vector>

class Gomoku {
  public:
    Gomoku()
        : _width(0),
          _height(0),
          _begin(false),
          _initialized(false),
          _timeoutTurn(0),
          _timeoutMatch(0),
          _maxMemory(0),
          _timeLeft(0) {
    }

    ~Gomoku() = default;

    int start(int size);
    int rectStart(int width, int height);
    void restart();

    void initializeBoard();
    void updateBoard(const Move &move, Player player);

    bool isValid(const Move &move) const;
    bool isOnBoard(const Move &move) const;

    Move getBestMove();
    int evaluate();
    int negamax(int depth, int alpha, int beta, Player player);
    std::vector<Move> generateMoves();

    void setWidth(int width) {
      _width = width;
    }

    void setHeight(int height) {
      _height = height;
    }

    void setInitialized(bool initialized) {
      _initialized = initialized;
    }

    bool isInitialized() const {
      return _initialized;
    }

    int getWidth() const {
      return _width;
    }

    int getHeight() const {
      return _height;
    }

    void setTimeoutTurn(int timeoutTurn) {
      _timeoutTurn = timeoutTurn;
    }

    void setTimeoutMatch(int timeoutMatch) {
      _timeoutMatch = timeoutMatch;
    }

    void setMaxMemory(int maxMemory) {
      _maxMemory = maxMemory;
    }

    void setTimeLeft(int timeLeft) {
      _timeLeft = timeLeft;
    }

    int getTimeoutTurn() const {
      return _timeoutTurn;
    }

    int getTimeoutMatch() const {
      return _timeoutMatch;
    }

    int getMaxMemory() const {
      return _maxMemory;
    }

    int getTimeLeft() const {
      return _timeLeft;
    }

    void setBegin(bool begin) {
      _begin = begin;
    }

  private:
    std::vector<std::vector<Player>> _board;

    int _width;
    int _height;
    bool _begin;
    bool _initialized;
    int _timeoutTurn;
    int _timeoutMatch;
    int _maxMemory;
    int _timeLeft;
};
