/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/23 18:12:35 by cbach             #+#    #+#             */
/*   Updated: 2020/05/30 21:40:48 by cbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char	*d;
	char	*s;
	size_t	i;

	if (n && (dest || src))
	{
		i = 0;
		d = (char *)dest;
		s = (char *)src;
		while (i++ < n)
			*(d++) = *(s++);
	}
	return (dest);
}
