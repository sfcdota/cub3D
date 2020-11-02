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

#include "cub3D.h"


void import_textures(t_data *data)
{
	int i = -1;
	while (++i < 5)
	{
		data->textures[i].img =
				mlx_xpm_file_to_image(data->mlx->mlx, data->mi->textures[i],
									  &data->textures[i].width, &data->textures[i].height);
		data->textures[i].addr =
				mlx_get_data_addr(data->textures[i].img,
								  &data->textures[i].bits_per_pixel, &data->textures[i].line_length,
								  &data->textures[i].endian);
	}
}



int		is_in_set(char c, char const *set)
{
	while (*set)
		if (c == *set++)
			return (1);
	return (0);
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

char	*next_till_space(char *line)
{
	while (*line && !is_space(*line))
		line++;
	return (line);
}


int			ft_atoi_modified(char **ptr, t_data *data)
{
	int	i;
	int error;

	error = 0;
	i = 0;
	while (is_space(**ptr))
		(*ptr)++;
	while (is_digit(**ptr))
		if (i > 1000000000)
			error = -1;
		else
			i = i * 10 + (*(*ptr)++ - 48);
	return error ? -1 : i;
}

int			ft_atoi_color(char **ptr, t_data *data)
{
	int	i;
	int error;

	error = 0;
	i = 0;
	while (is_space(**ptr))
		(*ptr)++;

	while (is_digit(**ptr))
		if (i > 255)
			error = -1;
		else
			i = i * 10 + (*(*ptr)++ - 48);
	if (**ptr == '.')
	{
		(*ptr)++;
		while (is_digit(**ptr))
			(*ptr)++;
	}
	*ptr = next_non_space(*ptr);
	return error ? -1 : i;
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

void	parse_r(char **line, t_mi *mi, t_data *data)
{
	*line += sizeof(char) * 1;
	mi->resolution[0] = ft_atoi_modified(line, data);
	mi->resolution[1] = ft_atoi_modified(line, data);
	if (mi->resolution[0] * mi->resolution[1] == 0 || **line)
		mi->error = 0;
}

void	parse_textures(char **line, t_mi *mi, int texture_index, t_data *data)
{
	char *temp;
	char *t;
	int fd;

	t = NULL;
	*line += sizeof(char) * (texture_index < 4 ? 2 : 1);
	*line = next_non_space(*line);
	temp = *line;
	*line = next_till_space(*line);
	if (**line != '\0' || *line - temp > 255)
	{
		mi->error = 12;
		return ;
	}
	temp = parse_path(temp, *line, data);
	fd = open(temp, O_RDONLY);
	if (read(fd, t, 0) < 0)
	{
		free(temp);
		mi->error = 10;
	}
	else
		mi->textures[texture_index] = temp;
	close(fd);
}

int		create_trgb(int t, int r, int g, int b)
{
	return(t << 24 | r << 16 | g << 8 | b);
}

void	parse_color(char **line, t_mi *mi, int color_index, t_data *data)
{
	int *ha;
	int i;

	i = -1;
	if(!(ha = malloc (sizeof(int) * 3)))
		sys_error(data);
	*line += sizeof(char) * 1;
	while (++i < 3 && *line)
	{
		if ((ha[i] = ft_atoi_color(line, data)) >= 0)
		{
			if (**line == ',' && i < 3)
				(*line)++;
			else if (**line)
					*line = NULL;
		}
		else
			*line = NULL;
	}
	if (*line)
		mi->colors[color_index] = create_trgb(0, ha[0], ha[1], ha[2]);
	else
		mi->error = 0;
	free(ha);
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


int		parse_map(char **line, t_mi *mi, int fd, t_data *data)
{
	int status = 1;
	t_list *temp;
	int len;

	mi->map_list = NULL;
	while (status > 0 && *next_non_space(*line) != '\0')
	{
		mi->lines++;
		len = ft_strlen(*line);
		if (mi->max_line_length < len)
			mi->max_line_length = len;
		if (!(temp = ft_lstnew(ft_strdup(*line))))
			return -1;
		ft_lstadd_back(&mi->map_list, temp);
		free(*line);
		if (status > 0)
			status = get_next_line(fd, line);
	}
	while (*next_non_space(*line) == '\0' && status > 0)
		status = get_next_line(fd, line);
	if (status < 0 || *next_non_space(*line) != '\0')
		mi->error = 0;
	return (0);
}


char	*line_copy(double width, char *src, t_data *data)
{
	char *dest;
	int j;

	j = 0;
	if(!(dest = ft_calloc(sizeof(char), (int)width + 1)))
		return NULL;
	while (j < width)
	{
		dest[j] = *src ? *src++ : ' ';
		j++;
	}
	dest[j] = '\0';
	return (dest);
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
					return (-1);
				else
					set_player_start_pos(mi, i, j);
			}
			if (is_in_set(mi->map[i][j], "NSWE0") && ((j == 0 ||
			j == mi->max_line_length - 1 || i == 0 || i == mi->lines - 1) ||
			(mi->map[i - 1][j] == ' ' || mi->map[i + 1][j] == ' ' ||
			mi->map[i][j - 1] == ' ' || mi->map[i][j + 1] == ' ')))
					return (-1);
			if (mi->map[i][j] == '2')
				mi->numSprites++;
		}
	}
	mi->map[(int)(mi->x)][(int)mi->y] = '0';
	return (1);
}

