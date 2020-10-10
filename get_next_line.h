/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/21 13:02:38 by cbach             #+#    #+#             */
/*   Updated: 2020/07/22 20:27:35 by cbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef FD_MAX_COUNT
#  define FD_MAX_COUNT 1024
# endif
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 0
# endif
# include <unistd.h>
# include <stdlib.h>
int		str_len(const char *s);
void	*ft_calloc(size_t n);
char	*ft_strdup(char *s);
int		destroy(void *p1, void *p2, void *p3, int status);
int		str_line_len(const char *s);
char	*str_join(char *s1, char *s2, int length);
int		read_line(int fd, char **line, char *buf, char **buffer_remains);
int		get_next_line(int fd, char **line);

#endif
