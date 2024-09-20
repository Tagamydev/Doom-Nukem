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
#include <time.h>

//==============================================

typedef struct s_engine_obj t_engine_obj;

clock_t	start_t;


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



//============================================== draw map editor in mlx_utils

//=============================================BSP IN ENGINE

float	cross_2d(t_point a, t_point b)
{
	return (a.px * b.py - b.px * a.py);
}

#define EPS 1e-4

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

int	draw_player(t_cub *cub, t_map_editor map_editor, t_color col, t_img *img)
{
	t_point	pos;
	t_point	fov1;
	t_point	fov2;

	pos = cub->player->camera->pos;

	fov1 = point(pos.px + (cub->max_dist * 5  * cub->fov1_deltas.px), 
	pos.py + (cub->max_dist * 5  * cub->fov1_deltas.py));

	fov2 = point(pos.px + (cub->max_dist * 5 * cub->fov2_deltas.px), 
	pos.py + (cub->max_dist  * 5 * cub->fov2_deltas.py));


	pos = remap_point(pos, map_editor.screen_zoom, map_editor.screen_center, img->resolution);
	fov1 = remap_point(fov1, map_editor.screen_zoom, map_editor.screen_center, img->resolution);
	fov2 = remap_point(fov2, map_editor.screen_zoom, map_editor.screen_center, img->resolution);

	pos.px = (int)pos.px;
	pos.py = (int)pos.py;
	draw_circle(5, img, color_point(pos, col));


	pos.color = col;
	fov1.color = col;
	fov2.color = col;

	draw_line(pos, fov1, img);
	draw_line(pos, fov2, img);
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
//==================================================================
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
	int		side;
	int		x;
	float	real_x;
	int		y;
	float	real_y;
	float	dist;
	float	real_dist;
	float	deltx;
	float	delty;
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

float	get_dist_delt(float delta_y, float y, float start_y)
{
	return ((y - start_y) / (delta_y));
}

int	dda_check_map(t_cub *cub, t_point pt)
{
	int	x;
	int	y;
	int	i;
	int	j;

	x = 0;
	y = 0;
	i = 0;
	j = 0;
	x = (int)pt.px;
	y = (int)pt.py;
	if (x < 0 || y < 0)
		return (-1);
	while (cub->map[i])
		i++;
	i--;
	while (cub->map[0][j])
		j++;
	j--;
	if (y > j || x > i)
		return (-1);
	if (cub->map[y][x] == '1')
		return (1);
	return (0);
}

t_point	dda_calculate_y_down(t_cub *cub, float delta_x, float delta_y)
{
	t_point	result;
	t_point	player;
	int		map_check;
	float	dist;


	result.pz = 2.0f;
	player = cub->player->camera->pos;
	result.py = (float)((int)player.py + 1.0f);
	result.py += 0.000001f;
	result.px = player.px + delta_x * (get_dist_delt(delta_y, result.py, player.py));

	map_check = dda_check_map(cub, result);
	if (map_check == 1)
		return (result);
	if (map_check == -1)
		return (result);
	dist = get_dist_delt(delta_y, result.py + 1.0f, result.py);
	while (!map_check)
	{
		result.py += delta_y * dist;
		result.px += delta_x * dist;
		map_check = dda_check_map(cub, result);
	}
	return (result);
}


t_point	dda_calculate_y_up(t_cub *cub, float delta_x, float delta_y)
{
	t_point	result;
	t_point	player;
	int		map_check;
	float	dist;

	result.pz = 0.0f;
	player = cub->player->camera->pos;
	result.py = (float)((int)player.py);
	result.py -= 0.000001f;
	result.px = player.px + delta_x * (get_dist_delt(delta_y, result.py, player.py));

	map_check = dda_check_map(cub, result);
	if (map_check == 1)
		return (result);
	if (map_check == -1)
		return (result);
	dist = get_dist_delt(delta_y, result.py - 1.0f, result.py);
	while (!map_check)
	{
		result.py += delta_y * dist;
		result.px += delta_x * dist;
		map_check = dda_check_map(cub, result);
	}
	return (result);
}

t_point	dda_calculate_x_right(t_cub *cub, float delta_x, float delta_y)
{
	t_point	result;
	t_point	player;
	int		map_check;
	float	dist;

	result.pz = 1.0f;
	player = cub->player->camera->pos;
	result.px = (float)((int)player.px + 1);
	result.px += 0.000001f;
	result.py = player.py + delta_y * (get_dist_delt(delta_x, result.px, player.px));

	map_check = dda_check_map(cub, result);
	if (map_check == 1)
		return (result);
	if (map_check == -1)
		return (result);
	dist = get_dist_delt(delta_x, result.px + 1.0f, result.px);

	while (!map_check)
	{
		result.py += delta_y * dist;
		result.px += delta_x * dist;
		map_check = dda_check_map(cub, result);
	}
	return (result);
}

t_point	dda_calculate_x_left(t_cub *cub, float delta_x, float delta_y)
{
	t_point	result;
	t_point	player;
	int		map_check;
	float	dist;
	t_point	tmp;

	result.pz = 3.0f;
	player = cub->player->camera->pos;
	result.px = (float)((int)player.px);
	result.px -= 0.000001f;
	result.py = player.py + delta_y * (get_dist_delt(delta_x, result.px, player.px));

	map_check = dda_check_map(cub, result);
	if (map_check == 1)
		return (result);
	if (map_check == -1)
		return (result);
	dist = get_dist_delt(delta_x, result.px - 1.0f, result.px);

	while (!map_check)
	{
		result.py += delta_y * dist;
		result.px += delta_x * dist;
		map_check = dda_check_map(cub, result);
	}
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
	if (dista < 0.0f)
		dista = dista * -1.0f;
	distb = distance_between_points(start, *dist2);
	if (distb < 0.0f)
		distb = distb * -1.0f;

	if (distb)
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

int	check_limits_dda_util(float angle)
{
	if (angle == 0)
		return (1);
	if (angle == 90)
		return (2);
	if (angle == 180)
		return (3);
	if (angle == 270)
		return (4);
	return (0);
}

int	check_limits_dda(float angle)
{
	int	angle_m;
	int	result;

	angle_m = (int)(angle);
	result = check_limits_dda_util(angle_m);
	return (result);
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
	int			limits;

	result = new_cub_ray_obj();

	player = cub->player->camera->pos;
	ray = player;
	delta_x = cos(deg2_rad(angle));
	delta_y = sin(deg2_rad(angle));
	ray.px += delta_x * distance * 100;
	ray.py += delta_y * distance * 100;
	ray.pz = -1.0f;

	limits = check_limits_dda(angle);

	if (limits)
	{
		if (limits == 1)
			tmp_ray1 = dda_calculate_x_right(cub, delta_x, delta_y);
		else if (limits == 2)
			tmp_ray1 = dda_calculate_y_down(cub, delta_x, delta_y);
		else if (limits == 3)
			tmp_ray1 = dda_calculate_x_left(cub, delta_x, delta_y);
		else if (limits == 4)
			tmp_ray1 = dda_calculate_y_up(cub, delta_x, delta_y);
		ray = compare_dists(player, &tmp_ray1, &ray, NULL);
	}
	else if ((int)angle < 90 && (int)angle > 0)
	{
		tmp_ray1 = dda_calculate_x_right(cub, delta_x, delta_y);
		tmp_ray2 = dda_calculate_y_down(cub, delta_x, delta_y);
		ray = compare_dists(player, &tmp_ray1, &tmp_ray2, &ray);
	}
	else if ((int)angle > 270 && (int)angle < 360)
	{
		tmp_ray1 = dda_calculate_x_right(cub, delta_x, delta_y);
		tmp_ray2 = dda_calculate_y_up(cub, delta_x, delta_y);
		ray = compare_dists(player, &tmp_ray1, &tmp_ray2, &ray);
	}
	else if (angle > 90.0f && angle < 180.0f)
	{
		tmp_ray1 = dda_calculate_x_left(cub, delta_x, delta_y);
		tmp_ray2 = dda_calculate_y_down(cub, delta_x, delta_y);
		ray = compare_dists(player, &tmp_ray1, &tmp_ray2, &ray);
	}
	else if (angle > 180.0f && angle < 270.0f)
	{
		tmp_ray1 = dda_calculate_x_left(cub, delta_x, delta_y);
		tmp_ray2 = dda_calculate_y_up(cub, delta_x, delta_y);
		ray = compare_dists(player, &tmp_ray1, &tmp_ray2, &ray);
	}
	/**/
	hypo = distance_between_points(player, ray);
	screen_dist = sin(deg2_rad(cub->player->camera->angle - angle)) * hypo;
	screen_dist = sqrt((hypo * hypo) - (screen_dist * screen_dist));
	result->x = (int)ray.px;
	result->real_x = ray.px;
	result->real_y = ray.py;
	result->y = (int)ray.py;
	result->dist = screen_dist;
	result->deltx = delta_x;
	result->delty = delta_y;
	result->real_dist = distance_between_points(ray, player);

	if (ray.pz > -1.0f)
	{
		result->side = (int)ray.pz;
		result->hit = 1;
	}
	else
	{
		result->hit = 0;
		result->side = (int)ray.pz;
	}
	if (result->dist < distance)
		result->hit = 1;
	return (result);
}

int	get_pixel_img(t_img *img, int x, int y)
{
	if (x < 0 || x > img->resolution.width || y < 0 || y > img->resolution.height)
		return (color_from_rgb(255, 0 ,255).hex);
	return (*(unsigned int *)((img->data_addr + \
	(y * img->line_size) + (x * img->bits_per_pixel / 8))));
}

draw_sky_and_ground(int wall_top, int wall_bottom, t_cub *cub, size_t wall_n, float lerp, int wall_height, t_cub_ray *ray, float angle, float max_dist)
{
	int		i;
	t_point	pixel;

	i = 0;
	pixel.px = (float)wall_n;
	while (i < wall_top)
	{
		pixel.py = (float)i;
		pixel.color = cub->ceiling;
		put_pixel(cub->game_img, pixel);
		i++;
	}
	i = wall_bottom;
	while (i < cub->main_window->res.height)
	{
		pixel.py = (float)i;
		pixel.color = cub->floor;
		put_pixel(cub->game_img, pixel);
		i++;
	}
}


int	ft_get_color(int color, int col)
{
	if (col == 0)
		return ((color >> 24) & 0xFF);
	if (col == 1)
		return ((color >> 16) & 0xFF);
	if (col == 2)
		return ((color >> 8) & 0xFF);
	if (col == 3)
		return ((color) & 0xFF);
	return (0);
}

t_color	color_from_hex(int hex)
{
	t_color	result;

	result.r = ft_get_color(hex, 1);
	result.g = ft_get_color(hex, 2);
	result.b = ft_get_color(hex, 3);
	result.alpha = (float)ft_get_color(hex, 0) / 255.0f;
	result.hex = hex;
	return (result);
}

float	get_real_pos_x(float x, float y, int side)
{
	float	tmp;
	float	result;

	if (side == 0 || side == 2)
	{
		tmp = (int)x;
		if (side == 0)
			return (x - tmp);
		else
			return (1.0f - (x - tmp));
	}
	tmp = (int)y;
	if (side == 1)
		return (y - tmp);
	else
		return (1.0f - (y - tmp));
}

int	check_wall_in_map(int x, int y, char **map)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (x < 0 || y < 0)
		return (0);
	while (map[i])
		i++;
	while (map[0][j])
		j++;
	if (y >= i || x >= j)
		return (0);
	if (map[y][x] == '1')
		return (1);
	return (0);
}

int	get_shadow_case_from_map(int x, int y, t_cub *cub, int side)
{
	int	up;
	int	down;
	int	left;
	int	right;
	int	result;

	if (!side)
	{
		up = check_wall_in_map(x, y - 1, cub->map);
		down = check_wall_in_map(x, y + 1, cub->map);
	}
	else
	{
		left = check_wall_in_map(x - 1, y, cub->map);
		right = check_wall_in_map(x + 1, y, cub->map);
	}
	if (side)
		result = left * 1 + right * 10 + side * 100;
	else
		result = up * 1 + down * 10 + side * 100;
	return (result);
}

int	get_shadow_case_from_ray(t_cub *cub, t_cub_ray *ray)
{
	int	n_case;	

	if (ray->side < 0)
		return (0);
	if (ray->side == 0)
		n_case = get_shadow_case_from_map(ray->x, ray->y + 1, cub, 1);
	if (ray->side == 1)
		n_case = get_shadow_case_from_map(ray->x - 1, ray->y, cub, 0);
	if (ray->side == 2)
		n_case = get_shadow_case_from_map(ray->x, ray->y - 1, cub, 1);
	if (ray->side == 3)
		n_case = get_shadow_case_from_map(ray->x + 1, ray->y, cub, 0);
	return (n_case);
}

void create_corner_texture(float	**shadow_tex, float size)
{
	float	**texture;
	float	half;
	float	disolver;
	int		i;
	int		j;

	texture = shadow_tex;
	half = size;
	i = 0;
	j = 0;
	while (i < (int)size)
	{
		j = 0;
		while (j < (int)size)
		{
			if (i < (int)half)
				texture[i][j] = ((float)i / half);
			else
				texture[i][j] = 1.0f;
			j++;
		}
		i++;
	}
}

void create_ao_texture(float	**shadow_tex, float size)
{
	float	**texture;
	float	half;
	int		i;
	int		j;

	texture = shadow_tex;
	half = size * 0.11f;
	i = 0;
	j = 0;
	while (i < (int)size)
	{
		j = 0;
		while (j < (int)size)
		{
			if (i < (int)half)
				texture[i][j] = (float)i / (half);
			else
				texture[i][j] = 1.0f;
			j++;
		}
		i++;
	}
}
void create_arrow_texture(float	**shadow_tex, float size)
{
	float	**texture;
	float	half;
	int		i;
	int		j;

	texture = shadow_tex;
	half = size;
	i = 0;
	j = 0;
	while (i < (int)size)
	{
		j = 0;
		while (j < (int)size)
		{
			if (i < (int)half)
				texture[i][j] = (float)i / half;
			else
				texture[i][j] = 1.0f;
			j++;
		}
		i++;
	}
}

float	get_shadow(t_cub *cub, int shadow_case, float x_lerp, float y_lerp, int side)
{
	float	size;
	float	tex1;
	float	tex2;
	float	tex3;
	float	tex4;
	float	result;
	float	inverted_x;
	float	inverted_y;

	tex2 = 1;
	tex3 = 1;
	if (y_lerp < 0.0f || y_lerp > 1)
		y_lerp = 0.0f;
	if (x_lerp < 0.0f || x_lerp > 1)
		x_lerp = 0.0f;
	inverted_x = 1.0f - x_lerp;
	inverted_y = 1.0f - y_lerp;
	size = cub->shadow_tex_size - 1.0f;
	tex1 = cub->shadow_tex[(int)(y_lerp * size)][(int)(x_lerp * size)];
	tex4 = cub->shadow_tex[(int)(inverted_y * size)][(int)(inverted_x * size)];
	result = tex1 * tex4;
	if (!shadow_case || shadow_case == 100)
		return (result);
	else
	{
		if (shadow_case < 99)
		{
			if (side == 1)
			{
				inverted_x = x_lerp;
				inverted_y = y_lerp;
				x_lerp = 1.0f - x_lerp;
				y_lerp = 1.0f - y_lerp;
			}
			if (shadow_case == 1)
			{
				tex2 = cub->shadow_tex[(int)(inverted_x * size)][(int)(inverted_y * size)];
				return (result * tex2);
			}
			if (shadow_case == 10)
			{
				tex2 = cub->shadow_tex[(int)(x_lerp * size)][(int)(y_lerp * size)];
				return (result * tex2);
			}
			if (shadow_case == 11)
			{
				tex1 = cub->shadow_tex[(int)(x_lerp * size)][(int)(y_lerp * size)];
				tex2 = cub->shadow_tex[(int)(inverted_x * size)][(int)(inverted_y * size)];
				return ((result * tex2 * tex1) / 0.2f);
			}
		}
		else
		{
			if (side == 0)
			{
				inverted_x = x_lerp;
				inverted_y = y_lerp;
				x_lerp = 1.0f - x_lerp;
				y_lerp = 1.0f - y_lerp;
			}
			if (shadow_case == 101)
			{
				tex2 = cub->shadow_tex[(int)(inverted_x * size)][(int)(inverted_y * size)];
				return (result * tex2);
			}
			if (shadow_case == 110)
			{
				tex2 = cub->shadow_tex[(int)(x_lerp * size)][(int)(y_lerp * size)];
				return (result * tex2);
			}
			if (shadow_case == 111)
			{
				tex1 = cub->shadow_tex[(int)(x_lerp * size)][(int)(y_lerp * size)];
				tex2 = cub->shadow_tex[(int)(inverted_x * size)][(int)(inverted_y * size)];
				return ((result * tex2 * tex1) / 0.2f);
			}
		}
	}
	return (0);
}

void draw_wall(float max_dist, int wall_height, t_cub *cub, size_t wall_n, t_cub_ray *ray, float angle)
{
	float	res_height;
    int		wall_top;
	int		min_top;
    int		wall_bottom;
	t_point	pixel;
	float	color_mix_lerp;
	int		y;
	int		real_pos;
	float	real_pos_x;
	float	shadow;
	int		shadow_case;
	int		mirror_y;
	int		mirror_helper;

	res_height = cub->game_img->resolution.height;
	wall_top = (int)(res_height - (float)wall_height) / 2;
	min_top = wall_top;
	wall_bottom = wall_top + wall_height;
    if (wall_top < 0)
		wall_top = 0;
    if (wall_bottom >= res_height) 
		wall_bottom = (int)(res_height - 1.0f);
	color_mix_lerp = ray->dist / (max_dist * 100);
	color_mix_lerp *= 4;
	if (color_mix_lerp > 1)
		color_mix_lerp = 1;
	if (!BONUS)
		draw_sky_and_ground(wall_top, wall_bottom, cub, wall_n, color_mix_lerp, wall_height, ray, angle, max_dist);
	y = wall_top;
	real_pos_x = get_real_pos_x(ray->real_x, ray->real_y, ray->side);
	shadow_case = get_shadow_case_from_ray(cub, ray);
	mirror_helper = 0;
	while (y < wall_bottom)
	{
		real_pos = y - min_top;
		pixel.px = (float)wall_n;
		pixel.py = (float)y;
		pixel.color	= color(WHITE);
		pixel.color = color_from_hex(get_pixel_img(cub->test_tex, real_pos_x * (float)cub->test_tex->resolution.width, ((float)real_pos / (float)wall_height) * (float)cub->test_tex->resolution.height));

		// SHADOW
		if (BONUS)
		{
			shadow = get_shadow(cub, shadow_case, real_pos_x, ((float)real_pos / (float)wall_height), ray->side);
			shadow = 1.0f - shadow;
			shadow *= cub->ambient_occlusion;
			shadow = 1.0f - shadow;
			pixel.color = color_mix(color(BLACK), pixel.color, shadow);
		}
		else if (ray->side == 1 || ray->side == 3)
			pixel.color = color_mix(pixel.color, color(BLACK), 0.75f);
		pixel.color = color_mix(pixel.color, color(BLACK), color_mix_lerp);

		t_color	tmp_color;
		t_color	tmp_color2;
		//tmp_color = color_from_hex(get_pixel_img(cub->test_tex, real_pos_x * (float)cub->test_tex->resolution.width, ((float)real_pos / (float)wall_height) * (float)cub->test_tex->resolution.height));
	//	pixel.color = color_mix(pixel.color, tmp_color, 0.5f);

		tmp_color = pixel.color;


	//	tmp_color2 = color_from_hex(get_pixel_img(cub->test_tex, real_pos_x * (float)cub->test_tex->resolution.width, ((float)real_pos / (float)wall_height) * (float)cub->test_tex->resolution.height));
	//	pixel.color = color_mix(color(BLACK), tmp_color2, ((float)pixel.color.r / 255.0f));

		put_pixel(cub->game_img, pixel);

	//	tmp_color = pixel.color;
		
		// MIRROR
		if (BONUS)
		{
			mirror_y = wall_bottom + (wall_bottom - y);
			pixel.py = (float)mirror_y;
			pixel.py -= 1.0f;
			pixel.color = color_mix(color(BLACK), pixel.color, 0.5);
			pixel.color = color_mix(pixel.color, color_from_hex(get_pixel_img(cub->game_img, (int)pixel.px, (int)pixel.py)), (1.0f - (float)real_pos / (float)wall_height));
			put_pixel(cub->game_img, pixel);

			// CEILING MIRROR
			mirror_y = wall_top - (y - wall_top);  // Reflect upwards from the wall top
			pixel.py = (float)mirror_y;
			pixel.py += 1.0f;  // Adjust the position to avoid overlap with the original pixel
			pixel.color = tmp_color;
			pixel.color = color_mix(color(BLACK), pixel.color, 0.5);
			pixel.color = color_mix(pixel.color, color_from_hex(get_pixel_img(cub->game_img, (int)pixel.px, (int)pixel.py)), ((float)real_pos / (float)wall_height));
			put_pixel(cub->game_img, pixel);
		}
		y++;
		mirror_helper++;
	}
}

char	sample_map(char **map, float sample_x, float sample_y)
{
	int	max_x;
	int	max_y;
	int		x;
	int		y;

	max_x = 0;
	max_y = 0;
	while (map[max_y])
		max_y++;
	while (map[0][max_x])
		max_x++;
	if (max_y)
		max_y--;
	if (max_x)
		max_x--;
	x = (int)(sample_x);
	y = (int)(sample_y);
	if (x < 0 || y < 0)
		return ('0');
	if (x > max_x || y > max_y)
		return ('0');
	return (map[y][x]);
}

int	get_shadow_case_from_map_no_side(int x, int y, t_cub *cub)
{
	int	up;
	int	down;
	int	left;
	int	right;
	int	result;

	up = check_wall_in_map(x, y - 1, cub->map);
	down = check_wall_in_map(x, y + 1, cub->map);
	left = check_wall_in_map(x - 1, y, cub->map);
	right = check_wall_in_map(x + 1, y, cub->map);
	result = (up * 1) + (down * 10) + (left * 100) + (right * 1000);
	return (result);
}

int	get_corner_case_from_map_no_side(int x, int y, t_cub *cub)
{
	int	up_left;
	int	up_right;
	int	down_left;
	int	down_right;
	int	result;

	up_left = check_wall_in_map(x - 1, y - 1, cub->map);
	up_right = check_wall_in_map(x + 1, y - 1, cub->map);
	down_left = check_wall_in_map(x - 1, y + 1, cub->map);
	down_right = check_wall_in_map(x + 1, y + 1, cub->map);
	result = (up_left * 1) + (up_right * 10) + (down_left * 100) + (down_right * 1000);
	return (result);
}

void	fake_mode_7(t_cub *cub, t_cub_ray *ray, size_t x, float max_dist, size_t wall_size, t_img *img)
{
	float	res_width;
	float	res_height;
	size_t	y;
	size_t	helper;
	t_point	pixel;
	float	min_dist;
	t_point	far_pt1;
	t_point	far_pt2;
	t_point	near_pt1;
	t_point	near_pt2;

	min_dist = cub->near_plane;

	res_width = img->resolution.width;
	res_height = img->resolution.height;
	max_dist = max_dist / cub->player->camera->pos.pz;

	far_pt1 = cub->player->camera->pos;
	far_pt2 = cub->player->camera->pos;
	near_pt1 = cub->player->camera->pos;
	near_pt2 = cub->player->camera->pos;

	far_pt1.px += cub->fov1_deltas.px * max_dist;
	far_pt1.py += cub->fov1_deltas.py * max_dist;

	far_pt2.px += cub->fov2_deltas.px * max_dist;
	far_pt2.py += cub->fov2_deltas.py * max_dist;

	near_pt1.px += cub->fov1_deltas.px * min_dist;
	near_pt1.py += cub->fov1_deltas.py * min_dist;

	near_pt2.px += cub->fov2_deltas.px * min_dist;
	near_pt2.py += cub->fov2_deltas.py * min_dist;

	float	sample_depth;
	float	sample_width;
	float	sample_x;
	float	sample_y;
	float	start_x;
	float	start_y;
	float	end_x;
	float	end_y;
	float	external_darker;
	char	map_sample;
	t_color	ceiling;

	y = res_height / 2;
	external_darker = 1;
	helper = 0;
	sample_width = (float)x / (float)res_width;
	if (wall_size > res_height)
		return ;
	else
	{
		wall_size *= 0.5;
		helper = wall_size; 
		wall_size = (cub->game_img->resolution.height / 2) + wall_size;
		y = wall_size;
	}
	while (y < res_height)
	{
		sample_depth = (float)helper / (float)(res_height / 2);

		start_x = (far_pt1.px - near_pt1.px) / (sample_depth) + near_pt1.px;
		start_y = (far_pt1.py - near_pt1.py) / (sample_depth) + near_pt1.py;

		end_x = (far_pt2.px - near_pt2.px) / (sample_depth) + near_pt2.px;
		end_y = (far_pt2.py - near_pt2.py) / (sample_depth) + near_pt2.py;

		sample_x = (end_x - start_x) * sample_width + start_x;
		sample_y = (end_y - start_y) * sample_width + start_y;

		map_sample = sample_map(cub->map, sample_x, sample_y);
		if (map_sample == '1')
		{
			ceiling = color(BLACK);
			pixel.color = color(BLACK);
		}
		else if (0)
		{
			if ((int)sample_x % 2 == 0)
			{
				if ((int)sample_y % 2 == 0)
					pixel.color = color(BLUE);
				else
					pixel.color = color(GREEN);
			}
			else
			{
				if ((int)sample_y % 2 == 0)
					pixel.color = color(GREEN);
				else
					pixel.color = color(BLUE);
			}

		}
		else
		{
			int	shadow_case;
			float	text_x;
			float	text_y;
			float	darker;
			float	tex_size;
			int		corner_case;

			tex_size = cub->shadow_tex_size - 1;
			darker = 1;
			shadow_case = get_shadow_case_from_map_no_side(sample_x, sample_y, cub);
			pixel.color = cub->floor;
			ceiling = cub->ceiling;
			text_x = sample_x - (float)((int)sample_x);
			text_y = sample_y - (float)((int)sample_y);
			if (shadow_case != 0)
			{
				float	value;

				if (text_x < 0 || text_x > 1)
					text_x = 0;
				if (text_y < 0 || text_y > 1)
					text_y = 0;
				if (shadow_case % 10)
				{
					value = cub->ao_tex[(int)(text_y * tex_size)][(int)(text_x * tex_size)];
					if (value < darker)
						darker = value;
				}
				if ((shadow_case / 10) % 10)
				{
					value = cub->ao_tex[(int)((1.0f - text_y) * tex_size)][(int)((1.0f - text_x) * tex_size)];
					if (value < darker)
						darker = value;
				}

				if ((shadow_case / 100) % 10)
				{
					value = cub->ao_tex[(int)(text_x * tex_size)][(int)(text_y * tex_size)];
					if (value < darker)
						darker = value;
				}

				if ((shadow_case / 1000) % 10)
				{
					value = cub->ao_tex[(int)((1.0f - text_x) * tex_size)][(int)((1.0f - text_y) * tex_size)];
					if (value < darker)
						darker = value;
				}

				darker = 1.0f - darker;
				darker *= cub->ambient_occlusion;
				darker = 1.0f - darker;
				if (darker != 1.0f)
				{
					pixel.color = color_mix(color(BLACK), pixel.color, darker);
					ceiling = color_mix(color(BLACK), ceiling, darker);
				}
			}
			else
			{
				corner_case = get_corner_case_from_map_no_side(sample_x, sample_y, cub);
				float	tmp_darker;
				float	value1;
				float	value2;

				tmp_darker = 1;

				if (corner_case)
				{
					if (text_x < 0 || text_x > 1)
						text_x = 0;
					if (text_y < 0 || text_y > 1)
						text_y = 0;
					if (corner_case % 10)
					{
						value1 = cub->ao_tex[(int)(text_x * tex_size)][(int)(text_y * tex_size)];
						value2 = cub->ao_tex[(int)(text_y * tex_size)][(int)(text_x * tex_size)];

						if (value1 > value2)
							tmp_darker = value1;
						else
							tmp_darker = value2;
					}
					if (darker > tmp_darker)
						darker = tmp_darker;
					if ((corner_case / 10) % 10)
					{
						value1 = cub->ao_tex[(int)((1.0f - text_x) * tex_size)][(int)((1.0f - text_y) * tex_size)];
						value2 = cub->ao_tex[(int)(text_y * tex_size)][(int)(text_x * tex_size)];

						if (value1 > value2)
							tmp_darker = value1;
						else
							tmp_darker = value2;
					}
					if (darker > tmp_darker)
						darker = tmp_darker;

					if ((corner_case / 100) % 10)
					{
						value1 = cub->ao_tex[(int)((1.0f - text_y) * tex_size)][(int)((1.0f - text_x) * tex_size)];
						value2 = cub->ao_tex[(int)(text_x * tex_size)][(int)(text_y * tex_size)];

						if (value1 > value2)
							tmp_darker = value1;
						else
							tmp_darker = value2;
					}
					if (darker > tmp_darker)
						darker = tmp_darker;

					if ((corner_case / 1000) % 10)
					{						
						value1 = cub->ao_tex[(int)((1.0f - text_y) * tex_size)][(int)((1.0f - text_x) * tex_size)];
						value2 = cub->ao_tex[(int)((1.0f - text_x) * tex_size)][(int)((1.0f - text_y) * tex_size)];

						if (value1 > value2)
							tmp_darker = value1;
						else
							tmp_darker = value2;
					}
					if (darker > tmp_darker)
						darker = tmp_darker;

					darker = 1.0f - darker;
					darker *= cub->ambient_occlusion;
					darker = 1.0f - darker;
				}
				if (darker != 1.0f)
				{
					pixel.color = color_mix(color(BLACK), pixel.color, darker);
					ceiling = color_mix(color(BLACK), ceiling, darker);
				}
			}
		}
		float	lerp;

		lerp = (float)(helper) / (((float)cub->game_img->resolution.height) / 2.0f);

		pixel.px = (float)x;
		pixel.py = (float)y;

		float	checker;
		if ((int)sample_x % 2 == 0)
		{
			if ((int)sample_y % 2 == 0)
				checker = 1.0f;
			else
				checker = 0.6f;
		}
		else
		{
			if ((int)sample_y % 2 == 0)
				checker = 0.6f;
			else
				checker = 1.0f;
		}

		ceiling = color_mix(color(BLACK), ceiling, checker);
		pixel.color = color_mix(color(BLACK), pixel.color, checker);
		ceiling = color_mix(color(BLACK), ceiling, lerp);
		pixel.color = color_mix(color(BLACK), pixel.color, lerp);
		put_pixel(img, pixel);

		pixel.py -= (float)(res_height / 2);
		pixel.py = (float)(res_height / 2) - pixel.py;
		pixel.color = ceiling;

		put_pixel(img, pixel);
		y++;
		helper++;
	}
}

void	draw_walls_from_ray(float max_dist, size_t ray_n, float angle, t_cub *cub, t_cub_ray *ray)
{
	t_point	tmp;
	int		wall_height;
	float	dist;
	float	tmp2;
	float	res_height;
	double	last_time;

	if (!ray)
		return ;

	res_height = (float)cub->game_img->resolution.height;
	tmp = cub->player->camera->pos;
	dist = ray->real_dist;
	dist = cub->player->camera->angle - angle;
	dist = deg2_rad(dist);
	dist = ray->real_dist * cos(dist);

	tmp2 = cub->player->camera->pos.pz;
	dist	= dist / tmp2;
	dist = res_height / dist;
	wall_height = (int)dist;

	last_time = (double)((float)(clock() - start_t) / (float)CLOCKS_PER_SEC);
	if (BONUS)
		fake_mode_7(cub, ray, ray_n, max_dist, wall_height, cub->game_img);
	last_time = (double)((float)(clock() - start_t) / (float)CLOCKS_PER_SEC);
	if (cub->game_mode == GAME)
		draw_wall(max_dist, wall_height, cub, ray_n, ray, angle);

	ray->del(ray);
}

int	ray_casting(t_cub *cub, t_map_editor minimap)
{
	size_t	number_of_rays;
	size_t	iterator;
	float	start_angle;
	float	multiplier;
	float	max_dist;

	number_of_rays = cub->game_img->resolution.width;
	multiplier = (float)cub->player->camera->fov / (float)number_of_rays;
	start_angle = cub->player->camera->angle;
	start_angle = start_angle - (cub->player->camera->fov / 2.0f);
	iterator = 0;
	max_dist = cub->max_dist;
	while (iterator < number_of_rays)
	{
		draw_walls_from_ray(max_dist, iterator, start_angle, cub, cub_cast_ray(cub, fix_angle(start_angle), max_dist, minimap));
		start_angle += multiplier;
		iterator++;
	}
}

void	put_img_to_render(t_img *img, t_img *render)
{
	int	x;
	int	y;
	float	x_prop;
	float	y_prop;
	float	aspect_ratio;
	t_point	pixel;

	y = 0;
	aspect_ratio = ((float)render->resolution.width / (float)render->resolution.height);
	while (y < render->resolution.height)
	{
		x = 0;
		y_prop = (float)y / (float)render->resolution.height;
		while (x < render->resolution.width)
		{
			x_prop = (float)x / (float)render->resolution.width;
			
			pixel.color = color_from_hex(get_pixel_img(img, (float)img->resolution.width * x_prop, (((float)img->resolution.height / aspect_ratio) * y_prop) + ((float)img->resolution.width / 2.0f) - ((float)img->resolution.height / (aspect_ratio * 2))));
			pixel.px = (float)x;
			pixel.py = (float)y;
			put_pixel(render, pixel);
			x++;
		}
		y++;
	}

}

int	render_normal_mode(t_cub *cub)
{
	mlx_put_image_to_window(cub->mlx, cub->main_window->mlx_win, cub->game_img->img, 0, 0);
}

int	render_bonus_mode(t_cub *cub)
{
	mlx_put_image_to_window(cub->mlx, cub->main_window->mlx_win, cub->game_img->img, 0, 0);
	mlx_put_image_to_window(cub->mlx, cub->main_window->mlx_win, cub->minimap_img->img, 0, 0);
}


int	game_mode(t_cub *cub)
{

	t_map_editor	minimap;

	if (cub->game_mode != GAME)
		clean_pixels(cub->game_img);

	cub->game_mode = GAME;
	minimap = map_editor();
	minimap.screen_zoom = 10;
	minimap.screen_center = point(cub->player->camera->pos.px * minimap.screen_zoom,
	cub->player->camera->pos.py * minimap.screen_zoom);//cub->player->camera->pos;
	
	clean_pixels(cub->minimap_img);

	if (BONUS)
	{
		draw_grid(minimap, cub->minimap_img, color_from_rgb(100, 100, 100));
		draw_map_walls(cub, minimap, cub->minimap_img);
		draw_player(cub, minimap, color_from_rgb(255, 255, 0), cub->minimap_img);
	}

	ray_casting(cub, minimap);

	if (BONUS)
		render_bonus_mode(cub);
	else
		render_normal_mode(cub);

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

	//draw_fov_intersection(cub, cub->map_editor, color_from_rgb(255, 0, 255), cub->editor_img);
	draw_map_walls(cub, cub->map_editor, cub->editor_img);

	ray_casting(cub, cub->map_editor);
	draw_player(cub, cub->map_editor, color_from_rgb(255, 255, 0), cub->editor_img);

	mlx_put_image_to_window(cub->mlx, cub->main_window->mlx_win, cub->editor_img->img, 0, 0);
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
		printf("player pos:%f,%f, angle:%f, fov:%f\n", 
		cub->player->camera->pos.px, cub->player->camera->pos.py, cub->player->camera->angle, cub->player->camera->fov);
		printf("last:%f\n", now);
		cub->frame = 0;
		last = now;
	}
	return (second);
}

