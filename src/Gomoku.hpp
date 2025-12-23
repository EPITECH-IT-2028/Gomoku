#pragma once

class Gomoku {
  public:
    Gomoku()
        : _width(0),
          _height(0),
          _initialized(false),
          _timeoutTurn(0),
          _timeoutMatch(0),
          _maxMemory(0),
          _timeLeft(0) {
    }

    ~Gomoku() = default;

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

  private:
    int _width;
    int _height;
    bool _initialized;
    int _timeoutTurn;
    int _timeoutMatch;
    int _maxMemory;
    int _timeLeft;
};
