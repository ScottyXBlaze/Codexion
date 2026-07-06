/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fifo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 11:46:28 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/06 11:46:40 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "codexion.h"

void	fifo_push(t_all *all, t_coder *coder);
t_coder	*fifo_front(t_all *all);
void	fifo_pop(t_all *all);
