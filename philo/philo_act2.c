/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_act2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hogkim <hogkim@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 08:23:39 by hogkim            #+#    #+#             */
/*   Updated: 2022/08/22 19:18:01 by hogkim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

void	spending_time(t_param *param, long long time_to_spend)
{
	long long	target_time;

	target_time = get_time(param) + time_to_spend;
	while (get_time(param) < target_time)
		usleep(100);
}

void	philo_eat(t_rule *rule, t_philo *philo, int tid)
{
	pthread_mutex_lock(&philo->param->is_dining_lock);
	if (rule->is_dining == FALSE)
		return ;
	pthread_mutex_unlock(&philo->param->is_dining_lock);
	print_terminal(philo->param, tid + 1, "is eating");
	philo->start_starving_time = get_time(philo->param);
	spending_time(philo->param, rule->time_to_eat);
	++(philo->eat_count);
}

void	philo_sleep(t_rule *rule, t_philo *philo, int tid)
{
	pthread_mutex_lock(&philo->param->is_dining_lock);
	if (rule->is_dining == FALSE)
		return ;
	pthread_mutex_unlock(&philo->param->is_dining_lock);
	print_terminal(philo->param, tid + 1, "is sleeping");
	spending_time(philo->param, rule->time_to_sleep);
}

void	philo_think(t_rule *rule, t_philo *philo, int tid)
{
	pthread_mutex_lock(&philo->param->is_dining_lock);
	if (rule->is_dining == FALSE)
		return ;
	pthread_mutex_unlock(&philo->param->is_dining_lock);
	print_terminal(philo->param, tid + 1, "is thinking");
	usleep(50);
}
