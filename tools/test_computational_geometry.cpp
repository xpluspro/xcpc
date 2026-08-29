#include <bits/stdc++.h>
using namespace std;
using LL = long long;
const int MAXN = 200005;

#include "../src/sections/ComputationalGeometry/assets/Geometry/8.1-declarations.cpp"
#include "../src/sections/ComputationalGeometry/assets/Geometry/8.2-point-vector.cpp"
#include "../src/sections/ComputationalGeometry/assets/Geometry/8.3-line.cpp"
#include "../src/sections/ComputationalGeometry/assets/Geometry/8.4-circle.cpp"
#include "../src/sections/ComputationalGeometry/assets/Geometry/凸包.cpp"
#include "../src/sections/ComputationalGeometry/assets/Geometry/闵可夫斯基和.cpp"
#include "../src/sections/ComputationalGeometry/assets/Geometry/旋转卡壳.cpp"
#include "../src/sections/ComputationalGeometry/assets/Geometry/多边形基础.cpp"
#include "../src/sections/ComputationalGeometry/assets/Geometry/半平面交.cpp"
#include "../src/sections/ComputationalGeometry/assets/Geometry/integral_hpi.cpp"
#include "../src/sections/ComputationalGeometry/assets/Geometry/convex_findmax.cpp"
#include "../src/sections/ComputationalGeometry/assets/Geometry/AirportConstruction.cpp"
#include "../src/sections/ComputationalGeometry/assets/Geometry/Triangulation.cpp"
#include "../src/sections/ComputationalGeometry/assets/Geometry/dynamic_convex_hull.cpp"
#include "../src/sections/ComputationalGeometry/assets/Geometry/DelaunayTriangulation.cpp"
#include "../src/sections/ComputationalGeometry/assets/Geometry/最近点对.cpp"
#include "../src/sections/ComputationalGeometry/assets/Geometry/三角形.cpp"
#include "../src/sections/ComputationalGeometry/assets/Geometry/圆并.cpp"
#include "../src/sections/ComputationalGeometry/assets/Geometry/多边形和圆的交.cpp"
#include "../src/sections/ComputationalGeometry/assets/yzh/circle_inversion.cpp"
#include "../src/sections/ComputationalGeometry/assets/Geometry/三维几何.cpp"
#include "../src/sections/ComputationalGeometry/assets/Geometry/三维凸包.cpp"
#include "../src/sections/ComputationalGeometry/assets/Geometry/最小覆盖球.cpp"

bool close(LD a, LD b, LD e = 1e-8L) { return fabsl(a - b) <= e; }

