#include"graph.h"
#include<bits/stdc++.h>
int n,m;
int edge_type;
int P;
struct edge1 {
	int start,to;
	double v;
};
//vector<edge1>Edge[N];
map<string,int>Predict;
int pre_tot;
struct edge_tmp {
	int start,to;
	double v;
	int p;
};
//vector<edge_tmp>Et[N];
//map<int,int>ma[N];
//int edsi[N];
//int Pre[N];
int x,y,p;
//int ssi[N],sso[N],tsi[N];
vector<int>sn,edsi,Pre,ssi,sso,tsi;
vector<vector<edge1>>Edge;
vector<map<int,int>>ma,mto,mti;
vector<vector<edge_tmp>>Et;
void init(int n,int m) {
	sn.resize(n);
	edsi.resize(n);
	ssi.resize(n);
	sso.resize(n);
	tsi.resize(n);
	Edge.resize(n);
	ma.resize(n);
	Et.resize(n);
	mto.resize(n);
	mti.resize(n);
	Pre.resize(m);
}
void read_edge(char *graph_name) {
	freopen(graph_name,"r",stdin);
	int id=0;
	scanf("%d%d",&n,&m);
	g_init(n+5,m+5);
	init(n+5,m+5);
	//printf("%d %d\n",n,m);
	//exit(0);
	while (scanf("%d%d%d",&x,&p,&y)!=EOF) {
		if (!x||!y) puts("Error"),exit(0);
		//if (ma[x][y]) continue;
		//ma[x][y]=ma[y][x]=1;
		if (x==y) {
			m--;
			continue;
		}
		sso[x]++;
		ssi[y]++;
		tsi[x]++;
		tsi[y]++;
		//printf("%d %%d d\n",x,p,y;
		double w=0;
		if (edge_type==2) w=(rand()%(100)+1)/100.0;
		Pre[p]++;
		Et[x].pb(edge_tmp{x,y,w,p});
		//Et[y].pb(edge_tmp{y,x,w,p});
	}
}

//map<int,int>mto[N];
//map<int,int>mti[N];

inline double Log(double x) {
	return log(x)/log(2);
}
inline bool cmp_ed(edge_tmp x,edge_tmp y) {
	if (x.to!=y.to) return x.to<y.to;
	return x.v<y.v;
}
int ssum;
inline void calc_edge() {
	for (int i=1;i<=n;i++)
	for (int j=0;j<Et[i].size();j++) {
		int p1=Et[i][j].p;
		mto[i][p1]++;
		mti[Et[i][j].to][p1]++;
	}
	int m1=0;
	//for (int i=1;i<=n;i++) ssi[i]=Et[i].size();
	for (int i=1;i<=n;i++) {
		for (int j=0;j<Et[i].size();j++) {
			int y=Et[i][j].to;
			int p=max(tsi[i],tsi[y])+1;
			int p1=Et[i][j].p;
			if (edge_type==1) {
				double w1=log(p)/log(2);
				Et[i][j].v=w1;
			}
			if (edge_type==3) {
				//PF-ITF
				assert(Pre[p1]!=0);
				double itfp=Log(1.*m/Pre[p1]);
				double w1=(1.*mto[i][p1]/sso[i]+1.*mti[y][p1]/ssi[y])*itfp;
				w1=2/w1;
				Et[i][j].v=w1;
			}
			if (edge_type==4) {
				// weight=1
				Et[i][j].v=1;
			}
			if (edge_type==5) {
				// Frequency-Based Weighting
				double w1=1.*mto[i][p1]/sso[i]+1.*mti[y][p1]/ssi[y];
				w1=2.0/w1;
				Et[i][j].v=w1;
			}
			if (edge_type==6) {
				//iw-glb
				double w1=Log(Pre[p1]);
				Et[i][j].v=w1;
			}
			if (edge_type==7) {
				//iw-loc
				double w1=mto[i][p1]+mti[y][p1]-1;
				Et[i][j].v=w1;
			}
			if (Et[i][j].v*P<0.5) Et[i][j].v=1.0/P;
		}
		sort(Et[i].begin(),Et[i].end(),cmp_ed);
		for (int j=0;j<(int)Et[i].size();j++) {
			if (j&&Et[i][j].to==Et[i][j-1].to) continue;
			if (i==Et[i][j].to) continue;
			if (i<Et[i][j].to)
				Edge[i].pb(edge1{Et[i][j].start,Et[i][j].to,Et[i][j].v});
			else 
				Edge[Et[i][j].to].pb(edge1{Et[i][j].to,Et[i][j].start,Et[i][j].v});
			m1++;
		}
	}
	m=m1;
}

char outname[1005];

int main(int argv, char *argc[]) {
	P=atoi(argc[3]);
	srand(time(0));
	//printf("%d\n",P);
	//return 0;
	edge_type=atoi(argc[1]);
	read_edge(argc[2]);
	//return 0;
	calc_edge();
	double p1=0,p2=1000000000;
	int s1=0;
	//return 0;
	sprintf(outname,"graph_weighted");
	//printf("%s\n",outname);
	freopen(outname,"w",stdout);
	printf("%d %d\n",n,m);
	for (int i=1;i<=n;i++) {
		for (int j=0;j<Edge[i].size();j++) {
			int y=Edge[i][j].to;
			double w=Edge[i][j].v;
			if (edge_type!=4) {
				if (w*P<=0.5) s1++;
				if (i<y) printf("%d %d %.0lf\n",i,y,w*P);
			}
			else {
				if (i<y) printf("%d %d 1\n",i,y);
			}
			p1=max(p1,(w*P));
			p2=min(p2,(w*P));
		}
	}	
	sprintf(outname,"graph_a");
        freopen(outname,"w",stdout);
	for (int i=1;i<=n;i++) {
		for (int j=0;j<Edge[i].size();j++) {
			int y=Edge[i][j].to;
			double w=Edge[i][j].v;
			if (edge_type!=4) {
				if (w*P<=0.5) s1++;
				if (i<y) printf("%d %d %.0lf\n",i,y,w*P);
			}
			else {
				if (i<y) printf("%d %d 1\n",i,y);
			}
			p1=max(p1,(w*P));
			p2=min(p2,(w*P));
		}
	}	
	sprintf(outname,"graph_PHL");
        freopen(outname,"w",stdout);
	for (int i=1;i<=n;i++) {
		for (int j=0;j<Edge[i].size();j++) {
			int y=Edge[i][j].to;
			double w=Edge[i][j].v;
			if (edge_type!=4) {
				if (i<y) printf("%d %d %.0lf\n",i-1,y-1,w*P);
			}
			else printf("%d %d 1\n",i-1,y-1);
		}
	}	
	sprintf(outname,"graph_HL");
        freopen(outname,"w",stdout);
	printf("%d\n",n);
	for (int i=1;i<=n;i++) {
		for (int j=0;j<Edge[i].size();j++) {
			int y=Edge[i][j].to;
			double w=Edge[i][j].v;
			if (edge_type!=4) {
				if (i<y) printf("%d %d %.0lf\n",i-1,y-1,w*P);
			}
			else printf("%d %d 1\n",i-1,y-1);
		}
	}	
	fprintf(stderr,"%lf %lf %d\n",p1,p2,s1);
}
