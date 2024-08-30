/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 22:32:43 by samusanc          #+#    #+#             */
/*   Updated: 2024/08/10 23:48:26 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "cub.h"


//===========================================================================//


//===========================================================================//
void	free_gen_struct(t_cub *cub)
{
	if (cub)
	{		
		if (cub->mlx)
			free(cub->mlx);
		if (cub->mlx_win)
			free(cub->mlx_win);

		if (cub->north_path)
			free(cub->north_path);
		if (cub->south_path)
			free(cub->south_path);
		if (cub->east_path)
			free(cub->east_path);
		if (cub->west_path)
			free(cub->west_path);

		/*
		
		if (map)
			afelicia_free_map();

		*/
		if (cub->player)
			;//

		if (cub->north)
			free_img(cub->north);
		if (cub->south)
			free_img(cub->south);
		if (cub->east)
			free_img(cub->east);
		if (cub->west)
			free_img(cub->west);

		ft_bzero(cub, sizeof(t_cub));
		free(cub);
	}
}

int	i_g_s_error(t_cub *cub)
{
	free_gen_struct(cub);
	return (0);
}

int	open_wall_tex(t_cub *cub)
{
	/*
	cub->north = open_img(cub->mlx, "");
	if (!cub->north)
		return (0);
	cub->south = open_img(cub->mlx, "");
	if (!cub->south)
		return (0);
	cub->east = open_img(cub->mlx, "");
	if (!cub->east)
		return (0);
	cub->west = open_img(cub->mlx, "");
	if (!cub->west)
		return (0);
	*/
	return (1);
}

//===========================================================================//

/*
	TODO: I want to implement the window array for a window managment system,
	so the general structure might change in the future
*/
int	init_gen_struct(t_cub *cub, char *map_path)
{
	cub->mlx = mlx_init();
	if (!cub->mlx)
		return (i_g_s_error(cub));
	// CHANGE THIS RESOLUTION AFTER!!!!
	cub->mlx_win = mlx_new_window(cub->mlx, 1920, 1080, "Default Windows");
	if (!cub->mlx_win)
		return (i_g_s_error(cub));
	if (!parsing(cub, map_path))
		return (i_g_s_error(cub));
	if (!open_wall_tex(cub))
		return (i_g_s_error(cub));
	return (1);
}

/*
	ft_constructor
	Allocates the hole struct, use bzero to NULL every element, 
	then initialize every single one of them with custom funtions.
	After bzero every error in funtion have to use the ft_freeGenStruct for error handling
	and return NULL after that
*/
t_cub	*ft_constructor(char *map_path)
{
	t_cub	*result;

	result = malloc(sizeof(t_cub));
	if (!result)
		return (NULL);
	ft_bzero(result, sizeof(t_cub));
	if (!init_gen_struct(result, map_path))
		return (NULL);
	return (result);
}


//=================================================== segment in engine

typedef	struct s_segment
{
	t_line	segment;
	t_point	vector;
}				t_segment;

t_segment	*segment(t_line line)
{
	t_segment	*result;
	t_point		a;
	t_point		b;

	result = malloc(sizeof(t_segment));
	if (!result)
		return (NULL);
	ft_bzero(result, sizeof(t_segment));
	result->segment = line;
	a = line.a;
	b = line.b;
	a.px = b.px - a.px;
	a.py = b.py - a.py;
	result->vector = a;
	return (result);
}

void	*default_node_free(void *ptr)
{
	free(ptr);
	return (NULL);
}

//=================================================== segment in engine


//============================================= Draw circle in mlx_utils

void	horizontal_line(int x0, int y0, int x1, t_img *img, t_color color)
{
	int x;

	x = x0;
	while (x <= x1)
	{
		++x;
		put_pixel(img, color_point(point(x, y0), color));
	}

}

void	plot4points(int cx, int cy, int x, int y, t_img *img, t_color color)
{
	horizontal_line(cx - x, cy + y, cx +x, img, color);
	if (y != 0)
		horizontal_line(cx - x, cy - y, cx +x, img, color);
}
			

void	draw_circle(int radius, t_img *img, t_point center)
{

	int	cx;
	int	cy;

	cx = center.px;
	cy = center.py;

	int error;
	int	x;
	int y;
	int	lasty;

	error = -radius;
	x = radius;
	y = 0;
	while (x >= y)
	{
			lasty = y;
			error += y++;
			error += y;
			plot4points(cx, cy, x, lasty, img, center.color);
			if (error >= 0)	
			{
				if (x != lasty)
					plot4points(cx, cy, lasty, x, img, center.color);
				error -= x--;
				error -= x;
			}
	}

}

