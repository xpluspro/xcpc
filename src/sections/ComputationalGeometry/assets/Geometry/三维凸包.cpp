// 随机增量；输入需不存在重复点且仿射维数为 3。
// 点数不足、共线或全部共面时 build 返回 false，face 为空。
struct ConvexHull3D {
	using Face = array<int,3>;
	vector<p3> p;
	vector<Face> face;

	LD volume(Face f, int x) const {
		return mix(p[f[1]]-p[f[0]],p[f[2]]-p[f[0]],p[x]-p[f[0]]);
	}
	Face outward(int a,int b,int c,int inside) const {
		Face f{a,b,c};
		if (sgn(volume(f,inside))>0) swap(f[1],f[2]);
		return f;
	}
	bool build(vector<p3> points) {
		p=move(points), face.clear();
		if (p.size()<4) return false;
		shuffle(p.begin(),p.end(),rnd);
		int n=(int)p.size(),b=1,c=-1,d=-1;
		for (int i=2;i<n;++i) if (cross(p[b]-p[0],p[i]-p[0]).len()>eps) {c=i;break;}
		if (c<0) return false;
		swap(p[2],p[c]);
		for (int i=3;i<n;++i) if (fabsl(mix(p[1]-p[0],p[2]-p[0],p[i]-p[0]))>eps) {d=i;break;}
		if (d<0) return false;
		swap(p[3],p[d]);
		face={outward(0,1,2,3),outward(0,3,1,2),
			outward(0,2,3,1),outward(1,3,2,0)};
		for (int x=4;x<n;++x) {
			set<pair<int,int>> horizon;
			vector<Face> keep;
			for (Face f:face) if (sgn(volume(f,x))>0) {
				for (int k=0;k<3;++k) {
					pair<int,int> e={f[k],f[(k+1)%3]},rev={e.second,e.first};
					if (horizon.count(rev)) horizon.erase(rev); else horizon.insert(e);
				}
			} else keep.push_back(f);
			for (auto [u,v]:horizon) keep.push_back({u,v,x});
			face.swap(keep);
		}
		return true;
	}
};