int main() {
	mt19937_64 gen(20260829);
	uniform_int_distribution<int> coord(-20, 20), count(2, 30);
	for (int tc = 0; tc < 500; ++tc) {
		vp p(count(gen));
		for (P &x : p) x = P(coord(gen), coord(gen));
		LD brute = INF;
		for (int i = 0; i < (int)p.size(); ++i)
			for (int j = i + 1; j < (int)p.size(); ++j)
				brute = min(brute, (p[i] - p[j]).len());
		assert(close(closest_pair(p), brute));

		vp h = convex_hull(p);
		LD far = 0;
		for (cp a : h) for (cp b : h) far = max(far, (a - b).len());
		assert(close(convex_diameter(h), far));
		if (h.size() >= 3) {
			LD brute_width = INF, brute_rectangle = INF;
			for (int i = 0; i < (int)h.size(); ++i) {
				P u = (h[(i + 1) % h.size()] - h[i]).unit(), v = u.rot90();
				LD min_u = INF, max_u = -INF, min_v = INF, max_v = -INF;
				for (cp x : h) {
					min_u = min(min_u, x*u), max_u = max(max_u, x*u);
					min_v = min(min_v, x*v), max_v = max(max_v, x*v);
				}
				brute_width = min(brute_width, max_v-min_v);
				brute_rectangle = min(brute_rectangle,
					(max_u-min_u)*(max_v-min_v));
			}
			assert(close(convex_min_width(h), brute_width));
			assert(close(min_area_rectangle(h).area, brute_rectangle));
			ConvexQuery q(h);
			for (cp x : p) assert(q.inside(x));
			for (int z = 0; z < 20; ++z) {
				P x(coord(gen), coord(gen));
				LD d = INF;
				for (int i = 0; i < (int)h.size(); ++i)
					d = min(d, point_to_segment(x, {h[i], h[(i + 1) % h.size()]}));
				if (q.inside(x)) d = 0;
				assert(close(q.distance(x), d));
			}
		}
	}

	vp rect{{0,0},{4,0},{4,2},{0,2}};
	assert(close(convex_min_width(rect), 2));
	assert(close(min_area_rectangle(rect).area, 8));
	assert(close(polygon_area(rect), 8));
	assert(close(polygon_perimeter(rect), 12));
	assert((polygon_centroid(rect) - P(2,1)).len() < 1e-9L);
	assert(point_in_polygon({2,1}, rect) == 1);
	assert(point_in_polygon({4,1}, rect) == 0);
	assert(point_in_polygon({5,1}, rect) == -1);
	assert(close(segment_to_segment({{0,0},{1,0}}, {{2,1},{2,-1}}), 1));
	vp square_hpi = hpi({{{0,0},{1,0}},{{1,0},{1,1}},
		{{1,1},{0,1}},{{0,1},{0,0}}});
	assert(square_hpi.size() == 4 && close(polygon_area(square_hpi),1));
	assert(cc_intersection(C(P(0,0),1),C(P(2,0),1)).size()==1);

	vp concave{{0,0},{3,0},{3,3},{1,1},{0,3}};
	auto tri = ear_clipping(concave);
	assert(tri.size() == concave.size() - 2);
	LD triangulated = 0;
	for (auto f : tri)
		triangulated += fabsl((concave[f[1]] - concave[f[0]])
			^ (concave[f[2]] - concave[f[0]])) / 2;
	assert(close(triangulated, polygon_area(concave)));
	vp collinear_polygon{{0,0},{1,0},{3,0},{3,2},{0,2}};
	auto collinear_tri=ear_clipping(collinear_polygon);
	LD collinear_area=0;
	for (auto f:collinear_tri)
		collinear_area+=fabsl((collinear_polygon[f[1]]-collinear_polygon[f[0]])
			^(collinear_polygon[f[2]]-collinear_polygon[f[0]]))/2;
	assert(close(collinear_area,polygon_area(collinear_polygon)));

	plane px({1,0,0},{1,0,0}), py({0,1,0},{0,2,0}), pz({0,0,1},{0,0,3});
	assert((three_plane_intersection(px,py,pz)-p3(1,2,3)).len() < 1e-9L);
	vector<p3> cube;
	for (int x : {-1,1}) for (int y : {-1,1}) for (int z : {-1,1})
		cube.push_back({(LD)x,(LD)y,(LD)z});
	Sphere s = min_sphere(cube);
	assert(s.c.len() < 1e-8L && close(s.r,sqrtl(3.0L)));
	for (p3 x : cube) assert(in_sphere(x,s));
	Sphere line_sphere=min_sphere({{-2,0,0},{1,0,0},{0,0,0}});
	assert((line_sphere.c-p3(-0.5,0,0)).len()<1e-9L&&close(line_sphere.r,1.5L));
	Sphere flat_sphere=min_sphere({{-1,-1,0},{1,-1,0},{1,1,0},{-1,1,0}});
	assert(flat_sphere.c.len()<1e-9L&&close(flat_sphere.r,sqrtl(2.0L)));
	ConvexHull3D hull3;
	assert(hull3.build(cube));
	assert(!hull3.face.empty());
	for (auto f : hull3.face)
		for (int i = 0; i < (int)hull3.p.size(); ++i)
			assert(sgn(hull3.volume(f,i)) <= 0);
	Triangulation delaunay;
	vp delaunay_points{{0,0},{3,0},{0,2},{2,3},{1,1}};
	delaunay.build(delaunay_points);
	for (Tri *t=triange_pool;t<tot_tri;++t) if (!t->has_ch()) {
		bool original=true;
		for (P v:t->p) {
			bool found=false;
			for (P x:delaunay_points) found|=v==x;
			original&=found;
		}
		if (!original) continue;
		assert(turn(t->p[0],t->p[1],t->p[2])>0);
		for (P x:delaunay_points)
			if (x!=t->p[0]&&x!=t->p[1]&&x!=t->p[2])
				assert(!in_circum(t->p[0],t->p[1],t->p[2],x));
	}

	circle_count=2;
	circles[0]=C(P(),1),circles[1]=C(P(),1);
	circle_union();
	assert(close(coverage_area[2],pi));
	cerr << "computational geometry tests passed\n";
}
