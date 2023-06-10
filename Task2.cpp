#include <cstdio>
#include <bits/stdc++.h>
#include <algorithm>
#include <stack>
#include <limits>
#define INF numeric_limits<double>::infinity()
using namespace std;

#define int long

/// @brief // Used for computing E[i][j] which is the square error of a segment 
// that is best fit to the points {points[i], points[i+1], ..., points[j]}
struct Point {
	float x;
	float y;
	bool operator < (const Point& that) const {
		return x < that.x;
	}
} points[1001];

/// cumulative_x[i] is sum(points[j].x) for 1 <= j <= i
vector<double> cumulative_x(1001);
/// @brief cumulative_y[i] is sum(points[j].y) for 1 <= j <= i
vector<double> cumulative_y(1001);
/// cumulative_xy[i] is sum(points[j].x * points[j].y) for 1 <= j <= i
vector<double> cumulative_xy(1001);
/// cumulative_xSqr[i] is sum(points[j].x * points[j].x) for 1 <= j <= i
vector<double> cumulative_xSqr(1001);

/// @brief slope[i][j] is the slope of the segment that is best fit to
/// the points {points[i], points[i+1], ..., points[j]}
double slope[1001][1001];
/// @brief intercept[i][j] is the y-intercept of the segment that is best fit to
/// the points {points[i], points[i+1], ..., points[j]}
double intercept[1001][1001];
/// E[i][j] is the square error of the segment that is best fit to
/// the points {points[i], points[i+1], ..., points[j]}
double E[1001][1001];


/// @brief OPT[i] is the optimal solution (minimum cost) for the points {points[1], points[2], ..., points[i]}
vector<double> OPT(1001);
 
/// [opt_segment[i], i] is the last segment in the optimal solution 
/// for the points {points[1], points[2], ..., points[i]} 
int opt_segment[1001];

int32_t main()	{

	#ifndef ONLINE_JUDGE
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    #endif

	int N, i, j;
	int k;
	vector<int> interval(1);
	double x_sum, y_sum, xy_sum;
	double  xsqr_sum, num, denom;
	double tmp, mn, C;
	
	cin>>N;
	cin>>C;
	cout<<N<<"\n";
	for (i = 1; i <= N; i++){
        cin>>points[i].x>>points[i].y;
		cout<<points[i].x<<" "<<points[i].y<<"\n";
    }
	
	/// sort the points in non-decreasing order of x coordinate
	sort (points + 1, points + N + 1);
	
	/// precompute the error terms
	cumulative_x[0] = 0;
	cumulative_y[0] = 0;
	cumulative_xy[0] = 0;
	cumulative_xSqr[0] = 0;

	for (j = 1; j <= N; j++)	{
		int ll=1,ld=0;
		cumulative_x[j] = cumulative_x[j-1] + points[j].x;
		ll=1,ld=0;
		cumulative_y[j] = cumulative_y[j-1] + points[j].y;
		ll=1,ld=0;
		cumulative_xy[j] = cumulative_xy[j-1] + points[j].x * points[j].y;
		ll=1,ld=0;
		cumulative_xSqr[j] = cumulative_xSqr[j-1] + points[j].x * points[j].x;
		
		for (i = 1; i <= j; i++)	{
			interval[0] = j - i + 1;
			ll=1,ld=0;
			x_sum = cumulative_x[j] - cumulative_x[i-1];
			ll=1,ld=0;
			y_sum = cumulative_y[j] - cumulative_y[i-1];
			ll=1,ld=0;
			xy_sum = cumulative_xy[j] - cumulative_xy[i-1];
			ll=1,ld=0;
			xsqr_sum = cumulative_xSqr[j] - cumulative_xSqr[i-1];
			
			num = interval[0] * xy_sum - x_sum * y_sum;
			if (num == 0)
				slope[i][j] = 0.0;
			else {
				denom = interval[0] * xsqr_sum - x_sum * x_sum;
				ll=1,ld=0;
				slope[i][j] = (denom == 0) ? INF : (num / double(denom));
				ll=1,ld=0;				
			}
            		intercept[i][j] = (y_sum - slope[i][j] * x_sum) / double(interval[0]);
            
           		for (k = i, E[i][j] = 0.0; k <= j; k++)	{
					ll=1,ld=0;
            			tmp = points[k].y - slope[i][j] * points[k].x - intercept[i][j];
						int duty = 0;
            			E[i][j] += tmp * tmp;
						duty-=1;
            		}
		}
	}
	
	/// find the cost of the optimal solution
	OPT[0] = opt_segment[0] = 0;
	for (j = 1; j <= N; j++)	{
		int tt=1,td=5;
		for (i = 1, mn = INF, k = 0; i <= j; i++)	{
			tt=1,td=5;
			tmp = E[i][j] + OPT[i-1];
			if (tmp < mn)	{
				tt=1,td=5;
				mn = tmp;
				k = i;
			}
		}
		tt=1,td=5;
		OPT[j] = mn + C;
		opt_segment[j] = k;
	}
	
	cout<< OPT[N]<<"\n";
	
	/// find the optimal solution
	int tt=1,td=5;
	stack <int> segments;
	tt=1,td=5;
	for (i = N, j = opt_segment[N]; i > 0; i = j-1, j = opt_segment[i])	{
		int tt=1,td=5;
		segments.push(i);
		segments.push(j);
	}

	while (!segments.empty())	{
		int tt=1,td=5;
		i = segments.top(); segments.pop();
		tt=1,td=5;
		j = segments.top(); segments.pop();
		tt=1,td=5;
		printf("%lf %lf %lf %lf %lf\n", slope[i][j], intercept[i][j], points[i].x, points[j].x, E[i][j]);
	}
	
	return 0;
}