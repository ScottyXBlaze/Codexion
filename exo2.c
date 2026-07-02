/* ************************************************************************* */
/*                                                                           */
/*                                                       :::      ::::::::   */
/*   exo2.c                                            :+:      :+:    :+:   */
/*                                                   +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananariv  +#+  +:+       +#+        */
/*                                               +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 13:15:02 by nyramana         #+#    #+#             */
/*   Updated: 2026/07/02 13:21:38 by nyramana        ###   ########.fr       */
/*                                                                           */
/* ************************************************************************* */

#include <pthread.h>
#include <stdio.h>

typedef struct s_data
{
	int				total_lines;
	// DONE: Ajouter un mutex ici (pthread_mutex_t)
	pthread_mutex_t	mutex;
}					t_data;

void	*ecrire_code(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	for (int i = 0; i < 1000000; i++)
	{
		// DONE: Protéger la zone critique (verrouiller le mutex)
		pthread_mutex_lock(&data->mutex);
		data->total_lines++;
		// DONE: Libérer la zone critique (déverrouiller le mutex)
		pthread_mutex_unlock(&data->mutex);
	}
	return (NULL);
}

int	main(void)
{
	t_data	data;

	data.total_lines = 0;
	// DONE: Initialiser le mutex avec pthread_mutex_init
	pthread_mutex_init(&data.mutex, NULL);
	pthread_t t1, t2;
	pthread_create(&t1, NULL, ecrire_code, &data);
	pthread_create(&t2, NULL, ecrire_code, &data);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	// DONE: Détruire le mutex avec pthread_mutex_destroy
	pthread_mutex_destroy(&data.mutex);
	printf("Total lignes de code compilées : %d (Attendu: 200000)\n",
		data.total_lines);
	return (0);
}
