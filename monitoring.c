/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hogkim <hogkim@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 17:18:41 by hogkim            #+#    #+#             */
/*   Updated: 2022/08/24 17:20:04 by hogkim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include <stdio.h>

static int	check_eat_count(t_param *param)
{
	int	i;
	int	num_of_hogs;

	num_of_hogs = 0;
	i = 0;
	while (i < param->rule->num_of_philo)
	{
		pthread_mutex_lock(&param->eat_count_lock);
		if (param->philo[i++].eat_count >= param->rule->count_of_must_eat)
		{
			++num_of_hogs;
		}
		pthread_mutex_unlock(&param->eat_count_lock);
	}
	if (num_of_hogs == param->rule->num_of_philo)
	{
		pthread_mutex_lock(&param->is_dining_lock);
		param->rule->is_dining = FALSE;
		pthread_mutex_unlock(&param->is_dining_lock);
		pthread_mutex_lock(&param->print_lock);
		printf("all philosophers have became hogs\n");
		return (KILL_PROCESS);
	}
	return (KEEP_PROCESS);
}

static int	check_death_of_philo(t_param *param)
{
	int	i;

	i = 0;
	while (i < param->rule->num_of_philo)
	{
		pthread_mutex_lock(&param->starving_time_lock);
		if ((get_time(param) - param->philo[i].start_starving_time \
			> param->rule->time_to_die))
		{
			pthread_mutex_unlock(&param->starving_time_lock);
			pthread_mutex_lock(&param->is_dining_lock);
			param->rule->is_dining = FALSE;
			pthread_mutex_unlock(&param->is_dining_lock);
			pthread_mutex_lock(&param->print_lock);
			printf("[%lld] %d %s\n", get_time(param) \
				- param->start_time, i + 1, "died");
			return (KILL_PROCESS);
		}
		pthread_mutex_unlock(&param->starving_time_lock);
		++i;
	}
	return (KEEP_PROCESS);
}

int	monitoring_philos(t_param *param)
{
	while (1)
	{
		if (check_death_of_philo(param) == KILL_PROCESS)
			break ;
		if (param->rule->if_count_of_must_eat == TRUE && \
			check_eat_count(param) == KILL_PROCESS)
			break ;
	}
	return (KILL_PROCESS);
}
