#include<iostream>
#include<math.h>

using namespace std;
const int MAX_NUM = 2021;
#define ERROR 0
/********************************************************/

/***A 3D Point Class***/
/********************************************************/
double Mod(Point3D a)
{
	return sqrt(a.m_x * a.m_x + a.m_y * a.m_y + a.m_z * a.m_z);
}
double Dist(Point3D a, Point3D b)
{
	Point3D vector = a - b;
	Mod(vector);
}
double Area(Point3D a, Point3D b, Point3D c)
{
	Point3D vector1 = b - a;
	Point3D vector2 = c - a;
	double area = Mod(vector1 * vector2) / 2.0;
}
double Volume(Point3D a, Point3D b, Point3D c, Point3D d)
{
	Point3D vector1 = b - a;
	Point3D vector2 = c - a;
	Point3D vector3 = d - a;
	double volume = abs(((vector1 * vector2) ^ vector3) / 6.0);
	return volume;
}
class Point3D
{
public:
	Point3D() {};
	Point3D(double x, double y, double z):m_x(x),m_y(y),m_z(z) {};
	Point3D operator= (const Point3D & other);
	Point3D operator+ (const Point3D &other) const;
	Point3D operator- (const Point3D &other) const;
	//Cross product
	Point3D operator* (const Point3D &other) const;
	//Dot product
	double operator^ (const Point3D &other) const;
	~Point3D() {};

	double m_x;
	double m_y;
	double m_z;
};
Point3D Point3D::operator= (const Point3D &other)
{
	(*this).m_x = other.m_x;
	(*this).m_y = other.m_y;
	(*this).m_z = other.m_z;

	return *this;
}
Point3D Point3D::operator+ (const Point3D &other) const
{
	Point3D result(0,0,0);
	result.m_x = (*this).m_x + other.m_x;
	result.m_y = (*this).m_y + other.m_y;
	result.m_z = (*this).m_z + other.m_z;

	return result;
}
Point3D Point3D::operator- (const Point3D &other) const
{
	Point3D result(0, 0, 0);
	result.m_x = (*this).m_x - other.m_x;
	result.m_y = (*this).m_y - other.m_y;
	result.m_z = (*this).m_z - other.m_z;

	return result;
}
Point3D Point3D::operator* (const Point3D &other) const
{
	Point3D result(0, 0, 0);
	result.m_x = (*this).m_y*other.m_z - (*this).m_z * other.m_y;
	result.m_y = (*this).m_z * other.m_x - (*this).m_x*other.m_z;
	result.m_z = (*this).m_x*other.m_y - (*this).m_y * other.m_x;

	return result;
}
double Point3D::operator^ (const Point3D &other) const
{
	double result = 0;
	result = (*this).m_x*other.m_x + 
		(*this).m_y*other.m_y + (*this).m_z * other.m_z;
	
	return result;
} 


class Plane
{
public:
	bool is_hull;

	int v1;
	int v2;
	int v3;
	Plane() :v1(0), v2(0), v3(0),is_hull(false) {};
	Plane(int x, int y, int z, bool h) :v1(x), v2(y), v3(z) ,is_hull(h) {};
};
/********************************************************/

/***3D Convex Hull Class***/
/********************************************************/
class Convex3D
{
public:
	Convex3D() {};
	~Convex3D() {};
	bool IsVisibleToP(Point3D& p, Plane& f);
	void AddPointP(int p, int a, int b);
	bool CreateOriginTetrahedron();
private:
	int n; /*total number of real vertex*/
	Point3D points[MAX_NUM];/*Array contains all the points*/
	int triangleNum = 0;/*the total number of triangle faces on Convex hull*/
	Plane triangleF[MAX_NUM]; /*All the triangle faces calculated before
								Including the faces that been deleted(is_hull = false)*/
	int orderedPlanes[MAX_NUM][MAX_NUM];


};

//We already made sure plane f is counterclock-reverse
bool Convex3D::IsVisibleToP(Point3D& p, Plane& f)
{
	Point3D vector1 = points[f.v2] - points[f.v1];
	Point3D vector2 = points[f.v3] - points[f.v1];
	Point3D vector3 = p - points[f.v1];
	return ((vector1*vector2) ^ vector3) > ERROR;
}
void Convex3D::AddPointP(int p, int a, int b)
{
	int face = orderedPlanes[a][b]; //face contains vector a->b
	if (triangleF[face].is_hull)
	{
		//if old hull plane is visible to point P, then need to delete the plane
		//and see its neighbors is also visible to P
		if (IsVisibleToP(points[p], triangleF[face]))
		{
			triangleF[triangleNum].is_hull = false;

			AddPointP(p, triangleF[face].v2, triangleF[face].v1);
			AddPointP(p, triangleF[face].v3, triangleF[face].v2);
			AddPointP(p, triangleF[face].v1, triangleF[face].v3);
		}
		//old hull plane is not visible to point P, we need P and a->b
		//to form new hull plane
		else
		{
			Plane newFace(b, a, p, true);
			orderedPlanes[p][b] = triangleNum;
			orderedPlanes[b][a] = triangleNum;
			orderedPlanes[a][p] = triangleNum;
			triangleNum += 1;
			triangleF[triangleNum] = newFace;
		}
	}
}
bool Convex3D::CreateOriginTetrahedron()
{
	for (int i = 1; i < n; i++)
	{
		Point3D p;
		p.Dist(points[0], points[i]);
	}
}