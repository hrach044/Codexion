/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnatsak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/07 16:15:20 by hmnatsak          #+#    #+#             */
/*   Updated: 2026/11/07 19:58:35 by hmnatsak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


int     is_valid_number(char *s)
{
    int i;

    i = 0;
    if (!s[0])
        return (0);
    while (s[i])
    {
        if (s[i] < '0' || s[i] > '9')
            return (0);
        i++;
    }
    return (1);
}

int 	parse_scheduler(char *s)
{
    if (strcmp(s, "fifo") == 0)
        return (0);
	else if (strcmp(s, "edf") == 0)
		return (1);
	return (-1);
}

int check_args(int argc, char **argv)
{
	int i;
	
	if (argc != 9)
		return (1);
	i = 1;
	while (i <= 7)
	{
		if (!is_valid_number(argv[i]) || strlen(argv[i]) > 9)
			return (1);
		if (i < 7 && atoi(argv[i]) < 1)
			return (1);
		i++;
	}
	if (parse_scheduler(argv[8]) == -1)
		return (1);
	return (0);
}

void	fill_config(t_config *cfg, char **argv)
{
	cfg->number_of_coders = atoi(argv[1]);
	cfg->time_to_burnout = atoi(argv[2]);
	cfg->time_to_compile = atoi(argv[3]);
	cfg->time_to_debug = atoi(argv[4]);
	cfg->time_to_refectore = atoi(argv[5]);
	cfg->number_of_compiles_required = atoi(argv[6]);
	cfg->dongle_cooldown = atoi(argv[7]);
	cfg->scheduler = parse_scheduler(argv[8]);
}