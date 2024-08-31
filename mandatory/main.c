/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 22:32:43 by samusanc          #+#    #+#             */
/*   Updated: 2024/08/31 18:25:33 by samusanc         ###   ########.fr       */
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

void	print_segment(t_segment *seg)
{
	float ax = seg->segment.a.px;
	float ay = seg->segment.a.py;
	float bx = seg->segment.b.px;
	float by = seg->segment.b.py;
	
	printf("segment:%p (ax:%f, ay:%f), (bx:%f, by:%f)\n",seg, ax, ay, bx, by);
}

void	draw_segment(t_segment *tmp1, t_map_editor map_editor, t_img *img, t_color color)
{
	t_point		tmp2;
	t_point		tmp3;

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
}

void	draw_segments(t_list segments, t_map_editor map_editor, t_img *img, t_color color)
{
	t_node		*tmp;
	t_segment	*tmp1;

	tmp = segments.head;
	while (tmp)
	{
		tmp1 = (t_segment *)tmp->content;
		draw_segment(tmp1, map_editor, img, color);
		tmp = tmp->next;
	}
}

//============================================== draw map editor in mlx_utils

//=============================================BSP IN ENGINE

typedef struct s_bbox_2d
{
	t_line	a;
	t_line	b;
}				t_bbox_2d;

t_bbox_2d	bbox_2d()
{
	t_bbox_2d	result;

	result.a = line(point(0, 0), point(0, 0));
	result.b = line(point(0, 0), point(0, 0));
	return (result);
}

typedef struct s_bsp
{
	struct s_bsp	*front;
	t_bbox_2d		front_bbox;
	struct s_bsp	*back;
	t_bbox_2d		back_bbox;
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
	int			split;

	splitter_seg = (t_segment *)segments.head->content;
	nd->splitter = malloc(sizeof(t_segment));
	if (!nd->splitter)
		return ;
	ft_memcpy(nd->splitter, splitter_seg, sizeof(t_segment));
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

		split = 0;

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
				split = 1;
			}
		}
		if (!split)
		{
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
		//else
			//print_segment(front->head->content);
		//print_segment(splitter_seg);
		//print_segment(front->head->content);
		//exit(0);

	}

	//if (!((front && front->head )|| (back && back->head)))
	ss_add_segment(*splitter_seg, new_segments, nd);
	return (1);
}


t_bbox_2d	easy_bbox_2d(t_segment *split, t_list *segments)
{
	t_bbox_2d	result;
	t_line		bound;

	bound = get_bounds(segments);
	result.a = line(point(bound.a.px, bound.a.py), point(bound.a.px, bound.b.py));
	result.b = line(point(bound.b.px, bound.a.py), point(bound.b.px, bound.b.py));
	return (result);
}

float	fix_angle(float angle)
{
	if (angle < 0)
		angle += 360.0;
	if (angle > 359.0)
		angle -= 360.0;
	return (angle);
}

float	deg2_rad(float angle)
{
	return (fix_angle(angle) * PI / 180.0);
}

t_point	rotate_point(t_point pt, float angle)
{
	float	x;
	float	y;

	x = pt.px;
	y = pt.py;
	pt.px = x * cos(deg2_rad(angle)) - y * sin(deg2_rad(angle));
	pt.py = x * sin(deg2_rad(angle)) + y * cos(deg2_rad(angle));
	return (pt);
}

t_line	rotate_line(t_line line, float angle)
{
	line.a = rotate_point(line.a, angle);
	line.b = rotate_point(line.b, angle);
	return (line);
}

t_bbox_2d	hard_bbox_2d(t_segment *split, t_list *segments)
{
	double		angle;
	double		angle2;
	t_node		*tmp;
	t_segment	*seg_tmp;

	angle = (atan2(split->segment.b.py - split->segment.a.py, 
	split->segment.b.px - split->segment.a.px));
	angle = angle * (180.0 / PI);
	if (angle < 0)
		angle += 360.0;
	if (angle > 359.0)
		angle -= 360.0;
	angle2 = 360.0 - angle;
	tmp = segments->head;
	while (tmp)
	{
		seg_tmp = tmp->content;
		seg_tmp->segment = rotate_line(seg_tmp->segment, angle);
		tmp = tmp->next;
	}
	split->segment = rotate_line(split->segment, angle);
	split->segment = rotate_line(split->segment, -angle);
	//split->segment = rotate_line(split->segment, angle);

	t_bbox_2d	result;
	t_line		bound;

	bound = get_bounds(segments);
	result.a = line(point(bound.a.px, bound.a.py), point(bound.a.px, bound.b.py));
	result.a = rotate_line(result.a, -angle);
	result.b = line(point(bound.b.px, bound.a.py), point(bound.b.px, bound.b.py));
	result.b = rotate_line(result.b, -angle);

	return (result);
}

