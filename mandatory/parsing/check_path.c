/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andie <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 15:50:09 by andie             #+#    #+#             */
/*   Updated: 2024/08/24 15:50:12 by andie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	check_exten_xpm(char *arg)
{
	size_t	length;

	length = ft_strlen(arg);
	if ((arg[length - 4] != '.' || arg[length - 3] != 'x'
			|| arg[length - 2] != 'p' || arg[length - 1] != 'm'))
		return (0);
	return (1);
}

int	check_path(char *texture_path)
{
	int	fd;

	if (check_dir(texture_path))
		return (print_msg("Path to texture is a directory.", 0));
	fd = open(texture_path, O_RDONLY);
	if (fd == -1)
	{
		return (print_msg("Path to texture is invalid.", 0));
	}
	close(fd);
	if (check_exten_xpm(texture_path) == 0)
		return (print_msg("Incorrect type of path texture.", 0));
	return (1);
}
