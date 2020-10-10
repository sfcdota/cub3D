#include "libft.h"
#include "get_next_line.h"
#include <fcntl.h>

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

int		is_in_set(char c, char const *set)
{
	while (*set)
		if (c == *set++)
			return (1);
	return (0);
}

char	*begin(char const *s1, char const *set)
{
	while (is_in_set(*s1, set))
		s1++;
	return ((char *)s1);
}

void	parse_r(char *line, t_mi *mi)
{

}

void	parse_no(char *line, t_mi *mi)
{

}

void	parse_so(char *line, t_mi *mi)
{

}

void	parse_we(char *line, t_mi *mi)
{

}

void	parse_ea(char *line, t_mi *mi)
{

}

void	parse_s(char *line, t_mi *mi)
{

}

void	parse_f(char *line, t_mi *mi)
{

}

void	parse_c(char *line, t_mi *mi)
{

}

void	parse_map(char *line, t_mi *mi, int fd)
{

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
		if (!is_all_spaces(line))
			counter++;
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
	char **input;
	char *spaces = {7,8,9,10,11,12,13,32};
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
	input = parse(open("i.cub", O_RDONLY));
	return (0);
}
