/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cub.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 02:02:49 by samusanc          #+#    #+#             */
/*   Updated: 2024/08/05 02:03:30 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB_H
# define CUB_H
# include <stdlib.h>
# include "libft.h"
# include "T_Engine.h"
# include "mlx_utils.h"
# ifndef BONUS
#  define BONUS 1
# endif

typedef struct s_bsp t_bsp;
typedef struct s_map_editor
{
	t_point			screen_center;
	int				screen_zoom;
}			t_map_editor;

typedef enum e_game_mode
{
	GAME,
	EDITOR
}				t_game_mode;

typedef struct s_cub
{

	t_list			segments;
	t_map_editor	map_editor;

	void			*mlx;
	void			*mlx_win;
	unsigned int	frame;
	float			delta_time;
	t_game_mode		game_mode;

	t_img			*tmp;

	char			*north_path;
	char			*south_path;
	char			*east_path;
	char			*west_path;
	t_color			floor;
	t_color			celling;
	char			**map;
	t_player		*player;


	t_bsp			*root_node;

	t_img			*north;
	t_img			*south;
	t_img			*east;
	t_img			*west;
}               t_cub;

#endif