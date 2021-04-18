#include<bits/stdc++.h>
#include "graph.h"
#include <sys/types.h>
#include <unistd.h>
using namespace std;
int n,m;
//int degree[N],pi[N],deep[N],npi[N];
vector<int>degree,pi,deep,npi;
bool pi_cmp(edge x,edge y) {
	return pi[x.to]>pi[y.to];
}


struct dp_tree_node {
	int fa;
	int deep;
	int F,si;
	vector<int>x;
	vector<int>val;
	vector<int>pos;
	vector<int>dis;
	vector<int>anc;
	vector<int>son;
};//X[N];
vector<dp_tree_node>X;


//vector<edge>G;
//edge que[N];
vector<edge>que;
struct set_tmp {
	int x,degree;
};

struct cmp_tmp {
	bool operator () (set_tmp x,set_tmp y) {
		if (x.degree!=y.degree) return x.degree<y.degree;	
		return x.x<y.x;
	}
};
set<set_tmp,cmp_tmp>Set_tmp;
//unordered_map<int,edge>gr[N];

vector<unordered_map<int,edge>>gr;
vector<int>pos,tmp_queue;
vector<map<int,int>>ma_1;
inline void init(int n,int m) {
	ma_1.resize(n);
	gr.resize(n);
	que.resize(n);
	X.resize(n);
	degree.resize(n);
	deep.resize(n);
	pi.resize(n);
	npi.resize(n);
	pos.resize(n);
	tmp_queue.resize(n);
}
void DP_Tree_decomposition(int n) {
	Set_tmp.clear();
	for (int i=1;i<=n;i++) Set_tmp.insert(set_tmp{i,degree[i]});
	for (int _i=1;_i<=n;_i++) {
		int x=(*Set_tmp.begin()).x;
		pi[x]=_i;
		npi[_i]=x;
		X[x].son.clear();
		Set_tmp.erase(set_tmp{x,degree[x]});
		degree[x]=0;
		/*G.clear();
		read_graph_tmp_x(G,x);
		printf("%d :\n",x);
		for (auto a:G) {
			printf("%d %d %lf\n",a.start,a.to,a.v);
		}
		puts("");*/
		/*C.db_init("zyb","graph_tmp_delete");
		for (auto a:G) {
			C.graph_tmp_delete(x,a.to);
			C.graph_tmp_delete(a.to,x);
			Set_tmp.erase(set_tmp{a.to,degree[a.to]});
			degree[a.to]--;
		}
		C.change_end();*/
		int tot=0;
		for (auto p:gr[x]) que[++tot]=p.second;
		//if (_i>=1300000&&_i%10==0)
		fprintf(stderr,"%d %d\n",_i,tot);
		for (int i=1;i<=tot;i++) {
			edge a=que[i];
			gr[a.to].erase(x);
			assert(pi[a.to]==0);
			//gr[x].erase(a.to);
			Set_tmp.erase(set_tmp{a.to,degree[a.to]});
			degree[a.to]--;
		}
		/*for (auto a:G)
		for (auto b:G) {
			if (a.to==b.to) continue;
			double val;
			int p=read_graph_tmp_x_y(a.to,b.to,val);
			if (p==-1) assert(0);
			if (p==1) {
				C.db_init("zyb","graph_tmp_insert");
				C.graph_tmp_insert(a.to,b.to,a.v+b.v);
				degree[a.to]++;
				C.change_end();
			}
			if (p==0) {
				if (val<=a.v+b.v) continue;
				C.db_init("zyb","graph_tmp_delete");
				C.graph_tmp_delete(a.to,b.to);
				C.change_end();
				C.db_init("zyb","graph_tmp_insert");
				C.graph_tmp_insert(a.to,b.to,a.v+b.v);
				C.change_end();
			}
		}
		
		for (auto a:G) {
			Set_tmp.insert(set_tmp{a.to,degree[a.to]});
		}*/
		for (int i=1;i<=tot;i++)
		for (int j=i+1;j<=tot;j++) {
			edge a=que[i];
			edge b=que[j];
			if (a.to==b.to) assert(0);
			int val;
			if (gr[a.to].find(b.to)==gr[a.to].end()) {
				gr[a.to].insert(make_pair(b.to,edge{a.to,b.to,a.v+b.v}));
				gr[b.to].insert(make_pair(a.to,edge{b.to,a.to,a.v+b.v}));
				degree[a.to]++;
				degree[b.to]++;
			}
			else {
				val=gr[a.to][b.to].v;
				if (val<=a.v+b.v) continue;
				gr[a.to][b.to].v=a.v+b.v;
				gr[b.to][a.to].v=a.v+b.v;
			}
		}
		
		for (int i=1;i<=tot;i++) {
			edge a=que[i];
			Set_tmp.insert(set_tmp{a.to,degree[a.to]});
		}
		//for (auto p:Set_tmp) printf("%d %d\n",p.x,p.degree);
		//puts("-----");
	}
	//fprintf(stderr,"%d\n",1);
	for (int x=1;x<=n;x++) {
                int tot=0;
                for (auto p:gr[x]) que[++tot]=p.second;
                sort(que.begin()+1,que.begin()+1+tot,pi_cmp);
                X[x].x.clear();
                X[x].val.clear();
                X[x].x.pb(0);
                X[x].val.pb(0);
                for (int i=1;i<=tot;i++) {
                        edge p=que[i];
                        X[x].x.pb(p.to),X[x].val.pb(p.v);
                }
                X[x].x.pb(x);
                X[x].val.pb(0);
        }

	for (int i=1;i<=n;i++) {
		int v=npi[i];
		X[v].fa=0;
		if (X[v].x.size()==2) continue;
		int gt=0;
		for (auto p:X[v].x) 
			if (p&&p!=v) {
				if (!gt) gt=p;
				else if (pi[gt]>pi[p]) gt=p;
			}
		X[v].fa=gt;
		//printf("A %d %d\n",v,X[v].fa);
		assert(pi[gt]>pi[v]);
	}
	for (int i=n;i;i--) {
		int v=npi[i];
		X[v].deep=X[X[v].fa].deep+1;
		deep[v]=X[v].deep;
		if (X[v].fa) X[X[v].fa].son.pb(v);
	}
}

