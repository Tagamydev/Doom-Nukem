/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_info.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 00:35:37 by andie             #+#    #+#             */
/*   Updated: 2024/08/21 18:35:46 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include "parsing.h"

static int	file_lines(char *path)
{
	int		count;
	int		fd;
	char	*line;

	count = 0;
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (print_msg("fd = -1", errno));
	line = get_next_line(fd);
	while (line != NULL)
	{
		count++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (count);
}

static void	fill_tab(int row, int column, int i, t_cubp *cubp)
{
	char	*line;

	line = get_next_line(cubp->map_info.fd);
	while (line != NULL)
	{
		cubp->map_info.file[row] = ft_calloc((ft_strlen(line) + 1), sizeof(char));
		if (cubp->map_info.file[row] == NULL)
		{
			print_msg("Malloc failed.", 1);
			free_tab((void **)cubp->map_info.file);
			return ;
		}
		while (line[i] != '\0')
		{
			cubp->map_info.file[row][column] = line[i];
			column++;
			i++;
		}
		cubp->map_info.file[row++][column] = '\0';
		column = 0;
		i = 0;
		free(line);
		line = get_next_line(cubp->map_info.fd);
	}
	cubp->map_info.file[row] = NULL;
}

void	parse_map(char *path, t_cubp *cubp)
{
	int	i;
	int	row;
	int	column;

	i = 0;
	row = 0;
	column = 0;
	cubp->map_info.lines = file_lines(path);
	cubp->map_info.path = path;
	cubp->map_info.file = ft_calloc(cubp->map_info.lines + 1, sizeof(char *));
	if (cubp->map_info.file == NULL)
	{
		print_msg("Malloc failed.", 1);
		return ;
	}
	cubp->map_info.fd = open(path, O_RDONLY);
	if (cubp->map_info.fd == -1)
	{
		print_msg("fd = -1", 1);
		return ;
	}
	fill_tab(row, column, i, cubp);
	close(cubp->map_info.fd);
}
