#include<bits/stdc++.h>
#include <sys/types.h>
#include <unistd.h>
using namespace std;
const int totpoint=102385;
const int N=totpoint+5;
#define pb push_back
void get_proc_mem(unsigned int pid){

        char file_name[64]={0};
        FILE *fd;
        char line_buff[512]={0};
        sprintf(file_name,"/proc/%d/status",pid);

        fd =fopen(file_name,"r");
        if(nullptr == fd){
                return ;
        }

        char name[64];
        int vmsize;
        for (int i=0; i<=10;i++){

                fgets(line_buff,sizeof(line_buff),fd);
                sscanf(line_buff,"%s %u",name,&vmsize);
        }

        fgets(line_buff,sizeof(line_buff),fd);
        sscanf(line_buff,"%s %u",name,&vmsize);
        fprintf(stderr,"%s=%u\n",name,vmsize);
        fclose(fd);
}
//int vis[N],v[N],tmpv[N];
vector<int>vis,v,tmpv;
struct edge {
	int start,to;
	int v;
};//q[N];
vector<edge>q;
struct hub {
	int x,v;
};
//vector<hub>label[N];
vector<vector<hub>>label;
//int degree[N];
vector<int>degree;
//unordered_map<int,edge>g[N];
vector<unordered_map<int,edge>>g;
struct graph {
	int n,m;
	vector<int>L;
	
};//G[N];
vector<graph>G;
bool cmp(int x,int y) {
	return degree[x]<degree[y];
}
int k;
void construct(int x) {
	sort(v.begin()+1,v.begin()+1+G[x].n,cmp);
	//fprintf(stderr,"%d %d\n",v[1],degree[v[1]]);
	for (int i=1;i<=G[x].n;i++) vis[v[i]]=0;
	for (int i=1;i<=G[x].n;i++) {
		int u=v[i];
		if (!vis[u]) {
			vis[u]=2;
			G[x].L.pb(u);
			for (auto p:g[u]) {
				edge now=p.second;
				vis[now.to]|=1;
			}
		}
		else {
			G[x+1].n++;
			tmpv[G[x+1].n]=u;
		}
	}
}

void construct_graph(int x) {
	for (int i=1;i<=G[x].n;i++) {
		int u=v[i];
		if (vis[u]&2) {
			for (auto p:g[u]) {
				int v=p.first;
				g[v].erase(g[v].find(u));
				degree[v]--;
			}
			degree[u]=0;
			continue;
		}
	}
	for (auto u:G[x].L) {
		int r=0;
		for (auto p:g[u]) {
			edge now=p.second;
			q[++r]=now;
		}
		for (int _i=1;_i<=r;_i++)
		for (int _j=_i+1;_j<=r;_j++) {
			int i=q[_i].to,j=q[_j].to;
			if (g[i].find(j)==g[i].end()) {
				g[i].insert(make_pair(j,edge{i,j,q[_i].v+q[_j].v}));
				g[j].insert(make_pair(i,edge{j,i,q[_i].v+q[_j].v}));
				degree[i]++;
				degree[j]++;
			}
			else {
				if (g[i][j].v>q[_i].v+q[_j].v) {
					g[i][j].v=q[_i].v+q[_j].v;
					g[j][i].v=q[_i].v+q[_j].v;
				}
			}
		}
	}
	for (int i=1;i<=G[x+1].n;i++) v[i]=tmpv[i];
}
inline int ask(int x,int y) {
	int dis=1000000000;
	for (auto p:label[x])
	for (auto p1:label[y]) {
		if (p.x==p1.x) dis=min(dis,p.v+p1.v);
	}
	return dis;
}
//int qq[N],dis[N],l[N];

vector<int>qq,dis,l;

void init(int n) {
	l.resize(n);
	dis.resize(n);
	qq.resize(n);
	label.resize(n);
	G.resize(n);
	degree.resize(n);
	vis.resize(n);
	tmpv.resize(n);
	v.resize(n);
	q.resize(n);
	g.resize(n);
}
int main(int argv,char *argc[]) {
	freopen(argc[1],"r",stdin);
	int n,m;
	scanf("%d%d",&n,&m);
	init(n+5);
	//scanf("%d%d",&G[1].n,&G[1].m);
	G[1].n=n; G[1].m=m;
	for (int i=1;i<=G[1].m;i++) {
		int x,y,z;
		scanf("%d%d%d",&x,&y,&z);
		g[x].insert(make_pair(y,edge{x,y,z}));
		g[y].insert(make_pair(x,edge{y,x,z}));
		degree[x]++;
		degree[y]++;
	}
	for (int i=1;i<=G[1].n;i++) v[i]=i;
	k=1;
	for (int i=1;G[i].n>0;i++,k=i) {
		fprintf(stderr,"%d %d\n",i,G[i].n);
		construct(i);
		construct_graph(i);
	}
	fprintf(stderr,"%d\n",k);
	for (int i=1;i<=G[1].n;i++) label[i].pb(hub{i,0}),l[i]=k;
	for (int i=1;i<k;i++) {
		for (auto p:G[i].L) {
			l[p]=i;
			for (auto p1:g[p]) {
				label[p].pb(hub{p1.second.to,p1.second.v});
			}
		}
	}
	for (int i=1;i<=G[1].n;i++) dis[i]=-1;
	for (int i=k-1;i>=1;i--) {
		for (auto x:G[i].L) {
			int tmpr=0;
			for (auto h:label[x]) dis[h.x]=h.v;
			for (auto h:label[x]) {
				if (h.x==x) continue;
				assert(l[x]<l[h.x]);
				if (dis[h.x]==-1) continue;
				for (auto h1:label[h.x]) {
					if (dis[h1.x]==-1) {
						dis[h1.x]=h1.v+h.v;
						qq[++tmpr]=h1.x;
					}
					else dis[h1.x]=min(dis[h1.x],h1.v+h.v);
				}
			}
			for (int j=1;j<=tmpr;j++) label[x].pb(hub{qq[j],dis[qq[j]]});
			for (int j=0;j<label[x].size();j++) {
				label[x][j].v=min(label[x][j].v,dis[label[x][j].x]);
				dis[label[x][j].x]=-1;
			}
		}
	}
	freopen(argc[2],"w",stdout);
	printf("%d\n",G[1].n);
	long long sum=0;
	for (int i=1;i<=G[1].n;i++) {
		printf("%d ",label[i].size());
		sum+=label[i].size();
		for (int j=0;j<label[i].size();j++) 
			printf("%d %d ",label[i][j].x,label[i][j].v);
		puts("");
	}
	fprintf(stderr,"%lf\n",1.*sum/G[1].n);
   	int pid=getpid();
	get_proc_mem(pid);
	fprintf(stderr,"%lf\n",1.*clock()/CLOCKS_PER_SEC);

}
