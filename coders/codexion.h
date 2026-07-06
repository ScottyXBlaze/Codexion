/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 19:02:26 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/06 11:45:35 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_coder	t_coder;

typedef struct s_scheduler_data
{
	t_coder			**queue;
	int				head;
	int				tail;
	int				size;
	pthread_mutex_t	mutex;
}	t_scheduler_data;


typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	long int		available_at;
}	t_dongle;
typedef enum s_scheduler
{
	fifo,
	edf,
}						t_scheduler;

typedef enum s_state
{
	compile,
	debug,
	refactor,
	take_dongle,
	burned_out,
}						t_state;

typedef struct s_params
{
	int					nb_coders;
	int					burnout;
	int					compile;
	int					debug;
	int					refactor;
	int					compiles_required;
	int					dongle_cooldown;
	t_scheduler			scheduler;
}						t_params;

typedef struct s_all
{
	t_params			params;
	long long			start_time;
	bool				running;
	pthread_mutex_t		running_mutex;
	pthread_mutex_t		message_mutex;
	pthread_t			monitor;

	t_dongle			*dongles;
	t_coder				*coders;

	t_scheduler_data	scheduler;
}						t_all;

typedef struct s_coder
{
	int					id;
	pthread_t			thread;
	pthread_mutex_t		mutex;
	long long			last_compile;

	bool				is_finished;
	long long			queue_id;
	int					compile_count;
	t_dongle			*l_dongle;
	t_dongle			*r_dongle;
	t_all				*all;
}						t_coder;

int						parsers(int argc, char **argv, t_all *all);
int						validate_int(char *str);
int						validate_scheduler(char *str);
int						init_dongles(t_all *all);
int						init_all(t_all *all, t_coder *coders);
t_coder					*init_coders(t_all *all, t_coder *coders);
int						destroy_dongles(t_all *all);
int						destroy_all(t_all *all, t_coder *coders);
int						destroy_coders(t_coder *coders);
long int				get_time(t_all *all);
void					ft_sleep(long long sleep_time, t_all *all);
int						can_take_dongle(t_all *all, t_dongle *dongle);
void					release_dongle(t_all *all, t_dongle *dongle);
void	lock_dongle(t_all *all, t_dongle *dongle);
void					release_dongle(t_all *all, t_dongle *dongle);


bool	is_running(t_all *all);

void	stop_simulation(t_all *all);
void	*monitor_loop(void *args);
void	print_state(t_coder *coder, t_state state);

void	unlock_dongle(t_all *all, t_dongle *dongle);
