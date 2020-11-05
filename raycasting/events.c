/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 04:40:16 by cbach             #+#    #+#             */
/*   Updated: 2020/11/04 23:40:35 by cbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	handle_move(int key, t_mi *mi, t_ray *ray)
{
	ray->temp_d = key == 13 || key == 0 ? mi->m_speed : -mi->m_speed;
	if (key == 13 || key == 1)
	{
		if (mi->map[(int)(ray->pos_x + 1.1 * ray->dir_x * ray->temp_d)]
		[(int)ray->pos_y] == '0')
			ray->pos_x += ray->dir_x * ray->temp_d;
		if (mi->map[(int)(ray->pos_x)]
		[(int)(ray->pos_y + 1.1 * ray->dir_y * ray->temp_d)] == '0')
			ray->pos_y += ray->dir_y * ray->temp_d;
	}
	if (key == 0 || key == 2)
	{
		if (mi->map[(int)(ray->pos_x - 1.1 * ray->dir_y * ray->temp_d)]
		[(int)ray->pos_y] == '0')
			ray->pos_x -= ray->dir_y * ray->temp_d;
		if (mi->map[(int)(ray->pos_x)]
		[(int)(ray->pos_y + 1.1 * ray->dir_x * ray->temp_d)] == '0')
			ray->pos_y += ray->dir_x * ray->temp_d;
	}
}

void	handle_rotate(t_ray *ray, double rotation)
{
	ray->temp_d = ray->dir_x;
	ray->dir_x = ray->dir_x * cos(rotation) - ray->dir_y * sin(rotation);
	ray->dir_y = ray->temp_d * sin(rotation) + ray->dir_y * cos(rotation);
	ray->temp_d = ray->plane_x;
	ray->plane_x = ray->plane_x * cos(rotation) - ray->plane_y * sin(rotation);
	ray->plane_y = ray->temp_d * sin(rotation) + ray->plane_y * cos(rotation);
}

int		handle_exit(t_data *data)
{
	clear(data->mi, data);
	ft_putstr_fd(EXIT_MESSAGE, 1);
	exit(0);
}

void	key_pressed(int key, t_mi *mi, t_ray *ray, t_data *data)
{
	if (key == 0 || key == 1 || key == 2 || key == 13)
		handle_move(key, mi, ray);
	if (key == 123 || key == 124)
		handle_rotate(ray, key == 123 ? mi->r_speed : -mi->r_speed);
	if (key == 53)
		handle_exit(data);
}
