struct Sphere {
	p3 c; LD r;
};
bool in_sphere(p3 p,const Sphere &s) {return sgn((p-s.c).len()-s.r)<=0;}
Sphere sphere(p3 a) {return {a,0};}
Sphere sphere(p3 a,p3 b) {p3 c=(a+b)/2;return {c,(a-c).len()};}
Sphere sphere3(p3 a,p3 b,p3 c) {
	p3 u=b-a,v=c-a,n=cross(u,v);
	if (!sgn(n.len2())) {
		Sphere s=sphere(a,b);
		array<pair<p3,p3>,2> candidate{{{a,c},{b,c}}};
		for (auto [x,y]:candidate) {
			Sphere t=sphere(x,y);
			if (t.r>s.r) s=t;
		}
		return s;
	}
	p3 o=a+(cross(n,u)*v.len2()+cross(v,n)*u.len2())
		/(2*n.len2());
	return {o,(a-o).len()};
}
Sphere sphere4(p3 a,p3 b,p3 c,p3 d) {
	p3 u=b-a,v=c-a,w=d-a;
	LD det=mix(u,v,w);
	if (!sgn(det)) {
		array<p3,4> q{a,b,c,d}; Sphere best{{},INF};
		for (int i=0;i<4;++i) for (int j=i+1;j<4;++j) {
			Sphere s=sphere(q[i],q[j]); bool ok=true;
			for (p3 x:q) ok&=in_sphere(x,s);
			if (ok&&s.r<best.r) best=s;
		}
		for (int i=0;i<4;++i) {
			Sphere s=sphere3(q[(i+1)%4],q[(i+2)%4],q[(i+3)%4]); bool ok=true;
			for (p3 x:q) ok&=in_sphere(x,s);
			if (ok&&s.r<best.r) best=s;
		}
		return best;
	}
	p3 o=a+(cross(v,w)*(u.len2()/2)+cross(w,u)*(v.len2()/2)
		+cross(u,v)*(w.len2()/2))/det;
	return {o,(a-o).len()};
}
Sphere min_sphere(vector<p3> p) { // 随机增量，O(n) 期望
	if (p.empty()) return {{},0};
	shuffle(p.begin(),p.end(),rnd); Sphere ret=sphere(p[0]);
	for (int i=1;i<(int)p.size();++i) if (!in_sphere(p[i],ret)) {
		ret=sphere(p[i]);
		for (int j=0;j<i;++j) if (!in_sphere(p[j],ret)) {
			ret=sphere(p[i],p[j]);
			for (int k=0;k<j;++k) if (!in_sphere(p[k],ret)) {
				ret=sphere3(p[i],p[j],p[k]);
				for (int l=0;l<k;++l) if (!in_sphere(p[l],ret))
					ret=sphere4(p[i],p[j],p[k],p[l]);
			}
		}
	}
	return ret;
}
