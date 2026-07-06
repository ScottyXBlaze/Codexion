/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 12:07:35 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/06 12:56:43 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdlib.h>
#include "codexion.h"

void		heap_push(t_heap *heap, t_coder *coder);
t_coder		*heap_peek(t_heap *heap);
void		heap_pop(t_heap *heap);

int	init_heap(t_heap *heap, int capacity)
{
	heap->array = malloc(sizeof(t_coder *) * capacity);
	if (!heap->array)
		return (0);

	heap->size = 0;
	heap->capacity = capacity;

	if (pthread_mutex_init(&heap->mutex, NULL))
	{
		free(heap->array);
		return (0);
	}
	return (1);
}

void destroy_heap(t_heap *heap)
{
	free(heap->array);
	pthread_mutex_destroy(&heap->mutex);
}

void		heap_push(t_heap *heap, t_coder *coder)
{
	(void)heap;
	(void)coder;
}

t_coder		*heap_peek(t_heap *heap)
{
	(void)heap;
	return NULL;
}

void		heap_pop(t_heap *heap)
{
	(void)heap;
}

void lock_dongle_edf(t_coder *coder, t_dongle *dongle)
{
	(void)coder;
	(void)dongle;
}