//============================================= Draw circle in mlx_utils

//============================================= get_bounds in engine

t_point	get_bounds_max(t_line tmp, t_point max)
{
	if (tmp.a.px > max.px)
		max.px = tmp.a.px;
	if (tmp.b.px > max.px)
		max.px = tmp.b.px;
	if (tmp.a.py > max.py)
		max.py = tmp.a.py;
	if (tmp.b.py > max.py)
		max.py = tmp.b.py;
	return (max);
}

t_point	get_bounds_min(t_line tmp, t_point max)
{
	if (tmp.a.px < max.px)
		max.px = tmp.a.px;
	if (tmp.b.px < max.px)
		max.px = tmp.b.px;
	if (tmp.a.py < max.py)
		max.py = tmp.a.py;
	if (tmp.b.py < max.py)
		max.py = tmp.b.py;
	return (max);
}

t_line	get_bounds(t_list *segments)
{
	t_node			*tmp;
	t_segment		*tmp2;
	t_point			max;
	t_point			min;

	tmp = segments->head;
	max = point(FLT_MIN, FLT_MIN);
	min = point(FLT_MAX, FLT_MAX);
	while (tmp)
	{
		tmp2 = (t_segment *)tmp->content;
		max = get_bounds_max(tmp2->segment, max);
		min = get_bounds_min(tmp2->segment, min);
		tmp = tmp->next;
	}
	return (line(max, min));
}

//============================================= get_bounds in engine

//============================================= s_map in t_engine
/*
typedef struct s_map_editor
{

}				t_map_editor;
*/
// the map editor type comes with the engine

t_map_editor	new_map_editor();

t_map_editor	new_map_editor()
{
	t_map_editor	result;

	result.screen_center = point(0, 0);
	result.screen_zoom = 100;
	return (result);
}

//============================================= s_map in t_engine

//                 normalize
//this needs check
t_point	ft_normalize(t_point point)
{
	t_point	result;
	double	xd;
	float	inv_sqr;

	result = point;
	xd = (point.px * point.px) + (point.py * point.py) + (point.pz * point.pz);
	inv_sqr = 0;
	if (xd)
	{
		inv_sqr = sqrt(xd);
		inv_sqr = 1 / inv_sqr;
	}
	result.px = point.px * inv_sqr;
	result.py = point.py * inv_sqr;
	result.pz = point.pz * inv_sqr;
	return (result);
}
//                 normalize

//============================================== draw map editor in mlx_utils

void	draw_normal(t_line line, t_img *img)
{
	t_point	p10;
	t_point	p0;
	t_point	p1;
	t_point	normal;

	p1 = line.a;
	p0 = line.a;
	p10 = line.a;
	p10.px = line.b.px - line.a.px;
	p10.py = line.b.py - line.a.py;
	normal = ft_normalize(point(-p10.py, p10.px));
	p0.px = (line.a.px + line.b.px) * 0.5f;
	p0.py = (line.a.py + line.b.py) * 0.5f;
	p1.px = p0.px + normal.px * 12.0f;
	p1.py = p0.py + normal.py * 12.0f;
	draw_line(p0, p1, img);
}

void	print_segment_list(t_list segments)
{
	t_node		*tmp;
	t_segment	*tmp1;

	tmp = segments.head;
	while (tmp)
	{
		tmp1 = tmp->content;
		printf("a:%f, %f, b:%f, %f\n", 
		tmp1->segment.a.px, tmp1->segment.a.py, tmp1->segment.b.px, tmp1->segment.a.py);
		tmp = tmp->next;
	}

}

t_point	undo_remap_point(t_point pt, int zoom, t_point center, t_resolution res)
{
	t_resolution	half;

	half = resolution(res.width / 2, res.height / 2);
	pt.px -= half.width - center.px;
	pt.py -= half.height - center.py;
	pt.px = pt.px / zoom;
	pt.py = pt.py / zoom;
	return(pt);
}

t_point	remap_point(t_point pt, int zoom, t_point center, t_resolution res)
{
	t_resolution	half;

	half = resolution(res.width / 2, res.height / 2);
	pt.px *= zoom;
	pt.py *= zoom;
	pt.px += half.width - center.px;
	pt.py += half.height - center.py;
	return(pt);
}

