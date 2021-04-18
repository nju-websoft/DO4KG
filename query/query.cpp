 #pragma comment(linker, "/STACK:102400000,102400000")
#include<bits/stdc++.h>
#include"A/labels.h"
#include"B/highway_cover_labelling.h"
#include"D/pruned_highway_labeling.h"
#define pb push_back
using namespace std;
class Hub_0{
	public :
	struct pa {
		int x,v;
	};
	vector<vector<pa>>Label;
	vector<int>rank;
	int n;
	//vector<pa>Label[N];
	//int rank[N];
	void init(int n) {
		Label.resize(n);
		rank.resize(n);
	}
	Hub_0 (char *rankname,char *hubname) {
		freopen(rankname,"r",stdin);
		scanf("%d",&n);
		init(n+5);
		for (int i=1;i<=n;i++) {
			int x;
			scanf("%d",&x);
			rank[x]=i;
		}
		freopen(hubname,"r",stdin);
		scanf("%d",&n);
		for (int i=1;i<=n;i++) {
			int k,x,v;
			scanf("%d",&k);
			for (int j=1;j<=k;j++) {
				scanf("%d%d",&x,&v);
				Label[i].pb(pa{x,v});
			}
		}
	}
	int getdis(int x,int y) {
		int i=0,j=0;
		int dis=inf;
		while (i<Label[x].size()&&j<Label[y].size()) {
			if (Label[x][i].x==Label[y][j].x) dis=min(dis,Label[x][i].v+Label[y][j].v),i++,j++;
			else if (rank[Label[x][i].x]<rank[Label[y][j].x]) i++;
			else j++;
		}
		return dis;
	}
};
class Hub_a{
	public :
	Label now;
	Hub_a (char *s) {
		now.load_labels(s);
	}	
	int getdis(int x,int y) {
		x--,y--;
		return now.query_p(x,y);
	}
};
class Hub_b {
	public :
	HighwayLabelling *hl;
	Hub_b (int k,char *graph,char *label) {
		hl= new HighwayLabelling(graph, k);
		puts("a");
		hl->LoadIndex(label);
		puts("b");
		int topk[k];
		hl->SelectLandmarks_HD(topk);
		hl->RemoveLandmarks(topk);
	}
	int getdis(int x,int y) {
		x--,y--;
		return hl->QueryDistance(x,y);
	}
};

struct dp_tree_node {
        int fa;
        int deep;
        int F,si;
        vector<int>x;
        vector<int>pos;
        vector<int>dis;
};//X[N];

vector<dp_tree_node>X;

class Hub_c {
	public:
	int n;
	void init(int n) {
		X.resize(n);
	}
	int lca(int x,int y) {
	        while (X[x].F!=X[y].F) {
        	        if (X[X[x].F].deep>X[X[y].F].deep) x=X[X[x].F].fa;
                	else y=X[X[y].F].fa;
        	}
        	if (X[x].deep>X[y].deep) return y;
        	return x;
	}
	int ask(int x,int y) {
        	int l=lca(x,y);
        	int dis=inf;
        	for (int i=1;i<X[l].pos.size();i++) {
        	        int s=X[l].pos[i];
        	        dis=min(dis,X[x].dis[s]+X[y].dis[s]);
        	}
        	return dis;
	}
	inline Hub_c(char *s) {
        	freopen(s,"r",stdin);
        	scanf("%d",&n);
		init(n+5);
        	for (int i=1;i<=n;i++) {
        	        scanf("%d%d%d",&X[i].fa,&X[i].F,&X[i].deep);
        	        int size;
        	        scanf("%d",&size);
        	        X[i].pos.resize(size,0);
        	        for (int j=0;j<size;j++) {
        	                int x;
        	                scanf("%d",&X[i].pos[j]);
        	        }
                	scanf("%d",&size);
                	X[i].dis.resize(size,0);
                	for (int j=0;j<size;j++) {
                        	int x;
                        	scanf("%d",&X[i].dis[j]);
                	}
        	}
	}
};

