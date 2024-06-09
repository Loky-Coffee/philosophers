# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/03 21:43:44 by aalatzas          #+#    #+#              #
#    Updated: 2024/06/08 23:19:12 by aalatzas         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Werror -Wextra -pthread -g -fsanitize=address

################################################################################
####		 				 PHILO_MANDATORY_PART_RULES					   #####
################################################################################

NAME = philo
OBJ_DIR = mandatory/obj/
SRC_DIR = mandatory/

SRC = \
			$(SRC_DIR)main.c \
			$(SRC_DIR)utils.c \
			$(SRC_DIR)philo_tasks.c \
			$(SRC_DIR)handel_fork.c \
			$(SRC_DIR)threads.c \

OBJ = $(addprefix $(OBJ_DIR), $(notdir $(SRC:.c=.o)))

all: $(NAME)
$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)

t1: $(NAME)
	@./$(NAME) 5 20 20 20 2

t2: $(NAME)
	@./$(NAME) 10 10 10 10
t: $(NAME)

re: fclean all

################################################################################
####						 PHILO_BONUS_PART_RULES 					   #####
################################################################################
NAME_BONUS = philo_bonus
OBJ_DIR_BONUS = bonus/obj/
SRC_DIR_BONUS = bonus/

SRC_BONUS = \
			$(SRC_DIR_BONUS)$(NAME_BONUS)_bonus.c \



OBJ_BONUS = $(addprefix $(OBJ_DIR_BONUS), $(notdir $(SRC_BONUS:.c=.o)))

bonus: $(NAME_BONUS)
	@$(CC) $(CFLAGS) -o $(NAME_BONUS) $(OBJ_BONUS) $(LIBFT1)

$(OBJ_DIR_BONUS)%.o: $(SRC_DIR_BONUS)%.c
	@mkdir -p $(OBJ_DIR_BONUS)
	$(CC) $(CFLAGS) -c $< -o $@

clean_bonus:
	@rm -rf $(OBJ_DIR_BONUS)

fclean_bonus: clean_bonus
	@rm -f $(NAME_BONUS)


tb1: $(NAME)

re_bonus: fclean_bonus bonus

testb: all

################################################################################
####							  EXTRA_RULES 							   #####
################################################################################

.PHONY: all clean fclean re start test libft re_libft clean_libft fclean_libft \
		mlx clean_mlx tb t re_bonus all_bonus clean_bonus fclean_bonus \
		re_bonus test_bonus
fcleanall: fclean fclean_bonus