void	draw_segments(t_list segments, t_map_editor map_editor, t_img *img, t_color color)
{
	t_node		*tmp;
	t_segment	*tmp1;
	t_point		tmp2;
	t_point		tmp3;

	tmp = segments.head;
	while (tmp)
	{
		tmp1 = (t_segment *)tmp->content;
		tmp2 = tmp1->segment.a;
		tmp3 = tmp1->segment.b;
		tmp2 = remap_point(tmp2, map_editor.screen_zoom, map_editor.screen_center, img->resolution);
		tmp3 = remap_point(tmp3, map_editor.screen_zoom, map_editor.screen_center, img->resolution);
		tmp2 = color_point(tmp2, color);
		tmp3 = color_point(tmp3, color);
		draw_line(tmp2, tmp3, img);
		draw_normal(line(tmp2, tmp3), img);
		draw_circle(map_editor.screen_zoom / 20, img, tmp2);
		draw_circle(map_editor.screen_zoom / 20, img, tmp3);
		tmp = tmp->next;
	}
}

//============================================== draw map editor in mlx_utils

//=============================================BSP IN ENGINE

typedef struct s_bsp
{
	struct s_bsp	*front;
	struct s_bsp	*back;
	t_segment		*splitter;
	int				id;
}				t_bsp;


t_bsp	*bsp(t_bsp *front, t_bsp *back, t_segment *splitter)
{
	t_bsp	*result;

	result = malloc(sizeof(t_bsp));
	if (!result)
		return (NULL);
	ft_bzero(result, sizeof(t_bsp));
	result->front = front;
	result->back = back;
	result->splitter = splitter;
	return (result);
}

float	cross_2d(t_point a, t_point b)
{
	return (a.px * b.py - b.px * a.py);
}

#define EPS 1e-4

void	ss_add_segment(t_segment splitter, t_list *new_segments, t_bsp *nd)
{
	t_segment	*content_tmp;
	content_tmp = segment(splitter.segment);
	if (content_tmp)
	{
		nd->id = new_segments->size;
		list_push_f(new_segments, node(content_tmp, &default_node_free));
	}
}

int	split_segments(t_list *front, t_list *back, t_list segments, t_bsp *nd, t_list *new_segments)
{
	t_segment	*splitter_seg;
	t_node		*tmp;
	t_segment	*seg_tmp;
	t_segment	*content_tmp;
	float		numerator;
	float		denominator;
	float		t;
	int			denominator_is_zero;
	int			numerator_is_zero;
	float		intersection;
	t_point		intersection_pt;
	t_segment	*r_segment;
	t_segment	*l_segment;
	t_segment	*third_segment;

	splitter_seg = (t_segment *)segments.head->content;
	nd->splitter = malloc(sizeof(t_segment));
	if (!nd->splitter)
		return ;
	ft_memcpy(nd->splitter, split_segments, sizeof(t_segment));
	tmp = segments.head;
	while (tmp->next)
	{
		tmp = tmp->next;
		seg_tmp = (t_segment *)tmp->content;
		numerator = cross_2d(
		point(
		seg_tmp->segment.a.px - splitter_seg->segment.a.px, 
		seg_tmp->segment.a.py - splitter_seg->segment.a.py
		), splitter_seg->vector);
		denominator = cross_2d(splitter_seg->vector, seg_tmp->vector);


		denominator_is_zero = ft_abs(denominator) < EPS;
		numerator_is_zero = ft_abs(numerator) < EPS;

		if (denominator_is_zero && numerator_is_zero)
		{
			content_tmp = malloc(sizeof(t_segment));
			if (content_tmp)
			{
				ft_memcpy(content_tmp, seg_tmp, sizeof(t_segment));
				list_push_f(front, node(content_tmp, &default_node_free));
			}
		}
		if (!denominator_is_zero)
		{
			intersection = numerator / denominator;

			if (0.0 < intersection && intersection < 1.0)
			{
				intersection_pt = point(seg_tmp->segment.a.px + intersection * seg_tmp->vector.px, 
				seg_tmp->segment.a.py + intersection * seg_tmp->vector.py);

				r_segment = segment(line(seg_tmp->segment.a, intersection_pt));
				l_segment = segment(line(intersection_pt, seg_tmp->segment.b));
				if (numerator > 0)
				{
					third_segment = l_segment;
					l_segment = r_segment;
					r_segment = third_segment;
				}
				list_push_f(front, node(r_segment, &default_node_free));
				list_push_f(back, node(l_segment, &default_node_free));
			}
		}
		if (numerator < 0 || (numerator_is_zero && denominator > 0))
		{
			content_tmp = malloc(sizeof(t_segment));
			if (content_tmp)
			{
				ft_memcpy(content_tmp, seg_tmp, sizeof(t_segment));
				list_push_f(front, node(content_tmp, &default_node_free));
			}
		}
		else if (numerator > 0 || (numerator_is_zero && denominator < 0))
		{
			content_tmp = malloc(sizeof(t_segment));
			if (content_tmp)
			{
				ft_memcpy(content_tmp, seg_tmp, sizeof(t_segment));
				list_push_f(back, node(content_tmp, &default_node_free));
			}
		}

	}
	if (!((front && front->head )|| (back && back->head)))
		ss_add_segment(*splitter_seg, new_segments, nd);
	return (1);
}

