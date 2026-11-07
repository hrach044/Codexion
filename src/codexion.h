/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnatsak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/07 16:13:57 by hmnatsak          #+#    #+#             */
/*   Updated: 2026/11/07 18:41:38 by hmnatsak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CODEXION_H
# define CODEXION_H

typedef struct s_config
{
    int     number_of_coders;
    int     time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refectore;
	int		number_of_compiles;
	int		number_of_compiles_required;
	int		dongle_cooldown;
	int		shceduler;
}   t_config;

int     is_valid_number(char *s);
int 	parse_scheduler(char *s);
int check_args(int argc, char **argv);

#endif
