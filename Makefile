NAME = webserv

FILES = main.cpp

IFILES = Utils.cpp MimeTypes.cpp
IHFILES = Utils.hpp MimeTypes.hpp

PFILES = ParsConfigFile.cpp Server.cpp Location.cpp
PHFILES = ParsConfigFile.hpp Server.hpp Location.hpp

MFILES = Webserv.cpp Client.cpp Request.cpp Get.cpp Post.cpp Delete.cpp Cgi.cpp 
MHFILES = Webserv.hpp Client.hpp Request.hpp StatusCodeExcept.hpp rediractionExcept.hpp responseGetExcept.hpp

IFILES := $(addprefix include/, $(IFILES))
IHFILES := $(addprefix include/, $(IHFILES))

PFILES := $(addprefix parse/, $(PFILES))
PHFILES := $(addprefix parse/, $(PHFILES))

MFILES := $(addprefix mutiplixing/, $(MFILES))
MHFILES := $(addprefix mutiplixing/, $(MHFILES))

CC = c++

FLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g3

OBJS = $(FILES:.cpp=.o) $(IFILES:.cpp=.o) $(PFILES:.cpp=.o) $(MFILES:.cpp=.o)

all: $(NAME)

%.o: %.cpp
	$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(OBJS) $(FILES) $(PFILES) $(MFILES) $(IFILES) $(PHFILES) $(MHFILES) $(IHFILES)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

clean:
	@echo "Cleaning object files"
	@rm -f $(OBJS)

fclean: clean
	@echo "Cleaning executable file"
	@rm -f $(NAME)

re: fclean all
