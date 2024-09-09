/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 22:32:43 by samusanc          #+#    #+#             */
/*   Updated: 2024/09/02 18:10:32 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "cub.h"

//==============================================

typedef struct s_engine_obj t_engine_obj;


//Plantilla gg
/*

void	del_plantilla_obj(t_plantilla *obj)
{
	if (obj)
	{

		ft_bzero(obj, sizeof(t_plantilla));
		free(obj);
	}
}

t_plantilla	*new_plantilla_obj()
{
	t_plantilla	*result;

	result = malloc(sizeof(t_plantilla));
	if (!result)
		return (NULL);
	ft_bzero(result, sizeof(t_plantilla));
	result->del = del_plantilla_obj;

	return (result);
}

*/


typedef enum e_event_type
{
	mouse,
	keyboard,
	focus
}			t_event_type;

typedef struct s_event
{
	t_event_type	type;
	int				id;
	int				triggered;
	int				ongoing;
}				t_event;

typedef	struct s_action
{
	t_event event;
	void	(*triggered)();
	void	(*ongoing)();
	void	(*del)();
}				t_action;

void	del_action_obj(t_action *obj)
{
	if (obj)
	{
		ft_bzero(obj, sizeof(t_action));
		free(obj);
	}
}

t_action	*new_action_obj(t_event event)
{
	t_action	*result;

	result = malloc(sizeof(t_action));
	if (!result)
		return (NULL);
	ft_bzero(result, sizeof(t_action));
	result->del = del_action_obj;
	result->triggered = NULL;
	result->ongoing = NULL;
	return (result);
}

typedef struct s_input_mapping
{
	t_list			actions;
	void			(*del)();
}				t_input_mapping;

void	del_input_mapping_obj(t_input_mapping *obj)
{
	if (obj)
	{
		list_clear(&obj->actions);
		ft_bzero(obj, sizeof(t_input_mapping));
		free(obj);
	}
}

t_input_mapping	*new_input_mapping_obj()
{
	t_input_mapping	*result;

	result = malloc(sizeof(t_input_mapping));
	if (!result)
		return (NULL);
	ft_bzero(result, sizeof(t_input_mapping));
	result->del = del_input_mapping_obj;
	return (result);
}

typedef	struct s_engine_game_mode
{
	char			*name;
	t_input_mapping	*input_context;
	void			(*engine_game_mode)();
	void			(*del)();
}				t_engine_game_mode;

void	*del_engine_game_mode(t_engine_game_mode *obj)
{
	if (obj)
	{
		free(obj->name);
		obj->input_context->del(obj->input_context);
		ft_bzero(obj, sizeof(t_engine_game_mode));
		free(obj);
	}
}

t_engine_game_mode	*new_engine_game_mode(char *name)
{
	t_engine_game_mode	*result;
	char		*name_tmp;

	result = malloc(sizeof(t_engine_game_mode));
	if (!result)
		return (NULL);
	ft_bzero(result, sizeof(t_engine_game_mode));
	result->del = del_engine_game_mode;
	name_tmp = ft_strdup(name);
	if (!name)
	{
		result->del(result);
		return (NULL);
	}
	return (result);
}

typedef struct s_engine
{
	t_cub				*cub;
	unsigned int		frame;
	double				delta_time;
	t_engine_game_mode	*actual_game_mode;
	t_list				game_modes;
	t_list				obj;
	void				(*del)();
}				t_engine;

void	del_engine_obj(t_engine *obj)
{
	if (obj)
	{
		obj->cub->del(obj->cub);
		list_clear(&obj->game_modes);
		list_clear(&obj->obj);

		ft_bzero(obj, sizeof(t_engine));
		free(obj);
	}
}

t_engine	*new_engine_obj()
{
	t_engine	*result;

	result = malloc(sizeof(t_engine));
	if (!result)
		return (NULL);
	ft_bzero(result, sizeof(t_engine));
	result->del = del_engine_obj;
	return (result);
}

typedef struct t_transform
{
	char			*name;
	void			*parent;
	t_point			position;
	t_point			rotation;
	t_list			components;
	t_engine_obj	*(*get_component)();
}				t_transform;

t_engine_obj	*get_component(t_engine_obj *parent, char *name)
{
	//srry implement latter or maybe never :p
	return (NULL);
}

t_transform	transform()
{
	t_transform	result;

	ft_bzero(&result, sizeof(t_transform));
	result.get_component = get_component;
	return (result);
}

typedef struct s_engine_obj
{
	void		*engine;
	void		*parent;
	t_transform	transform;
	int			(*start)();
	int			(*update)();
	int			(*on_exit)();
	void			(*del)();
}				t_engine_obj;

void			del_obj_engine_obj(t_engine_obj *obj)
{
	if (obj)
		list_clear(&obj->transform.components);
	free(obj);
	return ;
}

t_engine_obj	*new_obj_engine_obj(void *engine)
{
	t_engine_obj	*result;

	result = malloc(sizeof(t_engine_obj));
	if (!result)
		return (NULL);
	ft_bzero(result, sizeof(t_engine_obj));

	result->engine = engine;
	result->transform = transform();
	result->transform.parent = engine;
	result->del = del_obj_engine_obj;
	return (result);
}

t_node	*add_component(t_engine_obj *obj)
{
	return (node(obj, obj->del));
}


//===========================================================================//


