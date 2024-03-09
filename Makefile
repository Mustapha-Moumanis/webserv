NAME = webserv

FILES = main.cpp

PFILES = ParsConfigFile.cpp Server.cpp Location.cpp
PHFILES = ParsConfigFile.hpp Server.hpp Location.hpp

MFILES = Webserv.cpp Client.cpp Request.cpp Utils.cpp
MHFILES = Webserv.hpp Client.hpp Request.hpp Utils.hpp

PFILES := $(addprefix parse/, $(PFILES))
PHFILES := $(addprefix parse/, $(PHFILES))

MFILES := $(addprefix mutiplixing/, $(MFILES))
MHFILES := $(addprefix mutiplixing/, $(MHFILES))

CC = c++

FLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g3

OBJS = $(FILES:.cpp=.o) $(PFILES:.cpp=.o) $(MFILES:.cpp=.o)

all: $(NAME)

%.o: %.cpp
	$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(OBJS) $(PHFILES) $(MHFILES)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

clean:
	@echo "Cleaning object files"
	@rm -f $(OBJS)

fclean: clean
	@echo "Cleaning executable file"
	@rm -f $(NAME)

re: fclean all
