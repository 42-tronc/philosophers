# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: croy <croy@student.42lyon.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/10 13:33:54 by croy              #+#    #+#              #
#    Updated: 2023/07/31 14:20:47 by croy             ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

# --------- GLOBAL VARIABLES ----------
SHELL := bash
.SHELLFLAGS := -eu -o pipefail -c # strict bash mode
MAKEFLAGS += --warn-undefined-variables # warn about Make variables that don’t exist
MAKEFLAGS += --no-print-directory # don’t print the directory name before executing a recipe
.DELETE_ON_ERROR:


# ------------ FORMATTING -------------
# see https://misc.flogisoft.com/bash/tip_colors_and_formatting
FG_BLACK 			:= \033[30m
FG_RED 				:= \033[31m
FG_GREEN 			:= \033[32m
FG_YELLOW 			:= \033[33m
FG_BLUE 			:= \033[34m
FG_MAGENTA 			:= \033[35m
FG_CYAN 			:= \033[36m
FG_LIGHT_GRAY 		:= \033[37m
FG_DEFAULT 			:= \033[39m
FG_DARK_GRAY 		:= \033[90m
FG_LIGHT_RED 		:= \033[91m
FG_LIGHT_GREEN 		:= \033[92m
FG_LIGHT_YELLOW 	:= \033[93m
FG_LIGHT_BLUE 		:= \033[94m
FG_LIGHT_MAGENTA 	:= \033[95m
FG_LIGHT_CYAN 		:= \033[96m
FG_WHITE 			:= \033[97m

BG_BLACK 			:= \033[40m
BG_RED 				:= \033[41m
BG_GREEN 			:= \033[42m
BG_YELLOW 			:= \033[43m
BG_BLUE 			:= \033[44m
BG_MAGENTA 			:= \033[45m
BG_CYAN 			:= \033[46m
BG_LIGHT_GRAY 		:= \033[47m
BG_DEFAULT 			:= \033[49m
BG_DARK_GRAY 		:= \033[100m
BG_LIGHT_RED 		:= \033[101m
BG_LIGHT_GREEN 		:= \033[102m
BG_LIGHT_YELLOW 	:= \033[103m
BG_LIGHT_BLUE 		:= \033[104m
BG_LIGHT_MAGENTA 	:= \033[105m
BG_LIGHT_CYAN 		:= \033[106m
BG_WHITE 			:= \033[107m

BOLD		:= \033[1m
DIM			:= \033[2m
ITALIC		:= \033[3m
UNDERLINE	:= \033[4m
BLINK		:= \033[5m
REVERSE		:= \033[7m
HIDDEN		:= \033[8m
STRIKED		:= \033[9m
RESET		:= \033[0m


# ---------- BASIC VARIABLES ----------
SRC_FOLDER := src/
OBJ_FOLDER := .obj/
HDR_FOLDER := header/

SRC = $(addprefix $(SRC_FOLDER), $(SRC_FILES))
OBJ = $(subst $(SRC_FOLDER),$(OBJ_FOLDER),$(SRC:.c=.o))

CFLAGS := -Wall -Wextra -Werror -pthread -I./$(HDR_FOLDER)
FSANITIZE := -fsanitize=thread -g3
RM := rm -rf


# --------- PROJECT VARIABLES ---------
NAME := philo
HEADER := $(HDR_FOLDER)/philo.h
SRC_FILES := free.c init.c main.c simulation.c utils.c

ARG ?= 10 500 200 200 10
# DELETE ME


# -------------- RECIPES --------------
all: $(NAME)

$(NAME): $(OBJ)
	${CC} ${CFLAGS} -o $(NAME) $(OBJ)
	@echo -e "$(BG_LIGHT_GREEN)Compiled:\t$(RESET) $(FG_WHITE)$(UNDERLINE)$(NAME)$(RESET) has been created."

$(OBJ_FOLDER)%.o : $(SRC_FOLDER)%.c $(HEADER) | $(OBJ_FOLDER)
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ_FOLDER) :
	@mkdir -p $(OBJ_FOLDER)

clean:
	@$(RM) $(OBJ)
	@$(RM) $(OBJ_FOLDER)
	@echo -e "$(FG_RED)Clean:\t\t $(FG_LIGHT_GRAY)$(ITALIC)$(NAME) object files$(RESET) have been deleted"

fclean: clean
	@$(RM) $(NAME)
	@echo -e "$(FG_RED)FClean:\t\t $(FG_LIGHT_GRAY)$(UNDERLINE)$(NAME)$(RESET) has been deleted."

re: fclean
	@$(MAKE) all

debug: CFLAGS += $(FSANITIZE)
debug: fclean $(NAME)

run : all
	./$(NAME) $(ARG)

valgrind: all
	@valgrind --tool=helgrind ./$(NAME) $(ARG)
	@valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./$(NAME) $(ARG) 2>&1 | \
		tee valgrind_output.tmp | \
		grep -q -E "definitely lost: [1-9]|indirectly lost: [1-9]|possibly lost: [1-9]|still reachable: [1-9]" \
		&& (echo -e "\nLEAKS ❌\n" && cat valgrind_output.tmp) \
		|| echo -e "\nOK ✅"
	@rm -f valgrind_output.tmp

.PHONY: all clean fclean re debug run valgrind
