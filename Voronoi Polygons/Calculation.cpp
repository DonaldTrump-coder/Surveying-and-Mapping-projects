#include "pch.h"
#include "Calculation.h"


Calculation::~Calculation()
{
	delete[] points;
}

void Calculation::get_data(std::string text)
{
	std::string* points_text = textsplit(text,'\n');
	points_num = get_split_num(text, '\n');
	points = new point[points_num];
	for (int i = 0; i < points_num; i++)
	{
		std::string* point_text=textsplit(points_text[i], ' ');
		points[i].x = std::stod(point_text[0]);
		points[i].y = std::stod(point_text[1]);
		points[i].index = i;
		delete[] point_text;
	}
	delete[] points_text;
}

void Calculation::init_triangles()
{
	double xmin, xmax, ymin, ymax;
	xmin = xmax = points[0].x;
	ymin = ymax = points[0].y;
	for (int i = 0; i < points_num; i++)
	{
		if (points[i].x < xmin)
			xmin = points[i].x;
		if (points[i].x > xmax)
			xmax = points[i].x;
		if (points[i].y < ymin)
			ymin = points[i].y;
		if (points[i].y > ymax)
			ymax = points[i].y;
	}
	double a = (xmax - xmin) * 10;
	double b = (ymax - ymin) * 10;
	xmin = xmin - a;
	xmax = xmax + a;
	ymin = ymin - b;
	ymax = ymax + b;
	triangles = new triangle[2 + 2 * points_num];

	A.x = xmin; A.y = ymin; A.index = -1;
	B.x = xmin; B.y = ymax; B.index = -2;
	C.x = xmax; C.y = ymax; C.index = -3;
	D.x = xmax; D.y = ymin; D.index = -4;
	triangles[0].vertices[0] = &A;
	triangles[0].vertices[1] = &B;
	triangles[0].vertices[2] = &C;
	triangles[0].index = 0;

	triangles[1].vertices[0] = &A;
	triangles[1].vertices[1] = &D;
	triangles[1].vertices[2] = &C;
	triangles[1].index = 1;

	triangles_num = 2;
}

void Calculation::swap_triangles(triangle& triangle1, triangle& triangle2)
{
	point* point1=NULL;
	point* point2=NULL;//现有对角线的两个顶点
	point* rest1=NULL;
	point* rest2=NULL;//未连接的两个顶点
	bool first = 1;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (triangle1.vertices[i]->index == triangle2.vertices[j]->index)
			{
				if (first == 1)
				{
					point1 = triangle1.vertices[i];
					first = 0;
					break;
				}
				else
				{
					point2 = triangle1.vertices[i];
					break;
				}
			}
		}
	}
	first = 1;
	for (int i = 0; i < 3; i++)
	{
		if (triangle1.vertices[i]->index != point1->index && triangle1.vertices[i]->index != point2->index)
		{
			if (first == 1)
			{
				rest1 = triangle1.vertices[i];
				first = 0;
			}
			else
			{
				rest2 = triangle1.vertices[i];
			}
		}
		if (triangle2.vertices[i]->index != point1->index && triangle2.vertices[i]->index != point2->index)
		{
			if (first == 1)
			{
				rest1 = triangle2.vertices[i];
				first = 0;
			}
			else
			{
				rest2 = triangle2.vertices[i];
			}
		}
	}
	triangle1.vertices[0] = point1;
	triangle1.vertices[1] = rest1;
	triangle1.vertices[2] = rest2;
	triangle2.vertices[0] = point2;
	triangle2.vertices[1] = rest1;
	triangle2.vertices[2] = rest2;
}

circle Calculation::get_bound_circle(triangle& tri)
{
	circle result;
	double A, B, C;
	A = tri.vertices[0]->x * (tri.vertices[1]->y - tri.vertices[2]->y) - tri.vertices[0]->y * (tri.vertices[1]->x - tri.vertices[2]->x) + tri.vertices[1]->x * tri.vertices[2]->y - tri.vertices[2]->x * tri.vertices[1]->y;
	B = (tri.vertices[0]->x * tri.vertices[0]->x + tri.vertices[0]->y * tri.vertices[0]->y) * (tri.vertices[2]->y - tri.vertices[1]->y) + (tri.vertices[1]->x * tri.vertices[1]->x + tri.vertices[1]->y * tri.vertices[1]->y) * (tri.vertices[0]->y - tri.vertices[2]->y) + (tri.vertices[2]->x * tri.vertices[2]->x + tri.vertices[2]->y * tri.vertices[2]->y) * (tri.vertices[1]->y - tri.vertices[0]->y);
	C = (tri.vertices[0]->x * tri.vertices[0]->x + tri.vertices[0]->y * tri.vertices[0]->y) * (tri.vertices[1]->x - tri.vertices[2]->x) + (tri.vertices[1]->x * tri.vertices[1]->x + tri.vertices[1]->y * tri.vertices[1]->y) * (tri.vertices[2]->x - tri.vertices[0]->x) + (tri.vertices[2]->x * tri.vertices[2]->x + tri.vertices[2]->y * tri.vertices[2]->y) * (tri.vertices[0]->x - tri.vertices[1]->x);
	result.center.x = -B / (2 * A);
	result.center.y = -C / (2 * A);
	result.radius = get_distance(result.center.x, result.center.y, tri.vertices[0]->x, tri.vertices[0]->y);
	return result;
}

