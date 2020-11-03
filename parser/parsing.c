/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 23:29:22 by cbach             #+#    #+#             */
/*   Updated: 2020/11/02 23:34:21 by cbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	check_map(t_mi *mi, t_data *data)
{
	int i;
	int j;

	i = -1;
	while (++i < mi->lines)
	{
		j = -1;
		while (++j < mi->max_line_length)
		{
			if (is_in_set(mi->map[i][j], "NSWE"))
			{
				if (mi->angle != -100)
					prog_error(data, MULTIPLE_START_PLAYER_POSITION);
				else
					set_player_start_pos(mi, i, j);
			}
			if (is_in_set(mi->map[i][j], "NSWE02"))
				is_closed(mi, data, i, j);
			if (mi->map[i][j] == '2')
				mi->sprites_count++;
		}
	}
	if (mi->x == -1 || mi->y == -1)
		prog_error(data, NO_START_PLAYER_POSITION);
	mi->map[(int)(mi->x)][(int)mi->y] = '0';
}

void	parse_sprites_info(t_mi *mi, t_data *data)
{
	int i;
	int j;
	int s;

	i = -1;
	s = 0;
	if (!(mi->sprites_distance = malloc(sizeof(double) * mi->sprites_count)) ||
	!(mi->sprites_order = malloc(sizeof(int) * mi->sprites_count)) ||
	!(mi->sprites = malloc(sizeof(t_sprite) * mi->sprites_count)))
		sys_error(data);
	while (++i < mi->lines)
	{
		j = -1;
		while (++j < mi->max_line_length)
		{
			if (mi->map[i][j] == '2')
			{
				mi->sprites[s].x = i;
				mi->sprites[s].y = j;
				s++;
			}
		}
	}
}

void	parse_config(t_mi *mi, char *filename, t_data *data)
{
	int		status;
	char	*temp;

	if ((mi->fd = open(filename, O_RDONLY)) == -1)
		sys_error(data);
	mi->fd_opened = 1;
	mi->current_line = NULL;
	while ((status = get_next_line(mi->fd, &mi->current_line)) >= 0)
	{
		if (is_in_set(*(temp = next_non_space(mi->current_line)), "NSWEFCR"))
			parse_flag(&temp, mi, data);
		else if (is_in_set(*temp, "012"))
			parse_map(&mi->current_line, mi, mi->fd, data);
		else if (*temp || *mi->current_line != '\0')
			prog_error(data, INVALID_CONFIG);
		clear_ptr(mi->current_line);
		if (status == 0)
			break ;
	}
	check_empty_params(mi, data);
	if ((close(mi->fd)) == -1 || status < 0)
		sys_error(data);
	mi->fd_opened = 0;
	map_list_to_array(mi, data);
	check_map(mi, data);
}

void	parsing(int argc, char **argv, t_mi *mi, t_data *data)
{
	size_t len;
	
	len = ft_strlen(argv[2]);
	if (argc == 1 || argc > 3)
		prog_error(data, NO_ARGUMENTS);
	if (argv[2][len - 1] != 'b' || argv[2][len - 2] != 'u' ||
	argv[2][len - 3] != 'c' || argv[2][len - 4] != '.')
		prog_error(data, INVALID_ARGUMENT);
	if (argc == 3)
	{
		if (!ft_strncmp("--save", argv[2], 10))
			mi->save = 1;
		else
			prog_error(data, INVALID_ARGUMENT);
	}
	parse_config(mi, argv[1], data);
	if (mi->sprites_count)
		parse_sprites_info(mi, data);
}
