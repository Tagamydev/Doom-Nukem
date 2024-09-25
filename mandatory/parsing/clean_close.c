/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_close.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andie <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 18:51:31 by andie             #+#    #+#             */
/*   Updated: 2024/08/24 18:51:34 by andie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include "parsing.h"

void	free_tab(void **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	if (tab)
	{
		free(tab);
		tab = NULL;
	}
}

void	free_textures(t_cubp *cubp)
{
	if (cubp->north_path)
	{
		free(cubp->north_path);
		cubp->north_path = NULL;
	}
	if (cubp->south_path)
	{
		free(cubp->south_path);
		cubp->south_path = NULL;
	}
	if (cubp->west_path)
	{
		free(cubp->west_path);
		cubp->west_path = NULL;
	}
	if (cubp->east_path)
	{
		free(cubp->east_path);
		cubp->east_path = NULL;
	}
}

void	free_parsing(t_cubp *cubp)
{
	if (!cubp)
		exit(1);
	if (cubp->map_info.fd > 0)
		close(cubp->map_info.fd);
	if (cubp->map_info.file)
		free_tab((void **)cubp->map_info.file);
	if (cubp->map)
		free_tab((void **)cubp->map);
	free_textures(cubp);
	exit(1);
}
