/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sprites.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 04:08:24 by cbach             #+#    #+#             */
/*   Updated: 2020/11/02 04:08:41 by cbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	sort_sprites(t_mi *mi, t_ray *ray)
{
	ray->temp_i = -1;
	while (++ray->temp_i < mi->sprites_count)
	{
		ray->temp_i2 = -1;
		while (++ray->temp_i2 < mi->sprites_count - 1)
		{
			if (mi->sprites_distance[ray->temp_i2] <
			mi->sprites_distance[ray->temp_i2 + 1])
			{
				ray->temp_i3 = mi->sprites_order[ray->temp_i2];
				mi->sprites_order[ray->temp_i2] =
						mi->sprites_order[ray->temp_i2 + 1];
				mi->sprites_order[ray->temp_i2 + 1] = ray->temp_i3;
				ray->temp_d = mi->sprites_distance[ray->temp_i2];
				mi->sprites_distance[ray->temp_i2] =
						mi->sprites_distance[ray->temp_i2 + 1];
				mi->sprites_distance[ray->temp_i2 + 1] = ray->temp_d;
			}
		}
	}
}

void	sprites_start_conditions(t_mi *mi, t_ray *ray, t_data *data)
{
	ray->i = -1;
	while (++ray->i < mi->sprites_count)
	{
		mi->sprites_order[ray->i] = ray->i;
		mi->sprites_distance[ray->i] =
			pow(ray->pos_x - mi->sprites[ray->i].x, 2) +
			pow(ray->pos_y - mi->sprites[ray->i].y, 2);
	}
	ray->i = -1;
	sort_sprites(mi, ray);
	ray->tex_num = 4;
	ray->tex_width = data->textures[ray->tex_num].width;
	ray->tex_height = data->textures[ray->tex_num].height;
}

void	calc_sprites(t_mi *mi, t_ray *ray)
{
	ray->sprite_x = mi->sprites[mi->sprites_order[ray->i]].x - ray->pos_x + 0.5;
	ray->sprite_y = mi->sprites[mi->sprites_order[ray->i]].y - ray->pos_y + 0.5;
	ray->inv_det =
		1.0 / (ray->plane_x * ray->dir_y - ray->dir_x * ray->plane_y);
	ray->transform_x = ray->inv_det *
		(ray->dir_y * ray->sprite_x - ray->dir_x * ray->sprite_y);
	ray->transform_y = ray->inv_det *
		(-ray->plane_y * ray->sprite_x + ray->plane_x * ray->sprite_y);
	ray->sprite_screen_x = (int)
		(mi->resolution[0] / 2 * (1 + ray->transform_x / ray->transform_y));
	ray->sprite_height = abs((int)(mi->resolution[1] / ray->transform_y));
	ray->draw_start_y = -ray->sprite_height / 2 + mi->resolution[1] / 2;
	if (ray->draw_start_y < 0)
		ray->draw_start_y = 0;
	ray->draw_end_y = ray->sprite_height / 2 + mi->resolution[1] / 2;
	if (ray->draw_end_y >= mi->resolution[1])
		ray->draw_end_y = mi->resolution[1] - 1;
	ray->sprite_width = abs((int)(mi->resolution[1] / ray->transform_y));
	ray->draw_start_x = -ray->sprite_width / 2 + ray->sprite_screen_x;
	if (ray->draw_start_x < 0)
		ray->draw_start_x = 0;
	ray->draw_end_x = ray->sprite_width / 2 + ray->sprite_screen_x;
	if (ray->draw_end_x >= mi->resolution[0])
		ray->draw_end_x = mi->resolution[0] - 1;
	ray->stripe = ray->draw_start_x - 1;
}

void	put_sprites(t_mi *mi, t_ray *ray, t_data *data)
{
	while (++ray->stripe < ray->draw_end_x)
	{
		ray->tex_x = ((int)(256.0 * (ray->stripe - (-ray->sprite_width / 2 +
			ray->sprite_screen_x)) * ray->tex_width / ray->sprite_width)) / 256;
		if (ray->transform_y > 0 && ray->stripe > 0
			&& ray->stripe < mi->resolution[0]
			&& ray->transform_y < ray->z_buffer[ray->stripe])
		{
			ray->y = ray->draw_start_y - 1;
			while (++ray->y < ray->draw_end_y)
			{
				ray->temp_i2 = ray->y * 256 - mi->resolution[1] * 128 +
					ray->sprite_height * 128;
				ray->tex_y = ((ray->temp_i2 * ray->tex_height) /
					ray->sprite_height) / 256;
				ray->temp_i2 = get_img_tex_color(&data->textures[ray->tex_num],
					ray->tex_x, ray->tex_y);
				if ((ray->temp_i2 & 0x00FFFFFFF) != 0)
					my_mlx_pixel_put(data->img, ray->stripe, ray->y,
						ray->temp_i2);
			}
		}
	}
}

void	draw_sprites(t_mi *mi, t_ray *ray, t_data *data)
{
	sprites_start_conditions(mi, ray, data);
	while (++ray->i < mi->sprites_count)
	{
		calc_sprites(mi, ray);
		put_sprites(mi, ray, data);
	}
}
