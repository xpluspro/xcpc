# Stern-Brocot 前 max_depth 层；生成 (分子, 分母, 深度)，根深度为 0
def stern_brocot(max_depth):
	st = [(0, 1, 1, 0, 0)]
	while st:
		a, b, c, d, h = st.pop()
		if h > max_depth: continue
		x, y = a + c, b + d; yield x, y, h
		st += [(x, y, c, d, h + 1), (a, b, x, y, h + 1)]

# Farey 序列 F_n，升序且包含 0/1 和 1/1
def farey(n):
	if n < 1: raise ValueError("n must be positive")
	a, b, c, d = 0, 1, 1, n; yield a, b
	while c <= n:
		yield c, d; k = (n + b) // d
		a, b, c, d = c, d, k * c - a, k * d - b
