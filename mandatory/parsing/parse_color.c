/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 02:38:15 by andie             #+#    #+#             */
/*   Updated: 2024/08/17 15:34:48 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include "parsing.h"

static int	check_digit(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '\n')
	{
		if (!ft_isdigit(str[i]) && !ft_isspace_no_nl(str[i]) && str[i] != ',')
			return (1);
		i++;
	}
	return (0);
}

static int	*rgb_array(char **rgb, int *color)
{
	int	i;

	i = 0;
	while (rgb[i])
	{
		color[i] = ft_atoi(rgb[i]);
		if (color[i] == -1 || check_digit(rgb[i]) == 1)
		{
			free_tab((void **)rgb);
			free(color);
			return (0);
		}
		i++;
	}
	free_tab((void **)rgb);
	return (color);
}

static int	*parse_rgb_values(char *line)
{
	char	**rgb;
	int		i;
	int		*color;

	rgb = ft_split(line, ',');
	i = 0;
	while (rgb[i])
		i++;
	if (i != 3)
	{
		free_tab((void **)rgb);
		return (0);
	}
	color = malloc(sizeof(int) * 3);
	if (color == NULL)
	{
		free_tab((void **)rgb);
		return (0);
	}
	return (rgb_array(rgb, color));
}

//returns 1 if the color is successfully parsed, 0 otherwise
int	fill_color(t_cubp *cubp, char *line, int i)
{
	if (line[i + 1] && ft_isprint_no_ws(line[i + 1]))
		return (print_msg("Invalid color.", 0));
	if (!cubp->ceiling && line[i] == 'C')
	{
		cubp->ceiling = parse_rgb_values(line + i + 1);
		if (!cubp->ceiling)
			return (print_msg("Incorrect ceiling color.", 0));
	}
	else if (!cubp->floor && line[i] == 'F')
	{
		cubp->floor = parse_rgb_values(line + i + 1);
		if (!cubp->floor)
			return (print_msg("Incorrect floor color.", 0));
	}
	else
		return (print_msg("Invalid color.", 0));
	return (1);
}
