#include "Commands.hpp"
#include "Macro.h"
#include <cstddef>
#include <iostream>
#include <string>

Commands::Commands() : _running(true) {
}

void Commands::run() {
  std::string input;
  while (_running && std::getline(std::cin, input)) {
    size_t end = input.find_last_not_of(" \n\r\t");
    if (end != std::string::npos) {
      input.erase(end + 1);
    } else {
      input.clear();
    }
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
  int size = std::stoi(args);
  if (_game.start(size) == FAILURE) {
    std::cout << "ERROR message - unsupported size or other error" << std::endl;
    return;
  }
  std::cout << "OK - everything is good" << std::endl;
}

void Commands::handleRectStart(const std::string &args) {
  size_t commaPos = args.find(',');
  if (commaPos == std::string::npos) {
    std::cout << "ERROR message - rectangular board is not supported or other error" << std::endl;
    return;
  }

  int width = std::stoi(args.substr(0, commaPos));
  int height = std::stoi(args.substr(commaPos + 1));

  if (_game.rectStart(width, height) == FAILURE) {
    std::cout << "ERROR message - rectangular board is not supported or other error" << std::endl;
    return;
  }

  std::cout << "OK - parameters are good" << std::endl;
}

void Commands::handleRestart(const std::string &args) {
  (void)args;

  _game.restart();
  std::cout << "OK" << std::endl;
}

void Commands::handleBegin(const std::string &args) {
  (void)args;

  _game.setBegin(true);
  Move move = _game.getBestMove();
  std::cout << move.x << "," << move.y << std::endl;
}

void Commands::handleTurn(const std::string &args) {
  size_t commaPos = args.find(',');
  if (commaPos == std::string::npos) {
    std::cout << "ERROR message - invalid move format" << std::endl;
    return;
  }

  int x = std::stoi(args.substr(0, commaPos));
  int y = std::stoi(args.substr(commaPos + 1));

  _game.updateBoard({x, y}, OPPONENT);
  Move move = _game.getBestMove();
  std::cout << move.x << "," << move.y << std::endl;
}

void Commands::handleBoard(const std::string &args) {
  (void)args;

  std::string line;

  while (_running && std::getline(std::cin, line)) {
    if (line == "DONE") {
      Move move = _game.getBestMove();
      std::cout << move.x << "," << move.y << std::endl;
      break;
    }

    size_t comaPos1 = line.find(',');
    size_t comaPos2 = line.find(',', comaPos1 + 1);

    if (comaPos1 != std::string::npos && comaPos2 != std::string::npos) {
      int x = std::stoi(line.substr(0, comaPos1));
      int y = std::stoi(line.substr(comaPos1 + 1, comaPos2 - comaPos1 - 1));
      int p = std::stoi(line.substr(comaPos2 + 1));

      Player player = (p == 1) ? ME : OPPONENT;
      _game.updateBoard({x, y}, player);
    }
  }
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
