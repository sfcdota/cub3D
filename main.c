/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/12 15:55:36 by cbach             #+#    #+#             */
/*   Updated: 2020/10/12 20:05:57 by cbach            ###   ########.fr       */
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

typedef struct		s_img
{
	void *img;
	char *addr;
	int	 	bits_per_pixel;
	int 	line_length;
	int 	endian;
	int 	offset;
}					t_img;

typedef struct		s_mlx
{
	void *mlx;
	void *win;

}					t_mlx;

typedef struct		s_texture
{
	void *img;
	int width;
	int height;
}					t_texture;

typedef struct		s_ray
{
	double posX;
	double posY;
	double angle;
	double dirX;
	double dirY;
	double planeX;
	double planeY;
	double cameraX;
	double rayDirX;
	double rayDirY;
	double sideDistX;
	double sideDistY;
	int mapX;
	int mapY;
	double deltaDistX;
	double deltaDistY;
	double perpWallDist;
	int stepX;
	int stepY;
	int hit;
	int side;
	int lineHeight;
	int drawStart;
	int drawEnd;

}					t_ray;

typedef struct		s_data
{
	t_mi  *mi;
	t_mlx *mlx;
	t_img *img;
	t_texture *textures;
	t_ray *ray;
}					t_data;




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
	mi->map[(int)(mi->x)][(int)mi->y] = '0';
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

void print_info(t_mi *mi)
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

