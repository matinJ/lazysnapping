/*
以点1为s，以点m为t，求最大流
Sample Input
	5 4
	1 2 40
	1 4 20
	2 4 20
	2 3 30
	3 4 10
	Sample Output
	50
*/


/* 方法一：
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
const int N = 1100;
const int INF =0x3f3f3f3f;

struct Node
{
	int to; //终点
	int cap; //容量
	int rev; //反向边
	Node(int t, int c, int r):to(t),cap(c),rev(r){};
};

vector<Node> v[N];
bool used[N];

void add_Node(int from, int to, int cap)
{
	v[from].push_back(Node(to, cap, v[to].size()));
	v[to].push_back(Node(from, 0, v[from].size()-1));
}

int dfs(int s, int t, int f)
{
	if(s == t)
		return f;
	used[s] = true;
	for(int i =0 ; i < v[s].size(); i++)
	{
		Node &tmp = v[s][i];
		if(used[tmp.to] == false && tmp.cap > 0)
		{
			int d =dfs(tmp.to, t, min(f,tmp.cap));
			if(d > 0)
			{
				tmp.cap -= d;
				v[tmp.to][tmp.rev].cap += d;
				return d;
			}
		}
	}
	return 0;
}

int max_flow(int s, int t)
{
	int flow = 0;
	for(;;)
	{
		memset(used, false, sizeof(used));
		int f = dfs(s, t, INF);
		if( f== 0)
			return flow;
		flow += f;
	}
}
int main()
{
	int n, m;
	while(~scanf("%d%d", &n,&m))
	{
		memset(v, 0, sizeof(v));
		for(int i = 0; i < n; i++)
		{
			int x, y, z;
			scanf("%d%d%d", &x, &y, &z);
			add_Node(x, y, z);
		}
		printf("%d\n", max_flow(1, m));
	}
}
*/


//方法二：
#include <iostream>
#include <queue>
using namespace std;
#define MAXM 201
int m, n;
int si, ei, ci;
int c[MAXM][MAXM];
int f[MAXM][MAXM];
int cf[MAXM][MAXM];
bool visit[MAXM];
int p[MAXM];
struct node
{
	int v, cf;
	void set(int vv, int ccf)
	{
		v = vv; cf =ccf;
	}
};

int main()
{
	while(scanf("%d%d",&n, &m) != EOF)
	{
		memset(c, 0, sizeof(c));
		memset(f, 0, sizeof(f));
		memset(cf, 0, sizeof(cf));
		while(n--)
		{
			scanf("%d%d%d", &si, &ei, &ci);
			c[si][ei] += ci;
			cf[si][ei] = c[si][ei];
		}
		bool flag = true;
		while(flag)
		{
			flag = false;
			memset(visit, 0 ,sizeof(visit));
			queue<node> q;
			node temp;
			temp.set(1, INT_MAX);
			p[1] =0;
			q.push(temp);
			visit[1] = true;
			while(!q.empty())
			{
				node temp = q.front();
				q.pop();
				for(int i = 1; i<= m; i++)
				{
					if(temp.v ==i || visit[i] || cf[temp.v][i] == 0)
						continue;
					node newNode;
					newNode.set(i, min(temp.cf, cf[temp.v][i]));
					p[i] = temp.v;
					q.push(newNode);
					visit[i] = true;
					if(i == m)
					{
						flag = true;
						break;
					}
				}
				if(flag)
					break;
			}
				if(flag )
				{
					int mincf = q.back().cf;
					int v1 = p[m], v2 = m;
					while( v1 != 0)
					{
						f[v1][v2] += mincf;     //修改流
						f[v2][v1] = -f[v1][v2];    
						cf[v1][v2] = c[v1][v2] - f[v1][v2]; //修改残留容量
						cf[v2][v1] = c[v2][v1] - f[v2][v1];
						v2 = v1;
						v1 = p[v1];
					}
				}
			}
			int res = 0;
			for(int i = 2; i <= m; i++)
				res += f[1][i];           //计算最大流
			printf("%d\n", res);
		}
	}
