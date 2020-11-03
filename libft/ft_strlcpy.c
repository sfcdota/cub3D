/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/17 15:43:19 by cbach             #+#    #+#             */
/*   Updated: 2020/05/30 03:15:06 by cbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t		ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t c;

	if (!size && src && !dst)
		return (0);
	c = 0;
	if (size && dst && src)
	{
		while (*src && c < size - 1)
		{
			*dst++ = *src++;
			c++;
		}
		*dst = '\0';
	}
	while (src && *src++)
		c++;
	return (c);
}
