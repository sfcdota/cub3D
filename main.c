/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/12 15:55:36 by cbach             #+#    #+#             */
/*   Updated: 2020/10/30 17:59:22 by cbach            ###   ########.fr       */
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


typedef struct		s_sprite
{
	double x;
	double y;
}					t_sprite;


typedef struct		s_mi
{
	int				*resolution; // width = 0; height = 1;
	char			**textures; // n = 0, s = 1; w = 2; e = 3; s = 4;
	int				*colors; // f = 0; c = 1;
	t_list			*map_list;
	double 			lines;
	double			max_line_length;
	char			**map;
	int				error;
	double			x;
	double 			y;//x = 0; y = 1
	double 			angle;
	int				save;
	int 			numSprites;
	t_sprite		*sprites;
	int 			*spriteOrder;
	double 			*spriteDistance;
	double 			rSpeed;
	double 			mSpeed;

}					t_mi;

typedef struct		s_img
{
	void *img;
	char *addr;
	int	 	bits_per_pixel;
	int 	line_length;
	int 	endian;
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
	char *addr;
	int	 	bits_per_pixel;
	int 	line_length;
	int 	endian;
}					t_texture;

typedef struct		s_ray
{
	double posX;
	double posY;
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
	double wallX;
	int texNum;
	int texWidth;
	int texHeight;
	int texX;
	int texY;
	double step;
	double texPos;
	int x;
	int y;
	double spriteX;
	double spriteY;
	double invDet;
	double transformX;
	double transformY;
	int spriteScreenX;
	int spriteHeight;
	int spriteWidth;
	int drawStartY;
	int drawEndY;
	int drawStartX;
	int drawEndX;
	int stripe;
	double *ZBuffer;
	int i;
	double temp_d;
	double temp_d2;
	int temp_i;
}					t_ray;

typedef struct		s_data
{
	t_mi  *mi;
	t_mlx *mlx;
	t_img *img;
	t_texture *textures;
	t_ray *ray;
}					t_data;


void	del(void *content)
{
	free(content);
	content = NULL;
}

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
	{
		(*ptr)++;
		while (is_digit(**ptr))
			(*ptr)++;
	}
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

int		create_trgb(int t, int r, int g, int b)
{
	return(t << 24 | r << 16 | g << 8 | b);
}

