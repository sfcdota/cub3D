/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/17 02:06:30 by cbach             #+#    #+#             */
/*   Updated: 2020/11/02 04:49:02 by cbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
# ifndef FD_MAX_COUNT
#  define FD_MAX_COUNT 1024
# endif
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 128
# endif
# include <stdlib.h>
# include <mlx.h>
# include <math.h>
# include <fcntl.h>
# include <errno.h>
# include <stdio.h>
# include "libft/libft.h"
# include "get_next_line/get_next_line.h"
# define ROTATION_SPEED 0.25
# define MOVE_SPEED 0.5
# define PLANE_X 0.0
# define PLANE_Y 0.66
# define KEY_PRESS_EVENT 2
# define KEY_PRESS_MASK 0
# define DESTROY_NOTIFY 17
# define DESTROY_NOTIFY_MASK 17
# define EXIT_MESSAGE "\nExit. Thanks for playing.\n"

/*
 **************		STRUCTS		**************
*/


typedef struct	s_sprite
{
	double		x;
	double		y;
}				t_sprite;

/*
 ** resolution[index]: 0 = width; 1 = height;
 ** textures[index]: 0 = N; 1 = S; 2 = W; 3 = E; 4 = S;
 ** colors[index]: 0 = F; 1 = C;
 */
typedef struct	s_mi
{
	int			*resolution;
	char		**textures;
	int			*colors;
	t_list		*map_list;
	double 		lines;
	double		max_line_length;
	char		**map;
	int			error;
	double		x;
	double 		y;
	double 		angle;
	int			save;
	int 		numSprites;
	t_sprite	*sprites;
	int 		*spriteOrder;
	double 		*spriteDistance;
	double 		rSpeed;
	double 		mSpeed;
	char 		*current_line;

}				t_mi;

typedef struct	s_img
{
	void		*img;
	char		*addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
}				t_img;

typedef struct	s_mlx
{
	void		*mlx;
	void		*win;
}				t_mlx;

typedef struct	s_texture
{
	void		*img;
	int			width;
	int			height;
	char		*addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
}				t_texture;

typedef struct	s_ray
{
	double		posX;
	double		posY;
	double		dirX;
	double		dirY;
	double		planeX;
	double		planeY;
	double		cameraX;
	double		rayDirX;
	double		rayDirY;
	double		sideDistX;
	double		sideDistY;
	int			mapX;
	int			mapY;
	double		deltaDistX;
	double		deltaDistY;
	double		perpWallDist;
	int			stepX;
	int			stepY;
	int			hit;
	int			side;
	int			lineHeight;
	int			drawStart;
	int			drawEnd;
	double		wallX;
	int			texNum;
	int			texWidth;
	int			texHeight;
	int			texX;
	int			texY;
	double		step;
	double		texPos;
	int			x;
	int			y;
	double		spriteX;
	double		spriteY;
	double		invDet;
	double		transformX;
	double		transformY;
	int			spriteScreenX;
	int			spriteHeight;
	int			spriteWidth;
	int			drawStartY;
	int			drawEndY;
	int			drawStartX;
	int			drawEndX;
	int			stripe;
	double		*ZBuffer;
	int			i;
	double		temp_d;
	double		temp_d2;
	int			temp_i;
	int			temp_i2;
	int			temp_i3;
}				t_ray;

typedef struct	s_data
{
	t_mi		*mi;
	t_mlx		*mlx;
	t_img		*img;
	t_texture	*textures;
	t_ray		*ray;
}				t_data;

/*
 **************		ERRORS		**************
*/
void	clear_ptr(void *ptr);
void	clear_ptrs(void *ptr1, void *ptr2, void *ptr3, void *ptr4);
void	clear(t_mi *mi, t_data *data);
void	sys_error(t_data *data);
void	prog_error(t_data *data, char *message, int error_code);

/*
 **************		EVENTS		**************
*/

void			handle_move(int key, t_mi *mi, t_ray *ray);
void			handle_rotate(t_ray *ray, double rotation);
int				handle_exit(t_data *data);
void			key_pressed(int key, t_mi *mi, t_ray *ray, t_data *data);

/*
 **************		RAYCASTING		**************
*/

int				get_img_tex_color(t_texture *texture, int x, int y);
void			my_mlx_pixel_put(t_img *img, int x, int y, unsigned int color);
void			draw_ray(t_mi *mi, t_ray *ray, t_data *data);
void			sort_sprites(t_mi *mi, t_ray *ray);
void			set_walls_start_conditions(t_mi *mi, t_ray *ray);
void			perform_dda(t_mi *mi, t_ray *ray);
void			draw_walls(t_mi *mi, t_ray *ray, t_data *data);
void			sprites_start_conditions(t_mi *mi, t_ray *ray, t_data *data);
void			calc_sprites(t_mi *mi, t_ray *ray);
void			put_sprites(t_mi *mi, t_ray *ray, t_data *data);
void			draw_sprites(t_mi *mi, t_ray *ray, t_data *data);
int				render(int key, t_data *data);
void			init_game(t_mi *mi, t_mlx *mlx, t_img *img, t_data *data);


#endif
