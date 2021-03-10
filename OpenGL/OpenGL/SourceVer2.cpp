#include <cctype>
#include <cerrno>
#include <cfloat>
#include <ciso646>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <algorithm>
using namespace std;
typedef long long ll;
const ll MAXN = 2050;
const double eps = 1e-8;
struct Point3 {
	//三维的点
	double x, y, z;
	Point3() {}
	Point3(double x, double y, double z) :x(x), y(y), z(z) {}///重载三维 点的计算
	Point3 operator + (Point3 B) { return Point3(x + B.x, y + B.y, z + B.z); }
	Point3 operator - (Point3 B) { return Point3(x - B.x, y - B.y, z - B.z); }
	Point3 operator * (double k) { return Point3(x*k, y*k, z*k); }
	Point3 operator / (double k) { return Point3(x / k, y / k, z / k); }
};
typedef Point3 Vector3;
double Dot(Vector3 A, Vector3 B) { return A.x*B.x + A.y*B.y + A.z*B.z; }///三维点积
Point3 Cross(Vector3 A, Vector3 B) { return Point3(A.y*B.z - A.z*B.y, A.z*B.x - A.x*B.z, A.x*B.y - A.y*B.x); }///三维叉积
double Len(Vector3 A) { return sqrt(Dot(A, A)); }
///向量的长度
double Area2(Point3 A, Point3 B, Point3 C) { return Len(Cross(B - A, C - A)); }
///四面体有向体积*6
double volume4(Point3 A, Point3 B, Point3 C, Point3 D) { return Dot(Cross(B - A, C - A), D - A); }
struct CH3D {
	struct face {
		ll a, b, c;
		///凸包的一个面上的3个点的编号
		bool ok;
		///该面是否在最终的凸包上
	};
	ll n;
	///初始顶点数
	Point3 P[MAXN];
	///初始顶点
	ll num;
	///凸包表面的三角形个数
	face F[8 * MAXN];
	///凸包表面的三角形
	ll g[MAXN][MAXN];/// g[x][y]表示点x到点y属于哪个面
	///点在面的同向
	double dblcmp(Point3 &p, face &f) {
		Point3 m = P[f.b] - P[f.a];
		Point3 n = P[f.c] - P[f.a];
		Point3 t = p - P[f.a];
		return Dot(Cross(m, n), t);
	}
	void deal(ll p, ll a, ll b) {
		ll f = g[a][b];
		///搜索与该边相邻的另一个平面
		face add;
		if (F[f].ok) {
			if (dblcmp(P[p], F[f]) > eps) {
				///如果如果从p点能看到该面f,则继续深度搜索f的三条边
				///以更新新的凸面（维护）
				F[f].ok = false;
				deal(p, F[f].b, F[f].a);
				deal(p, F[f].c, F[f].b);
				deal(p, F[f].a, F[f].c);
			}
			else {
				///如果如果从p点不能看到f面，则p点和a、b点组成一个三角形
				add.a = b;
				add.b = a;
				add.c = p;
				add.ok = true;
				g[p][b] = g[a][p] = g[b][a] = num;
				F[num++] = add;
			}
		}
	}
	void dfs(ll p, ll now) {
		///维护凸包，如果点p在凸包外则更新凸包
		F[now].ok = false;
		deal(p, F[now].b, F[now].a);
		deal(p, F[now].c, F[now].b);
		deal(p, F[now].a, F[now].c);
	}
	bool same(ll s, ll t) {
		///判断两个面是否为同一面
		Point3 &a = P[F[s].a];
		Point3 &b = P[F[s].b];
		Point3 &c = P[F[s].c];
		return fabs(volume4(a, b, c, P[F[t].a])) < eps &&
			fabs(volume4(a, b, c, P[F[t].b])) < eps &&
			fabs(volume4(a, b, c, P[F[t].c])) < eps;
	}
	void create() {
		///构建三维凸包
		ll i, j, tmp;
		face add;
		num = 0;
		if (n < 4)return;
		bool flag = true;
		for (i = 1; i < n; i++) {
			///使前两个点不共点
			if (Len(P[0] - P[i]) > eps) {
				swap(P[1], P[i]);
				flag = false;
				break;
			}
		}
		if (flag)return;
		flag = true;
		for (i = 2; i < n; i++) {
			///使前三个点不共线
			if (Len(Cross(P[0] - P[1], P[1] - P[i])) > eps) {
				swap(P[2], P[i]);
				flag = false;
				break;
			}
		}
		if (flag)return;
		flag = true;
		for (ll i = 3; i < n; i++) {
			///使前四个点不共面
			if (fabs(Dot(Cross(P[0] - P[1], P[1] - P[2]), P[0] - P[i])) > eps) {
				swap(P[3], P[i]);
				flag = false;
				break;
			}
		}
		if (flag)return;
		for (i = 0; i < 4; i++) {
			///构建初始四面体
			///四个点为：
			///p[0],p[1],p[2],p[3]
			add.a = (i + 1) % 4;
			add.b = (i + 2) % 4;
			add.c = (i + 3) % 4;
			add.ok = true;
			if (dblcmp(P[i], add) > 0)swap(add.b, add.c);
			///保证逆时针，即法向量朝外，这样新点才可看到
			g[add.a][add.b] = g[add.b][add.c] = g[add.c][add.a] = num;
			///逆向的有向边保存
			F[num++] = add;
		}
		for (i = 4; i < n; i++) {
			///构建&更新凸包
			for (j = 0; j < num; j++) {
				///判断点是否在当前三维凸包内，i表示当前点，j表示当前面
				if (F[j].ok && dblcmp(P[i], F[j]) > eps) {
					///对当前凸包面进行判断，看点能否看到这个面
					dfs(i, j);
					///点能看到当前面，更新凸包的面
					break;
				}
			}
		}
		tmp = num;
		for (i = num = 0; i < tmp; i++) {
			if (F[i].ok)F[num++] = F[i];
		}
	}
	///凸包的表面积
	double area() {
		double res = 0;
		for (ll i = 0; i < num; i++)res += Area2(P[F[i].a], P[F[i].b], P[F[i].c]);
		return res / 2.0;
	}
	///体积
	double volume() {
		double res = 0;
		Point3 tmp(0, 0, 0);
		for (ll i = 0; i < num; i++)res += volume4(tmp, P[F[i].a], P[F[i].b], P[F[i].c]);
		return fabs(res / 6.0);
	}
	///表面三角形个数
	ll triangle() {
		return num;
	}
	///表面多边形个数
	ll polygon() {
		ll i, j, res, flag;
		for (i = res = 0; i < num; i++) {
			flag = true;
			for (j = 0; j < i; j++) {
				if (same(i, j)) {
					flag = false;
					break;
				}
			}
			res += flag;
		}
		return res;
	}
};
CH3D hull;
int main() {
	scanf("%lld", &hull.n);
	for (ll i = 0; i < hull.n; i++)scanf("%lf %lf %lf", &hull.P[i].x, &hull.P[i].y, &hull.P[i].z);
	hull.create();
	printf("%.3lf\n", hull.area());
	///问啥输啥就完了
	return 0;
}