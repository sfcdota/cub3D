/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/24 17:18:38 by cbach             #+#    #+#             */
/*   Updated: 2020/05/30 22:20:45 by cbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_over(long long int i, int multiplier, int current)
{
	if ((i == 922337203685477580 && current > 54 && multiplier == 1)
	|| (i == 922337203685477580 && current > 55 && multiplier == -1)
	|| i > 922337203685477580)
		return (1);
	else
		return (0);
}

int			ft_atoi(const char *nptr)
{
	long long int	i;
	int				multiplier;

	i = 0;
	multiplier = 1;
	while ((*nptr > 6 && *nptr < 14) || *nptr == 32)
		nptr++;
	if (*nptr == 45 || *nptr == 43)
	{
		if (*nptr == 45)
			multiplier *= -1;
		nptr++;
	}
	while (ft_isdigit(*nptr))
	{
		if (is_over(i, multiplier, *nptr))
			return (multiplier == 1 ? -1 : 0);
		i = i * 10 + (*nptr++ - 48);
	}
	return (i * multiplier);
}