t_bsp	*build_bsp(t_list segments, t_list *new_segments);

t_bsp	*build_bsp(t_list segments, t_list *new_segments)
{
	t_bsp	*result;
	t_list	front;
	t_list	back;

	if (!segments.head)
		return (NULL);
	result = bsp(NULL, NULL, NULL);
	if (!result)
		return (NULL);
	ft_bzero(&front, sizeof(t_list));
	ft_bzero(&back, sizeof(t_list));
	if (!split_segments(&front, &back, segments, result, new_segments))
		return (NULL);

	if (front.head)
		result->front = build_bsp(front, new_segments);
	if (back.head)
		result->back = build_bsp(back, new_segments);

	list_clear(&front);
	list_clear(&back);
	return (result);
}

//=============================================BSP IN ENGINE

//============================================ print bsp

void	print_bsp(t_bsp *root)
{
	
}

//============================================ print bsp

//============================================= t_camera in t-engine
t_camera	*new_camera(t_point pos, float angle, float fov, t_screen *screen)
{
	t_camera	*result;

	result = malloc(sizeof(t_camera));
	if (!result)
		return(NULL);
	result->angle = angle;
	result->pos = pos;
	result->fov = fov;
	result->screen = screen;
	return (result);
}

//============================================= t_camera in t-engine
//============================================= t_player in t-engine


t_player	*new_player(t_camera *camera)
{
	t_player	*result;

	result = malloc(sizeof(t_player));
	if (!result)
		return (NULL);
	result->camera = new_camera(point(0, 0), 90, 90, NULL);
	result->update = NULL;
	result->start = NULL;
	if (!result->camera)
	{
		free(result);
		return (NULL);
	}
	return(result);
}
//============================================= t_player in t-engine

t_game_mode	change_game_mode(t_game_mode mode)
{
	static	t_game_mode	last = EDITOR;

	if (mode == LAST)
		return (last);
	else
	{
		return (mode);
	}
}

int	pause_mode(t_cub *cub)
{
	static	double	x = 100;
	static	int		sign = 1;

	if (cub->game_mode != PAUSE)
		fill_img(cub->tmp, color_from_rgb(255, 0, 0));
	cub->game_mode = PAUSE;

	draw_circle(50, cub->tmp, color_point(point(x, 200), color_from_rgb(255, 0, 0)));
	x += (float)100 * (float)sign * cub->delta_time;
	//printf("x:%f\n", x);
	mlx_mouse_show(cub->mlx, cub->mlx_win);
	unsigned int i;

	draw_circle(50, cub->tmp, color_point(point(x, 200), color_from_rgb(255, 255, 255)));
	mlx_put_image_to_window(cub->mlx, cub->mlx_win, cub->tmp->img, 0, 0);
	//mlx_do_sync(cub->mlx);
	return (1);
}

int	game_mode(t_cub *cub)
{
	if (cub->game_mode != GAME)
		fill_img(cub->tmp, color_from_rgb(255, 0, 255));
	cub->game_mode = GAME;
	if (!cub->focus)
		return (pause_mode(cub));
	mlx_mouse_hide(cub->mlx, cub->mlx_win);
	mlx_mouse_move(cub->mlx, cub->mlx_win, cub->tmp->resolution.width / 2, cub->tmp->resolution.height / 2);
	mlx_put_image_to_window(cub->mlx, cub->mlx_win, cub->tmp->img, 0, 0);
}

