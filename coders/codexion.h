/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 19:02:26 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/02 23:06:03 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	long long		available_at;
	bool			is_taken;
}					t_dongle;

typedef enum s_scheduler
{
	fifo,
	edf,
}					t_scheduler;

typedef enum s_state
{
	compile,
	debug,
	refactor,
}					t_state;

typedef struct s_params
{
	int				nb_coders;
	int				burnout;
	int				compile;
	int				debug;
	int				refactor;
	int				compiles_required;
	int				dongle_cooldown;
	t_scheduler		scheduler;
}					t_params;

typedef struct s_all
{
	t_params		params;
	long long		start_time;
	bool			running;
	pthread_mutex_t	running_mutex;
	pthread_mutex_t	message_mutex;
	t_dongle		*dongles;
}					t_all;

typedef struct s_coder
{
	int				id;
	t_state			state;
	pthread_t		thread;
	long long		time;
	int				nb_activity;
	t_dongle		l_dongle;
	t_dongle		r_dongle;
	t_all			*all;
}					t_coder;

int					parsers(int argc, char **argv, t_all *all);
int					validate_int(char *str);
int					validate_scheduler(char *str);
int					init_dongles(t_all *all);
int					init_all(t_all *all, t_coder *coders);
t_coder				*init_coders(t_all *all, t_coder *coders);
int					destroy_dongles(t_all *all);
int					destroy_all(t_all *all, t_coder *coders);

int					destroy_coders(t_coder *coders);
