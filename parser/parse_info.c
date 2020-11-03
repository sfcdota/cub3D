/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_info.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 23:29:14 by cbach             #+#    #+#             */
/*   Updated: 2020/11/02 23:35:29 by cbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	parse_r(char **line, t_mi *mi, t_data *data)
{
	int r1;
	int r2;

	if (mi->resolution[0] != -1 || mi->resolution[1] != -1)
		prog_error(data, DUPLICATE_FLAGS);
	(*line)++;
	r1 = parse_pos_num(line, data);
	r2 = parse_pos_num(line, data);
	if (**line)
		prog_error(data, INVALID_RESOLUTION);
	if (r1 == -1 || r2 == -1)
		prog_error(data, INVALID_RESOLUTION);
	if (!mi->save)
	{
		mi->resolution[0] = r1 > RESOLUTION_WIDTH_MAX ? RESOLUTION_WIDTH_MAX :
							r1;
		mi->resolution[1] = r2 > RESOLUTION_HEIGHT_MAX ? RESOLUTION_HEIGHT_MAX :
							r2;
	}
	else
	{
		mi->resolution[0] = r1;
		mi->resolution[1] = r2;
	}
}

void	parse_textures(char **line, t_mi *mi, int texture_index, t_data *data)
{
	char	*temp;
	int		fd;

	if (mi->textures[texture_index])
		prog_error(data, DUPLICATE_FLAGS);
	*line += sizeof(char) * (texture_index < 4 ? 2 : 1);
	*line = next_non_space(*line);
	temp = *line;
	*line = next_till_eol(*line);
	if (**line != '\0')
		prog_error(data, INVALID_TEXTURE);
	mi->textures[texture_index] = parse_path(temp, *line, data);
	if ((fd = open(mi->textures[texture_index], O_RDONLY)) < 0)
		prog_error(data, SYS_ERROR);
	if ((close(fd)) < 0)
		sys_error(data);
	init_texture(data, texture_index);
}

void	parse_color(char **line, t_mi *mi, int color_index, t_data *data)
{
	int *color_array;
	int i;

	if (mi->colors[color_index] != -1)
		prog_error(data, DUPLICATE_FLAGS);
	i = -1;
	if (!(color_array = malloc(sizeof(int) * 3)))
		sys_error(data);
	(*line)++;
	while (++i < 3 && *line)
	{
		if ((color_array[i] =
		parse_pos_num(line, data)) >= 0 && color_array[i] < 256)
		{
			if (**line == ',' && i < 2)
				(*line)++;
			else if (i == 2 && **line)
				prog_error(data, INVALID_COLOR);
		}
		else
			prog_error(data, INVALID_COLOR);
	}
	mi->colors[color_index] = create_trgb(0, color_array[0],
	color_array[1], color_array[2]);
	clear_ptr(color_array);
}

void	parse_flag(char **line, t_mi *mi, t_data *data)
{
	if (**line == 'R')
		parse_r(line, mi, data);
	if (**line == 'F')
		parse_color(line, mi, 0, data);
	if (**line == 'C')
		parse_color(line, mi, 1, data);
	if (**line == 'N' && *(*line + sizeof(char) * 1) == 'O')
		parse_textures(line, mi, 0, data);
	if (**line == 'S' && *(*line + sizeof(char) * 1) == 'O')
		parse_textures(line, mi, 1, data);
	if (**line == 'W' && *(*line + sizeof(char) * 1) == 'E')
		parse_textures(line, mi, 2, data);
	if (**line == 'E' && *(*line + sizeof(char) * 1) == 'A')
		parse_textures(line, mi, 3, data);
	if (**line == 'S' && *(*line + sizeof(char) * 1) != 'O')
		parse_textures(line, mi, 4, data);
}

void	parse_map(char **line, t_mi *mi, int fd, t_data *data)
{
	int		status;
	t_list	*temp;
	size_t	len;

	status = 1;
	while (status >= 0)
	{
		check_map_line(*line, data);
		mi->lines++;
		len = ft_strlen(*line);
		if (mi->max_line_length < len)
			mi->max_line_length = len;
		if (!(temp = ft_lstnew(*line)))
			sys_error(data);
		ft_lstadd_back(&mi->map_list, temp);
		*line = NULL;
		if (!status)
			break ;
		status = get_next_line(fd, line);
	}
	if (status < 0)
		sys_error(data);
}