int	draw_grid(t_map_editor editor, t_img *img, t_color color)
{
	t_point end;
	t_point start;
	t_point tmp;
	int		i;
	int		j;
	
	end = undo_remap_point(point(img->resolution.width, img->resolution.height),
	editor.screen_zoom, editor.screen_center, img->resolution);
	start = undo_remap_point(point(0, 0),
	editor.screen_zoom, editor.screen_center, img->resolution);
	j = (int)start.py;
	j -= 1;
	while (j < (int)end.py + 1)
	{
		i = (int)start.px;
		i -= 1;
		while (i < (int)end.px + 1)
		{
			tmp = point((float)i, (float)j);
			tmp = remap_point(tmp, editor.screen_zoom, editor.screen_center, img->resolution);
			tmp.px = (unsigned int)tmp.px;
			tmp.py = (unsigned int)tmp.py;
			tmp.color = color;
			put_pixel(img, tmp);
			i++;
		}
		j++;
	}
}

t_point	mouse_pos_relative(t_cub *cub)
{
	t_point	result;
	int		x;
	int		y;

	x = 0;
	y = 0;
	result = cub->map_editor.screen_center;
	mlx_mouse_get_pos(cub->mlx, cub->mlx_win, &x, &y);

	result.px = cub->last_mouse_grab.px - x;
	result.py = cub->last_mouse_grab.py - y;

	result.px = result.px * 100.0f / (float)(200.0f - cub->map_editor.screen_zoom);
	result.py = result.py * 100.0f / (float)(200.0f - cub->map_editor.screen_zoom);
	result.px += cub->map_editor.screen_center.px;
	result.py += cub->map_editor.screen_center.py;
	cub->last_mouse_grab = point(x, y);
	return (result);
}

int editor_mode(t_cub *cub)
{
	static t_map_editor	last = {0};

	if (cub->game_mode != EDITOR)
		fill_img(cub->tmp, color_from_rgb(0, 0, 0));
	cub->game_mode = EDITOR;
	if (cub->mouse_press)
		cub->map_editor.screen_center = mouse_pos_relative(cub);
	mlx_mouse_show(cub->mlx, cub->mlx_win);
	draw_grid(last, cub->tmp, color_from_rgb(0, 0, 0));
	draw_grid(cub->map_editor, cub->tmp, color_from_rgb(100, 100, 100));
	draw_segments(cub->segments, last, cub->tmp, color_from_rgb(0, 0, 0));
	draw_segments(cub->segments, cub->map_editor, cub->tmp, color_from_rgb(255, 255, 255));
	last = cub->map_editor;
	mlx_put_image_to_window(cub->mlx, cub->mlx_win, cub->tmp->img, 0, 0);
}

#include <time.h>


int	frame(void *p_cub)
{
	t_cub	*cub;
	double	now;
	static	double last_f;
	static	int	last;


	cub = (t_cub *)p_cub;
	cub->frame += 1;
	now = (double)(clock() * 5) / CLOCKS_PER_SEC;
	cub->delta_time = now - last_f;
	last_f = now;
	if ((int)now > last)
	{
		printf("fps:%d\n", cub->frame);
		printf("last:%f\n", now);
		cub->frame = 0;
		last = now;
	}
	if (cub->delta_time < 0.016f)
		cub->delta_time = 0.016f;
	if (cub->delta_time > 0.1f)
		cub->delta_time = 0.1f;
	//printf("delta_time:%f\n", cub->delta_time);
	//cub->delta_time = 0.01;

	if (cub->game_mode == GAME)
		game_mode(cub);
	else if (cub->game_mode == EDITOR)
		editor_mode(cub);
	else if (cub->game_mode == PAUSE)
		pause_mode(cub);

	//block mouse in play mode
	//printf("frame:%d\n", cub->frame);
	return (0);
}

int	mouse_press(int key, int x, int y, void *param)
{
	printf("key:%d, intx;%d, inty:%d\n", key, x, y);
	t_cub		*cub;

	cub = (t_cub *)param;
	if (key == 1)
	{
		cub->mouse_press = 1;
		cub->last_mouse_grab = point(x, y);
	}

	if (cub->game_mode == EDITOR)
	{
		if (key == 5)
			cub->map_editor.screen_zoom -= 10;
		if (key == 4)
			cub->map_editor.screen_zoom += 10;
		if (cub->map_editor.screen_zoom <= 0)
			cub->map_editor.screen_zoom = 10;
		if (cub->map_editor.screen_zoom >= 150)
			cub->map_editor.screen_zoom = 150;
		printf("zoom:%d\n", cub->map_editor.screen_zoom);
	}
}
int	mouse_release(int key, int x, int y, void *param)
{
	printf("key:%d, intx;%d, inty:%d\n", key, x, y);
	t_cub		*cub;

	cub = (t_cub *)param;
	if (key == 1)
	{
		cub->mouse_press = 0;
		cub->last_mouse_grab = point(x, y);
	}
}

