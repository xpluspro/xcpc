C inv_c2c(P O, LD R, C A) {
	LD OA = (A.c - O).len();
	LD RB = 0.5 * R * R * (1 / (OA - A.r) - 1 / (OA + A.r));
	LD OB = OA * RB / A.r;
	P B = O + (A.c - O) * (OB / OA);
	return {B, RB};
}  // 点 O 在圆 A 外，求圆 A 的反演圆 B，R 是反演半径
C inv_l2c(P O, LD R, L l) {
	P p = project_to_line(O, l);
	LD d = (O - p).len();
	LD RB = R * R / (2 * d);
	P VB = (p - O) / d * RB;
	return {O + VB, RB};
}  // 不过 O 点的直线反演为过 O 点的圆，R 是反演半径
L inv_c2l(P O, LD R, C A) {
	LD t = R * R / (2 * A.r);
	P p = O + (A.c - O).unit() * t;
	return {p, p + (O - p).rot90()};
}  // 过 O 点的圆反演为不过 O 点的直线，R 是反演半径
