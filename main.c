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

int	main(int argc, char **argv)
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
	init_messages(&mi, &data);
	initialize_mi(&mi, &data);
	if (!(data.mlx->mlx = mlx_init()))
		prog_error(&data, SYS_ERROR);
	parsing(argc, argv, &mi, &data);
	init_game(&mi, &mlx, &img, &data);
	render(-1, &data);
	if (!mi.save)
	{
		mlx_hook(mlx.win, KEY_PRESS_EVENT, KEY_PRESS_MASK, render, &data);
		mlx_hook(mlx.win, DESTROY_NOTIFY, DESTROY_NOTIFY_MASK,
			handle_exit, &data);
		mlx_loop(mlx.mlx);
	}
	return (0);
}
