/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 19:02:08 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/06 10:48:47 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdio.h>
#include <stdlib.h>

static int			validate_args(int argc, char **argv);
static t_scheduler	scheduler(char *str);

int	parsers(int argc, char **argv, t_all *all)
{
	if (validate_args(argc, argv) == 0)
		return (0);
	all->params.nb_coders = atoi(argv[1]);
	all->params.burnout = atoi(argv[2]);
	all->params.compile = atoi(argv[3]);
	all->params.debug = atoi(argv[4]);
	all->params.refactor = atoi(argv[5]);
	all->params.compiles_required = atoi(argv[6]);
	all->params.dongle_cooldown = atoi(argv[7]);
	all->params.scheduler = scheduler(argv[8]);
	if (all->params.nb_coders < 2) {
		printf("[ERROR] Not enough coder\n");
		return (0);
	}
	return (1);
}

static int	validate_args(int argc, char **argv)
{
	int	i;

	if (argc != 9)
	{
		printf("[ERROR] Invalid number of argument %d\n", argc - 1);
		printf("Should be 8\n");
		return (0);
	}
	i = 1;
	while (i < 8)
	{
		if (!validate_int(argv[i]))
		{
			printf("Check parameter nb %d\n", i - 1);
			return (0);
		}
		i++;
	}
	if (!validate_scheduler(argv[i]))
	{
		printf("Check last parameter\n");
		return (0);
	}
	return (1);
}

static t_scheduler	scheduler(char *str)
{
	if (strcpy(str, "fifo") == 0)
		return (fifo);
	return (edf);
}
