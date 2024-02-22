NAME = webserv

CFILES = main.cpp ParsConfigFile.cpp Server.cpp Location.cpp

HFILES = ParsConfigFile.hpp Server.hpp Location.hpp

CC = c++

FLAGS = -Wall -Wextra -Werror -std=c++98 #-fsanitize=address -g3

OBJS = $(CFILES:.cpp=.o)

all: $(NAME)

$(OBJS) : $(CFILES)
	$(CC) $(FLAGS) -c $(CFILES)

$(NAME): $(OBJS) $(HFILES)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

clean:
	@echo "cleaning OBJS files"
	@rm -f $(OBJS)

fclean: clean
	@echo "cleaning btc file"
	@rm -f $(NAME)

re: fclean all