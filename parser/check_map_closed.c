/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map_closed.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 23:28:13 by cbach             #+#    #+#             */
/*   Updated: 2020/11/02 23:29:01 by cbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	check_left(t_mi *mi, t_data *data, int i, int j)
{
	while (!is_in_set(mi->map[i][j], " 1") && j > 0)
		j--;
	if (is_in_set(mi->map[i][j], " 02NSWE"))
		prog_error(data, MAP_NOT_CLOSED);
}

void	check_right(t_mi *mi, t_data *data, int i, int j)
{
	while (!is_in_set(mi->map[i][j], " 1") && j < mi->max_line_length - 1)
		j++;
	if (is_in_set(mi->map[i][j], " 02NSWE"))
		prog_error(data, MAP_NOT_CLOSED);
}

void	check_up(t_mi *mi, t_data *data, int i, int j)
{
	while (!is_in_set(mi->map[i][j], " 1") && i > 0)
		i--;
	if (is_in_set(mi->map[i][j], " 02NSWE"))
		prog_error(data, MAP_NOT_CLOSED);
}

void	check_down(t_mi *mi, t_data *data, int i, int j)
{
	while (!is_in_set(mi->map[i][j], " 1") && i < mi->lines - 1)
		i++;
	if (is_in_set(mi->map[i][j], " 02NSWE"))
		prog_error(data, MAP_NOT_CLOSED);
}

void	is_closed(t_mi *mi, t_data *data, int i, int j)
{
	check_left(mi, data, i, j);
	check_right(mi, data, i, j);
	check_up(mi, data, i, j);
	check_down(mi, data, i, j);
}
