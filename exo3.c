/* ************************************************************************* */
/*                                                                           */
/*                                                       :::      ::::::::   */
/*   exo3.c                                            :+:      :+:    :+:   */
/*                                                   +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananariv  +#+  +:+       +#+        */
/*                                               +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 13:22:57 by nyramana         #+#    #+#             */
/*   Updated: 2026/07/02 13:28:47 by nyramana        ###   ########.fr       */
/*                                                                           */
/* ************************************************************************* */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define SEUIL 100000

typedef struct s_data
{
	int				total_lines;
	pthread_mutex_t	mutex;
	// TODO: Ajouter une variable de condition (pthread_cond_t)
	pthread_cond_t	condition;
}					t_data;

void	*verifier_statut(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	pthread_mutex_lock(&data->mutex);
	// TODO: Tant que data->total_lines n'a pas atteint le SEUIL,
	// se mettre en attente passive avec pthread_cond_wait
	pthread_cond_wait(&data->condition, &data->mutex);
	printf("⚠️ [MONITEUR] Le seuil de %d lignes est atteint ! Alerte lancée.\n",
		SEUIL);
	pthread_mutex_unlock(&data->mutex);
	return (NULL);
}

void	*ecrire_code(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	for (int i = 0; i < 80000; i++)
	{
		pthread_mutex_lock(&data->mutex);
		data->total_lines++;
		// TODO: Si on vient d'atteindre exactement le SEUIL,
		// envoyer un signal avec pthread_cond_signal
		if (data->total_lines == SEUIL)
		{
			pthread_cond_signal(&data->condition);
		}
		pthread_mutex_unlock(&data->mutex);
		usleep(1); // Simule un petit délai de travail
	}
	return (NULL);
}

int	main(void)
{
	t_data	data;

	data.total_lines = 0;
	pthread_mutex_init(&data.mutex, NULL);
	// TODO: Initialiser la variable de condition
	pthread_cond_init(&data.condition, NULL);
	pthread_t dev1, dev2, moniteur;
	// On crée le moniteur d'abord pour qu'il se mette en attente
	pthread_create(&moniteur, NULL, verifier_statut, &data);
	pthread_create(&dev1, NULL, ecrire_code, &data);
	pthread_create(&dev2, NULL, ecrire_code, &data);
	pthread_join(dev1, NULL);
	pthread_join(dev2, NULL);
	pthread_join(moniteur, NULL);
	pthread_mutex_destroy(&data.mutex);
	// TODO: Détruire la variable de condition
	pthread_cond_destroy(&data.condition);
	printf("Fin de la simulation. Lignes totales : %d\n", data.total_lines);
	return (0);
}