void Calculation::check_triangles(triangle& triangle1, triangle& triangle2)
{
	point* rest1 = NULL;//三角形1中无对角线连接的顶点
	point* rest2 = NULL;//三角形2中无对角线连接的顶点

	for (int i = 0; i < 3; i++)
	{
		bool rest = 0;
		for (int j = 0; j < 3; j++)
		{
			if (triangle1.vertices[i]->index == triangle2.vertices[j]->index)
			{
				rest = 1;
			}
		}
		if (rest == 0)
		{
			rest1 = triangle1.vertices[i];
			break;
		}
	}

	for (int i = 0; i < 3; i++)
	{
		bool rest = 0;
		for (int j = 0; j < 3; j++)
		{
			if (triangle2.vertices[i]->index == triangle1.vertices[j]->index)
			{
				rest = 1;
			}
		}
		if (rest == 0)
		{
			rest2 = triangle2.vertices[i];
			break;
		}
	}

	circle bound1, bound2;
	bound1 = get_bound_circle(triangle1);
	bound2 = get_bound_circle(triangle2);
	if ((get_distance(rest1->x, rest1->y, bound2.center.x, bound2.center.y) < bound2.radius) || (get_distance(rest2->x, rest2->y, bound1.center.x, bound1.center.y) < bound1.radius))
	{
		swap_triangles(triangle1, triangle2);
	}
}

double Calculation::get_S(triangle& triangle)
{
	double x1 = triangle.vertices[0]->x;
	double y1 = triangle.vertices[0]->y;
	double x2 = triangle.vertices[1]->x;
	double y2 = triangle.vertices[1]->y;
	double x3 = triangle.vertices[2]->x;
	double y3 = triangle.vertices[2]->y;
	return abs(x1 * (y2 - y3) - y1 * (x2 - x3) + x2 * y3 - x3 * y2);
}

double Calculation::get_S(double x1, double y1, double x2, double y2, double x3, double y3)
{
	return abs(x1 * (y2 - y3) - y1 * (x2 - x3) + x2 * y3 - x3 * y2);
}

bool Calculation::point_in_triangle(point* point, triangle& triangle)
{
	double S0 = get_S(triangle);
	double S1, S2, S3;
	S1 = get_S(point->x, point->y, triangle.vertices[0]->x, triangle.vertices[0]->y, triangle.vertices[1]->x, triangle.vertices[1]->y);
	S2 = get_S(point->x, point->y, triangle.vertices[1]->x, triangle.vertices[1]->y, triangle.vertices[2]->x, triangle.vertices[2]->y);
	S3 = get_S(point->x, point->y, triangle.vertices[2]->x, triangle.vertices[2]->y, triangle.vertices[0]->x, triangle.vertices[0]->y);
	if (S0 - S1 - S2 - S3 > -0.01 && S0 - S1 - S2 - S3 < 0.01)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void Calculation::insert_point(point* p)
{
	int tri_index;
	for (int i = 0; i < triangles_num; i++)
	{
		if (point_in_triangle(p, triangles[i]))
		{
			tri_index = i;
			break;
		}
	}
	point* A;
	point* B;
	point* C;
	A = triangles[tri_index].vertices[0];
	B = triangles[tri_index].vertices[1];
	C = triangles[tri_index].vertices[2];
	triangles[tri_index].vertices[0] = p;

	triangles[triangles_num].vertices[0] = p;
	triangles[triangles_num].vertices[1] = A;
	triangles[triangles_num].vertices[2] = B;
	triangles[triangles_num].index = triangles_num;

	triangles[triangles_num + 1].vertices[0] = p;
	triangles[triangles_num + 1].vertices[1] = A;
	triangles[triangles_num + 1].vertices[2] = C;
	triangles[triangles_num + 1].index = triangles_num + 1;

	process_triangle(triangles[tri_index], triangles[tri_index].vertices[1], triangles[tri_index].vertices[2]);
	process_triangle(triangles[triangles_num], triangles[triangles_num].vertices[1], triangles[triangles_num].vertices[2]);
	process_triangle(triangles[triangles_num + 1], triangles[triangles_num + 1].vertices[1], triangles[triangles_num + 1].vertices[2]);

	triangles_num += 2;
}

void Calculation::process_triangle(triangle& triangle, point* point1, point* point2)
{
	for (int i = 0; i < triangles_num; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (triangles[i].vertices[j]->index == point1->index || triangles[i].vertices[j]->index == point2->index)
			{
				for (int k = j + 1; k < 3; k++)
				{
					if (triangles[i].vertices[k]->index == point1->index || triangles[i].vertices[k]->index == point2->index)
					{
						if (triangles[i].index != triangle.index)
						{
							check_triangles(triangles[i], triangle);
						}
					}
				}
			}
		}
	}
}

