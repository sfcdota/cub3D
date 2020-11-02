/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/12 15:55:36 by cbach             #+#    #+#             */
/*   Updated: 2020/11/02 04:43:18 by cbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


void init_texture(t_data *data, int index)
{
	if ((data->textures[index].img =
		mlx_xpm_file_to_image(data->mlx->mlx, data->mi->textures[index],
		&data->textures[index].width, &data->textures[index].height)) == NULL)
		prog_error(data, INVALID_TEXTURE);
	if ((data->textures[index].addr = mlx_get_data_addr(data->textures[index].img,
	&data->textures[index].bits_per_pixel, &data->textures[index].line_length,
		&data->textures[index].endian)) == NULL)
		prog_error(data, INVALID_TEXTURE);
}



int		is_in_set(char c, char const *set)
{
	while (*set)
		if (c == *set++)
			return (1);
	return (*set == c ? 1 : 0);
}

void 	set_player_start_pos(t_mi *mi, int i, int j)
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

int 	is_space(char c)
{
	return (c > 6 && c < 14) || c == 32 ? 1 : 0;
}

int		is_digit(char c)
{
	return (c > 47 && c < 58 ? 1 : 0);
}

char 	*next_non_space(char *line)
{
	while (is_space(*line))
		line++;
	return (line);
}

char	*next_non_digit(char *line)
{
	while (*line && is_digit(*line))
		line++;
	return (line);
}

char 	*next_till_eol(char *line)
{
	while (*line)
		line++;
	return (line);
}

char	*parse_path(char *begin, const char *end, t_data *data)
{
	char *path;
	char *temp;

	if (!(path = malloc((end - begin)/sizeof(char) + 1)))
		sys_error(data);
	temp = path;
	while (begin <= end)
		*path++ = *begin++;
	*path = '\0';
	return (temp);
}

int		parse_pos_num(char **line, t_mi *mi, t_data *data)
{
	int num;

	*line = next_non_space(*line);
	if (!is_digit(**line) || (**line == '0' && *(*line + 1) == '0'))
		prog_error(data, INVALID_CONFIG);
	num = ft_atoi(*line);
	*line = next_non_digit(*line);
	if (**line == '.')
		(*line)++;
	*line = next_non_digit(*line);
	*line = next_non_space(*line);
	return (num);
}

void	parse_r(char **line, t_mi *mi, t_data *data)
{
	int r1;
	int r2;

	if (mi->resolution[0] != -1 || mi->resolution[1] != -1)
		prog_error(data, DUPLICATE_FLAGS);
	(*line)++;
	r1 = parse_pos_num(line, mi, data);
	r2 = parse_pos_num(line, mi, data);
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
}

void	parse_textures(char **line, t_mi *mi, int texture_index, t_data *data)
{
	char *temp;
	int fd;

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

int		create_trgb(int t, int r, int g, int b)
{
	return(t << 24 | r << 16 | g << 8 | b);
}

void	parse_color(char **line, t_mi *mi, int color_index, t_data *data)
{
	int *color_array;
	int i;

	if (mi->colors[color_index] != -1)
		prog_error(data, DUPLICATE_FLAGS);
	i = -1;
	if(!(color_array = malloc (sizeof(int) * 3)))
		sys_error(data);
	(*line)++;
	while (++i < 3 && *line)
	{
		if ((color_array[i] = parse_pos_num(line, mi, data)) >= 0 && color_array[i] < 256)
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

void	check_map_line(char *str, t_data *data)
{

	if (!next_non_space(str))
		prog_error(data, INVALID_MAP);
	while (*str)
		if (!is_in_set(*str++, "NSWE012 "))
			prog_error(data, INVALID_MAP);
}

void	parse_map(char **line, t_mi *mi, int fd, t_data *data)
{
	int status;
	t_list *temp;
	size_t len;

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
			break;
		status = get_next_line(fd, line);
	}
	if (status < 0)
		sys_error(data);
}


char	*line_copy(double width, char *src, t_data *data)
{
	char *dest;
	int j;

	j = -1;
	if(!(dest = ft_calloc(sizeof(char), (size_t)width + 1)))
		sys_error(data);
	while (++j < width)
		dest[j] = *src ? *src++ : ' ';
	dest[j] = '\0';
	return (dest);
}

void check_left(t_mi *mi, t_data *data, int i, int j)
{
	while (!is_in_set(mi->map[i][j], " 1") && j > 0)
		j--;
	if (is_in_set(mi->map[i][j]," 02NSWE"))
		prog_error(data, MAP_NOT_CLOSED);
}

void check_right(t_mi *mi, t_data *data, int i, int j)
{
	while (!is_in_set(mi->map[i][j], " 1") && j <= mi->max_line_length)
		j++;
	if (is_in_set(mi->map[i][j]," 02NSWE"))
		prog_error(data, MAP_NOT_CLOSED);
}

void check_up(t_mi *mi, t_data *data, int i, int j)
{
	while (!is_in_set(mi->map[i][j], " 1") && i > 0)
		i--;
	if (is_in_set(mi->map[i][j]," 02NSWE"))
		prog_error(data, MAP_NOT_CLOSED);
}

void check_down(t_mi *mi, t_data *data, int i, int j)
{
	while (!is_in_set(mi->map[i][j], " 1") && i <= mi->lines)
		i++;
	if (is_in_set(mi->map[i][j]," 02NSWE"))
		prog_error(data, MAP_NOT_CLOSED);
}

void is_closed(t_mi *mi, t_data *data, int i, int j)
{
	check_left(mi, data, i, j);
	check_right(mi, data, i, j);
	check_up(mi, data, i, j);
	check_down(mi, data, i, j);
}

int check_map(t_mi *mi, t_data *data)
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
			if (is_in_set(mi->map[i][j], "NSWE0"))
				is_closed(mi, data, i, j);
			if (mi->map[i][j] == '2')
				mi->sprites_count++;
		}
	}
	if (mi->x == -1 || mi->y == -1)
		prog_error(data, NO_START_PLAYER_POSITION);
	mi->map[(int)(mi->x)][(int)mi->y] = '0';
}

void	map_list_to_array(t_mi *mi, t_data *data)
{
	char **map;
	int i;
	t_list *map_list;

	map_list = mi->map_list;
	i = -1;
	if(!(map = malloc(sizeof(char *) * (int)mi->lines)))
		sys_error(data);
	while (++i < mi->lines)
	{
		if(!(map[i] = line_copy(mi->max_line_length, (char *)map_list->content, data)))
			sys_error(data);
		map_list = map_list->next;
	}
	ft_lstclear(&(mi->map_list), clear_ptr);
	mi->map = map;
}

void	parse_sprites_info(t_mi *mi, t_data *data)
{
	int i;
	int j;

	i = -1;
	int s;
	s = 0;
	if(!(mi->sprites_distance = malloc (sizeof(double) * mi->sprites_count)) ||
	!(mi->sprites_order = malloc (sizeof(int) * mi->sprites_count)) ||
	!(mi->sprites = malloc(sizeof(t_sprite) * mi->sprites_count)))
		sys_error(data);
	while(++i < mi->lines)
	{
		j = -1;
		while(++j < mi->max_line_length)
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

void	check_empty_params(t_mi *mi, t_data *data)
{
	if (mi->colors[0] == -1 || mi->colors[1] == -1 || mi->resolution[0] == -1 ||
		mi->resolution[1] == -1 || mi->textures[0] == NULL ||
		mi->textures[1] == NULL || mi->textures[2] == NULL ||
		mi->textures[3] == NULL || mi->textures[4] == NULL ||
		mi->map_list->content == NULL)
		prog_error(data, MISSING_FLAG);
}

void	parse_config(t_mi *mi, char *filename, t_data *data)
{
	int status;
	char *temp;

	if ((mi->fd = open(filename, O_RDONLY)) == -1)
		sys_error(data);
	mi->fd_opened = 1;
	mi->current_line = NULL;
	while ((status = get_next_line(mi->fd, &mi->current_line)) > 0)
	{
		if (is_in_set(*(temp = next_non_space(mi->current_line)), "NSWEFCR"))
			parse_flag(&temp, mi, data);
		else if (is_in_set(*temp, "012"))
			parse_map(&mi->current_line, mi, mi->fd, data);
		else if (*temp)
			prog_error(data, INVALID_CONFIG);
		clear_ptr(mi->current_line);
	}
	check_empty_params(mi, data);
	if ((close(mi->fd)) == -1 || status < 0)
		sys_error(data);
	mi->fd_opened = 0;
	map_list_to_array(mi, data);
	check_map(mi, data);
}


void	init_messages(t_mi *mi, t_data *data)
{
	if (!(mi->error_messages = malloc(ERROR_COUNT * sizeof(char *))))
		sys_error(data);
	if (!(mi->error_messages[SYS_ERROR] = ft_strdup("SYSTEM ERROR")))
		sys_error(data);
	if (!(mi->error_messages[INVALID_RESOLUTION] = ft_strdup("INVALID RESOLUTION")))
		sys_error(data);
	if (!(mi->error_messages[INVALID_COLOR] = ft_strdup("INVALID COLOR")))
		sys_error(data);
	if (!(mi->error_messages[INVALID_TEXTURE] = ft_strdup("INVALID TEXTURE")))
		sys_error(data);
	if (!(mi->error_messages[DUPLICATE_FLAGS] = ft_strdup("DUPLICATE FLAGS")))
		sys_error(data);
	if (!(mi->error_messages[MISSING_FLAG] = ft_strdup("MISSING FLAG")))
		sys_error(data);
	if (!(mi->error_messages[INVALID_MAP] = ft_strdup("INVALID MAP")))
		sys_error(data);
	if (!(mi->error_messages[NO_START_PLAYER_POSITION] = ft_strdup("NO START POSITION")))
		sys_error(data);
	if (!(mi->error_messages[MULTIPLE_START_PLAYER_POSITION] = ft_strdup("MULTIPLE START POSITION")))
		sys_error(data);
	if (!(mi->error_messages[MAP_NOT_CLOSED] = ft_strdup("MAP NOT CLOSED")))
		sys_error(data);
	if (!(mi->error_messages[INVALID_CONFIG] = ft_strdup("INVALID CONFIG")))
		sys_error(data);
	if (!(mi->error_messages[INVALID_ARGUMENT] = ft_strdup("INVALID ARGUMENT")))
		sys_error(data);
	if (!(mi->error_messages[NO_ARGUMENTS] = ft_strdup("NO ARGUMENTS")))
		sys_error(data);
}

void	initialize_mi(t_mi *mi, t_data *data)
{
	int i;

	i = -1;

	//инициализировать мессаги и очищать в clear!
	if (!(data->textures = malloc (sizeof(t_texture) * 5)))
		sys_error(data);
	while (++i < 5)
	{
		data->textures->img = NULL;
		data->textures->addr = NULL;
	}
	i = -1;
	if (!((mi->colors = malloc(sizeof(int) * 2)) &&
	(mi->textures = malloc(sizeof(char *) * 5)) &&
	(mi->resolution = malloc (sizeof(int) * 2))))
		sys_error(data);
	mi->map_list = NULL;
	mi->lines = 0;
	mi->max_line_length = 0;
	mi->map = NULL;
	mi->x = -1;
	mi->y = -1;
	mi->angle = -100;
	mi->save = 0;
	mi->sprites_count = 0;
	mi->sprites = NULL;
	mi->sprites_order = NULL;
	mi->sprites_distance = NULL;
	mi->r_speed = ROTATION_SPEED;
	mi->m_speed = MOVE_SPEED;
	mi->current_line = NULL;
	init_messages(mi, data);
	mi->fd = -228;
	mi->fd_opened = 0;
	while (++i < 5)
	{
		if (i < 2)
		{
			mi->colors[i] = -1;
			mi->resolution[i] = -1;
		}
		mi->textures[i] = NULL;
	}
}

void parsing(int argc, char **argv, t_mi *mi, t_data *data)
{
	if (argc == 1 || argc > 3)
		prog_error(data, NO_ARGUMENTS);
	if (argc == 3)
	{
		if (ft_strncmp("--save", argv[2], 10))
			mi->save = 1;
		else
			prog_error(data, INVALID_ARGUMENT);
	}
	parse_config(mi, argv[1], data);
	if (mi->sprites_count)
		parse_sprites_info(mi, data);
}


int main (int argc, char **argv)
{
	t_mi mi;
	t_img img;
	t_mlx mlx;
	t_data data;
	t_ray ray;

	data.mlx = &mlx;
	data.img = &img;
	data.mi = &mi;
	data.ray = &ray;
	init_messages(&mi, &data);
	initialize_mi(&mi, &data);
	if (!(data.mlx->mlx = mlx_init()))
		prog_error(&data, SYS_ERROR);
	parsing(argc, argv, &mi, &data);
	init_game(&mi, &mlx, &img, &data);
	render(-1, &data);
	if (!mi.save)
	{
		mlx_hook(mlx.win, KEY_PRESS_EVENT, KEY_PRESS_MASK, render, &data);
		mlx_hook(mlx.win, DESTROY_NOTIFY, DESTROY_NOTIFY_MASK, handle_exit, &data);
		mlx_loop(mlx.mlx);
	}
	return (0);
}
