NAME				= app

OBJS_DIR			= ./objs

CXX_HEADERS			= .

CXX_FILES			= main.cpp cmd/Command.cpp cmd/CommandHandler.cpp logger/Logger.cpp \
						core/Server.cpp core/Socket.cpp core/Client.cpp core/Channel.cpp \
						cmd/PasswordCommand.cpp cmd/NickCommand.cpp cmd/UserCommand.cpp \
						cmd/PrivMsgCommand.cpp cmd/NoticeCommand.cpp cmd/QuitCommand.cpp \
						cmd/JoinCommand.cpp cmd/PartCommand.cpp cmd/TopicCommand.cpp \
						cmd/KickCommand.cpp #cmd/ModeCommand.cpp
#						cmd/PingCommand.cpp cmd/PongCommand.cpp cmd/NamesCommand.cpp

CXX_DIRS			= $(dir $(CXX_FILES))
vpath %.cpp $(CXX_DIRS)

SRCS				= $(CXX_FILES)
OBJS				= $(addprefix $(OBJS_DIR)/, $(notdir $(CXX_FILES:.cpp=.o)))
DEPENDENCIES		= $(OBJS:.o=.d)

CC					= clang++
CFLAGS				= -std=c++98 -O3 -I$(CXX_HEADERS)
#CFLAGS				= -std=c++98 -O3 -Wall -Werror -Wextra -I$(CXX_HEADERS)
LINKER				= $(CFLAGS)

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
	$(CC) $(LINKER) -o $(NAME) $(OBJS)

all : $(NAME)

bonus : $(NAME)

clean :
	$(RM) $(RMFLAGS) $(OBJS) $(DEPENDENCIES)

fclean : clean
	$(RM) $(RMFLAGS) $(NAME) $(OBJS_DIR)

norm :
	norminette $(SRCS_DIR) $(HDRS_DIR)

val : $(NAME)
	valgrind --leak-check=full \
		--show-leak-kinds=all \
		--track-origins=yes \
		--verbose \
		--log-file=valgrind-out.txt \
		./$(NAME)

re: fclean all
