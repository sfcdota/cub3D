/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 23:29:17 by cbach             #+#    #+#             */
/*   Updated: 2020/11/02 23:33:12 by cbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	init_texture(t_data *data, int index)
{
	if ((data->textures[index].img =
	mlx_xpm_file_to_image(data->mlx->mlx, data->mi->textures[index],
	&data->textures[index].width, &data->textures[index].height)) == NULL)
		prog_error(data, INVALID_TEXTURE);
	data->textures[index].addr =
	mlx_get_data_addr(data->textures[index].img,
	&data->textures[index].bits_per_pixel, &data->textures[index].line_length,
	&data->textures[index].endian);
}

void	set_player_start_pos(t_mi *mi, int i, int j)
{
	int direction;

	if (mi->map[i][j] == 'E')
		direction = 0;
	if (mi->map[i][j] == 'N')
		direction = 1;
	if (mi->map[i][j] == 'W')
		direction = 2;
	if (mi->map[i][j] == 'S')
		direction = 3;
	mi->x = i + 0.5;
	mi->y = j + 0.5;
	mi->angle = direction * M_PI_2;
}

char	*parse_path(char *begin, const char *end, t_data *data)
{
	char *path;
	char *temp;

	if (!(path = malloc((end - begin) / sizeof(char) + 1)))
		sys_error(data);
	temp = path;
	while (begin <= end)
		*path++ = *begin++;
	*path = '\0';
	return (temp);
}

int		parse_pos_num(char **line, t_data *data)
{
	int num;

	*line = next_non_space(*line);
	if (!ft_isdigit(**line) || (**line == '0' && *(*line + 1) == '0'))
		prog_error(data, INVALID_CONFIG);
	num = ft_atoi(*line);
	*line = next_non_digit(*line);
	if (**line == '.')
		(*line)++;
	*line = next_non_digit(*line);
	*line = next_non_space(*line);
	return (num);
}

void	check_map_line(char *str, t_data *data)
{
	if (*next_non_space(str) == '\0')
		prog_error(data, INVALID_MAP);
	while (*str)
		if (!is_in_set(*str++, "NSWE012 "))
			prog_error(data, INVALID_MAP);
}
