/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_walls.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 04:08:34 by cbach             #+#    #+#             */
/*   Updated: 2020/11/02 04:23:23 by cbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	draw_ray(t_mi *mi, t_ray *ray, t_data *data)
{
	ray->y = -1;
	while (++ray->y < mi->resolution[1])
	{
		if (ray->y < ray->draw_start)
			my_mlx_pixel_put(data->img, ray->x, ray->y, mi->colors[0]);
		if (ray->y >= ray->draw_start && ray->y < ray->draw_end)
		{
			ray->tex_y = (int)ray->tex_pos & (ray->tex_height - 1);
			ray->tex_pos += ray->step;
			my_mlx_pixel_put(data->img, ray->x, ray->y,
				get_img_tex_color(&data->textures[ray->tex_num], ray->tex_x,
				ray->tex_y));
		}
		if (ray->y >= ray->draw_end)
			my_mlx_pixel_put(data->img, ray->x, ray->y, mi->colors[1]);
	}
}

void	set_walls_start_conditions(t_mi *mi, t_ray *ray)
{
	ray->camera_x = 2.0 * ray->x / mi->resolution[0] - 1;
	ray->ray_dir_y = ray->dir_y + ray->plane_y * ray->camera_x;
	ray->ray_dir_x = ray->dir_x + ray->plane_x * ray->camera_x;
	ray->map_x = (int)ray->pos_x;
	ray->map_y = (int)ray->pos_y;
	ray->delta_dist_x = fabs(1 / ray->ray_dir_x);
	ray->delta_dist_y = fabs(1 / ray->ray_dir_y);
	ray->hit = 0;
	ray->step_y = ray->ray_dir_y < 0 ? -1 : 1;
	ray->side_dist_y =
		ray->ray_dir_y < 0 ? (ray->pos_y - ray->map_y) * ray->delta_dist_y
		: (ray->map_y + 1.0 - ray->pos_y) * ray->delta_dist_y;
	ray->step_x = ray->ray_dir_x < 0 ? -1 : 1;
	ray->side_dist_x =
		ray->ray_dir_x < 0 ? (ray->pos_x - ray->map_x) * ray->delta_dist_x
		: (ray->map_x + 1.0 - ray->pos_x) * ray->delta_dist_x;
}

void	perform_dda(t_mi *mi, t_ray *ray)
{
	while (ray->hit == 0)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->map_x += ray->step_x;
			ray->side = ray->ray_dir_x > 0 ? 0 : 1;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->map_y += ray->step_y;
			ray->side = ray->ray_dir_y > 0 ? 3 : 2;
		}
		if (mi->map[ray->map_x][ray->map_y] == '1')
			ray->hit = mi->map[ray->map_x][ray->map_y] - '0';
	}
	ray->perp_dist = ray->side == 0 || ray->side == 1 ?
		(ray->map_x - ray->pos_x + (1 - ray->step_x) / 2) / ray->ray_dir_x :
		(ray->map_y - ray->pos_y + (1 - ray->step_y) / 2) / ray->ray_dir_y;
	ray->line_height = (int)(mi->resolution[1] / ray->perp_dist);
	ray->draw_start = -ray->line_height / 2 + mi->resolution[1] / 2;
	ray->draw_start = ray->draw_start < 0 ? 0 : ray->draw_start;
	ray->draw_end = ray->line_height / 2 + mi->resolution[1] / 2;
}

void	draw_walls(t_mi *mi, t_ray *ray, t_data *data)
{
	ray->x = -1;
	while (++ray->x < mi->resolution[0])
	{
		set_walls_start_conditions(mi, ray);
		perform_dda(mi, ray);
		if (ray->draw_end >= mi->resolution[1])
			ray->draw_end = mi->resolution[1] - 1;
		ray->wall_x = ray->side == 0 || ray->side == 1 ?
		ray->pos_y + ray->perp_dist * ray->ray_dir_y :
		ray->pos_x + ray->perp_dist * ray->ray_dir_x;
		ray->wall_x -= floor(ray->wall_x);
		ray->tex_num = ray->side;
		ray->tex_width = data->textures[ray->tex_num].width;
		ray->tex_height = data->textures[ray->tex_num].height;
		ray->tex_x = (int)(ray->wall_x * 1.0 * (ray->tex_width));
		if ((!ray->side && ray->ray_dir_x > 0) ||
			(ray->side == 2 && ray->ray_dir_y < 0))
			ray->tex_x = ray->tex_width - ray->tex_x - 1;
		ray->step = 1.0 * ray->tex_height / ray->line_height;
		ray->tex_pos = (ray->draw_start - mi->resolution[1] / 2 +
		ray->line_height / 2) * ray->step;
		draw_ray(mi, ray, data);
		ray->z_buffer[ray->x] = ray->perp_dist;
	}
}
