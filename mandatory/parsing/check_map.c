/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 21:27:11 by andie             #+#    #+#             */
/*   Updated: 2024/08/19 03:13:40 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include "parsing.h"

// checks that there is no more info after a map
static int	map_end(t_cubp *cubp)
{
	int	y;
	int	x;

	y = cubp->map_info.index_end_of_map;
	while (cubp->map_info.file[y])
	{
		x = 0;
		while (cubp->map_info.file[y][x])
		{
			if (!(ft_isspace(cubp->map_info.file[y][x])))
				return (0);
			x++;
		}
		y++;
	}
	return (1);
}

//top and bottom are surrounded by walls
static int	check_top_bottom(char **map_tab, int i, int j)
{
	if (!map_tab || !map_tab[i] || !map_tab[i][j])
		return (0);
	while (ft_isspace_no_nl(map_tab[i][j]))
		j++;
	while (map_tab[i][j])
	{
		if (map_tab[i][j] != '1')
			return (0);
		j++;
	}
	return (1);
}

int	map_sides(t_map_info *map_info, char **map)
{
	int	y;
	int	x;

	if (check_top_bottom(map, 0, 0) == 0)
		return (0);
	y = 1;
	while (y < (map_info->height - 1))
	{
		if (ft_strlen(map[y]) == 1 && map[y][0] == '1')
		{
			y++;
			continue ;
		}
		x = ft_strlen(map[y]) - 1;
		if (map[y][x] != '1')
			return (0);
		y++;
	}
	if (check_top_bottom(map, y, 0) == 0)
		return (0);
	return (1);
}

int	check_map(t_cubp *cubp, char **map)
{
	if (!cubp->map)
		return (print_msg("No map.", 0));
	if (map_sides(&cubp->map_info, map) == 0)
		return (print_msg("Map is not surrounded by walls.", 0));
	if (cubp->map_info.width < 3 || cubp->map_info.height < 3)
		return (print_msg("Map is too small.", 0));
	if (check_player(cubp, map) == 0)
		return (print_msg("Player not found or more than one.", 0));
	if (replace_player_with_floor(cubp, map) == 0)
		return (print_msg("Player not found or more than one.", 0));
	if (map_end(cubp) == 0)
		return (print_msg("Invalid map.", 0));
	return (1);
}
