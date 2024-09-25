/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 02:02:49 by samusanc          #+#    #+#             */
/*   Updated: 2024/08/27 01:31:35 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H
# include "libft.h"
# include "T_Engine.h"
# include "mlx_utils.h"
# include "cub.h"
# include <fcntl.h>
# include <stddef.h>
# include <unistd.h>
# include <stdlib.h>
# include <ctype.h>
# include <errno.h>
# include <stdio.h>
# include <string.h> 

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 100
# endif

typedef struct s_player_pos
{
	double		x;
	double		y;
	char	dir;
	double	plane_x;
	double	plane_y;
}	t_player_pos;

typedef struct s_map_info
{
	int			fd;
	int			lines;
	char		*path;
	char		**file;
	int			height;
	int			width;
	int			index_end_of_map;
}	t_map_info;

typedef struct s_cubp
{
//
	t_map_info	map_info;
//
	char			*north_path;
	char			*south_path;
	char			*east_path;
	char			*west_path;
	int				*floor;
	int				*ceiling;
	unsigned long	x_floor;
	unsigned long	x_ceiling;
	char			**map;
	t_player_pos	player_pos;
}	t_cubp;


int		parsing(t_cubp *cubp, char **argv);
void	parse_map(char *path, t_cubp *cubp);
int		check_file(char *arg);
int		print_msg(char *msg, int ret);
int		ft_isspace(char c);
int		ft_isspace_no_nl(char c);
int		fill_map(t_cubp *cubp, char **file, int i);
int		check_map(t_cubp *cubp, char **map);
void	free_parsing(t_cubp *cubp);
void	free_tab(void **tab);
int		fill_textures(t_cubp *cubp, char *line, int i);
int		fill_color(t_cubp *cubp, char *line, int i);
int		check_dir(char *arg);
int		check_texture(t_cubp *cubp);
int		check_path(char *arg);
void	player_dir(t_cubp *cubp);
void	change_space_to_empty(t_cubp *cubp);
int		map_alloc_height(t_cubp *cubp, char **file, int i);
int		replace_player_with_floor(t_cubp *cubp, char **map);
int		valid_pos(t_cubp *cubp, char **map);
int		check_player(t_cubp *cubp, char **map);
int		ft_isprint_no_ws(int c);
//char	*ft_strdup_gnl(const char *s);
//char	*ft_strchr_gnl(char *str, int c);
//void	ft_free_gnl(char **str, char **str2, char **str3);
int		parsingmap(char **argv, t_cub *cub);

#endif
