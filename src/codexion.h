/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrach <hrach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/07 16:13:57 by hmnatsak          #+#    #+#             */
/*   Updated: 2026/11/15 20:47:18 by hrach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_config
{
	int		number_of_coders;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		number_of_compiles_required;
	int		dongle_cooldown;
	int		scheduler;
}	t_config;

typedef struct s_req
{
	long	key;
	int		id;
}	t_req;

typedef struct s_heap
{
	t_req	*data;
	int		size;
}	t_heap;

typedef struct s_dongle
{
	t_heap			queue;
	int				in_use;
	long			free_at;
}	t_dongle;

typedef struct s_sim
{
	t_config		cfg;
	int				stop;
	long			start_time;
	pthread_mutex_t	log_mutex;
	t_dongle		*dongles;
	struct s_coder	*coders;
	pthread_t		*threads;
	int				dongles_ready;
	pthread_mutex_t	sched_mutex;
	pthread_cond_t	sched_cond;
	long			seq;
}	t_sim;

typedef struct s_coder
{
	int			id;
	t_sim		*sim;
	t_dongle	*left;
	t_dongle	*right;
	long		last_start;
	int			compiles;
}	t_coder;

int		is_valid_number(char *s);
int		parse_scheduler(char *s);
int		check_args(int argc, char **argv);
void	fill_config(t_config *cfg, char **argv);
long	get_time(void);
void	print_log(t_sim *sim, int id, char *msg);
void	*coder_routine(void *arg);
void	msleep(long ms);
int		init_dongle(t_dongle *d);
int		acquire_dongles(t_coder *c);
void	release_dongles(t_coder *c);
int		init_sim(t_sim *sim);
void	destroy_sim(t_sim *sim);
int		heap_init(t_heap *h, int capacity);
int		req_less(t_req a, t_req b);
void	heap_push(t_heap *h, t_req r);
t_req	heap_pop(t_heap *h);
int		is_stopped(t_sim *sim);
void	*monitor_routine(void *arg);
void	mark_start(t_coder *c);
void	count_compile(t_coder *c);

#endif
