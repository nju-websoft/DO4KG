#include "graph.h"
#include <sys/types.h>
#include <unistd.h>
#include "tree.h"
#include<ext/pb_ds/priority_queue.hpp>
using namespace std;
long long tothub;
//int deep[N];
vector<int>deep;
struct pa1 {
	int x,dis;
};//Hub_inv[N];
vector<pa1>Hub_inv;
int Tmp;
//int tmpw[M];
vector<int>tmpw,X,Y,A,vis,q1,e,B,B1,q2,vis1,fa,eid;
map<string,int>entity,pre;
//int X[M],Y[M];
int n,m;
//double BC[N],a[N],b[N];
vector<double>BC,a,b;
__gnu_pbds::priority_queue<pa,greater<pa>,__gnu_pbds::pairing_heap_tag> pq;

//int A[N];
//int e[N],B[N],B1[N],q2[N],t2,vis1[N];
int t2;
void init(int n,int m) {
	deep.resize(n);
	Hub_inv.resize(n);
	tmpw.resize(m);
	X.resize(m);
	Y.resize(m);
	BC.resize(n);
	a.resize(n);
	b.resize(n);
	A.resize(n);
	vis.resize(n);
	q1.resize(n);
	e.resize(n);
	B.resize(n);
	B1.resize(n);
	q2.resize(n);
	vis1.resize(n);
	fa.resize(n);
	eid.resize(m);
}
int getdis(int x,int y) {
	/*int i=0,j=0;
	int dis=inf;
	while (i<Hub[x].size()&&j<Hub[y].size()) {
		if (Hub[x][i].x==Hub[y][j].x) {
			dis=min(dis,Hub[x][i].dis+Hub[y][j].dis);
			i++,j++;
		}
		else if (nid[Hub[x][i].x]<nid[Hub[y][j].x]) i++;
		else j++;
	}*/
	int dis=inf;
	for (auto p:Hub[y]) dis=min(dis,p.dis+A[p.x]);
	return dis;
}

void print_hub(char *hubname) {
	int tt=0;
	tothub=0;
	freopen(hubname,"w",stdout);
	printf("%d\n",n);
	for (int i=1;i<=n;i++) {
		printf("%d\n",Hub[i].size());
		for (auto p:Hub[i]) {
			//A.db_insert_Hub(i,p.x,p.y,p.last,p.v);
			printf("%d %d\n",p.x,p.dis);
			tt=max(tt,p.dis);
			tothub++;
		}
		//Hub[i].clear();
	}
	//if (&A.mysql!=NULL) mysql_commit(&A.mysql);
	fprintf(stderr,"%lld %d\n",tothub,tt);
}

void print_rank(char *rankname) {
	freopen(rankname,"w",stdout);
	printf("%d\n",n);
	for (int i=1;i<=n;i++) printf("%d\n",id[i]);
	//if (&A.mysql!=NULL) mysql_commit(&A.mysql);
}
//int vis[N];
//int q1[N],tt;
int tt;
void dij(int x,int n) {
	for (int i=1;i<=tt;i++) dis[q1[i]]=inf,vis[q1[i]]=0;
	dis[x]=0;
	L[x]=0;
	tt=0;
	pq.push(pa{x,0});
	//read_Hub_u(H1,x);
	for (auto p:Hub[x]) A[p.x]=p.dis;
	int tmp=0;
	while (!pq.empty()) {
		tmp++;
		pa gt=pq.top();
		pq.pop();
		if (vis[gt.x]) continue;
		vis[gt.x]=1;
		q1[++tt]=gt.x;
		if (getdis(x,gt.x)>(gt.dis)) {
			Hub[gt.x].pb(pa{x,gt.dis});
			tothub++;
			
			//A.db_insert_Hub(gt.x,x,gt.y,L[gt.x],dis[gt.x]-dis[L[gt.x]]);
			for (auto p:EDGE[gt.x]) {
				if (dis[p.to]>dis[gt.x]+p.v) {
					dis[p.to]=dis[gt.x]+p.v;
					L[p.to]=gt.x;
					pq.push(pa{p.to,dis[p.to]});
				}
			}
		}
	}
	for (auto p:Hub[x]) A[p.x]=inf;
	//fprintf(stderr,"B %d\n",tmp);
	//if (Tmp>=100000) insert_hub(n);
}
queue<int>qq;
//int e[N],B[N],B1[N],q2[N],t2,vis1[N];
void spfa(int x,int n) {
	int tmp=0;
	for (int i=1;i<=tt;i++) dis[q1[i]]=inf,vis[q1[i]]=0;
	dis[x]=0;
	e[x]=1;
	L[x]=0;
	tt=0;
	qq.push(x);
	//read_Hub_u(H1,x);
	for (auto p:Hub[x]) A[p.x]=p.dis;
	while (!qq.empty()) {
		tmp++;
		int now=qq.front();
		qq.pop();
		e[now]=0;
		if (!vis[now]) vis[now]=1,q1[++tt]=now;
		if (B1[now]!=x) B1[now]=x,B[now]=getdis(x,now);
		if (B[now]<=dis[now]) continue;
		if (!vis1[now])
		q2[++t2]=now,vis1[now]=1;
			//A.db_insert_Hub(gt.x,x,gt.y,L[gt.x],dis[gt.x]-dis[L[gt.x]]);
		for (auto p:EDGE[now]) {
			if (dis[p.to]>dis[now]+p.v) {
				dis[p.to]=dis[now]+p.v;
				L[p.to]=now;
				if (!e[p.to]) qq.push(p.to),e[p.to]=1;
			}
		}
	}
	
	for (int i=1;i<=t2;i++) {
		int now=q2[i];
		Hub[now].pb(pa{x,dis[now]});
		vis1[now]=0;
		tothub++;
	}
	t2=0;
	for (auto p:Hub[x]) A[p.x]=inf;
	//fprintf(stderr,"A %d\n",tmp);
	//if (Tmp>=100000) insert_hub(n);
}

