#define cp const P &
#define cl const L &
#define cc const C &
#define vp vector<P>
#define cvp const vector<P> &
#define D double
#define LD long double

mt19937 rnd(time(nullptr));
const LD eps = 1e-12L;
const LD pi = acosl(-1.0L);
const LD INF = 1e18L;

int sgn(LD x) {
	return x > eps ? 1 : (x < -eps ? -1 : 0);
}