void	parse_color(char **line, t_mi *mi, int color_index)
{
	int color;
	int multiplier;
	int *ha;
	int i;

	i = -1;
	ha = malloc (sizeof(int) * 3);

	*line += sizeof(char) * 1;
	multiplier = 1000000;
	color = 0;
	while (++i < 3 && *line)
	{
		if ((ha[i] = ft_atoi_color(line)) >= 0)
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
			if (mi->map[i][j] == '2')
				mi->numSprites++;
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

void	parse_sprites_info(t_mi *mi)
{
	int i;
	int j;

	i = -1;
	int s;
	s = 0;
	mi->spriteDistance = malloc (sizeof(double) * mi->numSprites);
	mi->spriteOrder = malloc (sizeof(int) * mi->numSprites);
	mi->sprites = malloc(sizeof(t_sprite) * mi->numSprites);
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
	mi->numSprites = 0;
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
	mi->save = 0;
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
		   "save = %d\n"
	 "ppos[0] = %lf\t ppos[1] = %lf\n"
  "angle = %lf\n", mi->resolution[0], mi->resolution[1],
		   mi->textures[0], mi->textures[1], mi->textures[2], mi->textures[3],
		   mi->textures[4], mi->colors[0], mi->colors[1], mi->save, mi->x, mi->y, mi->angle);

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
	printf("ppos[0] = %lf\t ppos[1] = %lf\n", data->ray->posY, data->ray->posX);
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

int import_config(int argc, char **argv, t_mi *mi)
{
	int status;

	//if ((argc == 2 || argc == 3) && correct_cub(argv[2]))
	if (argc == 2 || argc == 3)
	{
		initialize_mi(mi);
		status = parse(mi, argv[1]);
		if (argc == 3 && ft_strncmp("--shared", argv[2], 10))
			mi->save = 1;
		if (status != -1 && mi->numSprites)
		{
			parse_sprites_info(mi);
		}
	}
	else
		status = -1;
	if (status == -1 || mi->error >= 0)
		printf("there is an error ... ERROR CODE = %d\n", status);
	return (status <= 0 ? -1 : 1);
}

void	handle_move(int key, t_mi *mi, t_ray *ray)
{
	ray->temp_d = key == 13 || key == 0 ? mi->mSpeed : -mi->mSpeed;
	if (key == 13 || key == 1)
	{
		if (mi->map[(int)(ray->posX + 1.5 * ray->dirX * ray->temp_d)][(int)ray->posY] == '0')
			ray->posX += ray->dirX * ray->temp_d;
		if (mi->map[(int)(ray->posX)][(int)(ray->posY +  1.5 * ray->dirY * ray->temp_d)] == '0')
			ray->posY += ray->dirY * ray->temp_d;
	}
	if (key == 0 || key == 2)
	{
		if (mi->map[(int)(ray->posX - 1.5 * ray->dirY * ray->temp_d)][(int)ray->posY] == '0')
			ray->posX -= ray->dirY * ray->temp_d;
		if (mi->map[(int)(ray->posX)][(int)(ray->posY +  1.5 * ray->dirX * ray->temp_d)] == '0')
			ray->posY += ray->dirX * ray->temp_d;
	}

}

void	handle_rotate(int key, t_mi *mi, t_ray *ray)
{
	ray->temp_d2 = key == 123 ? mi->rSpeed : -mi->rSpeed;
	if (key == 123 || key == 124)
	{
		ray->temp_d = ray->dirX;
		ray->dirX = ray->dirX * cos(ray->temp_d2) - ray->dirY * sin(ray->temp_d2);
		ray->dirY = ray->temp_d * sin(ray->temp_d2) + ray->dirY * cos(ray->temp_d2);
		ray->temp_d = ray->planeX;
		ray->planeX = ray->planeX * cos(ray->temp_d2) - ray->planeY * sin (ray->temp_d2);
		ray->planeY = ray->temp_d * sin (ray->temp_d2) + ray->planeY * cos (ray->temp_d2);
	}
}

void handle_exit()
{

}


void key_pressed(int key, t_mi *mi, t_ray *ray)
{
	handle_move(key, mi, ray);
	handle_rotate(key, mi, ray);

}

int 			get_img_tex_color(t_texture *texture, int x, int y)
{
	return (*(unsigned int*)(texture->addr + (y * texture->line_length + x * (texture->bits_per_pixel / 8))));
}

void            my_mlx_pixel_put(t_img *img, int x, int y, unsigned int color)
{
	char *dst;

	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*(unsigned int *) dst = color;
}


void	drawRay(t_mi *mi, t_ray *ray, t_data *data)
{
	ray->y = -1;
	while (++ray->y < mi->resolution[1])
	{
		if (ray->y < ray->drawStart)
			my_mlx_pixel_put(data->img, ray->x, ray->y, mi->colors[0]);
		if (ray->y >= ray->drawStart && ray->y < ray->drawEnd)
		{
			ray->texY = (int)ray->texPos & (ray->texHeight - 1);
			ray->texPos += ray->step;
			my_mlx_pixel_put(data->img, ray->x, ray->y, get_img_tex_color(&data->textures[ray->texNum], ray->texX, ray->texY));
		}
		if (ray->y > ray->drawEnd)
			my_mlx_pixel_put(data->img, ray->x, ray->y, mi->colors[1]);
	}
}


void	sort_sprites(t_mi *mi)
{
	int i;
	int j;
	int temp1;
	double temp2;

	i = -1;
	while (++i < mi->numSprites)
	{
		j = -1;
		while (++j < mi->numSprites - 1)
		{
			if (mi->spriteDistance[j] < mi->spriteDistance[j + 1])
			{
				temp1 = mi->spriteOrder[j];
				mi->spriteOrder[j] = mi->spriteOrder[j + 1];
				mi->spriteOrder[j + 1] = temp1;
				temp2 = mi->spriteDistance[j];
				mi->spriteDistance[j] = mi->spriteDistance[j + 1];
				mi->spriteDistance[j + 1] = temp2;
			}
		}
	}
}

void	setWallsStartConditions(t_mi *mi, t_ray *ray, t_data *data)
{
	ray->cameraX = 2.0 * ray->x / mi->resolution[0] - 1;
	ray->rayDirY = ray->dirY + ray->planeY * ray->cameraX;
	ray->rayDirX = ray->dirX + ray->planeX * ray->cameraX;
	ray->mapX = (int) ray->posX;
	ray->mapY = (int) ray->posY;
	ray->deltaDistX = fabs(1 / ray->rayDirX);
	ray->deltaDistY = fabs(1 / ray->rayDirY);
	ray->hit = 0;
	ray->stepY = ray->rayDirY < 0 ? -1 : 1;
	ray->sideDistY = ray->rayDirY < 0 ? (ray->posY - ray->mapY) * ray->deltaDistY
									  : (ray->mapY + 1.0 - ray->posY) * ray->deltaDistY;
	ray->stepX = ray->rayDirX < 0 ? -1 : 1;
	ray->sideDistX = ray->rayDirX < 0 ? (ray->posX - ray->mapX) * ray->deltaDistX
									  : (ray->mapX + 1.0 - ray->posX) * ray->deltaDistX;
}

void	performDDA(t_mi *mi, t_ray *ray, t_data *data)
{
	while (ray->hit == 0)
	{
		if (ray->sideDistX < ray->sideDistY)
		{
			ray->sideDistX += ray->deltaDistX;
			ray->mapX += ray->stepX;
			ray->side = ray->rayDirX > 0 ? 0 : 1;
		}
		else
		{
			ray->sideDistY += ray->deltaDistY;
			ray->mapY += ray->stepY;
			ray->side = ray->rayDirY > 0 ? 3 : 2;
		}
		if (mi->map[ray->mapX][ray->mapY] == '1')
			ray->hit = mi->map[ray->mapX][ray->mapY] - '0';
	}
	ray->perpWallDist = ray->side == 0 || ray->side == 1 ?
						(ray->mapX - ray->posX + (1 - ray->stepX) / 2) / ray->rayDirX
														 :(ray->mapY - ray->posY + (1 - ray->stepY) / 2) / ray->rayDirY;
	ray->lineHeight = (int) (mi->resolution[1] / ray->perpWallDist);
	ray->drawStart = -ray->lineHeight / 2 + mi->resolution[1] / 2;
	ray->drawStart = ray->drawStart < 0 ? 0 : ray->drawStart;
	ray->drawEnd = ray->lineHeight / 2 + mi->resolution[1] / 2;
	ray->drawEnd = ray->drawEnd >= mi->resolution[1] ? mi->resolution[1] - 1 : ray->drawEnd;
}

void	drawWalls(t_mi *mi, t_ray *ray, t_data *data)
{
	ray->x = -1;
	while(++ray->x < mi->resolution[0])
	{
		setWallsStartConditions(mi, ray, data);
		performDDA(mi, ray, data);
		ray->wallX = ray->side == 0 || ray->side == 1 ?
			ray->posY + ray->perpWallDist * ray->rayDirY :
			ray->posX + ray->perpWallDist * ray->rayDirX;
		ray->wallX -= floor(ray->wallX);
		ray->texNum = ray->side;
		ray->texWidth = data->textures[ray->texNum].width;
		ray->texHeight = data->textures[ray->texNum].height;
		ray->texX = (int) (ray->wallX * 1.0 * (ray->texWidth));
		if ((!ray->side && ray->rayDirX > 0) ||
			(ray->side == 2 && ray->rayDirY < 0))
			ray->texX = ray->texWidth - ray->texX - 1;
		ray->step = 1.0 * ray->texHeight / ray->lineHeight;
		ray->texPos = (ray->drawStart - mi->resolution[1] / 2 +
							 ray->lineHeight / 2)
							* ray->step;
		drawRay(mi, ray, data);
		ray->ZBuffer[ray->x] =ray->perpWallDist;
	}
}

void	spritesStartConditions(t_mi *mi, t_ray *ray, t_data *data)
{
	ray->i = -1;
	while (++ray->i < mi->numSprites)
	{
		mi->spriteOrder[ray->i] = ray->i;
		mi->spriteDistance[ray->i] = ((ray->posX - mi->sprites[ray->i].x) * (ray->posX - mi->sprites[ray->i].x) +
									  (ray->posY - mi->sprites[ray->i].y) * (ray->posY - mi->sprites[ray->i].y));
	}
	ray->i = -1;
	sort_sprites(mi);
	ray->texNum = 4;
	ray->texWidth = data->textures[ray->texNum].width;
	ray->texHeight = data->textures[ray->texNum].height;
}

void	calcSprites(t_mi *mi, t_ray *ray, t_data *data)
{
	ray->spriteX =  mi->sprites[mi->spriteOrder[ray->i]].x - ray->posX + 0.5;
	ray->spriteY =  mi->sprites[mi->spriteOrder[ray->i]].y - ray->posY + 0.5;
	ray->invDet = 1.0 / (ray->planeX * ray->dirY - ray->dirX * ray->planeY);
	ray->transformX =
			ray->invDet * (ray->dirY * ray->spriteX - ray->dirX * ray->spriteY);
	ray->transformY =
			ray->invDet * (-ray->planeY * ray->spriteX + ray->planeX * ray->spriteY);
	ray->spriteScreenX =
			(int)(mi->resolution[0] / 2 * (1 + ray->transformX/ray->transformY));
	ray->spriteHeight = abs((int)(mi->resolution[1] / ray->transformY));
	ray->drawStartY = -ray->spriteHeight / 2 + mi->resolution[1] / 2;
	if (ray->drawStartY < 0)
		ray->drawStartY = 0;
	ray->drawEndY = ray->spriteHeight / 2 + mi->resolution[1] / 2;
	if (ray->drawEndY >= mi->resolution[1])
		ray->drawEndY = mi->resolution[1] - 1;
	ray->spriteWidth = abs((int)(mi->resolution[1] / ray->transformY));
	ray->drawStartX = -ray->spriteWidth / 2 + ray->spriteScreenX;
	if (ray->drawStartX < 0)
		ray->drawStartX = 0;
	ray->drawEndX = ray->spriteWidth / 2 + ray->spriteScreenX;
	if (ray->drawEndX >= mi->resolution[0])
		ray->drawEndX = mi->resolution[0] - 1;
	ray->stripe = ray->drawStartX - 1;
}

void	putSprites(t_mi *mi, t_ray *ray, t_data *data)
{
	while (++ray->stripe < ray->drawEndX)
	{
		ray->texX =
		((int)(256.0 * (ray->stripe - (-ray->spriteWidth / 2 + ray->spriteScreenX))
		* ray->texWidth / ray->spriteWidth)) / 256;
		if (ray->transformY > 0 && ray->stripe > 0 && ray->stripe < mi->resolution[0]
			&& ray->transformY < ray->ZBuffer[ray->stripe])
		{
			ray->y = ray->drawStartY - 1;
			while (++ray->y < ray->drawEndY)
			{
				ray->temp_i =
				ray->y * 256 - mi->resolution[1] * 128 + ray->spriteHeight * 128;
				ray->texY = ((ray->temp_i * ray->texHeight) / ray->spriteHeight)/256;
				//drawSprites(data, i);
				ray->temp_i = get_img_tex_color(&data->textures[ray->texNum],
						ray->texX, ray->texY);
				if ((ray->temp_i & 0x00FFFFFFF) != 0)
					my_mlx_pixel_put(data->img, ray->stripe, ray->y, ray->temp_i);
			}
		}
	}
}

void	drawSprites(t_mi *mi, t_ray *ray, t_data *data)
{
	spritesStartConditions(mi, ray, data);
	while (++ray->i < mi->numSprites)
	{
		calcSprites(mi, ray, data);
		putSprites(mi, ray, data);
	}
}

void	raycasting(t_mi *mi, t_ray *ray, t_data *data)
{
	drawWalls(mi,ray,data);
	drawSprites(mi, ray, data);
	mlx_put_image_to_window(data->mlx->mlx, data->mlx->win, data->img->img, 0, 0);
}


int render(int key, t_data *data)
{
	key_pressed(key, data->mi, data->ray);
	raycasting(data->mi, data->ray, data);
}

void	import_texture(t_data *data, int texNum)
{
	data->textures[texNum].img =
	mlx_xpm_file_to_image(data->mlx->mlx, data->mi->textures[texNum],
	&data->textures[texNum].width, &data->textures[texNum].height);
	data->textures[texNum].addr =
	mlx_get_data_addr(data->textures[texNum].img,
	&data->textures[texNum].bits_per_pixel, &data->textures[texNum].line_length,
	&data->textures[texNum].endian);

}


void import_textures(t_data *data)
{
	int i = -1;
	while (++i < 5)
	{
		import_texture(data, i);
	}
}


void	adjust_vectors(t_data *data)
{
	double diff_angle;

	diff_angle = data->mi->angle - M_PI_2;
	double oldDirX = data->ray->dirX;
	data->ray->dirX = data->ray->dirX * cos(diff_angle) - data->ray->dirY * sin (diff_angle);
	data->ray->dirY = oldDirX * sin(diff_angle) + data->ray->dirY * cos(diff_angle);
	double oldPlaneX = data->ray->planeX;
	data->ray->planeX = data->ray->planeX * cos(diff_angle) - data->ray->planeY * sin (diff_angle);
	data->ray->planeY = oldPlaneX * sin (diff_angle) + data->ray->planeY * cos (diff_angle);
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
	t_mi mi;
	t_img img;
	t_mlx mlx;
	t_data data;
	t_ray ray;
	t_texture *textures;
	textures = malloc (sizeof(t_texture) * 5);
	if (import_config(argc, argv, &mi) >= 0)
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
		ray.planeX = 0.0;
		ray.planeY = 0.66;
		ray.dirX = -1;
		ray.dirY = 0;
		mi.rSpeed = 0.5;
		mi.mSpeed = 0.5;
		ray.ZBuffer = malloc (mi.resolution[0] * sizeof(double));
		if (mi.angle != M_PI_2)
			adjust_vectors(&data);
		data.textures = textures;
		import_textures(&data);
		render(-1, &data);
		mlx_hook(mlx.win, 2, 0, render, &data);
		mlx_loop(mlx.mlx);
	}
	else
		clear_mi(&mi);
	return (0);
}
