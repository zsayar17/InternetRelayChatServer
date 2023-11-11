#c++ main.cpp Channel/*.cpp  Server/*.cpp Socket/*.cpp User/*.cpp Utils/*.cpp Command/*.cpp -std=c++98 -Wall -Werror -Wextra

COMPILER = c++
FLAGS = -std=c++98 -Wall -Werror -Wextra

SRC_DIR = .
OBJ_DIR = obj

SRCS := $(shell find $(SRC_DIR) -type f -name "*.cpp")

OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

all: $(OBJS)
	$(COMPILER) $(OBJS) $(FLAGS) -o ircserv


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp

	@mkdir -p $(@D)
	$(COMPILER) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f ircserv

re: fclean
	$(MAKE)

.PHONY: clean fclean
