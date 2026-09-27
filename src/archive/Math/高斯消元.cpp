int gauss(){ // 返回方程自由元的个数
	int r=0,c=0;
	for(;c<n;c++){
		int cur=r;
		for(int i=r;i<n;i++)if(fabs(a[i][c])>fabs(a[cur][c]))cur=i;
		if(fabs(a[cur][c])<eps)continue;
		swap(a[cur],a[r]);
		for(int i=n;i>=c;i--)a[r][i]/=a[r][c];
		for(int i=r+1;i<n;i++)if(fabs(a[i][c])>eps)
			for(int j=n;j>=c;j--)a[i][j]-=a[r][j]*a[i][c];
		r++;
	}
	if(r<n){
		for(int i=r;i<n;i++)if(fabs(a[i][n])>eps)return -1;
		return n-r;
	}
	for(int i=n-2;~i;i--)for(int j=i+1;j<n;j++)
		a[i][n]-=a[j][n]*a[i][j];
	return 0;
}