bool si_cmp(int x,int y) {
	return si[x]>si[y];
}

bool BC_cmp(int x,int y) {
	return BC[x]>BC[y];
}

bool dis_cmp(int x,int y) {
	return dis[x]<dis[y];
}
void getBC(int x,int n) {
	/*for (int i=1;i<=n;i++) dis[i]=inf,L[i]=0;
	dis[x]=0;
	pq.push(pa{x,0,0});
	deep[x]=1;
	while (!pq.empty()) {
		pa gt=pq.top();
		pq.pop();
		for (auto p:EDGE[gt.x]) {
			//if (getdis(x,gt.x)>(gt.y)) {
				if (dis[p.to]>dis[gt.x]+p.v) {
					dis[p.to]=dis[gt.x]+p.v;
					deep[p.to]=deep[gt.x]+1;
					pq.push(pa{p.to,dis[p.to],0});
				}
			//}
		}
	}*/
	for (int i=1;i<=n;i++) dis[i]=inf,e[i]=0;
	dis[x]=0;
	e[x]=1;
	qq.push(x);
	while (!qq.empty()) {
		int now=qq.front();
		qq.pop();
		e[now]=0;
		for (auto p:EDGE[now]) {
			if (dis[p.to]>dis[now]+p.v) {
				dis[p.to]=dis[now]+p.v;
				if (!e[p.to]) qq.push(p.to),e[p.to]=1;
			}
		}
	}
	for (int i=1;i<=n;i++) q[i]=i,a[i]=0,b[i]=0;
	sort(q.begin()+1,q.begin()+n+1,dis_cmp);
	a[x]=1;
	for (int i=1;i<=n;i++) {
		int now=q[i];
		for (auto p:EDGE[now])
			if (dis[p.to]==dis[now]+p.v) a[p.to]+=a[now];
	}
	for (int i=n;i;i--) {
		int now=q[i];
		BC[now]+=b[now];
		for (auto p:EDGE[now])
			if (dis[now]==dis[p.to]+p.v) 
				b[p.to]+=a[p.to]*1./a[now]*(1+b[now]);
	}
}


//int fa[N];
void work(int n) {
	for (int i=1;i<=n;i++) BC[i]=0,id[i]=i;
	sort(id.begin()+1,id.begin()+1+n,si_cmp);
	for (int i=1;i<=min(1000,n);i++) /*fprintf(stderr,"%d\n",i),*/getBC(id[i],n);
}



