/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 04:08:29 by cbach             #+#    #+#             */
/*   Updated: 2020/11/04 17:46:51 by cbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"
#define FILE_HEADER_SIZE 14
#define INFO_HEADER_SIZE 40
#define OPEN_FLAGS O_WRONLY | O_TRUNC | O_APPEND | O_CREAT

void			init_bmp(int extrabytes, t_mi *mi, t_data *data)
{
	static int				filesize;
	static unsigned char	fheader[14] =
	{'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0};
	static unsigned char	iheader[40] =
	{40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0};

	filesize = FILE_HEADER_SIZE + INFO_HEADER_SIZE +
	(3 * data->mi->resolution[0] + extrabytes) * data->mi->resolution[1];
	fheader[2] = (unsigned char)(filesize);
	fheader[3] = (unsigned char)(filesize >> 8);
	fheader[4] = (unsigned char)(filesize >> 16);
	fheader[5] = (unsigned char)(filesize >> 24);
	iheader[4] = (unsigned char)(data->mi->resolution[0]);
	iheader[5] = (unsigned char)(data->mi->resolution[0] >> 8);
	iheader[6] = (unsigned char)(data->mi->resolution[0] >> 16);
	iheader[7] = (unsigned char)(data->mi->resolution[0] >> 24);
	iheader[8] = (unsigned char)(data->mi->resolution[1]);
	iheader[9] = (unsigned char)(data->mi->resolution[1] >> 8);
	iheader[10] = (unsigned char)(data->mi->resolution[1] >> 16);
	iheader[11] = (unsigned char)(data->mi->resolution[1] >> 24);
	if ((mi->fd = open("img.bmp", OPEN_FLAGS, 0666)) < 0)
		sys_error(data);
	mi->fd_opened = 1;
	if (write(mi->fd, fheader, 14) < 0 || write(mi->fd, iheader, 40) < 0)
		sys_error(data);
}

void			create_bmp(t_mi *mi, t_data *data)
{
	unsigned int	color;
	int				i;
	int				j;
	int				extrabytes;

	extrabytes = (4 - ((data->mi->resolution[0] * 3) % 4)) % 4;
	init_bmp(extrabytes, mi, data);
	i = data->mi->resolution[1];
	while (--i >= 0)
	{
		j = -1;
		while (++j < data->mi->resolution[0])
		{
			color = get_img_color(data->img, j, i);
			if (write(mi->fd, &color, 3) < 0)
				sys_error(data);
		}
		j = -1;
		while (++j < extrabytes)
			if (write(1, 0, 1) < 0)
				sys_error(data);
	}
	if (close(mi->fd) < 0)
		sys_error(data);
	mi->fd_opened = 0;
}

unsigned int	get_img_tex_color(t_texture *texture, int x, int y)
{
	return (*(unsigned int*)(texture->addr +
	(y * texture->line_length + x * (texture->bits_per_pixel / 8))));
}

unsigned int	get_img_color(t_img *img, int x, int y)
{
	return (*(unsigned int*)(img->addr +
		(y * img->line_length + x * (img->bits_per_pixel / 8))));
}

void			my_mlx_pixel_put(t_img *img, int x, int y, unsigned int color)
{
	*(unsigned int *)(img->addr + (y * img->line_length + x *
	(img->bits_per_pixel / 8))) = color;
}
