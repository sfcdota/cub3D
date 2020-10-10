/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/21 13:02:35 by cbach             #+#    #+#             */
/*   Updated: 2020/07/22 22:38:08 by cbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*str_dup(char *s)
{
	char	*t;
	char	*bt;
	int		len;

	if (!(len = str_len(s)))
		return (NULL);
	if (!(t = ft_calloc(sizeof(char) * (len + 1))))
		return (NULL);
	bt = t;
	while (s && *s)
		*t++ = *s++;
	return (bt);
}

int		destroy(void *p1, void *p2, void *p3, int status)
{
	if (p1)
	{
		free(p1);
		p1 = NULL;
	}
	if (p2)
	{
		free(p2);
		p2 = NULL;
	}
	if (p3)
	{
		free(p3);
		p3 = NULL;
	}
	return (status > 0 ? 1 : status);
}

char	*str_join(char *s1, char *s2, int length)
{
	char *out;
	char *begin;
	char *oldbegin;

	if (!(out = ft_calloc(sizeof(char) * (length + 1))))
		return (NULL);
	begin = out;
	oldbegin = s1;
	while (s1 && *s1 && length--)
		*begin++ = *s1++;
	while (s2 && *s2 && length--)
		*begin++ = *s2++;
	if (oldbegin)
		free(oldbegin);
	return (out);
}

int		read_line(int fd, char **line, char *buf, char **buffer_remains)
{
	int		status;
	char	*temp;
	int		index;

	temp = NULL;
	status = str_len(buf);
	//в цикле идет reading of freed memory (buf) надо попробовать избавиться от него
	while ((index = str_line_len(buf)) > 0)
	{
		if (!(temp = str_join(temp, buf, str_len(buf) + str_len(temp))))
			status = -1;
		else
		{
			free(buf);
			buf = ft_calloc(sizeof(char) * (BUFFER_SIZE + 1));
			status = read(fd, buf, BUFFER_SIZE);
		}
	}
	*buffer_remains = str_dup(&buf[1 - index]);
	//ошибка снизу - в том, что если буф = "" и темп = нуль то внутри стрджоин все равно аллокейтится память, хотя у скарру выдает пустую строку
	*line = str_join(temp, buf, str_len(temp) - index);
	return (destroy(buf, NULL, NULL, status));
}

int		get_next_line(int fd, char **line)
{
	static char	*buffer_remains[FD_MAX_COUNT];
	char		*buf;
	int			status;

	status = 1;
	if (fd > FD_MAX_COUNT || BUFFER_SIZE < 1 || read(fd, buffer_remains[fd], 0))
		status = -1;
	if (buffer_remains[fd])
	{
		if (!(buf = str_dup(buffer_remains[fd])))
			status = -1;
		else
		{
			free(buffer_remains[fd]);
			buffer_remains[fd] = NULL;
		}
	}
	else
		status = read(fd,
		buf = ft_calloc(sizeof(unsigned char) * (BUFFER_SIZE + 1)), BUFFER_SIZE);
	return (status != -1 ? read_line(fd, line, buf, &buffer_remains[fd])
	: destroy(buf, NULL, NULL, status));
}