int key_press_game(int key, t_cub *cub)
{

}

int key_press_editor(int key, t_cub *cub)
{
	if (key == 65362)
		cub->map_editor.screen_center.py += 100 * cub->delta_time;
	if (key == 65363)
		cub->map_editor.screen_center.px -= 100 * cub->delta_time;
	if (key == 65364)
		cub->map_editor.screen_center.py -= 100 * cub->delta_time;
	if (key == 65361)
		cub->map_editor.screen_center.px += 100 * cub->delta_time;

}

int	key_press(int key, void *param)
{
	t_cub		*cub;

	cub = (t_cub *)param;
	printf("key:%d\n", key);
	if (key == 109)
	{
		if (cub->game_mode == GAME)
			return (editor_mode(cub));
		else
			return (game_mode(cub));
	}
	if (cub->game_mode == GAME)
		key_press_game(key, cub);
	if (cub->game_mode == EDITOR)
		key_press_editor(key, cub);
}

int	focus_in(void *param)
{
	t_cub		*cub;

	cub = (t_cub *)param;
	cub->focus = 1;
	printf("this is focus\n");
}

int	focus_out(void *param)
{
	t_cub		*cub;

	cub = (t_cub *)param;
	cub->focus = 0;

	printf("this is not focus\n");
}

int	main(int argc, char **argv)
{
	t_cub	*cub;

	if (argc > 2 || (argc < 2 && !BONUS))
	{
		write(2, "Error: wrong number of arguments\n", 34);
		return (-1);
	}


	cub = ft_constructor(argv[1]);
	//================================================================================

	cub->tmp = init_img(cub->mlx, resolution(1920, 1080));
	fill_img(cub->tmp, color(red));

	// this part is parsing one map ======================================================
	t_list	segments;

	segments = list(NULL);
	list_push_b(&segments, node(segment(line(point(0, 0), point(7, 1))), &default_node_free));
	list_push_b(&segments, node(segment(line(point(7, 1), point(7, 8))), &default_node_free));
	list_push_b(&segments, node(segment(line(point(7, 8), point(1, 8))), &default_node_free));
	list_push_b(&segments, node(segment(line(point(1, 8), point(0, 0))), &default_node_free));

	list_push_b(&segments, node(segment(line(point(2, 4), point(2, 3))), &default_node_free));
	list_push_b(&segments, node(segment(line(point(2, 3), point(5, 5))), &default_node_free));
	list_push_b(&segments, node(segment(line(point(5, 5), point(3, 6))), &default_node_free));
	list_push_b(&segments, node(segment(line(point(3, 6), point(2, 4))), &default_node_free));

	t_map_editor map_edit;

	//map_edit = new_map_editor(segments, 50, cub->tmp->resolution);

	// this part is parsing the map ======================================================
	t_list	new_segments;

	new_segments = list(NULL);
	cub->root_node = build_bsp(segments, &new_segments);

	cub->segments = new_segments;
	
	cub->map_editor = new_map_editor();
	cub->game_mode = PAUSE;

	mlx_put_image_to_window(cub->mlx, cub->mlx_win, cub->tmp->img, 0, 0);

	if (!cub)
	{
		write(2, "Error: cannot initialize the general struct\n", 45);
		return (-1);
	}
	mlx_hook(cub->mlx_win, 2, (1L<<0), key_press, cub);
	mlx_hook(cub->mlx_win, 9, (1L<<21), focus_in, cub);
	mlx_hook(cub->mlx_win, 10, (1L<<21), focus_out, cub);
	mlx_hook(cub->mlx_win, 4, (1L<<2), mouse_press, cub);
	mlx_hook(cub->mlx_win, 5, (1L<<3), mouse_release, cub);
	mlx_mouse_move(cub->mlx, cub->mlx_win, 500, 500);
	//mlx_hook(cub->mlx_win, 4, 0, (int (*)())mouse_press, cub);
	mlx_loop_hook(cub->mlx, frame, cub);
	mlx_loop(cub->mlx);
}
