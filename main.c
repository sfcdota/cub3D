#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "get_next_line.h"

int		is_in_set(char c, char const *set)
{
	while (*set)
		if (c == *set++)
			return (1);
	return (0);
}

int 	is_space(char c)
{
	return (c > 6 && c < 14) || c == 32 ? 1 : 0;
}

int		is_digit(char c)
{
	return (c > 47 && c < 58 ? 1 : 0);
}


void 	next_non_space(char **line)
{
	char *next;

	next = *line;
	while (*next && is_space(*next))
		next++;
}

void	next_till_space(char **line)
{
	char *next;

	next = *line;
	while (*next && !is_space(*next))
		next++;
}


int			ft_atoi_modified(const char **nptr)
{
	int	i;
	char *ptr;

	ptr = *nptr;
	i = 0;
	while (is_space(*ptr))
		ptr++;
	while (is_digit(*ptr))
		if (i > 1000000000)
			return (-1);
		else
			i = i * 10 + (*ptr++ - 48);
	return i;
}

int			ft_atoi_color(const char **nptr)
{
	int	i;
	char *ptr;

	ptr = *nptr;
	i = 0;
	while (is_space(*ptr))
		ptr++;
	while (is_digit(*ptr))
		if (i > 255)
			return (-1);
		else
			i = i * 10 + (*ptr++ - 48);
	if (*ptr == '.')
		while (is_digit(*ptr))
			ptr++;
	next_non_space(ptr);
	return i;
}


typedef struct		s_map
{
	char			*map_line;
	struct s_map	*next;
}					t_map;


typedef struct		s_mi
{
	int				width;
	int				height;
	char			*no;
	char			*so;
	char			*we;
	char			*ea;
	char			*s;
	int				*f;
	int				*c;
	char			**map;
	int				map_height;
	int				error;
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

void	parse_r(char *line, t_mi *mi)
{
	mi->width = ft_atoi_modified(&line);
	mi->height = ft_atoi_modified(&line);
	next_non_space(&line);
	if (mi->width * mi->height == 0 || *line)
		mi->error = 0;
}

void	parse_textures(char *line, t_mi *mi, char identifier)
{
	char *begin;

	next_non_space(&line);
	begin = line;
	next_till_space(&line);
	if (*line != '\0')
	{
		mi->error = 11;
		return ;
	}
	if (line - begin > 255)
	{
		mi->error = 12;
		return ;
	}
	if (identifier == 'n')
		mi->no = parse_path(begin, line);
	if (identifier == 's')
		mi->so = parse_path(begin, line);
	if (identifier == 'w')
		mi->we = parse_path(begin, line);
	if (identifier == 'e')
		mi->ea = parse_path(begin, line);
	if (identifier == 'p')
		mi->s = parse_path(begin, line);
}

void	parse_color(char *line, t_mi *mi, char identifier)
{
	int color;
	int multiplier;
	int temp;

	multiplier = 1000000;
	color = 0;

	while (multiplier && line)
	{
		if ((temp = ft_atoi_color(&line)) >= 0)
		{
			color += temp * multiplier;
			multiplier /= 1000;
			if (*line == ',' && multiplier)
				line++;
			else if (*line)
					line = NULL;
		}
	}
	mi->error == line ? mi->error : NULL;
	if (identifier == 'f' && mi->error >= 0)
		mi->f = color;
	if (identifier == 'c' && mi->error >= 0)
		mi->c = color;
}

t_map	*mapnew(char *content)
{
	t_map *t;

	t = malloc(sizeof(t_map));
	if (t)
	{
		t->content = content;
		t->next = NULL;
	}
	return (t);
}


void	map_add_back(t_map **lst, t_map *new)
{
	t_map *t;

	if (lst && new)
	{
		new->next = NULL;
		if (!*lst)
			*lst = new;
		else
		{
			t = *lst;
			while (t->next)
				t = t->next;
			t->next = new;
		}
	}
}

void	mapdelone(t_map *lst)
{
	if (lst)
	{
		free(lst->map_line);
		free(lst);
		lst = NULL;
	}
}


void	mapclear(t_map **lst)
{
	t_map *t;

	while (lst && *lst)
	{
		t = (*lst)->next;
		mapdelone(*lst);
		*lst = t;
	}
	lst = NULL;
}


int 	correct_begin(char *line)
{
	int correct;
	int position;

	position = 0;
	correct = 1;
	next_non_space(&line);
	if (*line != '1')
		correct = 0;
	while (correct)
	{
		if ()
	}
}

int 	correct_end(char *line)
{
	while (*line)
		line++;
	line--;
	while (is_space(*line))
		line--;
	return *line == '1' ? 1 : 0;
}

void	parse_map(char *line, t_mi *mi, int fd)
{
	t_map *map;
	int counter;
	int status;

	counter = 1;
	map_add_back(&map, mapnew(line));
	free(line);
	while ((status = get_next_line(fd, &line)))
	{
		map_add_back(&map, mapnew(line));
		free(line);
	}
}

void	parse_error(int fd)
{

}

char **parse(int fd)
{
	char *line;
	int status;
	int counter;
	char **array;
	int i;
	counter = 0;

	while ((status = get_next_line(fd, &line)) > 0)
	{

		free(line);
	}
	array = malloc (sizeof(char *) * counter);
	close(fd);
	fd = open("i.cub", O_RDONLY);
	i = 0;
	while (i < counter)
	{
		if (!is_all_spaces(line))
			array[i] = ft_strdup(line);
	}
}

int main (int argc, char **argv)
{
	char *spaces = "\a\b\t\n\v\f\r ";
	char *spacenum = "\a\b\t\n\v\f\r 0123456789";
	char **identifier = {
							"R",
							"NO",
							"SO",
							"WE",
							"EA",
							"S",
							"F",
							"C",
							"1"
						};
	char **error_msg = 	{
							"0. INVALID MAP CONFIG. CHECK IDENTIFIERS USAGE.",
							"9. INVALID MAP",
							"10. SYSTEM ERROR (I.E. MEMORY ALLOCATION, OVERFLOW).",
							"11. CHECK FOR SPACES AFTER PATH INFO. UNDEFINED BEHAVIOR.",
							"12. filename length > 255",
						};
	return (0);
}
