/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/23 22:16:16 by cbach             #+#    #+#             */
/*   Updated: 2020/05/29 23:57:05 by cbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char *c;
	char *t;

	if (!s1 && s2)
		return (ft_strdup(s2));
	if (s1 && !s2)
		return (ft_strdup(s1));
	c = s1 && s2 ? malloc(ft_strlen(s1) + ft_strlen(s2) + 1) : NULL;
	if (c)
	{
		t = c;
		while (s1 && *s1)
			*t++ = *s1++;
		while (s2 && *s2)
			*t++ = *s2++;
		*t = '\0';
	}
	return (c);
}
