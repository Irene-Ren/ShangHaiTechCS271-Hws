#include<iostream>
#include<math.h>

using namespace std;
const int MAX_NUM = 50;
#define ERROR 0
/********************************************************/

/***A 3D Point Class***/
/********************************************************/

class Point3D
{
public:
	Point3D() {};
	Point3D(double x, double y, double z) :m_x(x), m_y(y), m_z(z) {};
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
	Point3D result(0, 0, 0);
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

/********************************************************/

/***Plane Class***/
/********************************************************/
class Plane
{
public:
	bool is_hull;

	int v1;
	int v2;
	int v3;
	Plane() :v1(0), v2(0), v3(0), is_hull(false) {};
	Plane(int x, int y, int z, bool h) :v1(x), v2(y), v3(z), is_hull(h) {};
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
	void ExtendConvexHull();

	int n; /*total number of real vertex*/
	Point3D points[MAX_NUM];/*Array contains all the points*/
	Plane triangleF[MAX_NUM]; /*All the triangle faces calculated before
								Including the faces that been deleted(is_hull = false)*/
	int orderedPlanes[MAX_NUM][MAX_NUM];
private:
	int triangleNum = 0;/*the total number of triangle faces on Convex hull*/

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
			triangleF[triangleNum] = newFace;

			triangleNum += 1;
		}
	}
}
double Mod(Point3D a)
{
	return sqrt(a.m_x * a.m_x + a.m_y * a.m_y + a.m_z * a.m_z);
};
double Dist(Point3D a, Point3D b)
{
	Point3D vector = a - b;
	return Mod(vector);
}
double Area(Point3D a, Point3D b, Point3D c)
{
	Point3D vector1 = b - a;
	Point3D vector2 = c - a;
	double area = Mod(vector1 * vector2) / 2.0;
	return area;
}
double Volume(Point3D a, Point3D b, Point3D c, Point3D d)
{
	Point3D vector1 = b - a;
	Point3D vector2 = c - a;
	Point3D vector3 = d - a;
	double volume = abs(((vector1 * vector2) ^ vector3) / 6.0);
	return volume;
}
bool Convex3D::CreateOriginTetrahedron()
{
	bool success = false;
	cout << "HELLO" << endl;
	for (int i = 1; i < n; i++)
	{
		if (Dist(points[0], points[i]) > ERROR)
		{
			swap(points[1], points[i]);
			success = true;
			cout << "Found the point!!!" << endl;
			break;
		}
	}
	if (!success)
	{
		cout << "Same vertex" << endl;
		return success;
	}
	success = false;
	for (int i = 2; i < n; i++)
	{
		if (Area(points[0], points[1], points[i]) > ERROR)
		{
			swap(points[2], points[i]);
			success = true;
			cout << "Found the Edge!!!" << endl;
			break;
		}
	}
	if (!success)
	{
		cout << "Same edge" << endl;
		return success;
	}
	success = false;
	for (int i = 3; i < n; i++)
	{
		if (Volume(points[0], points[1], points[2], points[i]) > ERROR)
		{
			swap(points[3], points[i]);
			success = true;
			cout << "Found the face!!!" << endl;
			break;
		}
	}
	if (!success)
	{
		cout << "Same face" << endl;
		return success;
	}

	Plane p1(0, 1, 2, true);
	Plane p2(0, 1, 3, true);
	Plane p3(0, 2, 3, true);
	Plane p4(1, 2, 3, true);

	if (!IsVisibleToP(points[3], p1))
	{
		swap(p1.v2, p1.v3);
	}
	orderedPlanes[p1.v1][p1.v2] = triangleNum;
	orderedPlanes[p1.v2][p1.v3] = triangleNum;
	orderedPlanes[p1.v3][p1.v1] = triangleNum;
	triangleF[triangleNum] = p1;
	triangleNum += 1;

	if (!IsVisibleToP(points[2], p2))
	{
		swap(p2.v2, p2.v3);
	}
	orderedPlanes[p2.v1][p2.v2] = triangleNum;
	orderedPlanes[p2.v2][p2.v3] = triangleNum;
	orderedPlanes[p2.v3][p2.v1] = triangleNum;
	triangleF[triangleNum] = p2;
	triangleNum += 1;

	if (!IsVisibleToP(points[1], p3))
	{
		swap(p3.v2, p3.v3);
	}
	orderedPlanes[p3.v1][p3.v2] = triangleNum;
	orderedPlanes[p3.v2][p3.v3] = triangleNum;
	orderedPlanes[p3.v3][p3.v1] = triangleNum;
	triangleF[triangleNum] = p3;
	triangleNum += 1;

	if (!IsVisibleToP(points[0], p4))
	{
		swap(p1.v2, p1.v3);
	}
	orderedPlanes[p1.v1][p1.v2] = triangleNum;
	orderedPlanes[p1.v2][p1.v3] = triangleNum;
	orderedPlanes[p1.v3][p1.v1] = triangleNum;
	triangleF[triangleNum] = p4;
	triangleNum += 1;

	return success;
}
void Convex3D::ExtendConvexHull()
{
	for (int i = 4; i < n; i++)
	{
		for (int j = 0; j < triangleNum; j++)
		{
			if (IsVisibleToP(points[i], triangleF[j]))
			{

			}
		}
	}
}

int main()
{
	int n;
	Convex3D convexHull;
	cin >> n;
	convexHull.n = n;
	for (int i = 0; i < n; i++)
	{
		cin >> convexHull.points[i].m_x >> convexHull.points[i].m_y >> convexHull.points[i].m_z;
	}
	if (convexHull.CreateOriginTetrahedron())
	{
		for (int i = 0; i < 4; i++)
		{
			cout << convexHull.triangleF[i].v1 << " " << convexHull.triangleF[i].v2 << " " << convexHull.triangleF[i].v3 << endl;
		}

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				cout << convexHull.orderedPlanes[i][j] << " ";
			}
			cout << endl;
		}
	}
	else
	{
		cout << "ERROE";
	}
	system("pause");
	return 0;
}