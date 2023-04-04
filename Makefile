CC = c++
INC = inc/

FLAGS= -std=c++98 -Wall -Wextra -Werror -g
MAIN= webserver.cpp
B_DIR = build
SERVER=$(addprefix server/, )
HEADER = $(addprefix inc/, headers.hpp  Location.hpp ServerCongif.hpp Server.hpp)
FILES= $(addprefix src/, $(SERVER))
OBJ= $(addprefix build/, $(FILES:.cpp=.o) $(MAIN:.cpp=.o))
NAME= webserv

all:$(NAME)

$(NAME) : $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -I $(INC) -o $(NAME) 

$(B_DIR)/%.o: %.cpp $(HEADER)
	@mkdir -p $(dir $@)
	$(CC) $(FLAGS) -I $(INC) -c $< -o $@
clean:
	rm -rf $(B_DIR)

fclean: clean
	rm  -f $(NAME)

re: fclean all