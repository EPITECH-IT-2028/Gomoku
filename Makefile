NAME = pbrain-gomoku-ai

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20

SRC += src/main.cpp \
		src/Gomoku.cpp \
		src/Commands.cpp \

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
