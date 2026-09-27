Val work(LL P, LL R, LL Q, LL n, Val VU, Val VR) {
	// $P\geq0,Q>0,n\geq0,0\leq R<Q$; $(Px+R)/Q$, $1\leq x\leq n$
	// 每次新增纵移记 VU，横移记 VR；经过整点时先 VU 再 VR
	// Val 的乘法需满足结合律，ksm(v, 0) 需返回乘法单位元
	if(!(((i128)n * P + R) / Q)) return ksm(VR, n);
	if(P>=Q) return work(P%Q,R,Q,n, VU, ksm(VU, P/Q) * VR);
	Val res; swap(VU,VR);
	res = ksm(VU, (Q-R-1)/P)*VR;
	LL m = ((i128)n * P + R) / Q;
	res = res * work(Q, (Q-R-1)%P, P, m-1, VU, VR);
	return res * ksm(VU, n - ((i128)m*Q - R - 1) / P); }
