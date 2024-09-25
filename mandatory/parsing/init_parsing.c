/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 14:03:11 by andie             #+#    #+#             */
/*   Updated: 2024/08/19 17:19:18 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include "parsing.h"

void	init_player(t_player_pos *player_pos)
{
	player_pos->dir = '\0';
	player_pos->x = 0.0;
	player_pos->y = 0.0;
	player_pos->plane_x = 0.0;
	player_pos->plane_y = 0.0;
}

void	init_info(t_map_info *map_info)
{
	map_info->fd = 0;
	map_info->lines = 0;
	map_info->path = NULL;
	map_info->file = NULL;
	map_info->height = 0;
	map_info->width = 0;
	map_info->index_end_of_map = 0;
}

int	init_parsing_info(t_cubp *cubp)
{
	cubp->north_path = NULL;
	cubp->south_path = NULL;
	cubp->east_path = NULL;
	cubp->west_path = NULL;
	cubp->map = NULL;
	cubp->floor = 0;
	cubp->ceiling = 0;
	cubp->x_floor = 0x0;
	cubp->x_ceiling = 0x0;
	init_player(&cubp->player_pos);
	init_info(&cubp->map_info);
	return (1);
}

static void	fill_structure(t_cub *cub, t_cubp *cubp)
{
	cub->north_path = cubp->north_path;
	cub->south_path = cubp->south_path;
	cub->east_path = cubp->east_path;
	cub->west_path = cubp->west_path;
	cub->map = cubp->map;
	printf("hello!!!!!!!!!!!!!!!!!!!!!!1\n");
	printf("address cub map:%p\n", cub->map);
	cub->floor = color_from_rgb(cubp->floor[0], cubp->floor[1], cubp->floor[2]);
	cub->ceiling = color_from_rgb(cubp->ceiling[0], cubp->ceiling[1], cubp->ceiling[2]);
	free_parsing(cubp);
}

int	parsingmap(char **argv, t_cub *cub)
{
	t_cubp	cubp;

	init_parsing_info(&cubp);
	if (parsing(&cubp, argv) == 0)
		return (0);
	fill_structure(cub, &cubp);
	return (1);
}
