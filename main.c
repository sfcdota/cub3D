/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/12 15:55:36 by cbach             #+#    #+#             */
/*   Updated: 2020/11/02 04:43:18 by cbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	start(t_data *data)
{
	init_game(data->mi, data->mlx, data->img, data);
	render(-1, data);
	if (!data->mi->save)
	{
		mlx_hook(data->mlx->win,
		KEY_PRESS_EVENT, KEY_PRESS_MASK, render, data);
		mlx_hook(data->mlx->win,
		DESTROY_NOTIFY, DESTROY_NOTIFY_MASK, handle_exit, data);
		mlx_loop(data->mlx->mlx);
	}
	else
		create_bmp(data->mi, data);
	clear(data->mi, data);
}

int		main(int argc, char **argv)
{
	t_mi	mi;
	t_img	img;
	t_mlx	mlx;
	t_data	data;
	t_ray	ray;

	data.mlx = &mlx;
	data.img = &img;
	data.mi = &mi;
	data.ray = &ray;
	initialize_mi(&mi, &data);
	if (!(data.mlx->mlx = mlx_init()))
		prog_error(&data, SYS_ERROR);
	parsing(argc, argv, &mi, &data);
	start(&data);
	return (0);
}