t_bbox_2d	get_bbox_2d(t_segment *split, t_list *segments)
{
	t_bbox_2d	result;

	result = bbox_2d();
	if (!split || !segments || !segments->head)
		return (result);
	if (split->segment.a.px == split->segment.b.px ||
	split->segment.a.py == split->segment.b.py)
		return (easy_bbox_2d(split, segments));
	else
		return (hard_bbox_2d(split, segments));
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
	{
		result->front = build_bsp(front, new_segments);
		result->front_bbox = get_bbox_2d(result->splitter, &front);
	}
	if (back.head)
	{
		result->back = build_bsp(back, new_segments);
		result->back_bbox = get_bbox_2d(result->splitter, &back);
	}

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
	if (!camera)
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
		return (mode);
}

int	pause_mode(t_cub *cub)
{
	static	double	x = 100;
	static	int		sign = 1;

	if (cub->game_mode != PAUSE)
		fill_img(cub->tmp, color_from_rgb(0, 0, 0));
		//fill_img(cub->tmp, color_from_rgb(255, 0, 0));
	cub->game_mode = change_game_mode(PAUSE);

	draw_circle(50, cub->tmp, color_point(point(x, 200), color_from_rgb(255, 0, 0)));
	x += (float)100 * (float)sign * cub->delta_time;
	mlx_mouse_show(cub->mlx, cub->mlx_win);

	draw_circle(50, cub->tmp, color_point(point(x, 200), color_from_rgb(255, 255, 255)));
	mlx_put_image_to_window(cub->mlx, cub->mlx_win, cub->tmp->img, 0, 0);
	return (1);
}

