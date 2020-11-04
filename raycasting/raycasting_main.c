/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_main.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 04:08:36 by cbach             #+#    #+#             */
/*   Updated: 2020/11/04 17:47:05 by cbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

int		render(int key, t_data *data)
{
	key_pressed(key, data->mi, data->ray, data);
	draw_walls(data->mi, data->ray, data);
	draw_sprites(data->mi, data->ray, data);
	if (!data->mi->save)
		mlx_put_image_to_window(data->mlx->mlx, data->mlx->win,
		data->img->img, 0, 0);
	return (1);
}
