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
	//��ά�ĵ�
	double x, y, z;
	Point3() {}
	Point3(double x, double y, double z) :x(x), y(y), z(z) {}///������ά ��ļ���
	Point3 operator + (Point3 B) { return Point3(x + B.x, y + B.y, z + B.z); }
	Point3 operator - (Point3 B) { return Point3(x - B.x, y - B.y, z - B.z); }
	Point3 operator * (double k) { return Point3(x*k, y*k, z*k); }
	Point3 operator / (double k) { return Point3(x / k, y / k, z / k); }
};
typedef Point3 Vector3;
double Dot(Vector3 A, Vector3 B) { return A.x*B.x + A.y*B.y + A.z*B.z; }///��ά���
Point3 Cross(Vector3 A, Vector3 B) { return Point3(A.y*B.z - A.z*B.y, A.z*B.x - A.x*B.z, A.x*B.y - A.y*B.x); }///��ά���
double Len(Vector3 A) { return sqrt(Dot(A, A)); }
///�����ĳ���
double Area2(Point3 A, Point3 B, Point3 C) { return Len(Cross(B - A, C - A)); }
///�������������*6
double volume4(Point3 A, Point3 B, Point3 C, Point3 D) { return Dot(Cross(B - A, C - A), D - A); }
struct CH3D {
	struct face {
		ll a, b, c;
		///͹����һ�����ϵ�3����ı��
		bool ok;
		///�����Ƿ������յ�͹����
	};
	ll n;
	///��ʼ������
	Point3 P[MAXN];
	///��ʼ����
	ll num;
	///͹������������θ���
	face F[8 * MAXN];
	///͹�������������
	ll g[MAXN][MAXN];/// g[x][y]��ʾ��x����y�����ĸ���
	///�������ͬ��
	double dblcmp(Point3 &p, face &f) {
		Point3 m = P[f.b] - P[f.a];
		Point3 n = P[f.c] - P[f.a];
		Point3 t = p - P[f.a];
		return Dot(Cross(m, n), t);
	}
	void deal(ll p, ll a, ll b) {
		ll f = g[a][b];
		///������ñ����ڵ���һ��ƽ��
		face add;
		if (F[f].ok) {
			if (dblcmp(P[p], F[f]) > eps) {
				///��������p���ܿ�������f,������������f��������
				///�Ը����µ�͹�棨ά����
				F[f].ok = false;
				deal(p, F[f].b, F[f].a);
				deal(p, F[f].c, F[f].b);
				deal(p, F[f].a, F[f].c);
			}
			else {
				///��������p�㲻�ܿ���f�棬��p���a��b�����һ��������
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
		///ά��͹���������p��͹���������͹��
		F[now].ok = false;
		deal(p, F[now].b, F[now].a);
		deal(p, F[now].c, F[now].b);
		deal(p, F[now].a, F[now].c);
	}
	bool same(ll s, ll t) {
		///�ж��������Ƿ�Ϊͬһ��
		Point3 &a = P[F[s].a];
		Point3 &b = P[F[s].b];
		Point3 &c = P[F[s].c];
		return fabs(volume4(a, b, c, P[F[t].a])) < eps &&
			fabs(volume4(a, b, c, P[F[t].b])) < eps &&
			fabs(volume4(a, b, c, P[F[t].c])) < eps;
	}
	void create() {
		///������ά͹��
		ll i, j, tmp;
		face add;
		num = 0;
		if (n < 4)return;
		bool flag = true;
		for (i = 1; i < n; i++) {
			///ʹǰ�����㲻����
			if (Len(P[0] - P[i]) > eps) {
				swap(P[1], P[i]);
				flag = false;
				break;
			}
		}
		if (flag)return;
		flag = true;
		for (i = 2; i < n; i++) {
			///ʹǰ�����㲻����
			if (Len(Cross(P[0] - P[1], P[1] - P[i])) > eps) {
				swap(P[2], P[i]);
				flag = false;
				break;
			}
		}
		if (flag)return;
		flag = true;
		for (ll i = 3; i < n; i++) {
			///ʹǰ�ĸ��㲻����
			if (fabs(Dot(Cross(P[0] - P[1], P[1] - P[2]), P[0] - P[i])) > eps) {
				swap(P[3], P[i]);
				flag = false;
				break;
			}
		}
		if (flag)return;
		for (i = 0; i < 4; i++) {
			///������ʼ������
			///�ĸ���Ϊ��
			///p[0],p[1],p[2],p[3]
			add.a = (i + 1) % 4;
			add.b = (i + 2) % 4;
			add.c = (i + 3) % 4;
			add.ok = true;
			if (dblcmp(P[i], add) > 0)swap(add.b, add.c);
			///��֤��ʱ�룬�����������⣬�����µ�ſɿ���
			g[add.a][add.b] = g[add.b][add.c] = g[add.c][add.a] = num;
			///���������߱���
			F[num++] = add;
		}
		for (i = 4; i < n; i++) {
			///����&����͹��
			for (j = 0; j < num; j++) {
				///�жϵ��Ƿ��ڵ�ǰ��ά͹���ڣ�i��ʾ��ǰ�㣬j��ʾ��ǰ��
				if (F[j].ok && dblcmp(P[i], F[j]) > eps) {
					///�Ե�ǰ͹��������жϣ������ܷ񿴵������
					dfs(i, j);
					///���ܿ�����ǰ�棬����͹������
					break;
				}
			}
		}
		tmp = num;
		for (i = num = 0; i < tmp; i++) {
			if (F[i].ok)F[num++] = F[i];
		}
	}
	///͹���ı����
	double area() {
		double res = 0;
		for (ll i = 0; i < num; i++)res += Area2(P[F[i].a], P[F[i].b], P[F[i].c]);
		return res / 2.0;
	}
	///���
	double volume() {
		double res = 0;
		Point3 tmp(0, 0, 0);
		for (ll i = 0; i < num; i++)res += volume4(tmp, P[F[i].a], P[F[i].b], P[F[i].c]);
		return fabs(res / 6.0);
	}
	///���������θ���
	ll triangle() {
		return num;
	}
	///�������θ���
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
	///��ɶ��ɶ������
	return 0;
}