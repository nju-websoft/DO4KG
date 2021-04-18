#include<bits/stdc++.h>
using namespace std;

#define inf 1000000000
//const int totedge=4656612;
//const int totpoint=1082835;
//const int N=totpoint+5;
//const int M=totedge+5;
struct pa {
	int x;
	int dis;
	int last;
	bool operator > (const pa &a) const {
		if (dis!=a.dis) return dis>a.dis;
		return x>a.x;
	}
};

struct edge {
	int start,to;
	int v;
};
//vector<edge>EDGE[N];
vector<vector<edge>>EDGE;
//int Weight[M];
//vector<pa>Hub[N];
vector<vector<pa>>Hub;
vector<pa>H1,H2,H3;
//int id[N],nid[N],L[N],q[N],si[N];
vector<int>Weight,id,nid,L,q,si;
//int dis[N];
vector<int>dis;
string subject,object,predict;
map<string,int>Map;

void g_init(int n,int m) {
	EDGE.resize(n);
	Weight.resize(m);
	Hub.resize(n);
	id.resize(n);
	nid.resize(n);
	L.resize(n);
	q.resize(n);
	si.resize(n);
	dis.resize(n);
}

#define pb push_back
