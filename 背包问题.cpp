#include<iostream>
#include<algorithm>
#include <Windows.h>
#include <sys/utime.h>
#include <time.h>
#include <cstdlib>
#include <ctime>
using namespace std;
int n, W;
int* w, * v;
int** dp;
int a, b;
float* p;
int* r, * r_new;
int m;
int va;
void set()  //初始化输入
{
	cin >> n >> W; //输入物品数n和背包容量W
	w = new int[n + 1];
	v = new int[n + 1];
	w[0] = 0;
	v[0] = 0;
	for (int i = 1; i <= n; i++)
	{
		cin >> w[i] >> v[i];
	}
	dp = new int* [n + 1];
	for (int i = 0; i <= n; i++) {
		dp[i] = new int[W + 1];
	}
	for (int i = 0; i <= W; i++) {
		dp[0][i] = 0;
	}
	return;
}
void findab() //LDP算法的一些初始化工作
{
	p = new float[n + 1];
	p[0] = 0;
	for (int i = 1; i <= n; i++) {
		p[i] = float(v[i]) / float(w[i]);
	}


	int maxIndex = 0, secondMaxIndex = -1;

	for (int i = 1; i <= n; i++) {
		if (p[i] > p[maxIndex]) {
			secondMaxIndex = maxIndex;
			maxIndex = i;
		}
		else if (p[i] < p[maxIndex] && p[i]>p[secondMaxIndex]) {
			secondMaxIndex = i;
		}
	}
	a = maxIndex, b = secondMaxIndex;
	m = W % w[a];	
	va = v[a];
	r = new int[w[a] + 1];
	cout << "pa:" << p[a] << " " << "pb:" << p[b] << endl;
	return;
}
int DP() //0-1背包的二维动态规划
{

	for (int i = 1; i <= n; i++) { //物品
		for (int j = 1; j <= W; j++) { //容量
			if (j < w[i]) {
				dp[i][j] = dp[i - 1][j];
			}
			else {
				dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - w[i]] + v[i]);
			}
		}
	}
	return dp[n][W];
}
int DP1() //完全背包的一维动态规划算法
{
	int* dp1 = new int[W+1];
	for (int i = 0; i < W + 1; i++)
	{
		dp1[i] = 0;
	}
	for (int i = 1; i <= n; i++) { //物品
		for (int j = w[i]; j <= W; j++) { //容量
			dp1[j] = max(dp1[j], dp1[j - w[i]] + v[i]);
		}
	}
	cout << "传统一维动态规划：最优解为 V=" << dp1[W] << endl;
	return dp1[W];
}
int DP_()  //完全背包的二位动态规划算法
{
	for (int i = 1; i <= n; i++) { //物品
		for (int j = 1; j <= W; j++) { //容量
			if (j < w[i]) {
				dp[i][j] = dp[i - 1][j];
			}
			else {
				dp[i][j] = max(dp[i - 1][j], dp[i][j - w[i]] + v[i]);
			}
		}
	}

	return dp[n][W];
}
int LDP()
{

	for (int i = 0; i < w[a] + 1; i++)
	{
		r[i] = 0;
	}
	for (int i = 1; i <= n; i++) { 
		for (int j = w[i]; j <= w[a]; j++) { 
			r[j] = max(r[j], r[j - w[i]] + v[i]);
		}
	}
	v[a] = 0;
	int k = (p[b] * m - r[m]) / ((p[a] - p[b]) * w[a]) + 1;
	int len_rnew = k * w[a] + m;
	r_new = new int[len_rnew + 1];
	for (int i = 0; i < len_rnew + 1; i++)
	{
		r_new[i] = 0;
	}
	for (int i = 1; i <= n; i++) { 
		for (int j = w[i]; j <= len_rnew; j++) { 
			r_new[j] = max(r_new[j], r_new[j - w[i]] + v[i]);
		}
	}
	int result = 0;
	for (int i = 0; i <= k; i++)
	{
		result = max(result,va*(W-m-i*w[a])/w[a] + r_new[m + i * w[a]]);
	}

	cout << "LDP算法：最优解为 V=" << result << endl;
	return result;
}
void testdata() //生成测试数据
{
	srand(time(NULL)); // 用当前时间作为随机数种子

	// 生成不同规模的测试数据
	for (int n = 5000; n <= 10000; n *= 10) {
		for (int t = 0; t < 1; t++) { // 每个规模生成10组数据
			int V = n * 10; // 背包容量为物品个数的10倍
			cout << n << " " << V << endl;
			for (int i = 1; i <= n; i++) {
				int v = rand() % 100 + 1; // 物品体积在1到100之间随机生成
				int w = rand() % 100 + 1; // 物品价值在1到100之间随机生成
				cout << v << " " << w << endl;
			}
			cout << "-------------------------" << endl;
		}
	}

	return ;
}
int main()
{

	//testdata();
	for (int l = 0; l < 10; l++)
	{

		set();
		findab();

		struct timespec sts, ets;
		timespec_get(&sts, TIME_UTC);
		for (int i = 0; i < 10; i++)
		{
			DP1();
		}

		timespec_get(&ets, TIME_UTC);
		time_t dsec = ets.tv_sec - sts.tv_sec;
		long dnsec = ets.tv_nsec - sts.tv_nsec;
		if (dnsec < 0)
		{
			dsec--;
			dnsec += 1000000000ll;
		}
		cout << "DP算法运行时间：";
		printf("%lld.%09lld\n", dsec, dnsec);

		struct timespec sts2, ets2;
		timespec_get(&sts2, TIME_UTC);
		for (int i = 0; i < 10; i++)
		{
			LDP();
		}

		timespec_get(&ets2, TIME_UTC);
		time_t dsec2 = ets2.tv_sec - sts2.tv_sec;
		long dnsec2 = ets2.tv_nsec - sts2.tv_nsec;
		if (dnsec2 < 0)
		{
			dsec2--;
			dnsec2 += 1000000000ll;
		}
		cout << "LDP算法运行时间：";
		printf("%lld.%09lld\n", dsec2, dnsec2);
	}



	return 0;
}
