CC = c++
INC = inc/ 

FLAGS= -g -std=c++98 -Wall -Wextra -Werror 
MAIN= webserver.cpp
B_DIR = build
SERVER=$(addprefix server/, )
PARSING=$(addprefix parsing/, prsn_cnfig_file.cpp prsn_req_res.cpp tools_for_parsing.cpp)
HEADER = $(addprefix inc/, headers.hpp  Location.hpp ServerCongif.hpp Server.hpp parsing.hpp mesage.hpp client.hpp methods.hpp)
REC_RES=$(addprefix request_response/, utils.cpp request.cpp Library.cpp)
CGI=$(addprefix cgi/, cgi.cpp)
COOKIES=$(addprefix cookies/, cookies.cpp)
FILES= $(addprefix src/, $(SERVER) $(PARSING) $(REC_RES) $(CGI) $(COOKIES))
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