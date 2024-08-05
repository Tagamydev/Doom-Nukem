
#ifndef POINT_H
# define POINT_H
# pragma once
# include "color.h"

typedef struct s_point
{
	float	px;
	float	py;
	t_color	color;
}		t_point;

t_point	point(float px, float py);

#endif