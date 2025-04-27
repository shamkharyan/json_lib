NAME = json_parser

CPP = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++17

SRCS_PATH = srcs/
INCS_PATH = includes/

SRCS = \
	$(SRCS_PATH)value.cpp \
	$(SRCS_PATH)main.cpp \
	$(SRCS_PATH)parsing.cpp \
	
OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CPP) -I$(INCS_PATH) $(CPPFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CPP) -I$(INCS_PATH) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re