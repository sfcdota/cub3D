/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 23:29:20 by cbach             #+#    #+#             */
/*   Updated: 2020/11/02 23:31:42 by cbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

int		create_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}

char	*line_copy(double width, char *src, t_data *data)
{
	char	*dest;
	int		j;

	j = -1;
	if (!(dest = ft_calloc(sizeof(char), (size_t)width + 1)))
		sys_error(data);
	while (++j < width)
		dest[j] = *src ? *src++ : ' ';
	dest[j] = '\0';
	return (dest);
}

void	map_list_to_array(t_mi *mi, t_data *data)
{
	char	**map;
	int		i;
	t_list	*map_list;

	map_list = mi->map_list;
	i = -1;
	if (!(map = malloc(sizeof(char *) * (int)mi->lines)))
		sys_error(data);
	while (++i < mi->lines)
	{
		if (!(map[i] =
		line_copy(mi->max_line_length, (char *)map_list->content, data)))
			sys_error(data);
		map_list = map_list->next;
	}
	ft_lstclear(&(mi->map_list), clear_ptr);
	mi->map = map;
}

void	check_empty_params(t_mi *mi, t_data *data)
{
	if (mi->colors[0] == -1 || mi->colors[1] == -1 || mi->resolution[0] == -1 ||
		mi->resolution[1] == -1 || mi->textures[0] == NULL ||
		mi->textures[1] == NULL || mi->textures[2] == NULL ||
		mi->textures[3] == NULL || mi->textures[4] == NULL ||
		mi->map_list->content == NULL)
		prog_error(data, MISSING_FLAG);
}
