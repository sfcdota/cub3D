/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/17 02:06:30 by cbach             #+#    #+#             */
/*   Updated: 2020/11/03 23:20:23 by cbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
# include <stdlib.h>
# include <mlx.h>
# include <math.h>
# include <fcntl.h>
# include <errno.h>
# include <stdio.h>
# include "libft/libft.h"
# include "get_next_line/get_next_line.h"
# include "minilibx/mlx.h"
# define ROTATION_SPEED 0.25
# define MOVE_SPEED 0.5
# define PLANE_X 0.0
# define PLANE_Y 0.66
# define KEY_PRESS_EVENT 2
# define KEY_PRESS_MASK 0
# define DESTROY_NOTIFY 17
# define DESTROY_NOTIFY_MASK 17
# define RESOLUTION_WIDTH_MAX 20000
# define RESOLUTION_HEIGHT_MAX 20000
# define EXIT_MESSAGE "\nExit. Thanks for playing.\n"
# define SYS_ERROR 0
# define INVALID_RESOLUTION 1
# define INVALID_COLOR 2
# define INVALID_TEXTURE 3
# define DUPLICATE_FLAGS 4
# define MISSING_FLAG 5
# define INVALID_MAP 6
# define NO_START_PLAYER_POSITION 7
# define MULTIPLE_START_PLAYER_POSITION 8
# define MAP_NOT_CLOSED 9
# define INVALID_CONFIG 10
# define INVALID_ARGUMENT 11
# define NO_ARGUMENTS 12
# define ERROR_COUNT 13

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
	double		lines;
	double		max_line_length;
	char		**map;
	double		x;
	double		y;
	double		angle;
	int			save;
	int			sprites_count;
	t_sprite	*sprites;
	int			*sprites_order;
	double		*sprites_distance;
	double		r_speed;
	double		m_speed;
	char		*current_line;
	char		**error_messages;
	int			fd;
	int			fd_opened;
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
	double		pos_x;
	double		pos_y;
	double		dir_x;
	double		dir_y;
	double		plane_x;
	double		plane_y;
	double		camera_x;
	double		ray_dir_x;
	double		ray_dir_y;
	double		side_dist_x;
	double		side_dist_y;
	int			map_x;
	int			map_y;
	double		delta_dist_x;
	double		delta_dist_y;
	double		perp_dist;
	int			step_x;
	int			step_y;
	int			hit;
	int			side;
	int			line_height;
	int			draw_start;
	int			draw_end;
	double		wall_x;
	int			tex_num;
	int			tex_width;
	int			tex_height;
	int			tex_x;
	int			tex_y;
	double		step;
	double		tex_pos;
	int			x;
	int			y;
	double		sprite_x;
	double		sprite_y;
	double		inv_det;
	double		transform_x;
	double		transform_y;
	int			sprite_screen_x;
	int			sprite_height;
	int			sprite_width;
	int			draw_start_y;
	int			draw_end_y;
	int			draw_start_x;
	int			draw_end_x;
	int			stripe;
	double		*z_buffer;
	int			i;
	double		temp_d;
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
 **************		UTILS		**************
*/

void			init_texture(t_data *data, int index);
int				is_in_set(char c, char const *set);
void			set_player_start_pos(t_mi *mi, int i, int j);
int				is_space(char c);
char			*next_non_space(char *line);
char			*next_non_digit(char *line);
char			*next_till_eol(char *line);
char			*parse_path(char *begin, const char *end, t_data *data);
int				parse_pos_num(char **line, t_data *data);
int				create_trgb(int t, int r, int g, int b);
void			check_map_line(char *str, t_data *data);
char			*line_copy(double width, char *src, t_data *data);
void			map_list_to_array(t_mi *mi, t_data *data);
void			check_empty_params(t_mi *mi, t_data *data);
void			init_messages(t_mi *mi, t_data *data);
void			initialize_mi(t_mi *mi, t_data *data);

/*
 **************		FLAGS & MAP		**************
*/

void			parse_r(char **line, t_mi *mi, t_data *data);
void			parse_textures(char **line, t_mi *mi, int texture_index,
		t_data *data);
void			parse_color(char **line, t_mi *mi, int color_index,
		t_data *data);
void			parse_flag(char **line, t_mi *mi, t_data *data);
void			parse_map(char **line, t_mi *mi, int fd, t_data *data);
void			check_left(t_mi *mi, t_data *data, int i, int j);
void			check_right(t_mi *mi, t_data *data, int i, int j);
void			check_up(t_mi *mi, t_data *data, int i, int j);
void			check_down(t_mi *mi, t_data *data, int i, int j);
void			is_closed(t_mi *mi, t_data *data, int i, int j);
void			check_map(t_mi *mi, t_data *data);
void			parse_sprites_info(t_mi *mi, t_data *data);
void			parse_config(t_mi *mi, char *filename, t_data *data);
void			parsing(int argc, char **argv, t_mi *mi, t_data *data);
/*
 **************		ERRORS		**************
*/
void			clear_ptr(void *ptr);
void			clear_ptrs(void *ptr1, void *ptr2, void *ptr3, void *ptr4);
void			clear(t_mi *mi, t_data *data);
void			sys_error(t_data *data);
void			prog_error(t_data *data, int error_code);

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

unsigned int	get_img_color(t_img *img, int x, int y);
unsigned int	get_img_tex_color(t_texture *texture, int x, int y);
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
unsigned char	*init_bmp(int extrabytes, t_mi *mi, t_data *data);
void			create_bmp(t_mi *mi, t_data *data);
#endif
