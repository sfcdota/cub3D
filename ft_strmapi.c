/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/23 21:31:32 by cbach             #+#    #+#             */
/*   Updated: 2020/05/29 23:52:45 by cbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*t;
	char			*a;
	unsigned int	i;

	i = 0;
	t = s ? malloc(ft_strlen(s) + 1) : NULL;
	if (t)
	{
		a = t;
		while (s[i])
		{
			*a++ = f(i, s[i]);
			i++;
		}
		*a = '\0';
	}
	return (t);
}
