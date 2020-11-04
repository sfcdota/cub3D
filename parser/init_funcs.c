/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_funcs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 04:42:35 by cbach             #+#    #+#             */
/*   Updated: 2020/11/04 19:35:01 by cbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	init_messages(t_mi *mi, t_data *data)
{
	if (!(mi->error_messages = malloc(13 * sizeof(char *))) ||
		!(mi->error_messages[0] = ft_strdup("SYSTEM ERROR")) ||
		!(mi->error_messages[1] = ft_strdup("INVALID RESOLUTION")) ||
		!(mi->error_messages[2] = ft_strdup("INVALID COLOR")) ||
		!(mi->error_messages[3] = ft_strdup("INVALID TEXTURE")) ||
		!(mi->error_messages[4] = ft_strdup("DUPLICATE FLAGS")) ||
		!(mi->error_messages[5] = ft_strdup("MISSING FLAG")) ||
		!(mi->error_messages[6] = ft_strdup("INVALID MAP")) ||
		!(mi->error_messages[7] = ft_strdup("NO START POSITION")) ||
		!(mi->error_messages[8] = ft_strdup("MULTIPLE START POSITION")) ||
		!(mi->error_messages[9] = ft_strdup("MAP NOT CLOSED")) ||
		!(mi->error_messages[10] = ft_strdup("INVALID CONFIG")) ||
		!(mi->error_messages[11] = ft_strdup("INVALID ARGUMENT")) ||
		!(mi->error_messages[12] = ft_strdup("NO ARGUMENTS")))
		sys_error(data);
}

void	initialize_mi2(t_mi *mi)
{
	int i;

	mi->save = 0;
	mi->sprites_count = 0;
	mi->sprites = NULL;
	mi->sprites_order = NULL;
	mi->sprites_distance = NULL;
	mi->r_speed = ROTATION_SPEED;
	mi->m_speed = MOVE_SPEED;
	mi->current_line = NULL;
	mi->fd = -228;
	mi->fd_opened = 0;
	i = -1;
	while (++i < 5)
	{
		if (i < 2)
		{
			mi->colors[i] = -1;
			mi->resolution[i] = -1;
		}
		mi->textures[i] = NULL;
	}
}

void	initialize_mi(t_mi *mi, t_data *data)
{
	int i;

	i = -1;
	if (!(data->textures = malloc(sizeof(t_texture) * 5)))
		sys_error(data);
	while (++i < 5)
	{
		data->textures[i].img = NULL;
		data->textures[i].addr = NULL;
	}
	if (!((mi->colors = malloc(sizeof(int) * 2)) &&
	(mi->textures = malloc(sizeof(char *) * 5)) &&
	(mi->resolution = malloc(sizeof(int) * 2))))
		sys_error(data);
	mi->map_list = NULL;
	mi->lines = 0;
	mi->max_line_length = 0;
	mi->map = NULL;
	mi->x = -1;
	mi->y = -1;
	mi->angle = -100;
	initialize_mi2(mi);
	init_messages(mi, data);
}

void	init_game(t_mi *mi, t_mlx *mlx, t_img *img, t_data *data)
{
	if (!mi->save)
	{
		mlx->win =
				mlx_new_window(mlx->mlx, mi->resolution[0], mi->resolution[1],
							"cub3D");
		if (!mlx->win)
			sys_error(data);
	}
	img->img = mlx_new_image(mlx->mlx, mi->resolution[0], mi->resolution[1]);
	if (!img->img)
		sys_error(data);
	img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel,
		&img->line_length, &img->endian);
	data->ray->pos_x = data->mi->x;
	data->ray->pos_y = data->mi->y;
	data->ray->plane_x = PLANE_X;
	data->ray->plane_y = PLANE_Y;
	data->ray->dir_x = -1;
	data->ray->dir_y = 0;
	if (mi->angle != M_PI_2)
		handle_rotate(data->ray, mi->angle - M_PI_2);
	if (!(data->ray->z_buffer = malloc(mi->resolution[0] * sizeof(double))))
		sys_error(data);
}