//int pos[N],tmp_queue[N];
int lca(int x,int y) {
	while (X[x].F!=X[y].F) {
		if (X[X[x].F].deep>X[X[y].F].deep) x=X[X[x].F].fa;
		else y=X[X[y].F].fa;
	}
	if (deep[x]>deep[y]) return y;
	return x;
}

void dfs(int x,int y) {
	if (!y) y=x;
	X[x].F=y;
	int gt=0,gtw=0;
	for (auto p:X[x].son) {
		if (X[p].si>gt) gt=X[gtw=p].si; 
	}
	if (gtw) dfs(gtw,y);
	for (auto p:X[x].son) {
		if (p==gtw) continue;
		dfs(p,0);
	}
}

void build_tree() {
	for (int i=1;i<=n;i++) X[i].si=0;
	for (int i=1;i<=n;i++) {
		int v=npi[i];
		X[v].si++;
		X[X[v].fa].si+=X[v].si;
	}
	dfs(npi[n],0);
}

long long label_size,size1;

void H2H(int n) {
	DP_Tree_decomposition(n);
	build_tree();
	//exit(0);
	//fprintf(stderr,"%d\n",1);
	//for (int i=1;i<=n;i++) printf("%d\n",X[i].F);
	//fprintf(stderr,"%d\n",2);
	for (int _i=n;_i;_i--) {

		int v=npi[_i];
		X[v].pos.clear();
		X[v].anc.clear();
		X[v].dis.clear();
		int t=deep[v];

		for (int x=v;x;x=X[x].fa) {

			tmp_queue[t]=x;
			pos[x]=t,t--;
		}

		X[v].anc.pb(0);
		X[v].dis.pb(0);
		X[v].pos.pb(0);
		for (int i=1;i<=deep[v];i++)
			X[v].anc.pb(tmp_queue[i]);
		for (int i=1;i<X[v].x.size();i++) {
			X[v].pos.pb(pos[X[v].x[i]]);
			assert(deep[X[v].x[i]]==pos[X[v].x[i]]);
		}
		for (int i=1;i<deep[v];i++) {
			int Dis=inf,d=0;
			for (int j=1;j<X[v].x.size()-1;j++) {
				if (X[v].pos[j]>i) d=X[X[v].x[j]].dis[i];
				else d=X[X[v].anc[i]].dis[X[v].pos[j]];
				Dis=min(Dis,X[v].val[j]+d);
			}
			X[v].dis.pb(Dis);
		}
		X[v].dis.pb(0);
	}
	label_size=0,size1=0;
	for (int i=1;i<=n;i++) { 
		label_size+=X[i].dis.size(),size1+=X[i].pos.size();
	}
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

//map<int,int>ma_1[N];

inline void print_label(char *s) {
	freopen(s,"w",stdout);
	printf("%d\n",n);
	for (int i=1;i<=n;i++) {
		printf("%d %d %d\n",X[i].fa,X[i].F,X[i].deep);
		printf("%d\n",X[i].pos.size());
		for (int j=0;j<X[i].pos.size();j++) {
			printf("%d ",X[i].pos[j]);
		}
		puts("");
		printf("%d\n",X[i].dis.size());
		for (int j=0;j<X[i].dis.size();j++) {
			printf("%d ",(int)X[i].dis[j]);
		}
		puts("");
	}
}

inline void load_label(char *s) {
	freopen(s,"r",stdin);
	scanf("%d",&n);
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
        printf("%s=%u\n",name,vmsize);
        fclose(fd);
}


int main(int argc,char *argv[]) {
	freopen(argv[1],"r",stdin);
	scanf("%d%d",&n,&m);
	//printf("%d %d\n",n,m);
	g_init(n+5,m+5);
	init(n+5,m+5);
	for (int i=1;i<=n;i++) degree[i]=0;
	for (int i=1;i<=m;i++) {
		int x,y,z;
		scanf("%d%d%d",&x,&y,&z);
		gr[x].insert(make_pair(y,edge{x,y,z}));
		gr[y].insert(make_pair(x,edge{y,x,z}));
		degree[x]++;
		degree[y]++;
	}
	//return 0;
	H2H(n);
	print_label(argv[2]);
	double t1=clock();
	freopen("time.out","w",stdout);
	int pid=getpid();
	get_proc_mem(pid);
	printf("Average %lf\n",1.*label_size/n);
        printf("Average1 %lf\n",1.*size1/n);
	printf("%lf\n",1.*t1/CLOCKS_PER_SEC);
	/*A.db_init("zyb","graph_tmp");
	A.dbvnsert_graph_tmp(1,2,3);
	mysql_commit(&A.mysql);
	A.write_end();
	double v;
	int p=read_graph_tmp_x_y(1,2,v);
	printf("%d\n",p);*/
	//todo:重边未处理
}