int	frame(void *p_cub)
{
	t_cub			*cub;



	start_t	= clock();
	cub = (t_cub *)p_cub;
	cub->frame += 1;
	// change this!!!!!!!!
	if (BONUS)
		acurated_delta_time(cub);
	else
		cub->delta_time = 0.016f;

	

	/*
	if (cub->delta_time < 0.016f)
		cub->delta_time = 0.016f;
	if (cub->delta_time > 0.1f)
		cub->delta_time = 0.1f;
		*/

		if (cub->game_mode == GAME)
			game_mode(cub);
		else if (cub->game_mode == EDITOR)
			editor_mode(cub);
	//exit(-1);
	return (0);
}

int	mouse_press(int key, int x, int y, void *param)
{
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
	if (key == XK_0)
	{
		cub->player->camera->fov -= 1.0f;
		cub->player->camera->pos.pz = cub->player->camera->fov * cub->height_multiplier;//0.0576f;
		update_player_angle(cub->player, &cub->p_deltas, &cub->fov1_deltas, &cub->fov2_deltas, cub->player->camera->angle);

	}
	if (key == XK_9)
	{
		cub->player->camera->fov += 1.0f;
		cub->player->camera->pos.pz = cub->player->camera->fov * cub->height_multiplier;//0.0576f;
		update_player_angle(cub->player, &cub->p_deltas, &cub->fov1_deltas, &cub->fov2_deltas, cub->player->camera->angle);
	}
	if (key == XK_1)
	{
		cub->height_multiplier -= 0.0001f;
		printf("nearrrrr=====================!!%f\n", cub->near_plane);
		printf("nearrrrr=====================!!%f\n", cub->near_plane);
		printf("nearrrrr=====================!!%f\n", cub->near_plane);
		printf("height!!%f\n", cub->height_multiplier);
		printf("fov!!%f\n", cub->player->camera->fov);
		printf("nearrrrr=====================!!%f\n", cub->near_plane);
	}
	if (key == XK_2)
	{
		cub->height_multiplier += 0.001f;
		printf("nearrrrr=====================!!%f\n", cub->near_plane);
		printf("nearrrrr=====================!!%f\n", cub->near_plane);
		printf("nearrrrr=====================!!%f\n", cub->near_plane);
		printf("height!!%f\n", cub->height_multiplier);
		printf("fov!!%f\n", cub->player->camera->fov);
		printf("nearrrrr=====================!!%f\n", cub->near_plane);
	}
	if (key == XK_3)
	{
		cub->wall_height -= 0.001f;
		printf("-s-ssssssss=======================\n");
		printf("height!!%f\n", cub->wall_height);
	}
	if (key == XK_4)
	{
		cub->wall_height += 0.001f;
		printf("-s-ssssssss=======================\n");
		printf("height!!%f\n", cub->wall_height);
	}
	if (key == XK_5)
	{
		cub->max_dist -= 0.01f;
		printf("max_dist !!!!!!!!%f\n", cub->max_dist);
	}
	if (key == XK_6)
	{
		cub->max_dist += 0.01f;
		printf("max_dist !!!!!!!!%f\n", cub->max_dist);
	}
	if (key == XK_7)
	{
		cub->ambient_occlusion += 0.01f;
		printf("ao !!!!!!!!%f\n", cub->ambient_occlusion);
	}
	if (key == XK_8)
	{
		cub->ambient_occlusion -= 0.01f;
		printf("ao !!!!!!!!%f\n", cub->ambient_occlusion);
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
	int	screen_sizex;
	int	screen_sizey;

	screen_sizex = 0;
	screen_sizey = 0;
	if (BONUS)
	{
		mlx_get_screen_size(cub->mlx, &screen_sizex, &screen_sizey);
		if (screen_sizex > 1000)
			screen_sizex = 1000;
		else
			screen_sizex = 500;
		if (screen_sizey > 1000)
			screen_sizey = 1000;
		else
			screen_sizey = 500;
	}
	else
	{
		screen_sizex = 500;
		screen_sizey = 500;
	}

	cub->main_window = new_window(cub->mlx, resolution(screen_sizex, screen_sizey), "main_window");

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

	cub->game_img = init_img(cub->mlx, resolution(cub->main_window->res.height, cub->main_window->res.height));
	if (!cub->game_img)
	{
		printf("error pls fix\n");
		exit(-1);

	}

	cub->minimap_img = init_img(cub->mlx, resolution(
		cub->main_window->res.height * 0.1, cub->main_window->res.height * 0.1));

	// this part is parsing one map ======================================================

   char **map;

	map = malloc(sizeof(char *) * 80);
	ft_bzero(map, sizeof(char *) * 80);
    // Fill the map with '1's on the borders and '0's inside
    for (int i = 0; i < 79; i++) {
		map[i] = malloc(sizeof(char) * 80);
		ft_bzero(map[i], sizeof(char) * 80);
        for (int j = 0; j < 79; j++) {
            if (i == 0 || i == 78 || j == 0 || j == 78) {
                map[i][j] = '1'; // Borders
            } else {
                map[i][j] = '0'; // Inside
            }
        }
    }

    // Place the player 'P' in the middle (3,3)

    map[1][1] = '1';
    map[1][3] = '1';
    map[3][3] = '1';
    map[3][5] = '1';
    map[4][4] = '1';
    map[4][5] = '1';

    // Print the map
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            printf("%c ", map[i][j]);
        }
        printf("\n");
    }
	cub->map = map;

	cub->map_editor = map_editor();
	cub->game_mode = GAME;
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
	cub->test_tex = open_img(cub->mlx, "./srcs/side.xpm");

	mlx_put_image_to_window(cub->mlx, cub->main_window->mlx_win, cub->editor_img->img, 0, 0);

	// here goes the real angle and the real camera
	//77 73
	cub->player->camera->pos.px = 2.884814;
	cub->player->camera->pos.py = 6.926975;
	cub->player->camera->fov = 46;
	//cub->player->camera->fov = 120;
	cub->shadow_tex_size = 1000.0f;
	cub->wall_height = 2.0f;

	cub->height_multiplier = 0.027255f;

	cub->player->camera->pos.pz = cub->player->camera->fov * cub->height_multiplier;//0.0576f;
	//cub->player->camera->pos.pz = 17;//0.0576f;
	update_player_angle(cub->player, &cub->p_deltas, &cub->fov1_deltas, &cub->fov2_deltas, 270);

	cub->shadow_tex = malloc((size_t)(sizeof(float *) * cub->shadow_tex_size));
	if (!cub->shadow_tex)
	{
		//error!!!!!
		return (0);
	}
	int	shadow_i;

	shadow_i = 0;
	while (shadow_i < (int)cub->shadow_tex_size)
	{
		cub->shadow_tex[shadow_i] = malloc((size_t)(sizeof(float) * cub->shadow_tex_size));
		if (!cub->shadow_tex[shadow_i])
		{
			//error!!!!!
			return (0);
		}
		shadow_i++;
	}
	create_arrow_texture(cub->shadow_tex, cub->shadow_tex_size);

	cub->ao_tex = malloc((size_t)(sizeof(float *) * cub->shadow_tex_size));
	if (!cub->ao_tex)
	{
		//error!!!!!
		return (0);
	}

	shadow_i = 0;
	while (shadow_i < (int)cub->shadow_tex_size)
	{
		cub->ao_tex[shadow_i] = malloc((size_t)(sizeof(float) * cub->shadow_tex_size));
		if (!cub->ao_tex[shadow_i])
		{
			//error!!!!!
			return (0);
		}
		shadow_i++;
	}
	create_ao_texture(cub->ao_tex, cub->shadow_tex_size);

	cub->ambient_occlusion = 0.875;
	cub->near_plane = 0.0f;
	cub->max_dist = 1.684f;
	cub->floor = color(RED);
	cub->ceiling = color(GREEN);

	cub->game_mode = GAME;
	// here goes the real angle and the real camera

	mlx_loop_hook(cub->mlx, frame, cub);
	mlx_loop(cub->mlx);
}