void get_hub(int n,char *rankname) {
	//srand(time(0));
	work(n);
	for (int i=1;i<=n;i++) id[i]=i;
	sort(id.begin()+1,id.begin()+1+n,BC_cmp);
	print_rank(rankname);
	//add_Rank(n);
	//read_Rank();
	//for (int i=1;i<=n;i++) printf("%d ",id[i]);
	tothub=0;
	//sort(id+1,id+1+n,si_cmp);
	
	//random_shuffle(id+1,id+1+n);
	for (int i=1;i<=n;i++) nid[id[i]]=i;
	//A.db_init("zyb","Hub");
	//if (&A.mysql!=NULL) mysql_commit(&A.mysql);
	fprintf(stderr,"index time=%lf\n",1.*clock()/CLOCKS_PER_SEC);
	for (int i=1;i<=n;i++) A[i]=inf,q1[++tt]=i;
	for (int i=1;i<=n;i++) {
		/*if (i&1)*/ spfa(id[i],n);
		//else dij(id[i],n);
		//if (i%1000==0||i<=1000) fprintf(stderr,"%d %lld\n",i,tothub);
	}
}
//int eid[M];
bool edge_cmp(int x,int y) {
	return Weight[x]<Weight[y];
}
int get(int x) {
	if (fa[x]==x) return x;
	return fa[x]=get(fa[x]);
}
void mst(int n,int m) {
	T.init(n+5);
	for (int i=1;i<=m;i++) eid[i]=i;
	sort(eid.begin()+1,eid.begin()+1+m,edge_cmp);
	T.n=n;
	T.clear();
	int ts=0;
	//exit(0);
	for (int i=1;i<=n;i++) fa[i]=i;
	for (int i=1;i<=m;i++) {
		int x=eid[i];
		if (get(X[x])!=get(Y[x])) {
			fa[get(X[x])]=get(Y[x]);
			T.add(X[x],Y[x],Weight[x]);
			T.add(Y[x],X[x],Weight[x]);
			
		}
	}
	for (int i=1;i<=n;i++) {
		if (get(i)==i) {
			T.build(i);
			T.dfs(i,0);
		}
	}	
	//T.build(1);
	//T.dfs(1,0);
	for (int i=1;i<=m;i++) {
		int xx=X[i],yy=Y[i];
		//si[xx]++,si[yy]++;
		if (T.getdis(xx,yy)+1e-5>=Weight[i]) {
			ts++;
			si[xx]++,si[yy]++;
			EDGE[xx].pb(edge{xx,yy,Weight[i]});
			EDGE[yy].pb(edge{yy,xx,Weight[i]});
		}
	}
	printf("mst success %d\n",ts);
}

char c1[10005];
int x;

inline void read_edge(char *s) {
	freopen(s,"r",stdin);
	scanf("%d%d",&n,&m);
	g_init(n+5,m+5);
	init(n+5,m+5);
	for (int i=1;i<=m;i++) {
		int x,y;
		int v;
		scanf("%d%d%d",&X[i],&Y[i],&Weight[i]);
	}
}
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
int main(int argv,char *argc[]) {
	//read_entity();
	//read_pre();
	
	read_edge(argc[1]);
	puts("a");
	mst(n,m);
	
	get_hub(n,argc[3]);
	//int s=0;
	//printf("%d\n",s);
	fprintf(stderr,"%lld\n",tothub);
	fprintf(stderr,"Average %lf\n",1.*tothub/n);
	double t1=clock();
	fprintf(stderr,"lable time=%lf\n",1.*t1/CLOCKS_PER_SEC);
	int pid=getpid();
	get_proc_mem(pid);
	print_hub(argc[2]);
	//print_rank();
	/*freopen("query.in","r",stdin);
	int qnum;
	scanf("%d",&qnum);
	for (int i=1;i<=qnum;i++) {
		int x,y;
		scanf("%d%d",&x,&y);
		printf("%.lf\n",getdis(x,y));
	}
	srand(110);
	freopen("100000q.out_t4","w",stdout);
	for (int i=1;i<=100000;i++) {
	  	int s,t;
	  	s=rand()%37745+1,t=rand()%37745+1;
	  	while (s==t) s=rand()%37745+1,t=rand()%37745+1;
	  	printf("%.lf\n",getdis(s,t));
  	}
  	int t2=clock();
  	t2-=t1;
	printf("lable time=%lf\n",1.*t2/CLOCKS_PER_SEC);
	freopen("weight.out","w",stdout);
	for (int i=1;i<=totedge;i++) printf("%d\n",tmpw[i]);*/
	
}
