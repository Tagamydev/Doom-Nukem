/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 20:19:11 by andie             #+#    #+#             */
/*   Updated: 2024/08/18 14:42:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include "parsing.h"

//checks whether the argument is a directory, return 1 if it is, 0 if it is not
int	check_dir(char *arg)
{
	int	fd;

	fd = open(arg, O_RDONLY | O_DIRECTORY);
	if (fd >= 0)
	{
		close(fd);
		return (1);
	}
	return (0);
}

//checks the .cub extension, return 1 if it is, 0 if it is not
static int	check_exten(char *arg)
{
	size_t	length;

	length = ft_strlen(arg);
	if ((arg[length - 4] != '.' || arg[length - 3] != 'c'
			|| arg[length - 2] != 'u' || arg[length - 1] != 'b'))
		return (0);
	return (1);
}

//returns 1 on success
int	check_file(char *arg)
{
	int	fd;

	if (check_dir(arg))
		return (print_msg("File is a directory.", 0));
	fd = open(arg, O_RDONLY);
	if (fd == -1)
		return (print_msg(arg, 0));
	close(fd);
	if (check_exten(arg) == 0)
		return (print_msg("Incorrect type of file.", 0));
	return (1);
}