int	game_mode(t_cub *cub)
{
	if (cub->game_mode != GAME)
		fill_img(cub->tmp, color_from_rgb(0, 0, 0));
		//fill_img(cub->tmp, color_from_rgb(255, 0, 255));
	cub->game_mode = GAME;
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

int	draw_bbox(t_bbox_2d bbox, t_map_editor map_editor, t_color color, t_img *img)
{
	t_point	p1;
	t_point	p2;
	t_point	p3;
	t_point	p4;

	p1 = bbox.a.a;
	p2 = bbox.a.b;
	p3 = bbox.b.a;
	p4 = bbox.b.b;

	p1 = remap_point(p1, map_editor.screen_zoom, map_editor.screen_center, img->resolution);
	p2 = remap_point(p2, map_editor.screen_zoom, map_editor.screen_center, img->resolution);
	p3 = remap_point(p3, map_editor.screen_zoom, map_editor.screen_center, img->resolution);
	p4 = remap_point(p4, map_editor.screen_zoom, map_editor.screen_center, img->resolution);

	p1.color = color;
	p2.color = color;
	p3.color = color;
	p4.color = color;

	draw_line(p1, p2, img);
	draw_line(p2, p4, img);
	draw_line(p3, p4, img);
	draw_line(p3, p1, img);
}

int	draw_bsp_node(t_bsp *node, t_cub *cub, t_map_editor map_editor, t_color col, int mode)
{
	if (node)
	{
		draw_segment(node->splitter, map_editor, cub->tmp, col);
		if (col.hex != color(BLACK).hex)
		{
			draw_bbox(node->back_bbox, map_editor, color(RED), cub->tmp);
			draw_bbox(node->front_bbox, map_editor, color(GREEN), cub->tmp);
		}
		else
		{
			draw_bbox(node->back_bbox, map_editor, col, cub->tmp);
			draw_bbox(node->front_bbox, map_editor, col, cub->tmp);
		}
		if (!mode)
		{
			if (node->front)
				draw_bsp_node(node->front, cub, map_editor, col, mode);
			if (node->back)
				draw_bsp_node(node->back, cub, map_editor, col, mode);
		}
	}
}

int	draw_bsp(t_cub *cub, t_map_editor map_editor, t_color col, int mode)
{
	static	t_bsp	*root = NULL;
	static	t_bsp	*last_root = NULL;
	static	float	frame = 0;
	t_bsp			*tmp_root;

	if (mode)
	{
		frame += cub->delta_time;
		if (!root)
		{
			root = cub->root_node;
			frame = 0;
		}
		if (root)
		{
			if (last_root)
				draw_bsp_node(last_root, cub, map_editor, color(BLACK), mode);
			draw_bsp_node(root, cub, map_editor, col, mode);
			if (frame > 10)
			{
				last_root = root;
				root = root->front;
				frame = 0;
			}
		}
	}
	else
	{
		tmp_root = cub->root_node;

		if (tmp_root)
			draw_bsp_node(tmp_root, cub, map_editor, col, mode);
	}
}

int	draw_player(t_cub *cub, t_map_editor map_editor, t_color col)
{
	// para comprobar la bbox del player y un sector simplemente saca el angulo entre el player y el pt medio del sector
	static	t_point	last_pos = {0};
	static	t_point	last_fov1 = {0};
	static	t_point	last_fov2 = {0};
	t_point	pos;
	t_point	fov1;
	t_point	fov2;

	pos = cub->player->camera->pos;

	fov1 = point(pos.px + (3.0  * cub->fov1_deltas.px), 
	pos.py + (3.0  * cub->fov1_deltas.py));

	fov2 = point(pos.px + (3.0  * cub->fov2_deltas.px), 
	pos.py + (3.0  * cub->fov2_deltas.py));


	pos = remap_point(pos, map_editor.screen_zoom, map_editor.screen_center, cub->tmp->resolution);
	fov1 = remap_point(fov1, map_editor.screen_zoom, map_editor.screen_center, cub->tmp->resolution);
	fov2 = remap_point(fov2, map_editor.screen_zoom, map_editor.screen_center, cub->tmp->resolution);

	pos.px = (int)pos.px;
	pos.py = (int)pos.py;
	draw_circle(5, cub->tmp, color_point(last_pos, color(BLACK)));
	draw_circle(5, cub->tmp, color_point(pos, col));

	draw_line(last_pos, last_fov1, cub->tmp);
	draw_line(last_pos, last_fov2, cub->tmp);

	last_pos = pos;
	last_fov1 = fov1;
	last_fov2 = fov2;

	pos.color = col;
	fov1.color = col;
	fov2.color = col;

	draw_line(pos, fov1, cub->tmp);
	draw_line(pos, fov2, cub->tmp);
}

t_line	bbox_intersect_make_fov(t_point delta, t_point pos)
{
	t_line	result;

	result = line(
	pos,  
	point(pos.px + (delta.px * 20.0f), pos.py + (delta.py * 20.0f))
	);
	return (result);
}

int	get_orientation_pt(t_point a, t_point b, t_point c)
{
	int	val;

	val = ((b.py - a.py) * (c.px - b.px) - (b.px - a.px) * (c.py - b.py));
	if (!val)
		return (0);
	else if (val > 0)
		return (1);
	return (2);
}

int on_segment(t_point a, t_point b, t_point c)
{
	if (b.px <= fmax(a.px, c.px) && b.px >= fmin(a.px, c.px) &&
		b.py <= fmax(a.py, c.py) && b.py >= fmin(a.py, c.py))
		return (1);
	return (0);
}

int	intersection_check(t_line a, t_line b)
{
	int	o1;
	int o2;
	int	o3;
	int	o4;

	o1 = get_orientation_pt(a.a, a.b, b.a);
	o2 = get_orientation_pt(a.a, a.b, b.b);
	o3 = get_orientation_pt(b.a, b.b, a.a);
	o4 = get_orientation_pt(b.a, b.b, a.b);

	if (o1 != o2 && o3 != o4)
		return (1);
	if (o1 == 0 && on_segment(a.a, b.a, a.b))
		return (1);
	if (o2 == 0 && on_segment(a.a, b.b, a.b))
		return (1);
	if (o3 == 0 && on_segment(b.a, a.a, b.b))
		return (1);
	if (o4 == 0 && on_segment(b.a, a.b, b.b))
		return (1);
	return (0);
}

	/*
	t_player	*player;
	t_line		fov_1;
	t_line		fov_2;
	t_line		up;
	t_line		down;
	t_line		left;
	t_line		right;

	player = cub->player;
	fov_1 = bbox_intersect_make_fov(cub->fov1_deltas, player->camera->pos);
	fov_2 = bbox_intersect_make_fov(cub->fov2_deltas, player->camera->pos);

	t_point	p1;
	t_point	p2;
	t_point	p3;
	t_point	p4;

	p1 = bbox.a.a;
	p2 = bbox.a.b;
	p3 = bbox.b.a;
	p4 = bbox.b.b;

	up = line(p1, p2);
	down = line(p2, p4);
	left = line(p3, p4);
	right = line(p3, p1);

	if (intersection_check(fov_1, up) || intersection_check(fov_2, up))
		return (1);
	if (intersection_check(fov_1, down) || intersection_check(fov_2, down))
		return (1);
	if (intersection_check(fov_1, left) || intersection_check(fov_2, left))
		return (1);
	if (intersection_check(fov_1, right) || intersection_check(fov_2, right))
		return (1);
	return (0);
	*/
int	this_bbox_intersect(t_cub *cub, t_bbox_2d bbox)
{
	t_player	*player;
	t_line		fov_1;
	t_line		fov_2;
	t_line		up;
	t_line		down;
	t_line		left;
	t_line		right;

	player = cub->player;
	fov_1 = bbox_intersect_make_fov(cub->fov1_deltas, player->camera->pos);
	fov_2 = bbox_intersect_make_fov(cub->fov2_deltas, player->camera->pos);

	t_point	p1;
	t_point	p2;
	t_point	p3;
	t_point	p4;

	p1 = bbox.a.a;
	p2 = bbox.a.b;
	p3 = bbox.b.a;
	p4 = bbox.b.b;

	up = line(p1, p2);
	down = line(p2, p4);
	left = line(p3, p4);
	right = line(p3, p1);

	if (intersection_check(fov_1, up) || intersection_check(fov_2, up))
		return (1);
	if (intersection_check(fov_1, down) || intersection_check(fov_2, down))
		return (1);
	if (intersection_check(fov_1, left) || intersection_check(fov_2, left))
		return (1);
	if (intersection_check(fov_1, right) || intersection_check(fov_2, right))
		return (1);
	return (0);
}

int	draw_bsp_segment_by_bbox(t_cub *cub, t_bsp *node, t_map_editor map_editor, t_color col)
{
	if (node)
	{
		if (this_bbox_intersect(cub, node->front_bbox))
		{
			draw_segment(node->splitter, map_editor, cub->tmp, col);
			/*
			if (col.hex != color(BLACK).hex)
				draw_bbox(node->front_bbox, map_editor, color(GREEN), cub->tmp);
			else
				draw_bbox(node->front_bbox, map_editor, color(BLACK), cub->tmp);
				*/
		}
		else if (this_bbox_intersect(cub, node->back_bbox))
		{
			draw_segment(node->splitter, map_editor, cub->tmp, col);
			/*
			if (col.hex != color(BLACK).hex)
				draw_bbox(node->front_bbox, map_editor, color(GREEN), cub->tmp);
			else
				draw_bbox(node->front_bbox, map_editor, color(BLACK), cub->tmp);
				*/
		}
		else
		{
			if (col.hex != color(BLACK).hex)
				draw_bbox(node->front_bbox, map_editor, color(GREEN), cub->tmp);
			else
				draw_bbox(node->front_bbox, map_editor, color(BLACK), cub->tmp);
		}
		if (node->front)
			draw_bsp_segment_by_bbox(cub, node->front, map_editor, col);
		if (node->back)
			draw_bsp_segment_by_bbox(cub, node->back, map_editor, col);
	}

}

int	draw_fov_intersection(t_cub *cub, t_map_editor map_editor, t_color col)
{
	if (cub->root_node)
		draw_bsp_segment_by_bbox(cub, cub->root_node, map_editor, col);
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
	draw_segments(cub->segments, cub->map_editor, cub->tmp, color_from_rgb(50, 50, 50));

	//draw_bsp(cub, last, color(BLACK), 0);
	//draw_bsp(cub, cub->map_editor, color(WHITE), 0);

	draw_fov_intersection(cub, last, color(BLACK));
	draw_fov_intersection(cub, cub->map_editor, color_from_rgb(255, 0, 255));

	draw_player(cub, last, color(BLACK));
	draw_player(cub, cub->map_editor, color_from_rgb(255, 255, 0));
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

	if (!cub->focus)
		pause_mode(cub);
	if (cub->game_mode == GAME)
		game_mode(cub);
	else if (cub->game_mode == EDITOR)
		editor_mode(cub);

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

int	calculate_deltas(t_player *player, t_point *deltas, t_point *fov1, t_point *fov2)
{
	float	angle;
	float	fov;

	if (player)
	{
		angle = player->camera->angle;
		fov = player->camera->fov;
		fov = fov / 2.0f;
		deltas->px = cos(deg2_rad(angle));
		deltas->py = sin(deg2_rad(angle));
		fov1->px = cos(deg2_rad(angle - fov));
		fov1->py = sin(deg2_rad(angle - fov));
		fov2->px = cos(deg2_rad(angle + fov));
		fov2->py = sin(deg2_rad(angle + fov));
	}
}

int	update_player_angle(t_player *player, t_point *deltas, t_point *fov1, t_point *fov2, float angle)
{
	if (player)
	{
		player->camera->angle = fix_angle(angle);
		calculate_deltas(player, deltas, fov1, fov2);
	}
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
	if (key == 44)
	{
		cub->player->camera->pos.px += 1.0 * cub->p_deltas.px * cub->delta_time;
		cub->player->camera->pos.py += 1.0 * cub->p_deltas.py * cub->delta_time;	
	}
	if (key == 111)
	{
		cub->player->camera->pos.px -= 1.0 * cub->p_deltas.px * cub->delta_time;
		cub->player->camera->pos.py -= 1.0 * cub->p_deltas.py * cub->delta_time;	
	}
	if (key == 97)
	{
		cub->player->camera->pos.px += 1.0 * cub->p_deltas.py * cub->delta_time;
		cub->player->camera->pos.py -= 1.0 * cub->p_deltas.px * cub->delta_time;	
	}
	if (key == 101)
	{
		cub->player->camera->pos.px -= 1.0 * cub->p_deltas.py * cub->delta_time;
		cub->player->camera->pos.py += 1.0 * cub->p_deltas.px * cub->delta_time;	
	}
	if (key == 46)
		update_player_angle(cub->player, &cub->p_deltas, &cub->fov1_deltas, &cub->fov2_deltas, 
		cub->player->camera->angle + 100.0 * cub->delta_time);
	if (key == 39)
		update_player_angle(cub->player, &cub->p_deltas, &cub->fov1_deltas, &cub->fov2_deltas, 
		cub->player->camera->angle - 100.0 * cub->delta_time);
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
}

int	focus_out(void *param)
{
	t_cub		*cub;

	cub = (t_cub *)param;
	cub->focus = 0;
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
	fill_img(cub->tmp, color(BLACK));

	// this part is parsing one map ======================================================
	t_list	segments;

	segments = list(NULL);
	list_push_b(&segments, node(segment(line(point(5, 6), point(2, 3))), &default_node_free));//hard
	list_push_b(&segments, node(segment(line(point(0, 0), point(7, 1))), &default_node_free));
	list_push_b(&segments, node(segment(line(point(7, 1), point(7, 8))), &default_node_free));
	list_push_b(&segments, node(segment(line(point(7, 8), point(1, 8))), &default_node_free));
	list_push_b(&segments, node(segment(line(point(1, 8), point(0, 0))), &default_node_free));

	list_push_b(&segments, node(segment(line(point(3, 6), point(5, 6))), &default_node_free));//easy
	list_push_b(&segments, node(segment(line(point(2, 4), point(3, 6))), &default_node_free));
	list_push_b(&segments, node(segment(line(point(2, 3), point(2, 4))), &default_node_free));

	t_map_editor map_edit;

	//map_edit = new_map_editor(segments, 50, cub->tmp->resolution);

	// this part is parsing the map ======================================================
	t_list	new_segments;

	new_segments = list(NULL);
	cub->root_node = build_bsp(segments, &new_segments);

	cub->segments = new_segments;
	
	cub->map_editor = new_map_editor();
	cub->game_mode = PAUSE;
	cub->player = new_player(NULL);
	cub->player->camera->angle = 45;
	calculate_deltas(cub->player, &cub->p_deltas, &cub->fov1_deltas, &cub->fov2_deltas);

	mlx_put_image_to_window(cub->mlx, cub->mlx_win, cub->tmp->img, 0, 0);

	if (!cub)
	{
		write(2, "Error: cannot initialize the general struct\n", 45);
		return (-1);
	}
	cub->player->camera->pos.px = 2;
	cub->player->camera->pos.py = 2;
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
