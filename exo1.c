/* ************************************************************************* */
/*                                                                           */
/*                                                       :::      ::::::::   */
/*   exo1.c                                            :+:      :+:    :+:   */
/*                                                   +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananariv  +#+  +:+       +#+        */
/*                                               +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 13:01:34 by nyramana         #+#    #+#             */
/*   Updated: 2026/07/02 14:26:31 by nyramana        ###   ########.fr       */
/*                                                                           */
/* ************************************************************************* */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Exercice 1 : L'Arène des Développeurs (Création et Structure)
//
// Objectif : Apprendre à allouer tes structures dans le main,
// initialiser un tableau de threads,
// et leur passer leurs données propres sans aucune variable globale.
//
// Consigne : Complète le code ci-dessous pour créer 3 threads (représentant des développeurs).
// Chaque thread doit recevoir sa propre structure t_dev,
// afficher son ID et le nombre total de développeurs dans l'arène,
// puis s'arrêter. Le main doit attendre que tout le monde ait fini avant de s'arrêter.

typedef struct s_arene
{
	int		nb_total_devs;
}			t_arene;

typedef struct s_dev
{
	int		id;
	t_arene	*arene;
}			t_dev;

void	*routine_developpeur(void *arg)
{
	t_dev	*dev;

	dev = arg;
	printf("DEV: %d\nTOTAL DEV: %d\n", dev->id, dev->arene->nb_total_devs);
	// 1. Caster le paramètre arg en (t_dev *)
	// 2. Afficher : "Développeur [ID] actif dans une arène de [NB] devs."
	return (NULL);
}

int	main(void)
{
	t_arene		arene;
	pthread_t	threads[3];
	t_dev		devs[3];
	int			i;

	i = 0;
	arene.nb_total_devs = 3;
	// 3. Écrire la boucle pour initialiser les structures devs et créer les 3 threads
	while (i < arene.nb_total_devs)
	{
		devs[i].id = i + 1;
		devs[i].arene = &arene;
		pthread_create(&threads[i], NULL, routine_developpeur, &devs[i]);
		i++;
	}
	i = 0;
	// 4. Écrire la boucle pour attendre (join) les threads
	while (i < arene.nb_total_devs)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	printf("Tous les développeurs ont terminé leur journée.\n");
	return (0);
}
