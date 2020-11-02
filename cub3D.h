/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/17 02:06:30 by cbach             #+#    #+#             */
/*   Updated: 2020/05/29 23:52:45 by cbach            ###   ########.fr       */
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
# include <unistd.h>
# include <stdlib.h>
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
#endif
