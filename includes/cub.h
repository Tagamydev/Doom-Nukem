/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 02:02:49 by samusanc          #+#    #+#             */
/*   Updated: 2024/09/02 18:07:57 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB_H
# define CUB_H
# include <stdlib.h>
# include "libft.h"
# include "T_Engine.h"
# include "mlx_utils.h"
# include <X11/keysym.h>
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
	PAUSE,
	LAST,
	EDITOR
}				t_game_mode;

typedef	struct s_hooks
{
	int	(*option1)();
	int	(*option2)();
}				t_hooks;

typedef struct s_win_hooks
{
	t_hooks	mouse;
	t_hooks	key;
	t_hooks	focus;
}				t_win_hooks;

typedef struct s_win
{
	void			*mlx_win;
	void			*data;
	t_resolution	res;
	unsigned int	id;
}				t_win;

typedef struct s_triple_buff_img
{
	t_img			*render;
	t_img			*buffer_a;
	t_img			*buffer_b;
}				t_triple_buff_img;

typedef struct s_cub
{

	t_list				segments;
	t_map_editor		map_editor;

	void				*mlx;
	t_win				*main_window;

	unsigned int		frame;
	double				delta_time;
	t_game_mode			game_mode;
	int					focus;
	int					mouse_press;
	t_point				last_mouse_grab;

	t_img				*editor_img;
	t_img				*game_img;
	t_img				*render_img;
	t_img				*minimap_img;
	t_img				*test_tex;

	float				**shadow_tex;
	float				shadow_tex_size;
	float				ambient_oclussion;

	float				max_dist;

	float				wall_height;

	float				near_plane;


	float				height_multiplier;




	char				*north_path;
	char				*south_path;
	char				*east_path;
	char				*west_path;
	t_color				floor;
	t_color				celling;
	char				**map;

	// PLAYER
	t_player			*player;
	t_point				p_deltas;
	t_point				fov1_deltas;
	t_point				fov2_deltas;
	t_point				fov1_screen;
	t_point				fov2_screen;


	t_bsp				*root_node;

	t_img				*north;
	t_img				*south;
	t_img				*east;
	t_img				*west;
	void			(*del)();
}               t_cub;

#endif
