#pragma once
#include "tools.h"

struct point {
	double x;
	double y;
	int index;
};

struct triangle
{
	point* vertices[3];
	int index;
};

struct Voronoi
{
	point* vertices;
	int vert_num;
};

struct circle
{
	point center;
	double radius;
};

class Calculation
{
private:
	int points_num;
	point* points;
	triangle* triangles;
	int triangles_num;
	point A, B, C, D;
	Voronoi* voronois;
public:
	~Calculation();
	void get_data(std::string text);
	void init_triangles();
	void check_triangles(triangle& triangle1,triangle& triangle2);
	void swap_triangles(triangle& triangle1, triangle& triangle2);
	circle get_bound_circle(triangle& tri);
	void insert_point(point* p);
	double get_S(triangle& triangle);
	double get_S(double x1, double y1, double x2, double y2, double x3, double y3);
	bool point_in_triangle(point* point,triangle& triangle);
	void process_triangle(triangle& triangle,point* point1,point* point2);
	void construct_triangles();
	void filter_triangles();
	void output_triangles();
	void get_Voronois();
	void output_voronois();
};

