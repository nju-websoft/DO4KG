struct msttree {
	int tot,root,n;
	//int last[N],to[2*N],Next[2*N];
	//int a[N],val[2*N];
	//int dis[N],fa[N],si[N],q[N],F[N];
	//int st[N],en[N];
	vector<int>last,to,Next,a,val,dis,fa,si,q,F,st,en;
	inline void init(int n) {
		last.resize(n);
		to.resize(2*n);
		Next.resize(2*n);
		a.resize(n);
		val.resize(2*n);
		st.resize(n);
		en.resize(n);
		dis.resize(n);	
		fa.resize(n);
		si.resize(n);
		q.resize(n);
		F.resize(n);
	}
	inline void clear() {
		for (int i=1;i<=n;i++) last[i]=si[i]=fa[i]=0;
		tot=0;
	}
	inline void add(int x,int y,int v) {
		Next[++tot]=last[x]; last[x]=tot; to[tot]=y;
		val[tot]=v;
	}
	inline void build(int x) {
		int l=0,r=1; q[1]=x;
		dis[x]=1;
		//for (int i=1;i<=n;i++) fa[i]=si[i]=0;
		while (l<r) {
			int k=q[++l];
			for (int i=last[k];i;i=Next[i]) {
				if (fa[k]==to[i]) continue;
				fa[q[++r]=to[i]]=k;
				dis[to[i]]=dis[k]+1;
				a[to[i]]=a[k]+val[i];
			}
		}
		for (int i=r;i;i--) si[q[i]]++,si[fa[q[i]]]+=si[q[i]];
	}
	inline void dfs(int x,int y) {
		if (!y) y=x;
		F[x]=y;
		st[x]=++tot;
		int gt=0,gtw=0;
		for (int i=last[x];i;i=Next[i]) {
			if (fa[x]==to[i]) continue;
			if (si[to[i]]>gt) gt=si[gtw=to[i]];
		}
		if (gt) dfs(gtw,y);
		for (int i=last[x];i;i=Next[i]) {
			if (fa[x]==to[i]||gtw==to[i]) continue;
			dfs(to[i],0);
		}
		en[x]=tot;
	}
	inline int getlca(int x,int y) {
		while (F[x]!=F[y]) {
			if (dis[F[x]]>dis[F[y]]) x=fa[F[x]];
			else y=fa[F[y]];
		}
		if (dis[x]>dis[y]) return y;
		return x;
	}
	inline int getdis(int x,int y) {
		int lca=getlca(x,y);
		return a[x]+a[y]-2*a[lca];
	}
	inline int getdis_fa(int x,int y) {
		return a[y]-a[x];
	}
}T;
