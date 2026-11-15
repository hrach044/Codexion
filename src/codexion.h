/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnatsak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/07 16:13:57 by hmnatsak          #+#    #+#             */
/*   Updated: 2026/11/07 20:00:03 by hmnatsak         ###   ########.fr       */
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
    int     number_of_coders;
    int     time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		number_of_compiles_required;
	int		dongle_cooldown;
	int		scheduler;
}   t_config;


typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	int				in_use;
	long			free_at;
}	t_dongle;

typedef struct s_sim
{
    t_config        cfg;
    long            start_time;
    pthread_mutex_t log_mutex;
	t_dongle		*dongles;
	struct s_coder	*coders;
	pthread_t		*threads;
}   t_sim;

typedef struct s_coder
{
	int			id;
	t_sim		*sim;
	t_dongle	*left;
	t_dongle	*right;
}	t_coder;

typedef struct  s_req
{
	long	key;
	int		id;
}	t_req;

typedef struct s_heap
{
	t_req	*data;
	int		size;
}	t_heap;


int     is_valid_number(char *s);
int 	parse_scheduler(char *s);
int 	check_args(int argc, char **argv);
void	fill_config(t_config *cfg, char **argv);
long	get_time(void);
void	print_log(t_sim *sim, int id, char *msg);
void	*coder_routine(void *arg);
void	msleep(long ms);
void	init_dongle(t_dongle *d);
int		try_take_dongle(t_dongle *d);
void	release_dongle(t_dongle *d, long cooldown);
int		init_sim(t_sim *sim);
void	destroy_sim(t_sim *sim);
int		heap_init(t_heap *h, int capacity);
int		req_less(t_req a, t_req b);
void	heap_push(t_heap *h, t_req r);

#endif