void Calculation::construct_triangles()
{
	init_triangles();
	for (int i = 0; i < points_num; i++)
	{
		insert_point(&points[i]);
	}
}

void Calculation::filter_triangles()
{
	int new_triangle_num = triangles_num;
	for (int i = 0; i < triangles_num; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (triangles[i].vertices[j]->index < 0)
			{
				new_triangle_num--;
				break;
			}
		}
	}
	triangle* temp = new triangle[new_triangle_num];
	int index = 0;
	for (int i = 0; i < triangles_num; i++)
	{
		bool dele = 0;
		for (int j = 0; j < 3; j++)
		{
			if (triangles[i].vertices[j]->index < 0)
			{
				dele = 1;
				break;
			}
		}
		if (dele == 1)
		{
			continue;
		}
		else
		{
			temp[index].vertices[0] = triangles[i].vertices[0];
			temp[index].vertices[1] = triangles[i].vertices[1];
			temp[index].vertices[2] = triangles[i].vertices[2];
			temp[index].index = index;
			index++;
		}
	}
	delete[] triangles;
	triangles = temp;
	triangles_num = new_triangle_num;

	for (int i = 0; i < triangles_num; i++)
	{
		process_triangle(triangles[i], triangles[i].vertices[0], triangles[i].vertices[1]);
		process_triangle(triangles[i], triangles[i].vertices[1], triangles[i].vertices[2]);
		process_triangle(triangles[i], triangles[i].vertices[0], triangles[i].vertices[2]);
	}
	for (int i = 0; i < triangles_num; i++)
	{
		process_triangle(triangles[i], triangles[i].vertices[0], triangles[i].vertices[1]);
		process_triangle(triangles[i], triangles[i].vertices[1], triangles[i].vertices[2]);
		process_triangle(triangles[i], triangles[i].vertices[0], triangles[i].vertices[2]);
	}
}

void Calculation::output_triangles()
{
	std::string result = "";
	for (int i = 0; i < triangles_num; i++)
	{
		result += std::to_string(triangles[i].vertices[0]->x);
		result += ",";
		result += std::to_string(triangles[i].vertices[0]->y);
		result += ",";
		result += std::to_string(triangles[i].vertices[1]->x);
		result += ",";
		result += std::to_string(triangles[i].vertices[1]->y);
		result += ",";
		result += std::to_string(triangles[i].vertices[2]->x);
		result += ",";
		result += std::to_string(triangles[i].vertices[2]->y);
		result += ",";
		result += "\n";
	}
	save_to_file(result);
}

void Calculation::get_Voronois()
{
	voronois = new Voronoi[points_num];
	for (int i = 0; i < points_num; i++)
	{
		int vert_num = 0;
		for (int j = 0; j < triangles_num; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				if (triangles[j].vertices[k]->index == points[i].index)
				{
					vert_num++;
					break;
				}
			}
		}
		voronois[i].vertices = new point [vert_num];
		int index = 0;
		for (int j = 0; j < triangles_num; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				if (triangles[j].vertices[k]->index == points[i].index)
				{
					circle temp=get_bound_circle(triangles[j]);
					voronois[i].vertices[index].x = temp.center.x;
					voronois[i].vertices[index].y = temp.center.y;
					index++;
				}
			}
		}
		voronois[i].vert_num = vert_num;

		for (int j = 0; j < vert_num; j++)
		{
			double min = atan2(points[i].y - voronois[i].vertices[j].y, points[i].x - voronois[i].vertices[j].x);
			for (int k = j ; k < vert_num; k++)
			{
				double angle= atan2(points[i].y - voronois[i].vertices[k].y, points[i].x - voronois[i].vertices[k].x);
				if (angle < min)
				{
					min = angle;
					double xtemp = voronois[i].vertices[j].x;
					double ytemp = voronois[i].vertices[j].y;
					voronois[i].vertices[j].x = voronois[i].vertices[k].x;
					voronois[i].vertices[j].y = voronois[i].vertices[k].y;
					voronois[i].vertices[k].x = xtemp;
					voronois[i].vertices[k].y = ytemp;
				}
			}
		}
	}
}

void Calculation::output_voronois()
{
	std::string result = "";
	for (int i = 0; i < points_num; i++)
	{
		for (int j = 0; j < voronois[i].vert_num; j++)
		{
			result += std::to_string(voronois[i].vertices[j].x);
			result += ",";
			result += std::to_string(voronois[i].vertices[j].y);
			result += ",";
		}
		result += "\n";
	}
	save_to_file(result);
}