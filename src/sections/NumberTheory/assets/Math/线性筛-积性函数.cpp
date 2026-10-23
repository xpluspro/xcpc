// 线性筛：同时求欧拉函数 phi、莫比乌斯函数 mu 及其前缀和。
const int MAXN = 1000000;
int prime[MAXN + 1], pcnt;
bool composite[MAXN + 1];
int phi[MAXN + 1], mu[MAXN + 1];
long long sumPhi[MAXN + 1], sumMu[MAXN + 1];

void sieve(int n) {
    phi[1] = mu[1] = 1;
    for (int i = 2; i <= n; ++i) {
        if (!composite[i]) {
            prime[pcnt++] = i;
            phi[i] = i - 1;
            mu[i] = -1;
        }
        for (int j = 0; j < pcnt && 1LL * i * prime[j] <= n; ++j) {
            int x = i * prime[j];
            composite[x] = true;
            if (i % prime[j] == 0) {
                phi[x] = phi[i] * prime[j];
                mu[x] = 0;
                break;
            }
            phi[x] = phi[i] * (prime[j] - 1);
            mu[x] = -mu[i];
        }
    }
    for (int i = 1; i <= n; ++i) {
        sumPhi[i] = sumPhi[i - 1] + phi[i];
        sumMu[i] = sumMu[i - 1] + mu[i];
    }
}