class Hub_d {
	public :
	PrunedHighwayLabeling phl;
	Hub_d (char *label) {
		phl.LoadLabel(label);
		phl.Statistics();	
	}
	int getdis(int x,int y) {
		x--,y--;
		return phl.Query(x,y);
	}
};
class Hub_g {
	public:
	struct pa {
	        int x,y;
	};
	//vector<pa>Label[N];
	vector<vector<pa>>Label;
	int n;
	int dis;
	vector<int>Dis;
	void init(int n) {
		Dis.resize(n);
		Label.resize(n);
	}
        Hub_g (char *label) {
                freopen(label,"r",stdin);
		scanf("%d",&n);
		init(n+5);
		for (int i=1;i<=n;i++) {
			int k;
			Dis[i]=0;
			scanf("%d",&k);
			for (int j=1;j<=k;j++) {
				int x,y;
				scanf("%d%d",&x,&y);
				Label[i].push_back(pa{x,y});
			}
		}
        }
        int getdis(int x,int y) {
		int dis=inf;
        	for (auto p:Label[x]) Dis[p.x]=p.y+1;
		for (auto p:Label[y]) if (Dis[p.x]) dis=min(Dis[p.x]+p.y-1,dis);
		for (auto p:Label[x]) Dis[p.x]=0;
		return dis;
        }
};
class Hub_h{
	public:
	int n,m;
	vector<int>a,Dis;
	struct pa {
		    int x,val;
		    friend bool operator ==(const pa &a,const pa &b) {
		    return a.x==b.x;
		}
	};
	//vector<pa>label[N];
	vector<vector<pa>>label,adj;
	void init(int n) {
		a.resize(n);
		Dis.resize(n);
		label.resize(n);
		adj.resize(n);
	}
	struct dij {
		int x,val,flag;
		friend bool operator <(const dij &a,const dij &b) {
			if (a.val!=b.val) return a.val>b.val;
			if (a.flag!=b.flag) return a.flag<b.flag;
			return a.x>b.x;
		}
	};
	inline int ask(int x,int y) {
		int dis=1000000000;
		for (auto p:label[x]) Dis[p.x]=p.val+1;
        	for (auto p:label[y]) if (Dis[p.x]) dis=min(Dis[p.x]+p.val-1,dis);
                for (auto p:label[x]) Dis[p.x]=0;
                return dis;
        }
	int query(int s,int t) {
	  if (s == t) {   return 0;  }
	  std::priority_queue<dij>que;
	  std::vector<int> qdist[2];
	  qdist[0].resize(n+1, inf);
	  qdist[1].resize(n+1, inf);
	  for (int dir = 0; dir < 2; dir++){
		int v = dir == 0 ? s : t;
		que.push(dij{v,0,dir});
		qdist[dir][v] = 0;
	  }
	  for (pa p:label[s]) {
		    if (a[p.x]) qdist[0][p.x]=p.val,que.push(dij{p.x,p.val,0});
	  }
	  for (pa p:label[t]) {
		    if (a[p.x]) qdist[1][p.x]=p.val,que.push(dij{p.x,p.val,1});
	  }
	  int res = ask(s, t);

		while (!que.empty()) {
		    dij v=que.top();
		    que.pop();
		    if (qdist[v.flag][v.x]!=v.val) continue;
		    qdist[v.flag][v.x]=v.val;
		    res=min(res,qdist[v.flag][v.x]+qdist[v.flag^1][v.x]);
		    if (v.flag==0&&((qdist[v.flag][v.x]<<1)>=res)) continue;
		    if (v.val>=res) continue;
		    for (pa w:adj[v.x]) {
				if (qdist[v.flag][w.x]<=qdist[v.flag][v.x]+w.val) continue;
				qdist[v.flag][w.x]=qdist[v.flag][v.x]+w.val;
				res=min(res,qdist[v.flag][w.x]+qdist[v.flag^1][w.x]);
				if (v.flag==1&&(qdist[v.flag][w.x]<<1)>res) continue;
				que.push(dij{w.x,qdist[v.flag][w.x],v.flag});
		    }
	  	}
	  	return res;
  	}
	void load_graph(char *s) {
        	freopen(s,"r",stdin);
        	int n;
		scanf("%d",&n);
        	for (int i=1;i<=n;i++) {
        	        int x;
        	        scanf("%d",&x);
        	        a[x]=1;
        	}
        	scanf("%d",&m);
        	for (int i=1;i<=m;i++) {
        	        int x,y,z;
        	        scanf("%d%d%d",&x,&y,&z);
        	        adj[x].pb(pa{y,z});
        	        adj[y].pb(pa{x,z});
        	}
	}
	void load_label(char *s) {
        	freopen(s,"r",stdin);
        	scanf("%d",&n);
		init(n+5);
        	for (int i=1;i<=n;i++) {
			Dis[i]=0;
			int k;
			scanf("%d",&k);
			for (int j=1;j<=k;j++) {
				int x,y;
				scanf("%d%d",&x,&y);
				label[i].pb(pa{x,y});
			}
        	}
	}
	Hub_h (char *graph_name,char *label_name) {
		load_label(label_name);
		load_graph(graph_name);
		printf("%s\n",graph_name);

	
	
	}
};
//int qrx[M+5],qry[M+5];
vector<int>qrx,qry;
int totinf,qnum;
int main(int argc, char* argv[]) {
	struct  timeval start;
        struct  timeval end;
	gettimeofday(&start,NULL);
	unsigned  long diff1,diff2;
	freopen(argv[2],"r",stdin);
	scanf("%d",&qnum);
	qrx.resize(qnum+5);
	qry.resize(qnum+5);
	for (int i=1;i<=qnum;i++) {
		scanf("%d%d",&qrx[i],&qry[i]);
		printf("%d %d\n",qrx[i],qry[i]);
	}
	int type=atoi(argv[1]);
	printf("%d\n",type);
	for (int i=0;i<argc;i++) printf("%s ",argv[i]);
	puts("");
	//return 0;
	double t1,t2;
	//Hub_h hub_h(argv[2],argv[3]);
	//return 0;
	if (type==0) {
		//return 0;
		Hub_0 hub_0(argv[3],argv[4]);
		gettimeofday(&end,NULL);
		diff1=1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
		start=end;
		freopen(argv[5],"w",stdout);
		//return 0;
		for (int i=1;i<=qnum;i++) {
			int dis;
			printf("%d\n",dis=hub_0.getdis(qrx[i],qry[i]));
			//fprintf(stderr,"%d\n",dis=hub_0.getdis(qrx[i],qry[i]));
			if (dis==inf) totinf++;
		}
		gettimeofday(&end,NULL);
                diff2=1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
		fprintf(stderr,"load time %lf\n",diff1/1000000.0);
		fprintf(stderr,"quary time %lf\n",diff2/1000000.0);
	}
	else if (type==1) {
		//return 0;
		Hub_a hub_a(argv[3]);
		//return 0;
		gettimeofday(&end,NULL);
                diff1=1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
                start=end;
		freopen(argv[4],"w",stdout);
		for (int i=1;i<=qnum;i++) {
			//fprintf(stderr,"%d\n",i);
			int dis;
			printf("%d\n",dis=hub_a.getdis(qrx[i],qry[i]));
			if (dis==inf) totinf++;
		}
		gettimeofday(&end,NULL);
                diff2=1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
                fprintf(stderr,"load time %lf\n",diff1/1000000.0);
                fprintf(stderr,"quary time %lf\n",diff2/1000000.0);
	}
	else if (type==2) {
		int k=atoi(argv[3]);
		Hub_b hub_b(k,argv[4],argv[5]);
		gettimeofday(&end,NULL);
                diff1=1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
                start=end;
		freopen(argv[6],"w",stdout);
		for (int i=1;i<=qnum;i++) {
			//fprintf(stderr,"%d\n",i);
			int dis;
			printf("%d\n",dis=hub_b.getdis(qrx[i],qry[i]));
			if (dis==inf) totinf++;
		}
		gettimeofday(&end,NULL);
                diff2=1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
                fprintf(stderr,"load time %lf\n",diff1/1000000.0);
                fprintf(stderr,"quary time %lf\n",diff2/1000000.0);

	}
	else if (type==3) {
		Hub_c hub_c(argv[3]);
		gettimeofday(&end,NULL);
		diff1=1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
		start=end;
		freopen(argv[4],"w",stdout);
		for (int i=1;i<=qnum;i++) {
		//fprintf(stderr,"%d\n",i);
			int dis;
		        printf("%d\n",dis=hub_c.ask(qrx[i],qry[i]));
		        if (dis==inf) totinf++;
		}
		gettimeofday(&end,NULL);
		diff2=1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
		fprintf(stderr,"load time %lf\n",diff1/1000000.0);
		fprintf(stderr,"quary time %lf\n",diff2/1000000.0);
	}
	else if (type==4) {
		Hub_d hub_d(argv[3]);
		//Hub_d hub_d("../d/1/label_50");
		gettimeofday(&end,NULL);
                diff1=1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
                start=end;
		freopen(argv[4],"w",stdout);
                for (int i=1;i<=qnum;i++) {
                        //fprintf(stderr,"%d\n",i);
                        int dis;
                        printf("%d\n",dis=hub_d.getdis(qrx[i],qry[i]));
                        if (dis==inf) totinf++;
                }
		gettimeofday(&end,NULL);
                diff2=1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
                fprintf(stderr,"load time %lf\n",diff1/1000000.0);
                fprintf(stderr,"quary time %lf\n",diff2/1000000.0);
                
               
              
	}
	else if (type==5) {	
		Hub_g hub_g(argv[3]);
		gettimeofday(&end,NULL);
                diff1=1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
                start=end;
		freopen(argv[4],"w",stdout);
                for (int i=1;i<=qnum;i++) {
                         int dis;
                        printf("%d\n",dis=hub_g.getdis(qrx[i],qry[i]));
                        if (dis==inf) totinf++;
                }
                gettimeofday(&end,NULL);
                diff2=1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
                fprintf(stderr,"load time %lf\n",diff1/1000000.0);
                fprintf(stderr,"quary time %lf\n",diff2/1000000.0); 
               
               
	}
	else if (type==6) {
		//break;
                Hub_h hub_h(argv[3],argv[4]);
		//return 0;
                gettimeofday(&end,NULL);
                diff1=1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
                start=end;
                freopen(argv[5],"w",stdout);

                for (int i=1;i<=qnum;i++) {
 			//fprintf(stderr,"%d\n",i);
                        int dis;
			
                        printf("%d\n",dis=hub_h.query(qrx[i],qry[i]));
			
                        if (dis==inf) totinf++;
                } 
		gettimeofday(&end,NULL);
                diff2=1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
                fprintf(stderr,"load time %lf\n",diff1/1000000.0);
                fprintf(stderr,"quary time %lf\n",diff2/1000000.0);
        }

	fprintf(stderr,"unconnected %d\n",totinf);
}
