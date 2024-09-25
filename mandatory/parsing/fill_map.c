/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andie <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 00:18:46 by andie             #+#    #+#             */
/*   Updated: 2024/08/25 00:18:47 by andie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	fill_map(t_cubp *cubp, char **file, int i)
{
	if (map_alloc_height(cubp, file, i) == 0)
		return (0);
	change_space_to_empty(cubp);
	return (1);
}
