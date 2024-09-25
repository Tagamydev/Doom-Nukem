/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_dir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 22:56:39 by andie             #+#    #+#             */
/*   Updated: 2024/08/24 23:01:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include "parsing.h"

static void	north_south(t_player_pos *player_pos)
{
	if (player_pos->dir == 'S')
	{
		player_pos->x = 0;
		player_pos->y = 1;
		player_pos->plane_x = -0.66;
		player_pos->plane_y = 0;
	}
	else if (player_pos->dir == 'N')
	{
		player_pos->x = 0;
		player_pos->y = -1;
		player_pos->plane_x = 0.66;
		player_pos->plane_y = 0;
	}
	else
		return ;
}

static void	east_west(t_player_pos *player_pos)
{
	if (player_pos->dir == 'W')
	{
		player_pos->x = -1;
		player_pos->y = 0;
		player_pos->plane_x = 0;
		player_pos->plane_y = -0.66;
	}
	else if (player_pos->dir == 'E')
	{
		player_pos->x = 1;
		player_pos->y = 0;
		player_pos->plane_x = 0;
		player_pos->plane_y = 0.66;
	}
	else
		return ;
}

void	player_dir(t_cubp *cubp)
{
	north_south(&cubp->player_pos);
	east_west(&cubp->player_pos);
}
