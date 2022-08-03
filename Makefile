NAME				= app

OBJS_DIR			= ./objs

CXX_HEADERS			= .

CXX_FILES			= main.cpp cmd/Command.cpp cmd/CommandHandler.cpp logger/Logger.cpp core/Utils.cpp\
						core/Server.cpp core/Socket.cpp core/Client.cpp core/Channel.cpp \
						cmd/PassCmd.cpp cmd/NickCmd.cpp cmd/UserCmd.cpp \
						cmd/PrivMsgCmd.cpp cmd/NoticeCmd.cpp cmd/QuitCmd.cpp \
						cmd/JoinCmd.cpp cmd/PartCmd.cpp cmd/TopicCmd.cpp \
						cmd/KickCmd.cpp cmd/ModeCmd.cpp cmd/OperCmd.cpp \
						cmd/KillCmd.cpp cmd/ListCmd.cpp cmd/MotdCmd.cpp \
						cmd/PingCmd.cpp cmd/PongCmd.cpp #cmd/NamesCommand.cpp

CXX_DIRS			= $(dir $(CXX_FILES))
vpath %.cpp $(CXX_DIRS)

SRCS				= $(CXX_FILES)
OBJS				= $(addprefix $(OBJS_DIR)/, $(notdir $(CXX_FILES:.cpp=.o)))
DEPENDENCIES		= $(OBJS:.o=.d)

CC					= clang++
CFLAGS				= -std=c++98 -O3 -Wall -Werror -Wextra -I$(CXX_HEADERS) #-g -fsanitize=address
#CFLAGS				= -std=c++98 -O3 -Wall -Werror -Wextra -I$(CXX_HEADERS)

AR					= ar
ARFLAGS				= rcs

RM					= rm
RMFLAGS				= -rf

MKDIR				= mkdir
MKDIRFLAGS			= -p

.PHONY : all clean fclean re bonus norm
.DEFAULT_GOAL := all

-include $(DEPENDENCIES)

$(OBJS_DIR)/%.o : %.cpp Makefile
	$(CC) $(CFLAGS) -c $< -o $@ -MD

$(OBJS_DIR) :
	$(MKDIR) $(MKDIRFLAGS) $(OBJS_DIR)

$(NAME) : $(SRCS) $(OBJS_DIR) $(OBJS) Makefile
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

all : $(NAME)

bonus : $(NAME)

clean :
	$(RM) $(RMFLAGS) $(OBJS) $(DEPENDENCIES)

fclean : clean
	$(RM) $(RMFLAGS) $(NAME) $(OBJS_DIR)

val : $(NAME)
	valgrind --leak-check=full \
		--show-leak-kinds=all \
		--track-origins=yes \
		--verbose \
		--log-file=valgrind-out.txt \
		./$(NAME)

re: fclean all
