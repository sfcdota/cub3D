/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 23:36:12 by cbach             #+#    #+#             */
/*   Updated: 2020/11/02 23:36:13 by cbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int		is_space(char c)
{
	return (c > 6 && c < 14) || c == 32 ? 1 : 0;
}

int		is_in_set(char c, char const *set)
{
	while (*set)
		if (c == *set++)
			return (1);
	return (0);
}

char	*next_non_space(char *line)
{
	while (is_space(*line))
		line++;
	return (line);
}

char	*next_non_digit(char *line)
{
	while (*line && ft_isdigit(*line))
		line++;
	return (line);
}

char	*next_till_eol(char *line)
{
	while (*line)
		line++;
	return (line);
}