//===========================================================================//
void	free_gen_struct(t_cub *cub)
{
	if (cub)
	{		
		if (cub->mlx)
			free(cub->mlx);

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

	void	(*del)();
}				t_segment;

int	del_segment_obj(t_segment *obj)
{
	if (obj)
	{

		ft_bzero(obj, sizeof(t_segment));
		free(obj);
	}
}

t_segment	*new_segment_obj(t_line line)
{
	t_segment	*result;
	t_point		a;
	t_point		b;

	result = malloc(sizeof(t_segment));
	if (!result)
		return (NULL);
	ft_bzero(result, sizeof(t_segment));
	result->del = del_segment_obj;
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

t_map_editor	map_editor();

t_map_editor	map_editor()
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
	pt.px = (int)pt.px;
	pt.py = (int)pt.py;
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
	content_tmp = new_segment_obj(splitter.segment);
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

				r_segment = new_segment_obj(line(seg_tmp->segment.a, intersection_pt));
				l_segment = new_segment_obj(line(intersection_pt, seg_tmp->segment.b));
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
		//return (easy_bbox_2d(split, segments));
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
		fill_img(cub->editor_img, color_from_rgb(0, 0, 0));
		//fill_img(cub->editor_img, color_from_rgb(255, 0, 0));
	cub->game_mode = change_game_mode(PAUSE);

	draw_circle(50, cub->editor_img, color_point(point(x, 200), color_from_rgb(255, 0, 0)));
	x += (float)100 * (float)sign * cub->delta_time;

	mlx_mouse_show(cub->mlx, cub->main_window->mlx_win);

	draw_circle(50, cub->editor_img, color_point(point(x, 200), color_from_rgb(255, 255, 255)));
	mlx_put_image_to_window(cub->mlx, cub->main_window->mlx_win, cub->editor_img->img, 0, 0);
	return (1);
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

t_point	mouse_pos_relative(t_cub *cub, t_win *win)
{
	t_point	result;
	int		x;
	int		y;

	x = 0;
	y = 0;
	result = cub->map_editor.screen_center;
	mlx_mouse_get_pos(cub->mlx, win->mlx_win, &x, &y);

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

int	draw_bsp_node(t_bsp *node, t_cub *cub, t_map_editor map_editor, t_color col, int mode, t_img *img)
{
	if (node)
	{
		draw_segment(node->splitter, map_editor, img, col);
		if (col.hex != color(BLACK).hex)
		{
			draw_bbox(node->back_bbox, map_editor, color(RED), img);
			draw_bbox(node->front_bbox, map_editor, color(GREEN), img);
		}
		else
		{
			draw_bbox(node->back_bbox, map_editor, col, img);
			draw_bbox(node->front_bbox, map_editor, col, img);
		}
		if (!mode)
		{
			if (node->front)
				draw_bsp_node(node->front, cub, map_editor, col, mode, img);
			if (node->back)
				draw_bsp_node(node->back, cub, map_editor, col, mode, img);
		}
	}
}

int	draw_bsp(t_cub *cub, t_map_editor map_editor, t_color col, int mode, t_img *img)
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
				draw_bsp_node(last_root, cub, map_editor, color(BLACK), mode, img);
			draw_bsp_node(root, cub, map_editor, col, mode, img);
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
			draw_bsp_node(tmp_root, cub, map_editor, col, mode, img);
	}
}

int	draw_player(t_cub *cub, t_map_editor map_editor, t_color col, t_img *img)
{
	// para comprobar la bbox del player y un sector simplemente saca el angulo entre el player y el pt medio del sector
	static	t_point	last_pos = {0};
	static	t_point	last_fov1 = {0};
	static	t_point	last_fov2 = {0};
	t_point	pos;
	t_point	fov1;
	t_point	fov2;

	pos = cub->player->camera->pos;

	fov1 = point(pos.px + (13.0  * cub->fov1_deltas.px), 
	pos.py + (13.0  * cub->fov1_deltas.py));

	fov2 = point(pos.px + (13.0  * cub->fov2_deltas.px), 
	pos.py + (13.0  * cub->fov2_deltas.py));


	pos = remap_point(pos, map_editor.screen_zoom, map_editor.screen_center, img->resolution);
	fov1 = remap_point(fov1, map_editor.screen_zoom, map_editor.screen_center, img->resolution);
	fov2 = remap_point(fov2, map_editor.screen_zoom, map_editor.screen_center, img->resolution);

	pos.px = (int)pos.px;
	pos.py = (int)pos.py;
	draw_circle(5, img, color_point(last_pos, color(BLACK)));
	draw_circle(5, img, color_point(pos, col));

	draw_line(last_pos, last_fov1, img);
	draw_line(last_pos, last_fov2, img);

	last_pos = pos;
	last_fov1 = fov1;
	last_fov2 = fov2;

	pos.color = col;
	fov1.color = col;
	fov2.color = col;

	draw_line(pos, fov1, img);
	draw_line(pos, fov2, img);
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


float	find_min_x_bbox(t_bbox_2d bbox)
{
	float	min;

	min = bbox.a.a.px;
	if (min < bbox.a.b.px)
		min = bbox.a.b.px;
	if (min < bbox.b.a.px)
		min = bbox.b.a.px;
	if (min < bbox.b.b.px)
		min = bbox.b.b.px;
	return (min);
}

float	find_min_y_bbox(t_bbox_2d bbox)
{
	float	min;

	min = bbox.a.a.py;
	if (min < bbox.a.b.py)
		min = bbox.a.b.py;
	if (min < bbox.b.a.py)
		min = bbox.b.a.py;
	if (min < bbox.b.b.py)
		min = bbox.b.b.py;
	return (min);
}

float	find_max_x_bbox(t_bbox_2d bbox)
{
	float	max;

	max = bbox.a.a.px;
	if (max > bbox.a.b.px)
		max = bbox.a.b.px;
	if (max > bbox.b.a.px)
		max = bbox.b.a.px;
	if (max > bbox.b.b.px)
		max = bbox.b.b.px;
	return (max);
}

float	find_max_y_bbox(t_bbox_2d bbox)
{
	float	max;

	max = bbox.a.a.py;
	if (max > bbox.a.b.py)
		max = bbox.a.b.py;
	if (max > bbox.b.a.py)
		max = bbox.b.a.py;
	if (max > bbox.b.b.py)
		max = bbox.b.b.py;
	return (max);
}


// a chunk of code for know if you are inside the bbox
	/*
	t_color	last;
	t_point	player;
	t_point	player_x;
	t_line	test;
	t_line	up;
	t_line	down;
	t_line	left;
	t_line	right;

	player = cub->player->camera->pos;
	player_x = point(find_min_x(bbox), player.py);
	test = line(player_x, player);

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

	int number;

	number = 0;

	if (intersection_check(test, up))
		number += 1;
	if (intersection_check(test, down))
		number += 1;
	if (intersection_check(test, left))
		number += 1;
	if (intersection_check(test, right))
		number += 1;
	if (number % 2 != 0)
	{
		srand(bbox.a.a.px);
		last = color_from_rgb(rand(), rand(), rand());

		draw_bbox(bbox, cub->map_editor, last, cub->tmp);
		return (1);
	}
	return (0);
	*/
typedef struct s_bbox_sides
{
	t_point	a;
	t_point	b;
	t_point	c;
	t_point	d;
	int		size;
}				t_bbox_sides;

t_bbox_sides	bbox_sides(t_line a, t_line b, int size)
{
	t_bbox_sides	result;

	result.a = a.a;
	result.b = a.b;
	result.c = b.a;
	result.b = b.b;
	result.size = size;
	return (result);
}

float	point_to_angle(t_point pos, t_point pt)
{
	t_point	delta;
	float	angle;

	delta.px = pt.px - pos.px;
	delta.py = pt.py - pos.py;
	angle = (atan2(delta.py, delta.px));
	angle = angle * (180.0 / PI);
	return (angle);
}

int	norm(float angle)
{
	return ((int)angle % 360);
}

// no quiero hacer esto otra vez;
	/*
	float	min_px;
	float	min_py;
	float	max_px;
	float	max_py;
	t_point	pos;
	t_color	last;
	t_bbox_sides	bs;


	pos = cub->player->camera->pos;
	min_px = find_min_x_bbox(bbox);
	min_py = find_min_y_bbox(bbox);
	max_px = find_max_x_bbox(bbox);
	max_py = find_max_y_bbox(bbox);
	t_point	a;
	t_point b;
	t_point c;
	t_point d;

	a = point(min_px, min_py);
	b = point(min_px, max_py);
	c = point(max_px, max_py);
	d = point(max_px, min_py);
	bs = bbox_sides(line(b, a), line(c, b), 2);
	if (pos.px < min_px)
	{
		if (pos.py > max_py)
			bs = bbox_sides(line(b, a), line(c, b), 2);
		else if (pos.py < min_py)
			bs = bbox_sides(line(b, a), line(a, d), 2);
		else
			bs = bbox_sides(line(b, a), line(c, b), 1);
	}
	if (pos.px > max_px)
	{
		if (pos.py > max_py)
			bs = bbox_sides(line(c, b), line(d, c), 2);
		else if (pos.py < min_py)
			bs = bbox_sides(line(b, a), line(d, c), 2);
		else
			bs = bbox_sides(line(b, a), line(c, b), 1);
	}
	else
	{
		if (pos.py > max_py)
			bs = bbox_sides(line(c, b), line(d, c), 1);
		else if (pos.py < min_py)
			bs = bbox_sides(line(a, d), line(d, c), 1);
		else
			return (1);
	}

	int i;
	float	angle1;
	float	angle2;
	int		span;
	int		span1;

	i = 0;
	while (i < bs.size)
	{
		if (i == 0)
		{
			angle1 = point_to_angle(pos, bs.a);
			angle2 = point_to_angle(pos, bs.b);
		}
		else
		{
			angle1 = point_to_angle(pos, bs.c);
			angle2 = point_to_angle(pos, bs.d);
		}

		span = norm(angle1 - angle2);
		angle1 -= cub->player->camera->angle;
		span1 = norm(angle1 + (cub->player->camera->fov / 2.0));
		if (span1 > (int)cub->player->camera->fov)
		{
			if (!(span1 >= span + cub->player->camera->fov))
				return (1);
		}
		i++;
	}
	return (0);
	*/


//t_polygon square(t_line a, t_line b)

int	check_point_in_front_of_segment(t_point pt, t_segment sp_seg)
{
	float		numerator;
	float		denominator;
	int			denominator_is_zero;
	int			numerator_is_zero;
	float		intersection;
	t_point		intersection_pt;

	numerator = cross_2d(point(pt.px - sp_seg.segment.a.px, pt.py - sp_seg.segment.a.py), sp_seg.vector);
	denominator = cross_2d(sp_seg.vector, pt);

	denominator_is_zero = ft_abs(denominator) < EPS;
	numerator_is_zero = ft_abs(numerator) < EPS;

	if (denominator_is_zero && numerator_is_zero)
		return (1);
	if (numerator < 0 || (numerator_is_zero && denominator > 0))
		return (1);
	else if (numerator > 0 || (numerator_is_zero && denominator < 0))
		return (0);
	return (0);
}
int	free_is_in_front_of_player(void **a, void **b, void **c)
{
	if (*a)
		free(a);
	if (*b)
		free(b);
	if (*c)
		free(c);
	*a = NULL;
	*b = NULL;
	*c = NULL;
}

typedef struct s_fov
{
	float	fov;
	float	h_fov;
	float	angle;
	float	fov1_angle;
	float	fov2_angle;
	t_point	fov1_delta;
	t_point	fov2_delta;
}				t_fov;

t_fov	make_fov_with_angles(float fov, float angle, float angle_fov1, float angle_fov2)
{
	t_fov	result;

	result.fov = fov;
	result.h_fov = fov / 2.0;
	result.angle = angle;
	if (angle_fov1 > angle)
		result.fov1_angle = angle;
	else
		result.fov1_angle = angle_fov1;
	if (angle_fov2 < angle)
		result.fov2_angle = angle;
	else
		result.fov2_angle = angle_fov2;
	result.fov1_delta.px = cos(deg2_rad(result.fov1_angle));
	result.fov1_delta.py = sin(deg2_rad(result.fov1_angle));
	result.fov2_delta.px = cos(deg2_rad(result.fov2_angle));
	result.fov2_delta.py = sin(deg2_rad(result.fov2_angle));
	return (result);
}

t_fov	make_fov(float fov, float angle)
{
	t_fov	result;

	result.fov = fov;
	result.h_fov = fov / 2.0;
	result.angle = angle;
	result.fov1_angle = angle - result.h_fov;
	result.fov2_angle = angle + result.h_fov;
	result.fov1_delta.px = cos(deg2_rad(result.fov1_angle));
	result.fov1_delta.py = sin(deg2_rad(result.fov1_angle));
	result.fov2_delta.px = cos(deg2_rad(result.fov2_angle));
	result.fov2_delta.py = sin(deg2_rad(result.fov2_angle));
	return (result);
}

float	det(t_point a, t_point b)
{
	return(a.px * b.py - a.py * b.px);
}

t_point	get_intersection_between_lines(t_line line1, t_line line2, int *error)
{
	t_point	xdiff;
	t_point	ydiff;
	t_point	d;
	float	div;
	float	x;
	float	y;

	xdiff = point(line1.a.px - line1.b.px, line2.a.px - line2.b.px);
	ydiff = point(line1.a.py - line1.b.py, line2.a.py - line2.b.py);
	div = det(xdiff, ydiff);
	if (div < EPS || div == 0)
	{
		*error = 1;
		return(point(0, 0));
	}
	d = point(det(line1.a, line1.b), det(line2.a, line2.b));
	x = det(d, xdiff) / div;
	y = det(d, ydiff) / div;
	return (point(x, y));
}

//old cut_segment
/*
void	cut_segment(t_cub *cub, int *cut, t_segment *seg, t_fov *fov, t_point fov1, t_point fov2, t_point pos, 
int is_in_front, 
int if_segment_a_is_in_front_of_fov1, 
int if_segment_b_is_in_front_of_fov1, 
int if_segment_a_is_in_front_of_fov2, 
int if_segment_b_is_in_front_of_fov2,
int intersect_fov1,
int intersect_fov2)
{
	int	error;
	t_line	fov1_l;
	t_line	fov2_l;
	t_point	x;
	t_point	y;
	t_point	cross;

	fov1_l = line(pos, fov1);
	fov2_l = line(pos, fov2);
	if (if_segment_a_is_in_front_of_fov1 && if_segment_a_is_in_front_of_fov2)
	{
		if (intersect_fov1)
		{
			error = 0;
			cross = get_intersection_between_lines(fov1_l, seg->segment, &error);
			if (error)
				return ;
			seg->segment.b = cross;
		}
		if (intersect_fov2)
		{
			error = 0;
			cross = get_intersection_between_lines(fov2_l, seg->segment, &error);
			if (error)
				return ;
			seg->segment.b = cross;
		}
	}
	else if (if_segment_b_is_in_front_of_fov1 && if_segment_b_is_in_front_of_fov2)
	{
		printf("\n\nb before:final_segment:p1(%f, %f),p2(%f, %f)\n", seg->segment.a.px, seg->segment.a.py,
		seg->segment.b.px, seg->segment.b.py);
		//draw_line_remap(seg->segment, cub->map_editor, cub->tmp, color(GREEN));
		if (intersect_fov1)
		{
			error = 0;
			cross = get_intersection_between_lines(fov1_l, seg->segment, &error);
			if (error)
				return ;
			seg->segment.a = cross;
		}
		if (intersect_fov2)
		{
			//printf("yeahh\n");
			error = 0;
			cross = get_intersection_between_lines(fov2_l, seg->segment, &error);
			if (error)
				return ;
			seg->segment.a = cross;
		}
		printf("b:final_segment:p1(%f, %f),p2(%f, %f)\n", seg->segment.a.px, seg->segment.a.py,
		seg->segment.b.px, seg->segment.b.py);
	}
	else //if (is_in_front)
	{
		error = 0;
		x = get_intersection_between_lines(fov1_l, seg->segment, &error);
		if (error)
			return ;
		error = 0;
		y = get_intersection_between_lines(fov2_l, seg->segment, &error);
		if (error)
			return ;
		if (if_segment_a_is_in_front_of_fov1)
		{
			seg->segment.a = y;
			seg->segment.b = x;
		}
		else
		{
			seg->segment.a = x;
			seg->segment.b = y;
		}
	}
}
*/

//==================================================================

void	cut_segment(t_cub *cub, t_segment *seg, t_point fov1, t_point fov2, int case_num) 
{
	t_point	cross;
	t_line	tmp;
	int		error;

	if (!seg || !case_num)
		return ;
	if (case_num == 1)
	{
		tmp = line(cub->player->camera->pos, fov1);
		error = 0;
		cross = get_intersection_between_lines(tmp, seg->segment, &error);
		if (error)
			return ;
		seg->segment.a = cross;
	}
	else if (case_num == 2)
	{
		tmp = line(cub->player->camera->pos, fov2);
		error = 0;
		cross = get_intersection_between_lines(tmp, seg->segment, &error);
		if (error)
			return ;
		seg->segment.b = cross;
	}
	else if (case_num == 3)
	{
		tmp = line(cub->player->camera->pos, fov1);
		error = 0;
		cross = get_intersection_between_lines(tmp, seg->segment, &error);
		if (error)
			return ;
		seg->segment.a = cross;
	}
	else if (case_num == 4)
	{
		tmp = line(cub->player->camera->pos, fov2);
		error = 0;
		cross = get_intersection_between_lines(tmp, seg->segment, &error);
		if (error)
			return ;
		seg->segment.b = cross;
	}
	else if (case_num == 5)
	{
		tmp = line(cub->player->camera->pos, fov2);
		error = 0;
		cross = get_intersection_between_lines(tmp, seg->segment, &error);
		if (error)
			return ;
		seg->segment.b = cross;
		tmp = line(cub->player->camera->pos, fov1);
		error = 0;
		cross = get_intersection_between_lines(tmp, seg->segment, &error);
		if (error)
			return ;
		seg->segment.a = cross;
	}
}
/*
*/

int	update_player_angle_from_angle(t_cub *cub, float angle);

int	is_in_front_of_player(t_cub *cub, t_segment *seg, int *cut, t_fov *fov)
{
	t_point		tmp;
	t_point		pos;
	t_point		screen_min;
	t_point		screen_max;
	t_point		fov1;
	t_point		fov2;
	t_segment	*tmp1;
	t_segment	*tmp2;
	t_segment	*tmp3;
	int			is_a_in_front_of_fov1;
	int			is_b_in_front_of_fov1;
	int			is_a_in_front_of_fov2;
	int			is_b_in_front_of_fov2;
	int			is_a_in_front_of_the_player;
	int			is_b_in_front_of_the_player;

	if (!seg)
		return (0);
	tmp.px = seg->segment.a.px + seg->segment.b.px;
	tmp.px = tmp.px / 2.0;
	tmp.py = seg->segment.a.py + seg->segment.b.py;
	tmp.py = tmp.py / 2.0;
	pos = cub->player->camera->pos;

	screen_min = cub->player->camera->pos;
	screen_max = cub->player->camera->pos;

	screen_min.px -= cub->p_deltas.py;
	screen_min.py += cub->p_deltas.px;
	screen_max.px += cub->p_deltas.py;
	screen_max.py -= cub->p_deltas.px;

	fov1 = cub->player->camera->pos;
	fov2 = cub->player->camera->pos;
	fov1.px += fov->fov1_delta.px * 100.0f;
	fov1.py += fov->fov1_delta.py * 100.0f;
	fov2.px += fov->fov2_delta.px * 100.0f;
	fov2.py += fov->fov2_delta.py * 100.0f;

	tmp1 = new_segment_obj(line(screen_min, screen_max));
	if (!tmp1)
		return (0);
	tmp2 = new_segment_obj(line(pos, fov1));
	if (!tmp2)
	{
		free(tmp1);
		return (0);
	}
	tmp3 = new_segment_obj(line(fov2, pos));
	if (!tmp3)
	{
		free(tmp1);
		free(tmp2);
		return (0);
	}
	is_a_in_front_of_fov1 = check_point_in_front_of_segment(seg->segment.a, *tmp2);
	is_b_in_front_of_fov1 = check_point_in_front_of_segment(seg->segment.b, *tmp2);
	is_a_in_front_of_fov2 = check_point_in_front_of_segment(seg->segment.a, *tmp3);
	is_b_in_front_of_fov2 = check_point_in_front_of_segment(seg->segment.b, *tmp3);

	is_a_in_front_of_the_player = check_point_in_front_of_segment(seg->segment.a, *tmp1);
	is_b_in_front_of_the_player = check_point_in_front_of_segment(seg->segment.b, *tmp1);

	int segment_intersect_with_fov1;
	int	segment_intersect_with_fov2;


	free(tmp1);
	free(tmp2);
	free(tmp3);

	/*
	if ((is_a_in_front_of_fov1 || is_b_in_front_of_fov1) && 
	(is_a_in_front_of_fov2 || is_b_in_front_of_fov2))
	{
		segment_intersect_with_fov1 = intersection_check(seg->segment, line(pos, fov1));
		segment_intersect_with_fov2 = intersection_check(seg->segment, line(pos, fov2));
		if (segment_intersect_with_fov1 || segment_intersect_with_fov2)
			cut_segment(cub, cut, seg, fov, fov1, fov2, pos, (is_a_in_front_of_the_player && is_b_in_front_of_the_player), is_a_in_front_of_fov1, is_b_in_front_of_fov1, is_a_in_front_of_fov2, is_b_in_front_of_fov2, segment_intersect_with_fov1, segment_intersect_with_fov2);
		if ((is_a_in_front_of_the_player && is_b_in_front_of_the_player))
		{
			*cut = (segment_intersect_with_fov1 * 1) + (segment_intersect_with_fov2 * 2);
			return (1);
		}
		else
		{
			if (segment_intersect_with_fov1 || segment_intersect_with_fov2)
			{
				if ((is_a_in_front_of_the_player && is_b_in_front_of_the_player))
					*cut = (segment_intersect_with_fov1 * 1) + (segment_intersect_with_fov2 * 2);
				return (1);
			}
			return (0);
		}
	}
	*/
	int	a_is_inside_fov;
	int	b_is_inside_fov;
	int	is_in_front_of_the_player;
	int	fov_cut_all_the_segment;

	a_is_inside_fov = (is_a_in_front_of_fov1 && is_a_in_front_of_fov2);
	b_is_inside_fov = (is_b_in_front_of_fov1 && is_b_in_front_of_fov2);
	is_in_front_of_the_player = (is_a_in_front_of_the_player && is_b_in_front_of_the_player);

	// this means the segments is inside without cutting anything
	if (a_is_inside_fov && b_is_inside_fov)
		return (1);
	else
	{
		segment_intersect_with_fov1 = intersection_check(seg->segment, line(pos, fov1));
		segment_intersect_with_fov2 = intersection_check(seg->segment, line(pos, fov2));

		if (!(a_is_inside_fov || b_is_inside_fov) 
		&& !(segment_intersect_with_fov1 && segment_intersect_with_fov2))
			return (0);
		if (is_a_in_front_of_the_player && a_is_inside_fov && !b_is_inside_fov)
		{
			if (segment_intersect_with_fov1)
				cut_segment(cub, seg, fov1, fov2, 1);
			if (segment_intersect_with_fov2)
				cut_segment(cub, seg, fov1, fov2, 2);
			return (1);
		}
		if (is_b_in_front_of_the_player && b_is_inside_fov && !a_is_inside_fov)
		{
			if (segment_intersect_with_fov1)
				cut_segment(cub, seg, fov1, fov2, 3);
			if (segment_intersect_with_fov2)
				cut_segment(cub, seg, fov1, fov2, 4);
			return (1);
		}
		if (is_in_front_of_the_player)
		{
			*cut = 3;
			cut_segment(cub, seg, fov1, fov2, 5);
			return (1);
		}
		if (segment_intersect_with_fov1 && segment_intersect_with_fov2)
		{
			if (!is_a_in_front_of_the_player && !is_b_in_front_of_the_player)
				return (0);
			if (is_b_in_front_of_fov2)
				return (0);
			if (is_a_in_front_of_the_player && is_a_in_front_of_fov1)
				return (0);
			*cut = 3;
			cut_segment(cub, seg, fov1, fov2, 5);
			return (1);
		}
		return (0);
	}
	// old cut segment function
	//		cut_segment(cub, cut, seg, fov, fov1, fov2, pos, (is_a_in_front_of_the_player && is_b_in_front_of_the_player), is_a_in_front_of_fov1, is_b_in_front_of_fov1, is_a_in_front_of_fov2, is_b_in_front_of_fov2, segment_intersect_with_fov1, segment_intersect_with_fov2);

	/*
	if ((is_a_in_front_of_fov1 && is_a_in_front_of_fov2 && is_a_in_front_of_the_player) || (is_b_in_front_of_fov1 && is_b_in_front_of_fov2 && is_b_in_front_of_the_player))
		return (1);
		*/
	return (0);
}
		/*
		draw_line_remap(line(screen_max, screen_min), cub->map_editor, cub->tmp, color(GREEN));
		fov_cut_all_the_segment = (segment_intersect_with_fov1 
		&& segment_intersect_with_fov2);

		if (a_is_inside_fov && !b_is_inside_fov)
		{
			//this means the b_edge is begin cut
			return (1);

		}
		else if (!a_is_inside_fov && b_is_inside_fov)
		{
			//this means the a_edge is begin cut
			return (1);

		}
		else
		{
			//extreme case
			if (fov_cut_all_the_segment && is_in_front_of_player)
			{
				return (1);

			}
		}
		return (0);
		*/

int	is_player_in_front(t_point pt, t_bsp *node)
{
	return (check_point_in_front_of_segment(pt, *(node->splitter)));
}

float	distance_between_points(t_point a, t_point b)
{
	float	result;

	result = sqrt(((b.px - a.px) * (b.px - a.px)) 
	+ ((b.py - a.py) * (b.py - a.py)));
	return (result);
}

int	draw_line_remap(t_line line, t_map_editor map_editor, t_img *img, t_color col)
{
	t_point	a;
	t_point	b;

	a = remap_point(line.a, map_editor.screen_zoom, map_editor.screen_center, img->resolution);
	b = remap_point(line.b, map_editor.screen_zoom, map_editor.screen_center, img->resolution);
	a.color = col;
	b.color = col;
	draw_line(a, b, img);
}

typedef	struct s_render_vertex
{
	t_line	vertex;
	float	min_dist;
	t_color	color;
}				t_render_vertex;

/*
	from now, i have to remade everything so when i say new = malloc
*/

t_render_vertex	*new_render_vertex()
{
	t_render_vertex	*result;

	result = malloc(sizeof(t_render_vertex));
	if (!result)
		return(NULL);
	return (result);
}

t_render_vertex	*new_render_vertex_from_line(t_line line)
{
	t_render_vertex	*result;

	result = new_render_vertex();
	if (!result)
		return(NULL);
	result->vertex = line;
	return (result);
}

float	get_min_dist(t_line line, t_point p)
{
	float dist1;
	float dist2;

	dist1 = distance_between_points(line.a, p);
	dist2 = distance_between_points(line.b, p);
	if (dist2 < dist1)
		return (dist2);
	return (dist1);
}

/*
	insert in back of the referece node
*/
void	list_instert_back(t_list *list, t_node *node_to_instert, t_node *reference_node)
{
	t_node	*tmp_back;
	t_node	*tmp_front;

	tmp_front = reference_node;
	tmp_back = NULL;
	if (reference_node)
		tmp_back = reference_node->back;
	if (tmp_back)
		tmp_back->next = node_to_instert;
	node_to_instert->back = tmp_back;
	if (tmp_front)
		tmp_front->back = node_to_instert;
	node_to_instert->next = tmp_front;
	list->size++;
}

/*
	insert in front of the referece node
*/
void	list_instert_front(t_list *list, t_node *node_to_insert, t_node *reference_node)
{
	t_node	*tmp_back;
	t_node	*tmp_front;

	tmp_back = reference_node;
	tmp_front = NULL;
	if (reference_node)
		tmp_front = reference_node->next;
	if (tmp_back)
		tmp_back->next = node_to_insert;
	node_to_insert->back = tmp_back;
	if (tmp_front)
		tmp_front->back = node_to_insert;
	node_to_insert->next = tmp_front;
	list->size++;
}

void	insert_vertex_to_render(t_render_vertex *node_to_insert, t_list *render_list)
{
	t_node			*tmp;
	t_node			*tail;
	t_render_vertex	*tmp_vertex;

	tmp_vertex = NULL;
	tmp = render_list->head;
	tail = render_list->tail;
	if (tail)
	{
		tmp_vertex = tail->content;
		if (tmp_vertex)
		{
			if (node_to_insert->min_dist > tmp_vertex->min_dist)
			{
				list_push_b(render_list, node(node_to_insert, &default_node_free));
				return ;
			}
		}
	}
	if (tmp)
	{
		tmp_vertex = tmp->content;
		if (tmp_vertex)
		{
			if (node_to_insert->min_dist < tmp_vertex->min_dist)
			{
				list_push_f(render_list, node(node_to_insert, &default_node_free));
				return ;
			}
		}
	}
	while (tmp)
	{
		tmp_vertex = tmp->content;
		if (node_to_insert->min_dist < tmp_vertex->min_dist)
		{
			list_instert_back(render_list, node(node_to_insert, &default_node_free), tmp);
			return ;
		}
		tmp = tmp->next;
	}
	list_push_b(render_list, node(node_to_insert, &default_node_free));
	return ;
}

int	get_render_vertex_from_bsp(t_cub *cub, t_bsp *node, t_fov *fov, int *lock, float *max_dist, t_list *render_list)
{
	int				cut;
	t_segment		*tmp;
	t_point			tmp1;
	t_render_vertex	*tmp2;
	int				checker;
	float			dist;
	float			min_dist;

	tmp = NULL;
	checker = 1;
	if (node)
	{
		if (is_player_in_front(cub->player->camera->pos, node))
		{
			cut = 0;
			if (node->splitter)
			{
				tmp = new_segment_obj(node->splitter->segment);
				if (is_in_front_of_player(cub, tmp, &cut, fov))
				{
					if (cut == 3 || *lock)
					{
						tmp1.px = node->splitter->segment.a.px + node->splitter->segment.b.px;
						tmp1.px = tmp1.px / 2.0;
						tmp1.py = node->splitter->segment.a.py + node->splitter->segment.b.py;
						tmp1.py = tmp1.py / 2.0;
						dist = distance_between_points(tmp1, cub->player->camera->pos);
						if (dist < *max_dist)
						{
							checker = 1;
							*max_dist = dist;
						}
						else
							checker = 0;
						*lock = 1;
					}
					if (checker)
					{
						min_dist = get_min_dist(tmp->segment, cub->player->camera->pos);
						tmp2 = new_render_vertex_from_line(tmp->segment);
						if (!tmp2)
						{
							free(tmp);
							return (0);
						}
						tmp2->min_dist = min_dist;
						srand(node->id);
						tmp2->color = color_from_rgb(rand() % 255, rand() % 255, rand() % 255);

						insert_vertex_to_render(tmp2, render_list);
						//draw_segment(tmp, cub->map_editor, cub->tmp, color(GREEN));
					}
				}
			}
			free(tmp);
			//draw_line_remap(node->splitter->segment, cub->map_editor, cub->tmp, color(BLUE));
		}
		if (node->front)
			get_render_vertex_from_bsp(cub, node->front, fov, lock, max_dist, render_list);
		if (node->back)
			get_render_vertex_from_bsp(cub, node->back, fov, lock, max_dist, render_list);
	}
	return (1);
}

int	draw_fov_intersection(t_cub *cub, t_map_editor map_editor, t_color col, t_img *img)
{
	t_fov	fov;
	int		lock;
	float	max_dist;
	t_list	render_list;

	lock = 0;
	fov = make_fov(cub->player->camera->fov, cub->player->camera->angle);
	max_dist = FLT_MAX;
	render_list = list(NULL);
	if (cub->root_node)
		get_render_vertex_from_bsp(cub, cub->root_node, &fov, &lock, &max_dist, &render_list);
	t_node	*tmp;
	t_render_vertex	*tmp1;

	tmp = render_list.head;
	while (tmp)
	{
		tmp1 = tmp->content;
		/*
		tmp1->vertex.a = rotate_point(tmp1->vertex.a, 360.0 - cub->player->camera->angle);
		tmp1->vertex.a.px += cub->player->camera->pos.px;
		tmp1->vertex.a.py += cub->player->camera->pos.py;
		tmp1->vertex.b = rotate_point(tmp1->vertex.b, 360.0 - cub->player->camera->angle);
		tmp1->vertex.b.px += cub->player->camera->pos.px;
		tmp1->vertex.b.py += cub->player->camera->pos.py;
		*/
		if (col.hex != color(BLACK).hex)
			draw_line_remap(tmp1->vertex, map_editor, img, tmp1->color);
		else
			draw_line_remap(tmp1->vertex, map_editor, img, col);
		tmp = tmp->next;
	}
}

int	proyect_point_to_screen(t_point pt, t_cub *cub)
{
	/*
	t_line	proyection;
	t_line	screen;
	t_point	cross;
	t_point	cross1;
	t_line	screen1;
	int		error;
	float	tmp;

	error = 0;
	pt.px += cub->player->camera->pos.px;
	pt.py += cub->player->camera->pos.py;
	pt = rotate_point(pt, -cub->player->camera->angle);
	screen = line(point(-((float)(cub->tmp->resolution.width + 10) / 2.0), 0), 
	point((float)(cub->tmp->resolution.width + 10) / 2.0, 0));
	pt.px *= 100;
	pt.py = 500;
	draw_circle(10, cub->tmp, pt);
	*/
	
	/*
	proyection = line(cub->player->camera->pos, pt);
	screen = line(cub->fov1_screen, cub->fov2_screen);
	cross = get_intersection_between_lines(proyection, screen, &error);
	cross.color = pt.color;
	if (!error)
	{
		cross = rotate_point(cross, -cub->player->camera->angle);
		cross.py += cub->player->camera->pos.py;
		cross.py = cub->tmp->resolution.height / 2;
		draw_circle(10, cub->tmp, cross);
	}
	*/
}

int	render_3d(t_cub *cub)
{
	t_fov	fov;
	int		lock;
	float	max_dist;
	t_list	render_list;

	// get render list
	lock = 0;
	fov = make_fov(cub->player->camera->fov, cub->player->camera->angle);
	max_dist = FLT_MAX;
	render_list = list(NULL);
	if (cub->root_node)
		get_render_vertex_from_bsp(cub, cub->root_node, &fov, &lock, &max_dist, &render_list);
	// get render list

	t_node	*tmp;
	t_render_vertex	*tmp1;

	tmp = render_list.head;
	while (tmp)
	{
		tmp1 = tmp->content;
		proyect_point_to_screen(color_point(tmp1->vertex.a, tmp1->color), cub);
		proyect_point_to_screen(color_point(tmp1->vertex.b, tmp1->color), cub);
		tmp = tmp->next;
	}
}



//#include <emmintrin.h>
#include <immintrin.h>

int fast_clean_pixels(long long *dest, t_resolution res)
{
    size_t i;
    size_t size;
    size_t simd_size;
    size_t remaining;
    __m256i *simd_dest;
    __m256i zero;

    zero = _mm256_setzero_si256();
    size = (res.width * res.height) / 2;
    simd_size = size / 4;
    remaining = size % 4;
    simd_dest = (__m256i *)dest;

	i = 0;
	while (i < simd_size)
	{
        _mm256_storeu_si256(&simd_dest[i], zero);
		i++;
	}
	i = 0;
	while (i < remaining)
	{
        dest[simd_size * 4 + i] = 0;
		i++;
	}
    return 0;
}

int	slow_clean_pixels(long long *dest, t_resolution res)
{
	size_t	i;
	size_t	size;

	size = (res.height * res.width) / 2;
	i = 0;
	while (i < size)
	{
		if (dest[i])
			dest[i] = 0;
		i++;
	}
	return (1);
}

int	screenshot_pixels(long long *dest, const long long *src, t_resolution res)
{
	size_t			i;
	size_t			size;
	size_t			simd_size;
	size_t			remaining;
	__m256i			*simd_dest;
	const __m256i	*simd_src = (const __m256i *)src;

	i = 0;
	size = (res.width * res.height) / 2;
	remaining = size % 4;
	simd_size = size / 4;
	simd_dest = (__m256i *)dest;
	while (i < size)
	{
		_mm256_storeu_si256(&simd_dest[i], _mm256_load_si256(&simd_src[i]));
		i++;
	}
	i = 0;
	while (i < remaining)
	{
		dest[size - remaining + i] = src[size - remaining + i];
		i++;
	}
}

int	clean_pixels(t_img *img)
{

	// change this!!!!!!!!
	if (!BONUS)
		fast_clean_pixels(img->data_addr, img->resolution);
	else
		slow_clean_pixels(img->data_addr, img->resolution);
}

typedef	struct s_cub_ray
{
	int		hit;
	int		x;
	int		y;
	float	dist;
	int		(*del)();
}				t_cub_ray;

void	del_cub_ray_obj(t_cub_ray *obj)
{
	if (obj)
	{
		ft_bzero(obj, sizeof(t_cub_ray));
		free(obj);
	}
}

t_cub_ray	*new_cub_ray_obj()
{
	t_cub_ray	*result;

	result = malloc(sizeof(t_cub_ray));
	if (!result)
		return (NULL);
	ft_bzero(result, sizeof(t_cub_ray));
	result->del = del_cub_ray_obj;
	return (result);
}

float	get_dist_delt_y(float delta_y, float y, float start_y)
{
	return ((y - start_y) / (delta_y));
}

t_point	dda_calculate_y_down(t_cub *cub, float delta_x, float delta_y)
{
	t_point	result;
	t_point	player;

	player = cub->player->camera->pos;
	result.py = (float)((int)player.py - 1);
	result.px = player.px + delta_x * (get_dist_delt_y(delta_y, result.py, player.py));




	//result.px = player.px + delta_x * 10.0f;
	//result.py = player.py + delta_y * 10.0f;
	result = remap_point(result, cub->map_editor.screen_zoom, cub->map_editor.screen_center, cub->editor_img->resolution);
	player = remap_point(player, cub->map_editor.screen_zoom, cub->map_editor.screen_center, cub->editor_img->resolution);
	player.color = color(GREEN);
	result.color = color(GREEN);
	draw_line(player, result, cub->editor_img);
	return (result);
}


t_point	compare_dists(t_point start, t_point *dist1, t_point *dist2, t_point *dist3)
{
	float	dista;
	float	distb;
	float	distc;
	float	result;

	distc = -1.0f;
	dista = distance_between_points(start, *dist1);
	distb = distance_between_points(start, *dist2);
	if (dist3)
		distc = distance_between_points(start, *dist3);
	if (dista < distb)
		result = dista;
	else
		result = distb;
	if (distc < result && distc != -1.0f)
		result = distc;
	if (result == dista)
		return (*dist1);
	if (result == distb)
		return (*dist2);
	return (*dist3);
}

t_cub_ray	*cub_cast_ray(t_cub *cub, float angle, float distance, t_map_editor minimap)
{
	t_cub_ray	*result;
	t_point		player;
	t_point		tmp_ray1;
	t_point		tmp_ray2;
	t_point		ray;
	float		hypo;
	float		screen_dist;
	float		delta_x;
	float		delta_y;

	result = NULL;//new_cub_ray_obj();
//	if (!result)
//		return (NULL);
	player = cub->player->camera->pos;
	ray = player;
	delta_x = cos(deg2_rad(angle));
	delta_y = sin(deg2_rad(angle));
	ray.px += delta_x * distance;
	ray.py += delta_y * distance;
	/*
	tmp_ray1 = dda_calculate_x_right(cub, delta_x, delta_y);
	tmp_ray2 = dda_calculate_y_up(cub, delta_x, delta_y);
	ray = compare_dists(player, &tmp_ray1, &tmp_ray2, &ray);
	*/
	if (angle < 90.0f && angle > 0.0f)
	{

		return (NULL);	
	}
	else if (angle > 270.0f && angle < 360.0f)
	{
	//	tmp_ray1 = dda_calculate_x_right(cub, delta_x, delta_y);
		tmp_ray2 = dda_calculate_y_down(cub, delta_x, delta_y);
	//	ray = compare_dists(player, &tmp_ray1, &tmp_ray2, &ray);
		ray = tmp_ray2;

		return (NULL);	
	}
	else if (angle > 90.0f && angle < 180.0f)
	{


		/*
		tmp_ray1 = dda_calculate_x_left(cub, delta_x, delta_y);
		tmp_ray2 = dda_calculate_y_up(cub, delta_x, delta_y);
		ray = compare_dists(player, &tmp_ray1, &tmp_ray2, &ray);
		*/
		return (NULL);	

	}
	else if (angle > 180.0f && angle < 270.0f)
	{
	//	tmp_ray1 = dda_calculate_x_left(cub, delta_x, delta_y);
		tmp_ray2 = dda_calculate_y_down(cub, delta_x, delta_y);
		//ray = compare_dists(player, &tmp_ray1, &tmp_ray2, &ray);
		ray = tmp_ray2;
		return (NULL);	
	}
	else if (angle == 0.0f && angle == 180.0f)
	{
		/*
		if (angle == 180)
			tmp_ray1 = dda_calculate_x_left(cub, delta_x, delta_y);
		else
			tmp_ray1 = dda_calculate_x_right(cub, delta_x, delta_y);
		ray = compare_dists(player, &tmp_ray1, &ray, NULL);
		*/
		return (NULL);	

	}
	else if (angle == 90.0f && angle == 270.0f)
	{
		/*
		if (angle == 90)
			tmp_ray1 = dda_calculate_y_up(cub, delta_x, delta_y);
		else
			tmp_ray1 = dda_calculate_y_down(cub, delta_x, delta_y);
		ray = compare_dists(player, &tmp_ray1, &ray, NULL);
		*/
		return (NULL);	

	}

	hypo = distance_between_points(player, ray);
	screen_dist = sin(deg2_rad(cub->player->camera->angle - angle)) * hypo;
	screen_dist = sqrt((hypo * hypo) - (screen_dist * screen_dist));
	/*
	result->x = (int)ray.px;
	result->y = (int)ray.py;
	result->dist = screen_dist;
	if (result->dist < distance)
		result->hit = 1;
		*/
	return (result);
}

void	draw_walls_from_ray(t_cub_ray *ray)
{
	if (!ray)
		return ;

	ray->del(ray);
}

int	ray_casting(t_cub *cub, t_map_editor minimap)
{
	size_t	number_of_rays;
	size_t	iterator;
	float	start_angle;
	float	multiplier;

	number_of_rays = cub->main_window->res.width;
	multiplier = (float)cub->player->camera->fov / (float)number_of_rays;
	start_angle = cub->player->camera->angle;
	start_angle = start_angle - (cub->player->camera->fov / 2.0f);
	iterator = 0;
	while (iterator < number_of_rays)
	{

		draw_walls_from_ray(cub_cast_ray(cub, start_angle, 10.0f, minimap));
		iterator++;
		start_angle += multiplier;
	}
}

int	game_mode(t_cub *cub)
{
	t_map_editor	minimap;

	if (cub->game_mode != GAME)
		fill_img(cub->game_img, color_from_rgb(0, 0, 0));
	cub->game_mode = GAME;
	minimap = map_editor();
	minimap.screen_zoom = 10;
	minimap.screen_center = point(cub->player->camera->pos.px * minimap.screen_zoom,
	cub->player->camera->pos.py * minimap.screen_zoom);//cub->player->camera->pos;
	

	clean_pixels(cub->game_img);
	clean_pixels(cub->minimap_img);

	draw_grid(minimap, cub->minimap_img, color_from_rgb(100, 100, 100));
	draw_map_walls(cub, minimap, cub->minimap_img);
	draw_player(cub, minimap, color_from_rgb(255, 255, 0), cub->minimap_img);

	ray_casting(cub, minimap);

	mlx_put_image_to_window(cub->mlx, cub->main_window->mlx_win, cub->game_img->img, 0, 0);
	mlx_put_image_to_window(cub->mlx, cub->main_window->mlx_win, cub->minimap_img->img, 0, 0);
}

int	draw_square(size_t length, t_img *img, t_point start)
{
	size_t	i;
	size_t	j;
	size_t	start_x;
	size_t	start_y;

	i = 0;
	j = 0;
	start_x = (size_t)start.px;
	while (i < length)
	{
		j = 0;
		start.px = start_x;
		while (j < length)
		{
			put_pixel(img, start);
			j++;
			start.px++;
		}
		i++;
		start.py++;
	}
	return (1);
}

int	draw_map_walls(t_cub *cub, t_map_editor editor, t_img *img)
{
	t_point	tmp;
	size_t	x;
	size_t	y;

	y = 0;
	while (cub->map[y])
	{
		x = 0;
		while (cub->map[y][x])
		{
			if (cub->map[y][x] == '1')
			{
				tmp = remap_point(point(x, y), 
				editor.screen_zoom, editor.screen_center, 
				img->resolution);
				tmp.color = color(WHITE);
				put_pixel(img, tmp);
				draw_square(editor.screen_zoom + 1, img, tmp);
			}
			x++;
		}
		y++;
	}
	return (1);
}

int editor_mode(t_cub *cub)
{
	if (cub->game_mode != EDITOR)
	{
		mlx_put_image_to_window(cub->mlx, cub->main_window->mlx_win, cub->editor_img->img, 0, 0);
		fill_img(cub->editor_img, color_from_rgb(0, 0, 0));
	}

	cub->game_mode = EDITOR;
	if (cub->mouse_press)
		cub->map_editor.screen_center = mouse_pos_relative(cub, cub->main_window);

	clean_pixels(cub->editor_img);


	mlx_mouse_show(cub->mlx, cub->main_window->mlx_win);

	draw_grid(cub->map_editor, cub->editor_img, color_from_rgb(100, 100, 100));

	//draw_segments(cub->segments, cub->map_editor, cub->editor_img, color_from_rgb(50, 50, 50));
	//draw_fov_intersection(cub, cub->map_editor, color_from_rgb(255, 0, 255), cub->editor_img);
	draw_map_walls(cub, cub->map_editor, cub->editor_img);

	ray_casting(cub, cub->map_editor);
	draw_player(cub, cub->map_editor, color_from_rgb(255, 255, 0), cub->editor_img);

	mlx_put_image_to_window(cub->mlx, cub->main_window->mlx_win, cub->editor_img->img, 0, 0);
}

#include <time.h>

void	change_buffers(t_triple_buff_img *img)
{
	t_img	*tmp;

	tmp = img->render;
	img->render = img->buffer_a;
	img->buffer_a = img->buffer_b;
	img->buffer_b = tmp;
}

int	acurated_delta_time(t_cub *cub)
{
	double	now;
	int		second;
	static	double last_f;
	static	int last;

	now = (double)(clock() * 5) / CLOCKS_PER_SEC;
	cub->delta_time = now - last_f;
	last_f = now;
	second = 0;
	if ((int)now > last)
	{
		second = 1;
		printf("fps:%d\n", cub->frame);
		printf("player pos:%f,%f, angle:%f\n", 
		cub->player->camera->pos.px, cub->player->camera->pos.py, cub->player->camera->angle);
		printf("last:%f\n", now);
		cub->frame = 0;
		last = now;
	}
	return (second);
}

int	frame(void *p_cub)
{
	t_cub			*cub;


	cub = (t_cub *)p_cub;
	cub->frame += 1;
	// change this!!!!!!!!
	if (!BONUS)
		acurated_delta_time(cub);
	else
		cub->delta_time = 0.016f;

	if (cub->delta_time < 0.016f)
		cub->delta_time = 0.016f;
	if (cub->delta_time > 0.1f)
		cub->delta_time = 0.1f;

		if (!cub->focus)
			pause_mode(cub);
		if (cub->game_mode == GAME)
			game_mode(cub);
		else if (cub->game_mode == EDITOR)
			editor_mode(cub);
	return (0);
}

int	mouse_press(int key, int x, int y, void *param)
{
//	printf("key:%d, intx;%d, inty:%d\n", key, x, y);
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
	printf("mouse:%d, intx;%d, inty:%d\n", key, x, y);
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

int	update_player_angle_from_angle(t_cub *cub, float angle)
{
	float fov;

		cub->player->camera->angle = angle;
		fov = cub->player->camera->fov;
		fov = fov / 2.0f;
		cub->p_deltas.px = cos(deg2_rad(angle));
		cub->p_deltas.py = sin(deg2_rad(angle));
		cub->fov1_deltas.px = cos(deg2_rad(angle - fov));
		cub->fov1_deltas.py = sin(deg2_rad(angle - fov));
		cub->fov2_deltas.px = cos(deg2_rad(angle + fov));
		cub->fov2_deltas.py = sin(deg2_rad(angle + fov));

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
	printf("super gofy key:%d\n", key);
	if (key == XK_m)
	{
		if (cub->game_mode == GAME)
			return (editor_mode(cub));
		else
			return (game_mode(cub));
	}
	if (key == XK_w)
	{
		cub->player->camera->pos.px += 1.0 * cub->p_deltas.px * cub->delta_time;
		cub->player->camera->pos.py += 1.0 * cub->p_deltas.py * cub->delta_time;	
	}
	if (key == XK_s)
	{
		cub->player->camera->pos.px -= 1.0 * cub->p_deltas.px * cub->delta_time;
		cub->player->camera->pos.py -= 1.0 * cub->p_deltas.py * cub->delta_time;	
	}
	if (key == XK_a)
	{
		cub->player->camera->pos.px += 1.0 * cub->p_deltas.py * cub->delta_time;
		cub->player->camera->pos.py -= 1.0 * cub->p_deltas.px * cub->delta_time;	
	}
	if (key == XK_d)
	{
		cub->player->camera->pos.px -= 1.0 * cub->p_deltas.py * cub->delta_time;
		cub->player->camera->pos.py += 1.0 * cub->p_deltas.px * cub->delta_time;	
	}
	if (key == XK_e)
	{
		update_player_angle(cub->player, &cub->p_deltas, &cub->fov1_deltas, &cub->fov2_deltas, 
		cub->player->camera->angle + 100.0 * cub->delta_time);
		cub->fov1_screen.px = cub->player->camera->pos.px * cub->fov1_deltas.px * cub->fov1_screen.pz;
		cub->fov1_screen.py = cub->player->camera->pos.py * cub->fov1_deltas.py * cub->fov1_screen.pz;

		cub->fov2_screen.px = cub->player->camera->pos.px * cub->fov2_deltas.px * cub->fov2_screen.pz;
		cub->fov2_screen.py = cub->player->camera->pos.py * cub->fov2_deltas.py * cub->fov2_screen.pz;
		printf("screen width:%f\n", distance_between_points(cub->fov1_screen, cub->fov2_screen));

	}
	if (key == XK_q)
	{
		update_player_angle(cub->player, &cub->p_deltas, &cub->fov1_deltas, &cub->fov2_deltas, 
		cub->player->camera->angle - 100.0 * cub->delta_time);

		cub->fov1_screen.px = cub->player->camera->pos.px * cub->fov1_deltas.px * cub->fov1_screen.pz;
		cub->fov1_screen.py = cub->player->camera->pos.py * cub->fov1_deltas.py * cub->fov1_screen.pz;

		cub->fov2_screen.px = cub->player->camera->pos.px * cub->fov2_deltas.px * cub->fov2_screen.pz;
		cub->fov2_screen.py = cub->player->camera->pos.py * cub->fov2_deltas.py * cub->fov2_screen.pz;
		printf("screen width:%f\n", distance_between_points(cub->fov1_screen, cub->fov2_screen));

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
}

int	focus_out(void *param)
{
	t_cub		*cub;

	cub = (t_cub *)param;
	cub->focus = 0;
}

t_hooks	hooks(int (*option1)(), int (*option2)())
{
	t_hooks	result;

	result.option1 = option1;
	result.option2 = option2;
	return (result);
}

t_win_hooks	win_hooks(t_hooks mouse, t_hooks key, t_hooks focus)
{
	t_win_hooks	result;

	result.mouse = mouse;
	result.key = key;
	result.focus = focus;
	return (result);
}

t_win	*new_window(void *mlx, t_resolution res, char *title)
{
	t_win				*result;
	static unsigned int	id = 0;

	result = malloc(sizeof(t_win));
	if (!result)
		return (NULL);
	result->mlx_win = mlx_new_window(mlx, res.width, res.height, title);
	if (!result->mlx_win)
	{
		free(result);
		return (NULL);
	}
	result->res = res;
	result->id = id++;
	return (result);
}

void	start_hooks_in_window (t_win *win, t_win_hooks win_hooks, void *data)
{
	if (!win)
		return ;
	if (win_hooks.mouse.option1)
		mlx_hook(win->mlx_win, 4, (1L<<2), win_hooks.mouse.option1, data);
	if (win_hooks.mouse.option2)
		mlx_hook(win->mlx_win, 5, (1L<<3), win_hooks.mouse.option2, data);
	if (win_hooks.key.option1)
		mlx_hook(win->mlx_win, 2, (1L<<0), win_hooks.key.option1, data);
	/* // in development
	if (win_hooks.key.option2)
	*/
	if (win_hooks.focus.option1)
		mlx_hook(win->mlx_win, 9, (1L<<21), win_hooks.focus.option1, data);
	if (win_hooks.focus.option2)
		mlx_hook(win->mlx_win, 10, (1L<<21), win_hooks.focus.option2, data);
}

t_triple_buff_img	*new_triple_buff_img(void *mlx, t_resolution res)
{
	t_triple_buff_img	*result;

	result = malloc(sizeof(t_triple_buff_img));
	if (!result)
		return (NULL);
	result->render = init_img(mlx, res);
	if (!result->render)
	{
		free(result);
		return (NULL);
	}
	result->buffer_a = init_img(mlx, res);
	if (!result->buffer_a)
	{
		free_img(result->render);
		free(result);
		return (NULL);
	}
	result->buffer_b = init_img(mlx, res);
	if (!result->buffer_b)
	{
		free_img(result->render);
		free_img(result->buffer_a);
		free(result);
		return (NULL);
	}
	fill_img(result->render, color(BLACK));
	fill_img(result->buffer_a, color(BLACK));
	fill_img(result->buffer_b, color(BLACK));
	return (result);
}


/*
t_engine_obj	*new_pawn_obj(void *engine)
{
	t_engine_obj	*result;

	result = new_actor_obj(engine);
	if (!result)
		return (NULL);
	result->transform = transform();
	result->transform.parent = engine;
	if (!add_component(result, new_player_controller(engine)))
	{
		result->free_obj(result);
		return (NULL);
	}
	return (result);
}

t_engine_obj	*new_player_controller(void *engine)
{
	t_engine_obj	*result;

	result = new_pawn_obj(engine);
	if (!result)
		return (NULL);
	result->transform = transform();
	result->transform.parent = engine;
	return (result);
}

t_engine_obj	*new_character_obj(void *engine)
{
	t_engine_obj	*result;

	result = new_pawn_obj(engine);
	if (!result)
		return (NULL);
	result->transform = transform();
	result->transform.parent = engine;
	if (!add_component(result, new_player_controller(engine)))
	{
		result->free_obj(result);
		return (NULL);
	}
	return (result);
}
*/

int	main(int argc, char **argv)
{
	t_cub	*cub;

	if (argc > 2 || (argc < 2 && !BONUS))
	{
		write(2, "Error: wrong number of arguments\n", 34);
		return (-1);
	}


	printf("proccess fine!!!2\n");
	cub = ft_constructor(argv[1]);

	if (!cub)
	{
		write(2, "Error: cannot initialize the general struct\n", 45);
		return (-1);
	}


	//================================================================================
	cub->main_window = new_window(cub->mlx, resolution(1920, 1080), "main_window");

	if (!cub->main_window)
	{
		printf("error pls fix this after\n");
		exit(-1);
	}
	start_hooks_in_window(cub->main_window, win_hooks(
	hooks(mouse_press, mouse_release),
	hooks(key_press, NULL),
	hooks(focus_in, focus_out)), cub);

	cub->editor_img = init_img(cub->mlx, cub->main_window->res);
	if (!cub->editor_img)
	{
		printf("error pls fix this after\n");
		exit(-1);
	}


	cub->game_img = init_img(cub->mlx, cub->main_window->res);
	if (!cub->game_img)
	{
		printf("error pls fix\n");
		exit(-1);

	}

	cub->minimap_img = init_img(cub->mlx, resolution(
		cub->main_window->res.height / 5, cub->main_window->res.height / 5));

	// this part is parsing one map ======================================================
	t_list	segments;

	segments = list(NULL);
	list_push_b(&segments, node(new_segment_obj(line(point(5, 6), point(2, 3))), &default_node_free));//hard
	list_push_b(&segments, node(new_segment_obj(line(point(0, 0), point(7, 1))), &default_node_free));
	list_push_b(&segments, node(new_segment_obj(line(point(7, 1), point(7, 8))), &default_node_free));
	list_push_b(&segments, node(new_segment_obj(line(point(7, 8), point(1, 8))), &default_node_free));
	list_push_b(&segments, node(new_segment_obj(line(point(1, 8), point(0, 0))), &default_node_free));

	list_push_b(&segments, node(new_segment_obj(line(point(-10, -10), point(17, 1))), &default_node_free));
	list_push_b(&segments, node(new_segment_obj(line(point(17, 1), point(17, 18))), &default_node_free));
	list_push_b(&segments, node(new_segment_obj(line(point(17, 18), point(1, 18))), &default_node_free));
	list_push_b(&segments, node(new_segment_obj(line(point(1, 18), point(-10, -10))), &default_node_free));

	list_push_b(&segments, node(new_segment_obj(line(point(3, 6), point(5, 6))), &default_node_free));//easy
	list_push_b(&segments, node(new_segment_obj(line(point(2, 4), point(3, 6))), &default_node_free));
	list_push_b(&segments, node(new_segment_obj(line(point(2, 3), point(2, 4))), &default_node_free));
	
   char **map;

	map = malloc(sizeof(char *) * 8);
	ft_bzero(map, sizeof(char *) * 8);
    // Fill the map with '1's on the borders and '0's inside
    for (int i = 0; i < 7; i++) {
		map[i] = malloc(sizeof(char) * 8);
		ft_bzero(map[i], sizeof(char) * 8);
        for (int j = 0; j < 7; j++) {
            if (i == 0 || i == 6 || j == 0 || j == 6) {
                map[i][j] = '1'; // Borders
            } else {
                map[i][j] = '0'; // Inside
            }
        }
    }

    // Place the player 'P' in the middle (3,3)
    //map[3][3] = 'P';

    // Print the map
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            printf("%c ", map[i][j]);
        }
        printf("\n");
    }
	cub->map = map;

	t_map_editor map_edit;

	//map_edit = new_map_editor(segments, 50, cub->tmp->resolution);

	// this part is parsing the map ======================================================
	t_list	new_segments;

	new_segments = list(NULL);
	cub->root_node = build_bsp(segments, &new_segments);

	cub->segments = new_segments;
	
	cub->map_editor = map_editor();
	cub->game_mode = PAUSE;
	cub->player = new_player(NULL);
	cub->player->camera->angle = 90;

	calculate_deltas(cub->player, &cub->p_deltas, &cub->fov1_deltas, &cub->fov2_deltas);

	// make screen limits
	cub->fov1_screen.pz = (float)(((float)(cub->main_window->res.width + 2) / 4.0f)) / cub->fov1_deltas.px;
	cub->fov2_screen.pz = cub->fov1_screen.pz;
	cub->fov1_screen.px = cub->player->camera->pos.px + (cub->fov1_deltas.px * cub->fov1_screen.pz);
	cub->fov1_screen.py = cub->player->camera->pos.py + (cub->fov1_deltas.py * cub->fov1_screen.pz);
	cub->fov2_screen.px = cub->player->camera->pos.px + (cub->fov2_deltas.px * cub->fov2_screen.pz);
	cub->fov2_screen.py = cub->player->camera->pos.py + (cub->fov2_deltas.py * cub->fov2_screen.pz);
	cub->fov1_screen.color = color(WHITE);
	cub->fov2_screen.color = color(WHITE);
	// make screen limits

	mlx_put_image_to_window(cub->mlx, cub->main_window->mlx_win, cub->editor_img->img, 0, 0);

	// here goes the real angle and the real camera

	cub->player->camera->pos.px = 2;
	cub->player->camera->pos.py = 2;

	// here goes the real angle and the real camera

	mlx_loop_hook(cub->mlx, frame, cub);
	mlx_loop(cub->mlx);
}
