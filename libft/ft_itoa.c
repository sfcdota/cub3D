/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/23 19:57:24 by cbach             #+#    #+#             */
/*   Updated: 2020/05/29 23:52:45 by cbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		i_length(int n)
{
	int i;

	i = 1;
	while (n / 10)
	{
		i++;
		n /= 10;
	}
	return (i);
}

char			*ft_itoa(int n)
{
	char		*a;
	char		*t;
	int			l;
	long int	new_n;

	l = i_length(n);
	a = malloc(l + 1 + (n < 0 ? 1 : 0));
	if (a)
	{
		new_n = n;
		t = a;
		*(t + l + (n < 0 ? 1 : 0)) = '\0';
		if (new_n < 0)
		{
			*t++ = '-';
			new_n *= -1;
		}
		while (l--)
		{
			*(t + l) = new_n % 10 + 48;
			new_n /= 10;
		}
	}
	return (a);
}
