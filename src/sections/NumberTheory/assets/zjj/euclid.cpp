Val work(LL P, LL R, LL Q, LL n, Val VU, Val VR) {
//(Px+R)/Q, 1<=x<=i, 经过整点先U再R
	if (Q <= 0 || n < 0) return VR;
	if (R >= Q || R < 0) {
		LL k = R / Q; R %= Q; if (R < 0) R += Q, --k;
		return work(P, R, Q, n, VU, ksm(VU, k) * VR); }
	if(!(((i128)n * P + R) / Q)) return ksm(VR, n);
	if(P>=Q) return work(P%Q,R,Q,n, VU, ksm(VU, P/Q) * VR);
	Val res; swap(VU,VR);
	res = ksm(VU, (Q-R-1)/P)*VR;
	LL m = ((i128)n * P + R) / Q;
	res = res * work(Q, (Q-R-1)%P, P, m-1, VU, VR);
	return res * ksm(VU, n - ((i128)m*Q - R - 1) / P); }