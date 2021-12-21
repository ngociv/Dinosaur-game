#ifndef OWN_STRUCT_H_
#define OWN_STRUCT_H_


struct point_2d
{
	int x;
	int y;
};

struct rectangle
{
	point_2d A;
	point_2d B;
	point_2d C;
	point_2d D;
};

struct polygon
{
	point_2d leftHead;
	point_2d rightHead;
	point_2d leftFoot;
	point_2d rightFoot;
	point_2d tail;
};

struct colorRGB
{
	int r;
	int g;
	int b;
};


#endif // !OWN_STRUCT_H_

