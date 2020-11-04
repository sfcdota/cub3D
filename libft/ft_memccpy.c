/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/23 18:11:56 by cbach             #+#    #+#             */
/*   Updated: 2020/05/27 14:51:50 by cbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dest, const void *src, int c, size_t n)
{
	unsigned char *s;
	unsigned char *d;
	unsigned char cc;

	cc = (unsigned char)c;
	s = (unsigned char *)src;
	d = (unsigned char *)dest;
	while (n--)
		if (*s == cc)
		{
			ft_memmove(d++, s++, 1);
			return (d);
		}
		else
			ft_memmove(d++, s++, 1);
	return (NULL);
}
