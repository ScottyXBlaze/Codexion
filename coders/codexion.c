/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 19:02:38 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/02 22:40:52 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_all	all;
	t_coder	*coders;

	memset(&all, 0, sizeof(all));
	coders = NULL;
	if (!parsers(argc, argv, &all))
		return (1);
	if (!init_dongles(&all) || !init_all(&all, coders))
		return (1);
	coders = init_coders(&all, coders);
	if (!coders)
		return (1);
	destroy_all(&all, coders);
	return (0);
}
