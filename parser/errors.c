/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 04:49:26 by cbach             #+#    #+#             */
/*   Updated: 2020/11/04 19:35:05 by cbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	clear_ptr(void *ptr)
{
	if (ptr)
	{
		free(ptr);
		ptr = NULL;
	}
}

void	clear_ptrs(void *ptr1, void *ptr2, void *ptr3, void *ptr4)
{
	clear_ptr(ptr1);
	clear_ptr(ptr2);
	clear_ptr(ptr3);
	clear_ptr(ptr4);
}

void	clear(t_mi *mi, t_data *data)
{
	int i;

	clear_ptrs(mi->resolution, mi->colors, mi->sprites, mi->sprites_distance);
	i = -1;
	while (mi->map && ++i < mi->lines && mi->map[i])
		clear_ptr(mi->map[i]);
	i = -1;
	while (++i < 5 && mi->textures[i])
		clear_ptrs(data->textures[i].img,
		NULL, mi->textures[i], NULL);
	i = -1;
	while (++i < ERROR_COUNT)
		clear_ptr(mi->error_messages[i]);
	clear_ptrs(data->textures, mi->sprites_order, mi->error_messages, NULL);
	clear_ptrs(data->img->img, NULL, mi->map_list, mi->textures);
	clear_ptrs(data->mlx->win, data->mlx->mlx, mi->map, NULL);
	if (mi->fd_opened)
	{
		if ((close(mi->fd)) < 0)
			sys_error(data);
		mi->fd_opened = 0;
	}
}

void	sys_error(t_data *data)
{
	perror(data->mi->error_messages[SYS_ERROR]);
	clear(data->mi, data);
	exit(errno);
}

void	prog_error(t_data *data, int error_code)
{
	errno = EINVAL;
	perror(ft_strjoin("Error\n", data->mi->error_messages[error_code]));
	clear(data->mi, data);
	exit(200 + abs(error_code));
}
