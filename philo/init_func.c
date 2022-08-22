/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hogkim <hogkim@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 08:23:10 by hogkim            #+#    #+#             */
/*   Updated: 2022/08/22 19:18:50 by hogkim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>

static void	destroy_mutex(int i, t_param *param, int step)
{
	int	start;

	if (step >= 1)
		pthread_mutex_destroy(&param->print_lock);
	if (step >= 2)
		pthread_mutex_destroy(&param->is_dining_lock);
	if (step >= 3)
	{
		pthread_mutex_destroy(&param->get_time_lock);
		start = 0;
		while (start <= i)
		{
			pthread_mutex_destroy(&param->forks[start]);
			++start;
		}
	}
}

static int	mutexes_init(t_param *param)
{
	int	i;

	if (pthread_mutex_init(&param->print_lock, NULL))
		return (FAIL);
	if (pthread_mutex_init(&param->is_dining_lock, NULL))
	{
		destroy_mutex(0, param, 1);
		return (FAIL);
	}
	if (pthread_mutex_init(&param->get_time_lock, NULL))
	{
		destroy_mutex(0, param, 2);
		return (FAIL);
	}
	i = 0;
	while (i < param->rule->num_of_philo)
	{
		if (pthread_mutex_init(&param->forks[i], NULL))
		{
			destroy_mutex(i, param, 3);
			return (FAIL);
		}
		++i;
	}
	return (SUCCESS);
}

static int	init_philo(t_param *param)
{
	int	i;

	param->philo = malloc(sizeof(t_philo) * param->rule->num_of_philo);
	if (!param->philo)
		return (FAIL);
	i = 0;
	while (i < param->rule->num_of_philo)
	{
		param->philo[i].param = param;
		param->philo[i].eat_count = 0;
		param->philo[i].start_starving_time = get_time(param);
		param->philo[i].left_fork = &param->forks[i];
		param->philo[i].right_fork = &param->forks[(i + 1) \
			% param->rule->num_of_philo];
		++i;
	}
	return (SUCCESS);
}

int	init_param(t_param *param, t_rule *rule)
{
	param->rule = rule;
	param->forks = malloc(sizeof(pthread_mutex_t) * rule->num_of_philo);
	if (!param->forks)
		return (FAIL);
	if (mutexes_init(param) == FAIL)
	{
		free(param->forks);
		return (FAIL);
	}
	param->tids = malloc(sizeof(pthread_t) * rule->num_of_philo);
	if (!param->tids)
	{
		destroy_mutex(param->rule->num_of_philo, param, 3);
		free(param->forks);
		return (FAIL);
	}
	if (init_philo(param) == FAIL)
	{
		destroy_mutex(param->rule->num_of_philo, param, 3);
		free(param->forks);
		free(param->tids);
		return (FAIL);
	}
	return (SUCCESS);
}
