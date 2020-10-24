/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/12 15:55:36 by cbach             #+#    #+#             */
/*   Updated: 2020/10/22 22:23:47 by cbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "cub3D.h"
#include <stdio.h>
#include <mlx.h>
#include <math.h>

typedef struct		s_mi
{
	int				*resolution; // width = 0; height = 1;
	char			**textures; // n = 0, s = 1; w = 2; e = 3; s = 4;
	int				*colors; // f = 0; c = 1;
	t_list			*map_list;
	double 			lines;
	double				max_line_length;
	char			**map;
	int				error;
	double			x;
	double 			y;//x = 0; y = 1
	double 			angle;
	int				isShared;
	double			step;
}					t_mi;



int		is_in_set(char c, char const *set)
{
	while (*set)
		if (c == *set++)
			return (1);
	return (0);
}

void 	set_angle(t_mi *mi, int i, int j)
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
	mi->x = j;
	mi->y = i;
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


int			ft_atoi_modified(char **ptr)
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

int			ft_atoi_color(char **ptr)
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
		while (is_digit(**ptr))
			(*ptr)++;
	*ptr = next_non_space(*ptr);
	return error ? -1 : i;
}




char	*parse_path(char *begin, const char *end)
{
	char *path;
	char *temp;

	if (!(path = malloc((end - begin)/sizeof(char) + 1)))
		return (NULL);
	temp = path;
	while (begin <= end)
		*path++ = *begin++;
	*path = '\0';

	return (temp);
}

void	parse_r(char **line, t_mi *mi)
{
	*line += sizeof(char) * 1;
	mi->resolution[0] = ft_atoi_modified(line);
	mi->resolution[1] = ft_atoi_modified(line);
	if (mi->resolution[0] * mi->resolution[1] == 0 || **line)
		mi->error = 0;
}

void	parse_textures(char **line, t_mi *mi, int texture_index)
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
	temp = parse_path(temp, *line);
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

void	parse_color(char **line, t_mi *mi, int color_index)
{
	int color;
	int multiplier;
	int temp;

	*line += sizeof(char) * 1;
	multiplier = 1000000;
	color = 0;
	while (multiplier && *line)
	{
		if ((temp = ft_atoi_color(line)) >= 0)
		{
			color += temp * multiplier;
			multiplier /= 1000;
			if (**line == ',' && multiplier)
				(*line)++;
			else if (**line)
					*line = NULL;
		}
	}
	if (*line)
		mi->colors[color_index] = color;
	else
		mi->error = 0;
}

void	parse_flag(char **line, t_mi *mi)
{
	if (**line == 'R')
		parse_r(line, mi);
	if (**line == 'F')
		parse_color(line, mi, 0);
	if (**line == 'C')
		parse_color(line, mi, 1);
	if (**line == 'N' && *(*line + sizeof(char) * 1) == 'O')
		parse_textures(line, mi, 0);
	if (**line == 'S' && *(*line + sizeof(char) * 1) == 'O')
		parse_textures(line, mi, 1);
	if (**line == 'W' && *(*line + sizeof(char) * 1) == 'E')
		parse_textures(line, mi, 2);
	if (**line == 'E' && *(*line + sizeof(char) * 1) == 'A')
		parse_textures(line, mi, 3);
	if (**line == 'S' && *(*line + sizeof(char) * 1) != 'O')
		parse_textures(line, mi, 4);
}

//int 	correct_sides_and_spaces(char *line)
//{
//	int correct;
//	char *begin;
//	char *end;
//
//	correct = 1;
//	while (*line && (is_space(*line) || *line == '1'))
//		line++;
//	begin = line;
//	while (*line)
//		line++;
//	line--;
//	while (line > begin && (is_space(*line) || *line == '1'))
//		line--;
//	end = line;
//	if (*(begin - 1) != '1' && *(end + 1) != '1')
//		correct = 0;
////	while (correct && begin < end)
////	{
////		if (is_space(*begin))
////			correct = 0;
////		begin++;
////	}
//	return (correct);
//}

//int 	get_line_info(char *line, int *directions, int *max_width, int *lines)
//{
//	int counter;
//
//	if (correct_sides_and_spaces(line))
//	{
//		counter = 0;
//		*lines += 1;
//		while (*line)
//		{
//			counter++;
//			if (is_in_set(*line, "NSEW"))
//				*directions += 1;
//			line++;
//		}
//		if (*max_width < counter)
//			*max_width = counter;
//		return *directions > 1 ? -1 : 1;
//
//	}
//	return -2;
//}

//int		parse_map(char **line, t_mi *mi, int fd)
//{
//	int directions;
//	int status = 1;
//	t_list *temp;
//
//	directions = 0;
//	mi->map_list = NULL;
//	while (status > 0 && *next_non_space(*line) != '\0')
//	{
//		status = get_line_info(*line, &directions, &mi->max_line_length, &mi->lines);
//		if (!(temp = ft_lstnew(ft_strdup(*line))))
//			return -1;
//		ft_lstadd_back(&mi->map_list, temp);
//		free(*line);
//		if (status > 0)
//			status = get_next_line(fd, line);
//	}
//	while (*next_non_space(*line) == '\0' && status > 0)
//		status = get_next_line(fd, line);
//	if (status < 0 || *next_non_space(*line) != '\0')
//		mi->error = 0;
//	return (0);
//}


int		parse_map(char **line, t_mi *mi, int fd)
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


char	*line_copy(double width, char *src)
{
	char *dest;
	int j;

	j = 0;
	if(!(dest = malloc(sizeof(char) * ((int)width + 1))))
		return NULL;
	while (j < width)
	{
		dest[j] = *src ? *src++ : ' ';
		j++;
	}
	dest[j] = '\0';
	return (dest);
}


void	del(void *content)
{
	free(content);
	content = NULL;
}

int check_map(t_mi *mi)
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
					set_angle(mi, i, j);
			}
			if (is_in_set(mi->map[i][j], "NSWE0") && ((j == 0 ||
			j == mi->max_line_length - 1 || i == 0 || i == mi->lines - 1) ||
			(mi->map[i - 1][j] == ' ' || mi->map[i + 1][j] == ' ' ||
			mi->map[i][j - 1] == ' ' || mi->map[i][j + 1] == ' ')))
					return (-1);
		}
	}
	mi->map[(int)(mi->y)][(int)mi->x] = '0';
	return (1);
}

int	map_list_to_array(t_mi *mi)
{
	char **map;
	int i;
	t_list *map_list;

	map_list = mi->map_list;
	i = 0;
	if(mi->lines == 0 || !(map = malloc(sizeof(char *) * (int)mi->lines)))
		return -1;
	while (i < mi->lines)
	{
		if(!(map[i] = line_copy(mi->max_line_length, (char *)map_list->content)))
			return -1;
		map_list = map_list->next;
		i++;
	}
	ft_lstclear(&(mi->map_list), del);
	mi->map = map;
	return check_map(mi);
}

int 	check_map_info(t_mi *mi)
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

int		parse(t_mi *mi, char *filename)
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
			parse_flag(&temp, mi);
		else if (*temp == '1')
			status = parse_map(&line, mi, fd);
		free(line);
		line = NULL;
	}
	close(fd);
	if (status < 0) // -1 = read error, other = msges
		mi->error = 10;
	else
		status = map_list_to_array(mi);
	if (status >= 0)
		status = check_map_info(mi);
	return status;
}

void	initialize_mi(t_mi *mi)
{
	int i;

	i = -1;
	mi->max_line_length = 0;
	mi->lines = 0;
	mi->map_list = NULL;
	mi->map = NULL;
	mi->angle = -100;
	mi->colors = malloc(sizeof(int) * 2);
	mi->textures = malloc(sizeof(char *) * 5);
	mi->resolution = malloc (sizeof(int) * 2);
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
	mi->isShared = 0;
}

int correct_cub(char *line)
{
	int length;

	length = ft_strlen(line);
	if (line[length - 1] == 'b' && line[length - 2] == 'u'
	&& line[length - 3] == 'c')
		return 1;
	return 0;
}

int print_info(t_mi *mi)
{
	int i = 0;
	int j = 0;
	while (i < mi->lines)
	{
		j = 0;
		while (j < mi->max_line_length)
		{
			printf("%c", i == (int)(mi->y) && j == (int) mi->x ? 'x'
																		  : mi->map[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
	printf("\n\n\n\nwidth = %d\t height = %d\n"
		   "no texture = %s\n"
		   "so texture = %s\n"
		   "we texture = %s\n"
		   "ea texture = %s\n"
		   "s texture = %s\n"
		   "f color = %d \n"
		   "c color = %d \n"
		   "isShared = %d\n"
	 "ppos[0] = %lf\t ppos[1] = %lf\n"
  "angle = %lf\n", mi->resolution[0], mi->resolution[1],
		   mi->textures[0], mi->textures[1], mi->textures[2], mi->textures[3],
		   mi->textures[4], mi->colors[0], mi->colors[1], mi->isShared, mi->x, mi->y, mi->angle);
}

void	print_map(t_mi *mi)
{
	int i;
	int j;

	i = 0;
	while (i < mi->lines)
	{
		j = 0;
		while (j < mi->max_line_length)
		{
			printf("%c", i == (int)(mi->y) && j == (int) mi->x ? 'x'
																		  : mi->map[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
	printf("ppos[0] = %lf\t ppos[1] = %lf\n"
		"angle = %lf", mi->x, mi->y, mi->angle);
}

void 	clear_mi(t_mi *mi)
{
	int i;

	i = -1;
	while (++i < mi->lines)
	{
		if (mi->map[i])
			free(mi->map[i]);
		if (i < 5 && mi->textures[i])
			free(mi->textures[i]);
	}
	if (mi->resolution)
		free(mi->resolution);
	if(mi->colors)
		free(mi->colors);
	if (mi->map)
		free(mi->map);
	if (mi->textures)
		free(mi->textures);
}


int import_config(int argc, char **argv, t_mi *mi, double step)
{
	int status;

	//if ((argc == 2 || argc == 3) && correct_cub(argv[2]))
	if (argc == 2 || argc == 3)
	{
		initialize_mi(mi);
		status = parse(mi, argv[1]);
		if (argc == 3 && ft_strncmp("--shared", argv[2], 10))
			mi->isShared = 1;
	}
	else
		status = -1;
	if (status == -1 || mi->error >= 0)
		printf("there is an error ... ERROR CODE = %d\n", status);
	else
	{
		mi->step = step;
		print_info(mi);
	}
	return (status <= 0 ? -1 : 1);
}





//int key_pressed(int key, t_mi *mi)
//{
//	double multiplier;
//	double x_step;
//	double y_step;
//
//	multiplier = key == 13 || key == 0 ? mi->step : -mi->step;
//	if (key == 13 || key == 1 || key == 0 || key == 2)
//	{
//		x_step = multiplier * cos(mi->angle + (key == 13 || key == 1 ? 0 : M_PI_2));
//		y_step = multiplier * sin(mi->angle +(key == 13 || key == 1 ? 0 : 3 * M_PI_2));
//		if (mi->map[(int)(-(mi->y + y_step))][(int)(mi->x)] == '0')
//			mi->y += y_step;
//		if (mi->map[(int)(-mi->y)][(int)(mi->x + x_step)] == '0')
//			mi->x += x_step;
//	}
//	if (key == 123)
//		mi->angle += 0.1;
//	if (key == 124)
//		mi->angle -= 0.1;
////	if (key == 53)
////		return (-1);
//	print_map(mi);
//	return(1);
//}

double 	my_abs(double x)
{
	if (x < 0)
		return (-x);
	return (x);
}

double length_to_barrier(t_mi *mi, double angle) // + for wall | - for sprite
{
	double dx;
	double dy;
	int i;
	int j;

	i = (int)mi->y;
	j = (int)mi->x;
	dx = cos(angle) >= 0 ? 1 : -1;
	dy = sin(angle) >= 0 ? 1 : -1;
	while (-i < mi->lines && -i >= 0)
	{

		while (j < mi->max_line_length)
		{

			j += dx;
		}
		i += dy;
	}
}


typedef struct		s_ri
{
	int mapX;
	int mapY;
	double posX;
	double posY;
	double dirX;
	double dirY;
	double planeX;
	double planeY;
	double sideDistX;
	double sideDistY;
	double deltaDistX;
	double deltaDistY;
	double dist;
	int stepX;
	int stepY;
	int hit;
	int side;

}					t_ri;




typedef struct		s_mlx
{
    void *mlx;
    void *win;
    void *img;

}					t_mlx;

typedef struct		s_ha
{
	t_ri *ri;
	t_mi *mi;
	t_mlx *mlx;
}					t_ha;

void	initialize_ray(t_mi *mi, t_ri *ri)
{
	ri->posX = mi->y;
	ri->posY = mi->x;
	ri->dirX = -1;
	ri->dirY = 0;
	ri->planeX = 0;
	ri->planeY = 0.66;
	ri->hit = 0;
}



//void test(t_ha *ha)
//{
//	int x;
//	initialize_ray(mi, ri);
//	while(1)
//	{
//		x = -1;
//		while(++x < mi->resolution[0])
//		{
//			double cameraX = 2 * x / (double)mi->resolution[0] - 1;
//			double rayDirX = ri->dirX + ri->planeX * cameraX;
//			double rayDirY = ri->dirY + ri->planeY * cameraX;
//			ri->mapX = (int)ri->posX;
//			ri->mapY = (int)ri->posY;
//			ri->deltaDistX = abs(1/rayDirX);
//			ri->deltaDistY = abs(1/rayDirY);
//			if (rayDirX < 0)
//			{
//				ri->stepX = -1;
//				ri->sideDistX = (ri->posX - ri->mapX) * ri->deltaDistX;
//			}
//			else
//			{
//				ri->stepX = 1;
//				ri->sideDistX = (ri->mapX + 1.0 - ri->posX) * ri->deltaDistX;
//			}
//			if (rayDirY < 0)
//			{
//				ri->stepY = -1;
//				ri->sideDistY = (ri->posY - ri->mapY) * ri->deltaDistY;
//			}
//			else
//			{
//				ri->stepY = 1;
//				ri->sideDistY = (ri->mapY + 1.0 - ri->posY) * ri->deltaDistY;
//			}
//			while (!ri->hit)
//			{
//				if (ri->sideDistX < ri->sideDistY)
//				{
//					ri->sideDistX += ri->deltaDistX;
//					ri->mapX += ri->stepX;
//					ri->side = 0;
//				}
//				else
//				{
//					ri->sideDistY += ri->deltaDistY;
//					ri->mapY += ri->stepY;
//					ri->side = 1;
//				}
//				if (mi->map[ri->mapY][ri->mapX])
//					ri->hit = mi->map[ri->mapY][ri->mapX];
//			}
//			if (!ri->side)
//				ri->dist = (ri->mapX - ri->posX + (1 - ri->stepX) / 2) / rayDirX;
//			else
//				ri->dist = (ri->mapY - ri->posY + (1 - ri->stepY) / 2) / rayDirY;
//			int lineHeight = (int)(mi->resolution[1] / ri->dist);
//			int drawStart = -lineHeight / 2 + mi->resolution[1] / 2;
//			if (drawStart < 0)
//				drawStart = 0;
//			int drawEnd = lineHeight / 2 + mi->resolution[1] / 2;
//			if (drawEnd >= mi->resolution[0])
//				drawEnd = mi->resolution[0] - 1;
//			test_draw(mi, mlx, drawStart, drawEnd, x);
//		}
//	}
//}

//void	ray_casting(void *mlx, void *win, t_mi *mi)
//{
//	int i;
//	int j;
//	double view_angle_ray;
//	double dist;
//	int color;
//
//	i = 0;
//	while (++i <= mi->resolution[0])
//	{
//		view_angle_ray = mi->angle + M_PI_4 - i * M_PI_2 / mi->resolution[0]; //меняется от вью + пи/4 до вью - пи/4 (90 градусов)
//		dist = length_to_barrier(mi, view_angle_ray);
//		j = 0;
//		while (++j <= mi->resolution[1])
//		{
//			color = j < (mi->resolution - dist) / 2 || s
//			mlx_pixel_put(mlx, win, i, j, 255000000);
//		}
//	}
//
//}


void    test_draw(t_mi *mi, t_mlx *mlx, int draw_start, int draw_end, int cur_ray)
{
	int i;

	i = -1;
	while (++i < mi->resolution[1])
	{
		if (i >= draw_start && i <= draw_end)
			(mlx->mlx, mlx->win, cur_ray, i, 1488);
		else
			mlx_pixel_put(mlx->mlx, mlx->win, cur_ray, i, 255000);
	}
}

void key_pressed(int key, t_ha *ha)
{
	double multiplier;
	double x_step;
	double y_step;
	t_mi *mi = ha->mi;
	t_ri *ri = ha->ri;
	t_mlx *mlx = ha->mlx;

	double moveSpeed = key == 13 || key == 0 ? 1.0 : -1.0;
	double rotSpeed = 0.25;

	multiplier = key == 13 || key == 0 ? mi->step : -mi->step;
	if (key == 13 || key == 1 || key == 0 || key == 2)
	{
		x_step = moveSpeed * ri->dirX;
		y_step = moveSpeed * ri->dirY;
		if (mi->map[(int)(mi->y + y_step)][(int)(mi->x)] == '0')
			mi->y += y_step;
		if (mi->map[(int)(mi->y)][(int)(mi->x + x_step)] == '0')
			mi->x += x_step;
	}
	if (key == 123 || key == 124)
	{
		rotSpeed *= key == 123 ? 1 : -1;
		double oldDirX = ri->dirX;
		ri->dirX = ri->dirX * cos(rotSpeed) - ri->dirY * sin(rotSpeed);
		ri->dirY = oldDirX * sin(rotSpeed) + ri->dirY * cos(rotSpeed);
		double oldPlaneX = ri->planeX;
		ri->planeX = ri->planeX * cos(rotSpeed) - ri->planeY * sin(rotSpeed);
		ri->planeY = oldPlaneX * sin(rotSpeed) + ri->planeY * cos(rotSpeed);
		mi->angle = atan(hypot(ri->dirX, ri->dirY)/hypot(ri->planeX, ri->planeY));

	}
	//if (key == 124)
	//	mi->angle -= 0.1;
//	if (key == 53)
//		return (-1);
	print_map(mi);




	int x;
	//mlx->img = mlx_new_image(mlx->mlx, mi->resolution[0], mi->resolution[1]);
	mlx_clear_window(mlx->mlx, mlx->win);
	x = -1;
	while(++x < mi->resolution[0])
	{
		double cameraX = 2 * x / (double) mi->resolution[0] - 1;
		double rayDirX = ri->dirX + ri->planeX * cameraX;
		double rayDirY = ri->dirY + ri->planeY * cameraX;
		ri->mapX = (int) ri->posX;
		ri->mapY = (int) ri->posY;
		ri->deltaDistX = (rayDirY == 0) ? 0 : ((rayDirX == 0) ? 1 : abs(1 / rayDirX));
		ri->deltaDistY = (rayDirX == 0) ? 0 : ((rayDirY == 0) ? 1 : abs(1 / rayDirY));
		if (rayDirX < 0) {
			ri->stepX = -1;
			ri->sideDistX = (ri->posX - ri->mapX) * ri->deltaDistX;
		} else {
			ri->stepX = 1;
			ri->sideDistX = (ri->mapX + 1.0 - ri->posX) * ri->deltaDistX;
		}
		if (rayDirY < 0) {
			ri->stepY = -1;
			ri->sideDistY = (ri->posY - ri->mapY) * ri->deltaDistY;
		} else {
			ri->stepY = 1;
			ri->sideDistY = (ri->mapY + 1.0 - ri->posY) * ri->deltaDistY;
		}
		while (!ri->hit) {
			if (ri->sideDistX < ri->sideDistY) {
				ri->sideDistX += ri->deltaDistX;
				ri->mapX += ri->stepX;
				ri->side = 0;
			} else {
				ri->sideDistY += ri->deltaDistY;
				ri->mapY += ri->stepY;
				ri->side = 1;
			}
			if (mi->map[ri->mapX][ri->mapY])
				ri->hit = mi->map[ri->mapX][ri->mapY] - '0';
		}
		if (!ri->side)
			ri->dist = fabs((ri->mapX - ri->posX + (1 - ri->stepX) / 2) / rayDirX);
		else
			ri->dist = fabs((ri->mapY - ri->posY + (1 - ri->stepY) / 2) / rayDirY);
		int lineHeight = (int) (mi->resolution[1] / ri->dist);
		int drawStart = -lineHeight / 2 + mi->resolution[1] / 2;
		if (drawStart < 0)
			drawStart = 0;
		int drawEnd = lineHeight / 2 + mi->resolution[1] / 2;
		if (drawEnd >= mi->resolution[0])
			drawEnd = mi->resolution[0] - 1;
		test_draw(mi, mlx, drawStart, drawEnd, x);
	}
	//
	//mlx_destroy_image(mlx->mlx, mlx->img);

}


void draw(t_mi *mi)
{
	void 	*win;
	t_mlx   mlx;
	int		loop;
    t_ri    ri;
    initialize_ray(mi, &ri);
	mlx.mlx = mlx_init();
	mlx.win = mlx_new_window(mlx.mlx, mi->resolution[0], mi->resolution[1], "cub3D");
	mlx.img = mlx_new_image(mlx.mlx, mi->resolution[0], mi->resolution[1]);
	mlx_put_image_to_window(mlx.mlx, mlx.win, mlx.img, 0, 0);
	t_ha ha;
	ha.mi = mi;
	ha.mlx = &mlx;
	ha.ri = &ri;
	key_pressed(0, &ha);
    mlx_key_hook(mlx.win, key_pressed, &ha);
    loop = mlx_loop(mlx.mlx);

   // test(mi, &ri, &mlx);
//	if (mlx_key_hook(win, key_pressed, mi) == -1)
//	{
//		mlx_destroy_window(mlx.mlx, win);
//		clear_mi(mi);
//	}


}


int main (int argc, char **argv)
{
//	char *spaces = "\a\b\t\n\v\f\r ";
//	char *spacenum = "\a\b\t\n\v\f\r 0123456789";
//	char **identifier = {
//							"R",
//							"NO",
//							"SO",
//							"WE",
//							"EA",
//							"S",
//							"F",
//							"C",
//							"1"
//						};
//	char **error_msg = 	{
//							"0. INVALID MAP CONFIG. CHECK IDENTIFIERS USAGE.",
//							"9. INVALID MAP",
//							"10. SYSTEM ERROR (I.E. MEMORY ALLOCATION, OVERFLOW).",
//							"11. CHECK FOR SPACES AFTER PATH INFO. UNDEFINED BEHAVIOR.",
//							"12. filename length > 255",
//						};
	double step;
	t_mi mi;

	step = 1;
	if (import_config(argc, argv, &mi, step) >= 0)
	{
		draw(&mi);
	}
	else
		clear_mi(&mi);
	return (0);
}
