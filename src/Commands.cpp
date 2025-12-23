#include "Commands.hpp"
#include <iostream>

Commands::Commands() : _running(true) {
}

void Commands::run() {
  std::string input;
  while (_running && std::getline(std::cin, input)) {
    input.erase(input.find_last_not_of(" \n\r\t") + 1);
    if (!input.empty()) {
      handleCommand(input);
    }
  }
}

std::pair<std::string, std::string> Commands::parseCommand(
    const std::string &input) {
  size_t spacePos = input.find(' ');

  if (spacePos == std::string::npos) {
    return {input, ""};
  } else {
    return {input.substr(0, spacePos), input.substr(spacePos + 1)};
  }
}

void Commands::handleCommand(const std::string &line) {
  auto [cmd, args] = parseCommand(line);

  if (cmd == "START") {
    handleStart(args);
  } else if (cmd == "RECTSTART") {
    handleRectStart(args);
  } else if (cmd == "RESTART") {
    handleRestart(args);
  } else if (cmd == "BEGIN") {
    handleBegin(args);
  } else if (cmd == "TURN") {
    handleTurn(args);
  } else if (cmd == "BOARD") {
    handleBoard(args);
  } else if (cmd == "INFO") {
    handleInfo(args);
  } else if (cmd == "ABOUT") {
    handleAbout(args);
  } else if (cmd == "END") {
    handleEnd(args);
  } else {
    std::cout << "UNKNOWN" << std::endl;
  }
}

void Commands::handleAbout(const std::string &args) {
  (void)args;
  std::cout << "name=\"pbrain-gomoku-ai\", version=\"1.0\", "
               "author=\"NolTienne\", country=\"France\""
            << std::endl;
}

void Commands::handleStart(const std::string &args) {
  (void)args;
}

void Commands::handleRectStart(const std::string &args) {
  (void)args;
}

void Commands::handleRestart(const std::string &args) {
  (void)args;
}

void Commands::handleBegin(const std::string &args) {
  (void)args;
}

void Commands::handleTurn(const std::string &args) {
  (void)args;
}

void Commands::handleBoard(const std::string &args) {
  (void)args;
}

void Commands::handleInfo(const std::string &args) {
  auto [key, value] = parseCommand(args);

  if (key == "timeout_turn") {
    _game.setTimeoutTurn(std::stoi(value));
  } else if (key == "timeout_match") {
    _game.setTimeoutMatch(std::stoi(value));
  } else if (key == "max_memory") {
    _game.setMaxMemory(std::stoi(value));
  } else if (key == "time_left") {
    _game.setTimeLeft(std::stoi(value));
  }
}

void Commands::handleEnd(const std::string &args) {
  (void)args;
  _running = false;
}
