NAME = webserv

CFILES = main.cpp Webserv.cpp ParsConfigFile.cpp Server.cpp Location.cpp Client.cpp

HFILES = Webserv.hpp ParsConfigFile.hpp Server.hpp Location.hpp Client.hpp

CC = c++

FLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g3

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

# NAME = webserv

# CFILES = main.cpp 

# PFILES = ParsConfigFile.cpp Server.cpp Location.cpp
# PFILES := $(addprefix parse/, $(PFILES))

# HFILES = ParsConfigFile.hpp Server.hpp Location.hpp
# HFILES := $(addprefix parse/, $(HFILES))

# CC = c++

# FLAGS = -Wall -Wextra -Werror -std=c++98 #-fsanitize=address -g3

# OBJ_DIR = obj

# OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(notdir $(CFILES) ))
# OBJS += $(patsubst parse/%.cpp,$(OBJ_DIR)/%.o,$(PFILES))


# # OBJS = $(addprefix parse/,$(PFILES:%.c=%.o)) $(CFILES:%.c=%.o)

# all: $(OBJ_DIR) $(NAME)

# $(OBJ_DIR)/%.o : %.cpp $(HFILES)
# 	$(CC) $(FLAGS) -c $< -o $@

# $(NAME): $(OBJS) $(HFILES)
# 	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

# $(OBJ_DIR):
# 	mkdir -p $(OBJ_DIR)

# clean:
# 	@echo "Cleaning OBJ files"
# 	@rm -rf $(OBJ_DIR)

# fclean: clean
# 	@echo "Cleaning executable file"
# 	@rm -f $(NAME)

# re: fclean all
