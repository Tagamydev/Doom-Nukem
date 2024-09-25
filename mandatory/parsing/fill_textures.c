/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_textures.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 00:59:44 by andie             #+#    #+#             */
/*   Updated: 2024/08/16 02:52:51 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include "parsing.h"

char	*texture_path(char *line, int i)
{
	int		j;
	int		len;
	char	*path;

	while (line[i] && ft_isspace_no_nl(line[i]))
		i++;
	len = i;
	while (line[len] && !ft_isspace_no_nl(line[len]))
		len++;
	path = malloc(sizeof(char) * (len - i + 1));
	if (path == NULL)
		return (NULL);
	j = 0;
	while (line[i] && !ft_isspace_no_nl(line[i]) && line[i] != '\n')
		path[j++] = line[i++];
	path[j] = '\0';
	while (line[i] && ft_isspace_no_nl(line[i]))
		i++;
	if (line[i] && line[i] != '\n')
	{
		free(path);
		path = NULL;
	}
	return (path);
}

int ft_isprint_no_ws(int c)
{
	if (c >= 33 && c < 127)
		return (1);
	return (0);
}

int	fill_textures(t_cubp *cubp, char *line, int i)
{
	if (line[i + 2] && ft_isprint_no_ws(line[i + 2]))
		return (0);
	if (line[i] == 'N' && line[i + 1] == 'O' && !cubp->north_path)
		cubp->north_path = texture_path(line, i + 2);
	else if (line[i] == 'S' && line[i + 1] == 'O' && !cubp->south_path)
		cubp->south_path = texture_path(line, i + 2);
	else if (line[i] == 'W' && line[i + 1] == 'E' && !cubp->west_path)
		cubp->west_path = texture_path(line, i + 2);
	else if (line[i] == 'E' && line[i + 1] == 'A' && !cubp->east_path)
		cubp->east_path = texture_path(line, i + 2);
	else
		return (0);
	return (1);
}
