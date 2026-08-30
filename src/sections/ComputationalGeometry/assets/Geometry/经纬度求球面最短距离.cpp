// lontitude 经度范围: $\pm\pi$, latitude 纬度范围: $\pm\pi/2$
LD sphereDis(LD lon1, LD lat1, LD lon2, LD lat2, LD R) {
	LD d = cosl(lat1) * cosl(lat2) * cosl(lon1 - lon2) + sinl(lat1) * sinl(lat2);
	return R * acosl(clamp(d, -1.0L, 1.0L)); }
