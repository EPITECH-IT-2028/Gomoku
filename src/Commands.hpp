#pragma once

#include <string>
#include "Gomoku.hpp"

class Commands {
  public:
    Commands();
    ~Commands() = default;

    void run();

  private:
    void handleCommand(const std::string &command);

    void handleAbout(const std::string &args);
    void handleStart(const std::string &args);
    void handleRectStart(const std::string &args);
    void handleRestart(const std::string &args);
    void handleBegin(const std::string &args);
    void handleTurn(const std::string &args);
    void handleBoard(const std::string &args);
    void handleInfo(const std::string &args);
    void handleEnd(const std::string &args);

    std::pair<std::string, std::string> parseCommand(const std::string &input);

    Gomoku _game;
    bool _running;
};
