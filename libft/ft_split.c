/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/24 19:14:31 by cbach             #+#    #+#             */
/*   Updated: 2020/05/30 20:01:45 by cbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		word_length(char const *s, char c)
{
	int i;

	i = 0;
	while (*s && *s != c)
	{
		i++;
		s++;
	}
	return (i);
}

static char		*next_word(char const *s, char c)
{
	while (*s && *s != c)
		s++;
	while (*s && *s == c)
		s++;
	return ((char *)s);
}

static int		words_count(char const *s, char c)
{
	int i;

	i = 1;
	while (*next_word(s, c))
	{
		s = next_word(s, c);
		i++;
	}
	return (i);
}

void			*ft_clear(char **t, int i)
{
	while (i >= 0)
	{
		free(t[i]);
		t[i] = NULL;
		i--;
	}
	free(t);
	t = NULL;
	return (t);
}

char			**ft_split(char const *s, char c)
{
	char	**t;
	int		words;
	int		i;

	i = 0;
	if (!s)
		return (NULL);
	s = *s == c ? next_word(s, c) : s;
	words = *s ? words_count(s, c) : 0;
	if (!(t = malloc((words + 1) * sizeof(char *))))
		return (NULL);
	t[words] = NULL;
	while (*s)
	{
		if (!(t[i] = malloc(word_length(s, c) + 1)))
			return (ft_clear(t, i));
		ft_strlcpy(t[i], s, word_length(s, c) + 1);
		s = next_word(s, c);
		i++;
	}
	return (t);
}