int	map_list_to_array(t_mi *mi, t_data *data)
{
	char **map;
	int i;
	t_list *map_list;

	map_list = mi->map_list;
	i = 0;
	if(mi->lines == 0)
		prog_error(data, "No map.", 0);
	if(!(map = malloc(sizeof(char *) * (int)mi->lines)))
		sys_error(data);
	while (i < mi->lines)
	{
		if(!(map[i] = line_copy(mi->max_line_length, (char *)map_list->content, data)))
			return -1;
		map_list = map_list->next;
		i++;
	}
	ft_lstclear(&(mi->map_list), clear_ptr);
	mi->map = map;
	return check_map(mi, data);
}

void	parse_sprites_info(t_mi *mi, t_data *data)
{
	int i;
	int j;

	i = -1;
	int s;
	s = 0;
	if(!(mi->spriteDistance = malloc (sizeof(double) * mi->numSprites)) ||
	!(mi->spriteOrder = malloc (sizeof(int) * mi->numSprites)) ||
	!(mi->sprites = malloc(sizeof(t_sprite) * mi->numSprites)))
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

int 	check_map_info(t_mi *mi, t_data *data)
{
	int i;

	i = 0;
	while (i < 5)
	{
		if (i < 2)
		{
			if (mi->colors[i] == -1 || mi->resolution[i] == -1 || mi->x == -1 || mi->y == -1)
				return (-1);
		}
		if (mi->textures[i] == NULL)
			return (-1);
		i++;
	}
	if (mi->lines == 0 || mi->angle == -100
	|| mi->max_line_length == 0 || mi->map == NULL || mi->error >= 0)
		return (-1);
	return (1);
}

int		parse(t_mi *mi, char *filename, t_data *data)
{
	int fd;
	int status;
	char *temp;
	char *line;

	fd = open(filename, O_RDONLY);
	line = NULL;
	status = 1;
	while (status && mi->error < 0 && (status = get_next_line(fd, &line)) > 0)
	{
		if (*(temp = next_non_space(line)) && *temp != '1')
			parse_flag(&temp, mi, data);
		else if (*temp == '1')
			status = parse_map(&line, mi, fd, data);
		free(line);
		line = NULL;
	}
	close(fd);
	if (status < 0) // -1 = read error, other = msges
		mi->error = 10;
	else
		status = map_list_to_array(mi, data);
	if (status >= 0)
		status = check_map_info(mi, data);
	return status;
}

void	initialize_mi(t_mi *mi, t_data *data)
{
	int i;

	i = -1;
	mi->numSprites = 0;
	mi->max_line_length = 0;
	mi->lines = 0;
	mi->map_list = NULL;
	mi->map = NULL;
	mi->angle = -100;
	if (!((mi->colors = malloc(sizeof(int) * 2)) &&
	(mi->textures = malloc(sizeof(char *) * 5)) &&
	(mi->resolution = malloc (sizeof(int) * 2))))
		sys_error(data);
	mi->x = -1;
	mi->y = -1;
	while (++i < 5)
	{
		if (i < 2)
		{
			mi->colors[i] = -1;
			mi->resolution[i] = -1;
		}
		mi->textures[i] = NULL;
	}
	mi->error = -100;
	mi->save = 0;
}

int import_config(int argc, char **argv, t_mi *mi, t_data *data)
{
	int status;

	if (argc == 2 || argc == 3)
	{
		initialize_mi(mi, data);
		status = parse(mi, argv[1], data);
		if (argc == 3 && ft_strncmp("--save", argv[2], 10))
			mi->save = 1;
		if (status != -1 && mi->numSprites)
		{
			parse_sprites_info(mi, data);
		}
	}
	else
		status = -1;
	if (status == -1 || mi->error >= 0)
		printf("there is an error ... ERROR CODE = %d\n", status);
	mi->rSpeed = ROTATION_SPEED;
	mi->mSpeed = MOVE_SPEED;
	data->mlx->mlx = mlx_init();
	if (!(data->textures = malloc (sizeof(t_texture) * 5)) || !data->mlx->mlx)
		sys_error(data);
	import_textures(data);

	return (status <= 0 ? -1 : 1);
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
	import_config(argc, argv, &mi, &data);
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
