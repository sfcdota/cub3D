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
int		is_in_set(char c, char const *set)
{
	while (*set)
		if (c == *set++)
			return (1);
	return (0);
}

int 	set_index(char c, char const *set)
{
	int index;

	index = 0;
	while (*set)
	{
		if (c == *set++)
			return index;
		index++;
	}
	return (-1);
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


typedef struct		s_map
{
	char			*map_line;
	struct s_map	*next;
}					t_map;


typedef struct		s_mi
{
	int				*resolution; // width = 0; height = 1;
	char			**textures; // n = 0, s = 1; w = 2; e = 3; s = 4;
	int				*colors; // f = 0; c = 1;
	t_list			*map_list;
	int 			lines;
	int				max_line_length;
	char			**map;
	int				error;
	int 			direction; // n = 0, s = 1, w = 2, e = 3
	int				isShared;
}					t_mi;


char	*parse_path(char *begin, char *end)
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

	*line += sizeof(char) * (texture_index < 4 ? 2 : 1);
	*line = next_non_space(*line);
	temp = *line;
	*line = next_till_space(*line);
	if (**line != '\0')
	{
		mi->error = 11;
		return ;
	}
	if (*line - temp > 255)
	{
		mi->error = 12;
		return ;
	}
	if ((temp = parse_path(temp, *line)) == NULL)
		mi->error = 10;
	else
		mi->textures[texture_index] = temp;
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

void	parse_flag(char **line, t_mi *mi, int fd, char *filename)
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

int 	correct_sides_and_spaces(char *line)
{
	int correct;
	char *begin;
	char *end;

	correct = 1;
	while (*line && (is_space(*line) || *line == '1'))
		line++;
	begin = line;
	while (*line)
		line++;
	line--;
	while (line > begin && (is_space(*line) || *line == '1'))
		line--;
	end = line;
	if (*(begin - 1) != '1' && *(end + 1) != '1')
		correct = 0;
//	while (correct && begin < end)
//	{
//		if (is_space(*begin))
//			correct = 0;
//		begin++;
//	}
	return (correct);
}

int 	get_line_info(char *line, int *directions, int *max_width, int *lines)
{
	int counter;

	if (correct_sides_and_spaces(line))
	{
		counter = 0;
		*lines += 1;
		while (*line)
		{
			counter++;
			if (is_in_set(*line, "NSEW"))
				*directions += 1;
			line++;
		}
		if (*max_width < counter)
			*max_width = counter;
		return *directions > 1 ? -1 : 1;

	}
	return -2;
}

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


char	*line_copy(int width, char *src)
{
	char *dest;
	int j;

	j = 0;
	if(!(dest = malloc(width + 1)))
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

	i = 0;
	while (i < mi->lines)
	{
		j = 0;
		while (j < mi->max_line_length)
		{
			if (is_in_set(mi->map[i][j], "NSWE"))
			{
				if (mi->direction != -1)
					return (-1);
				else
					mi->direction = set_index(mi->map[i][j], "NSWE");
			}
			if (j == 0 || j == mi->max_line_length - 1 || i == 0 || i == mi->lines - 1)
			{
				if (is_in_set(mi->map[i][j], "NSWE0"))
					return (-1);
			}
			else if (mi->map[i - 1][j] == ' ' || mi->map[i + 1][j] == ' ' || mi->map[i][j - 1] == ' ' || mi->map[i][j + 1] == ' ')
					return (-1);
			j++;
		}
		i++;
	}
	return (1);
}

int	map_list_to_array(t_mi *mi)
{
	char **map;
	int i;
	t_list *map_list;

	map_list = mi->map_list;
	i = 0;
	if(!(map = malloc(sizeof(char *) * mi->lines)))
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
			parse_flag(&temp, mi, fd, filename);
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
	return status;
}

t_mi	*initialize_mi(t_mi *mi)
{
	mi->max_line_length = 0;
	mi->lines = 0;
	mi->map_list = NULL;
	mi->map = NULL;
	mi->colors = malloc(sizeof(int) * 2);
	mi->textures = malloc(sizeof(char *) * 5);
	mi->resolution = malloc (sizeof(int) * 2);
	mi->error = -100;
	mi->isShared = 0;
	mi->direction = -1;
	return (mi);
}

int correct_cub(char *line)
{
	int length;

	length = ft_strlen(line);
	if (line[length - 1] == 'b' && line[length - 2] == 'u' && line[length - 3] == 'c')
		return 1;
	return 0;
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
	int status;
	t_mi temp;
	t_mi *mi = &temp;
	status = 0;
	if (argc == 2 || argc == 3)
	{
		status = parse(initialize_mi(mi), argv[1]);
		if (argc == 3 && ft_strncmp("--shared", argv[2], 10))
			mi->isShared = 1;
	}
	else
		status = -1;
	if (status == -1)
		printf("there is an error ... ERROR CODE = %d\n", status); //ERROR NA PARSINGE!!!!!
	else
	{
		int i = 0;
		while (i < mi->lines) {
			printf("%s\n", mi->map[i]);
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
			   "isShared = %d\n", mi->resolution[0], mi->resolution[1],
			   mi->textures[0], mi->textures[1], mi->textures[2], mi->textures[3],
			   mi->textures[4], mi->colors[0], mi->colors[1], mi->isShared);
	}
	return (0);
}