void	print_map(t_data *data)
{
	int i = 0;
	int j = 0;
	while (i < data->mi->lines)
	{
		j = 0;
		while (j < data->mi->max_line_length)
		{
			printf("%c", i == data->ray->posY && j == data->ray->posX ? 'x'
																		  : data->mi->map[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
	printf("ppos[0] = %lf\t ppos[1] = %lf\n"
		"angle = %lf", data->ray->posY, data->ray->posX, data->ray->angle);
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
	}
	return (status <= 0 ? -1 : 1);
}




int key_pressed(int key, t_data *data)
{
	double multiplier;
	double x_step;
	double y_step;

	double rotSpeed = 0.25;
	double moveSpeed = key == 13 || key == 0 ? 0.5 : -0.5;
	multiplier = key == 13 || key == 0 ? 1 : -1;
//	if (key == 13 || key == 1 || key == 0 || key == 2)
//	{
//		x_step = multiplier * cos(data->ray->angle + (key == 13 || key == 1 ? 0 : M_PI_2));
//		y_step = multiplier * sin(data->ray->angle +(key == 13 || key == 1 ? 0 : 3 * M_PI_2));
//		if (data->mi->map[(int)((data->ray->posX + x_step))][(int)(data->ray->posY)] == '0')
//			data->ray->posX += x_step;
//		if (data->mi->map[(int)(data->ray->posX)][(int)(data->ray->posY + y_step)] == '0')
//			data->ray->posY += y_step;
//	}

	if (key == 13)
	{
		if (data->mi->map[(int)(data->ray->posX + data->ray->dirX *moveSpeed)][(int)data->ray->posY] == '0')
			data->ray->posX += data->ray->dirX * moveSpeed;
		if (data->mi->map[(int)(data->ray->posX)][(int)(data->ray->posY + data->ray->dirY * moveSpeed)] == '0')
			data->ray->posY += data->ray->dirY * moveSpeed;
	}

	if (key == 1)
	{
		if (data->mi->map[(int)(data->ray->posX + data->ray->dirX * moveSpeed)][(int)data->ray->posY] == '0')
			data->ray->posX += data->ray->dirX * moveSpeed;
		if (data->mi->map[(int)(data->ray->posX)][(int)(data->ray->posY + data->ray->dirY * moveSpeed)] == '0')
			data->ray->posY += data->ray->dirY * moveSpeed;
	}
	if (key == 0)
	{
		if (data->mi->map[(int)(data->ray->posX + data->ray->dirY * moveSpeed)][(int)data->ray->posY] == '0')
			data->ray->posX += data->ray->dirY * moveSpeed;
		if (data->mi->map[(int)(data->ray->posX)][(int)(data->ray->posY + data->ray->dirX * moveSpeed)] == '0')
			data->ray->posY += data->ray->dirX * moveSpeed;
	}

	if (key == 2)
	{
		if (data->mi->map[(int)(data->ray->posX + data->ray->dirY * moveSpeed)][(int)data->ray->posY] == '0')
			data->ray->posX += data->ray->dirY * moveSpeed;
		if (data->mi->map[(int)(data->ray->posX)][(int)(data->ray->posY + data->ray->dirX * moveSpeed)] == '0')
			data->ray->posY += data->ray->dirX * moveSpeed;
	}
	if (key == 123 || key == 124)
	{
		rotSpeed *= key == 123 ? 1 : -1;
		if (key == 123) {
			if (data->ray->angle > 4 * M_PI)
				data->ray->angle -= 2 * M_PI;
		}
		if (key == 124) {
			if (data->ray->angle < -4 * M_PI)
				data->ray->angle += 2 * M_PI;
		}
//		data->ray->dirX = cos(data->ray->angle);
//		data->ray->dirY = sin(data->ray->angle);
		double oldDirX = data->ray->dirX;
		data->ray->dirX = data->ray->dirX * cos(rotSpeed) - data->ray->dirY * sin (rotSpeed);
		data->ray->dirY = oldDirX * sin(rotSpeed) + data->ray->dirY * cos(rotSpeed);
		double oldPlaneX = data->ray->planeX;
		data->ray->planeX = data->ray->planeX * cos(rotSpeed) - data->ray->planeY * sin (rotSpeed);
		data->ray->planeY = oldPlaneX * sin (rotSpeed) + data->ray->planeY * cos (rotSpeed);
		data->ray->angle += rotSpeed;
	}
	print_map(data);
	return(1);
}

void            my_mlx_pixel_put(t_img *img, int x, int y, unsigned int color)
{
	char *dst;

	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*(unsigned int *) dst = color;
}


void	verLine(int x, int drawStart, int drawEnd, t_data *data)
{
	for (int i = 0; i < data->mi->resolution[1]; i++)
		my_mlx_pixel_put(data->img, x, i, i >= drawStart && i <= drawEnd ? 0x00FF0000 : 0x000000FF);

}


void	calcDist(t_mi *mi, t_ray *ray, t_data *data)
{
	int i;

	i = -1;
	while(++i < mi->resolution[0])
	{
		ray->cameraX = 2 * i / (double)mi->resolution[1] - 1;
		ray->rayDirY = ray->dirY + ray->planeY * ray->cameraX;
		ray->rayDirX = ray->dirX + ray->planeX * ray->cameraX;
		ray->mapX = (int)ray->posX;
		ray->mapY = (int)ray->posY;
		ray->deltaDistX = fabs(1/ray->rayDirX);
		ray->deltaDistY = fabs(1/ray->rayDirY);
		ray->hit = 0;
		if (ray->rayDirY < 0)
		{
			ray->stepY = -1;
			ray->sideDistY = (ray->posY - ray->mapY) * ray->deltaDistY;
		}
		else
		{
			ray->stepY = 1;
			ray->sideDistY = (ray->mapY + 1.0 - ray->posY) * ray->deltaDistY;
		}
		if (ray->rayDirX < 0)
		{
			ray->stepX = -1;
			ray->sideDistX = (ray->posX - ray->mapX) * ray->deltaDistX;
		}
		else
		{
			ray->stepX = 1;
			ray->sideDistX = (ray->mapX + 1.0 - ray->posX) * ray->deltaDistX;
		}
		while (ray->hit == 0)
		{
			if (ray->sideDistX < ray->sideDistY)
			{
				ray->sideDistX += ray->deltaDistX;
				ray->mapX += ray->stepX;
				ray->side = 0;
			}
			else
			{
				ray->sideDistY += ray->deltaDistY;
				ray->mapY += ray->stepY;
				ray->side = 1;
			}
			if (mi->map[ray->mapX][ray->mapY] != '0')
				ray->hit = mi->map[ray->mapX][ray->mapY] - '0';
		}
		if (ray->side == 0)
			ray->perpWallDist = (ray->mapX - ray->posX + (1 - ray->stepX) / 2) / ray->rayDirX;
		else
			ray->perpWallDist = (ray->mapY - ray->posY + (1 - ray->stepY) / 2) / ray->rayDirY;
		ray->lineHeight = (int)(mi->resolution[1] / ray->perpWallDist);
		ray->drawStart = -ray->lineHeight / 2 + mi->resolution[1] / 2;
		if (ray->drawStart < 0)
			ray->drawStart = 0;
		ray->drawEnd = ray->lineHeight / 2 + mi->resolution[1] / 2;
		if (ray->drawEnd >= mi->resolution[1])
			ray->drawEnd = mi->resolution[1] - 1;
		double wallX;
		if (ray->side == 0)
			wallX = ray->posY + ray->perpWallDist * ray->rayDirY;
		else
			wallX = ray->posX + ray->perpWallDist * ray->rayDirX;
		wallX -= floor(wallX);
		int texX = (int)(wallX * (double)(data->textures[ray->side].width));
		if (side )
		verLine(i, ray->drawStart, ray->drawEnd, data);
	}
	mlx_put_image_to_window(data->mlx->mlx, data->mlx->win, data->img->img, 0, 0);
}


int render(int key, t_data *data)
{
	double dist;


	key_pressed(key, data);

	calcDist(data->mi, data->ray, data);

}



void import_textures(t_data *data)
{
	data->textures[0].img = mlx_xpm_file_to_image(data->mlx->mlx, data->mi->textures[0], &data->textures[0].width, &data->textures[0].height);
	data->textures[1].img = mlx_xpm_file_to_image(data->mlx->mlx, data->mi->textures[1], &data->textures[1].width, &data->textures[1].height);
	data->textures[2].img = mlx_xpm_file_to_image(data->mlx->mlx, data->mi->textures[2], &data->textures[2].width, &data->textures[2].height);
	data->textures[3].img = mlx_xpm_file_to_image(data->mlx->mlx, data->mi->textures[3], &data->textures[3].width, &data->textures[3].height);
	data->textures[4].img = mlx_xpm_file_to_image(data->mlx->mlx, data->mi->textures[4], &data->textures[4].width, &data->textures[4].height);

}


void	adjust_vectors(t_data *data)
{
	double diff_angle;

	diff_angle = data->ray->angle - M_PI_2;
	double oldDirX = data->ray->dirX;
	data->ray->dirX = data->ray->dirX * cos(diff_angle) - data->ray->dirY * sin (diff_angle);
	data->ray->dirY = oldDirX * sin(diff_angle) + data->ray->dirY * cos(diff_angle);
	double oldPlaneX = data->ray->planeX;
	data->ray->planeX = data->ray->planeX * cos(diff_angle) - data->ray->planeY * sin (diff_angle);
	data->ray->planeY = oldPlaneX * sin (diff_angle) + data->ray->planeY * cos (diff_angle);
	data->ray->angle += diff_angle;
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
	t_img img;
	t_mlx mlx;
	t_data data;
	t_ray ray;
	t_texture *textures;
	textures = malloc (sizeof(t_texture) * 5);
	step = 1;
	if (import_config(argc, argv, &mi, step) >= 0)
	{
		mlx.mlx = mlx_init();
		mlx.win = mlx_new_window(mlx.mlx, mi.resolution[0], mi.resolution[1], "cub3D");
		img.img = mlx_new_image(mlx.mlx, mi.resolution[0], mi.resolution[1]);
		img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);
		data.mlx = &mlx;
		data.img = &img;
		data.mi = &mi;
		data.ray = &ray;
		ray.posX = data.mi->x;
		ray.posY = data.mi->y;
		ray.angle = data.mi->angle;
		ray.planeX = 0.0;
		ray.planeY = 0.66;
		ray.dirX = -1;
		ray.dirY = 0;
		if (ray.angle != M_PI_2)
			adjust_vectors(&data);
		import_textures(&data);
		render(-1, &data);
		mlx_hook(mlx.win, 2, 0, render, &data);
		mlx_loop(mlx.mlx);
	}
	else
		clear_mi(&mi);
	return (0);
}
