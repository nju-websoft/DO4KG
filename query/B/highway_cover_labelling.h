#ifndef HGHWAY_LABELING_H_
#define HGHWAY_LABELING_H_


#define inf 1000000000
#include <stdint.h>
#include <iostream>
#include <thread>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <utility>

#include "two_layer_queue.h"

//
// NOTE: Currently only unweighted and undirected graphs are supported.
//

struct tmp_pa {
	int x,val;
	friend bool operator ==(const tmp_pa &a,const tmp_pa &b) {
        return a.x==b.x;
    }
};

struct dij {
	int x,val,flag;
	friend bool operator <(const dij &a,const dij &b) {
        if (a.val!=b.val) return a.val>b.val;
        if (a.flag!=b.flag) return a.flag<b.flag;
        return a.x>b.x;
    }
};

class HighwayLabelling {
 public:
  // Constructs an index from a graph, given as a list of edges.
  HighwayLabelling(const char *filename, int k);
  HighwayLabelling();
  ~HighwayLabelling();

  void ConstructHighwayLabelling(int topk[]);

  void SelectLandmarks_HD(int topk[]);

  long LabellingSize();
  int min(int a, int b);

  // Returns distance vetween vertices v and w if they are connected.
  // Otherwise, returns inf.
  int QueryDistanceUB_naive(int s, int t);
  int QueryDistanceUB_opt(int s, int t);
  int QueryDistance(int s, int t);

  bool IsLandmark(int arr[], int left, int right, int value);
  void RemoveLandmarks(int landmarks[]);
  void printgraph(int topk[]);
  // Loads the highway labelling.
  void LoadIndex(const char *filename);

  // Stores the highway labelling.
  void StoreIndex(const char *filename);

 private:
  int V;  // total number of vertices
  long E; // total number of edges
  int K; // total number of landmarks

  int **vertices;
  int **distances;
  int **highway;
  int *C;

  std::unordered_map<int, std::vector<tmp_pa> > adj;
};

bool cmp(tmp_pa x,tmp_pa y) {
	return x.x<y.x;
}
HighwayLabelling::HighwayLabelling() { }

HighwayLabelling::~HighwayLabelling() {

  for(int i = 0; i < V; i++) {
    delete [] distances[i];
  }
  delete [] distances;

  if(vertices != NULL) {
    for(int i = 0; i < V; i++) {
      delete [] vertices[i];
    }
    delete [] vertices;
  }
  delete [] C;

  for(int i = 0; i < K; i++)
    delete [] highway[i];
  delete [] highway;

}

HighwayLabelling::HighwayLabelling(const char *filename, int k) {
  V = 0; E = 0; K = k;

  std::ifstream ifs(filename);
  if (ifs.is_open()) {

    int v, w,val; std::string query;
    //std::unordered_map<int, int> vertex2id;
    getline(ifs, query);
    std::istringstream iss(query);
    iss>>V;
	int t=0;
    while (getline(ifs, query)){
      std::istringstream iss(query);
      iss >> v >> w>>val;
      
      /*if (vertex2id.count(v) == 0) vertex2id[v] = V++;
      if (vertex2id.count(w) == 0) vertex2id[w] = V++;
      v = vertex2id[v];
      w = vertex2id[w];*/
      t++;
      if (v != w) {
        adj[v].push_back(tmp_pa{w,val});
        adj[w].push_back(tmp_pa{v,val});
      }
    }
    printf("t:%d\n",t);
//    exit(0);
    ifs.close();

    for (int v = 0; v < V; v++) {
      std::sort(adj[v].begin(), adj[v].end(),cmp);
      //adj[v].erase(std::unique(adj[v].begin(), adj[v].end()), adj[v].end());
    }

    for(int i = 0; i < V; i++)
      E += adj[i].size();
    std::cout << "V : " << V << " E : " << E << std::endl << std::endl;

  } else
      std::cout << "Unable to open file" << std::endl;
}

void HighwayLabelling::RemoveLandmarks(int landmarks[]) {

  for(int i = 0; i < K; i++) {
    for (std::vector<tmp_pa>::iterator it=adj[landmarks[i]].begin(); it!=adj[landmarks[i]].end(); ++it)
      adj[(*it).x].erase(std::find(adj[(*it).x].begin(), adj[(*it).x].end(), tmp_pa{landmarks[i],0}));
    adj.erase(landmarks[i]);
  }

}

long HighwayLabelling::LabellingSize() {
  long size = 0;
  for (int i = 0; i < V; i++) {
    for (int j = 0; j < K; j++) {
      if(distances[i][j] != inf)
        size++;
    }
  }

  return size;
}

bool HighwayLabelling::IsLandmark(int arr[], int left, int right, int value) {

  while (left <= right) {
    int middle = (left + right) / 2;
    if (arr[middle] == value)
      return true;
    else if (arr[middle] > value)
      right = middle - 1;
    else
      left = middle + 1;
  }
  return false;
}

void HighwayLabelling::ConstructHighwayLabelling(int topk[]) {

  // Initialization
  distances = new int*[V];
  for(int i = 0; i < V; i++) {
    distances[i] = new int[K];
    for(int j = 0; j < K; j++)
      distances[i][j] = inf;
  }

  highway = new int*[K];
  for(int i = 0; i < K; i++)
    highway[i] = new int[K];

  // Start computing Highway Labelling (HL)
  for (int i = 0; i < K; i++) {
    int *P = new int[V];
    int *vis = new int[V];
    for(int j = 0; j < V; j++)
      P[j] = inf,vis[j]=0;

	//
	std::priority_queue<dij> que;
	que.push(dij{topk[i],0,0});
	P[topk[i]]=0;
	distances[topk[i]][i]=0;
	while (!que.empty()) {
		dij u=que.top();
		que.pop();
		if (vis[u.x]) continue;
		if (u.flag==0)
		distances[u.x][i]=P[u.x];
		vis[u.x]=1;
		for (tmp_pa w:adj[u.x]) {
			if (IsLandmark(topk, 0, K, w.x)||u.flag==1) {
				if (P[w.x]>=u.val+w.val) {
					P[w.x]=u.val+w.val;		
					que.push(dij{w.x,P[w.x],1});
				}	
			}
			else {
				if (P[w.x]>u.val+w.val) {
					P[w.x]=u.val+w.val;		
					que.push(dij{w.x,P[w.x],0});
					
				}
			}
			
		}
	}
	//
    /*std::queue<int> que[2];

    que[0].push(topk[i]); que[0].push(-1);
    distances[topk[i]][i] = 0; P[topk[i]] = 0; int use = 0;
    while (!que[0].empty()) {
      int u = que[use].front();
      que[use].pop();

      if(u == -1) {
        use = 1 - use;
        que[use].push(-1);
        continue;
      }

      for (tmp_pa w : adj[u]) {
        if (P[w.x] == inf) {
          P[w.x] = P[u] + w.val;
          if(use == 1 || IsLandmark(topk, 0, K, w.x))
            que[1].push(w.x);
          else {
            que[0].push(w.x);
            distances[w.x][i] = P[w.x];
          }
        }
      }
    }*/

    for(int j = 0; j < K; j++) {
      if(P[topk[j]] != inf) {
        highway[i][j] = P[topk[j]];
        highway[j][i] = P[topk[j]];
      }
    }

    delete [] P;
    delete [] vis;
  }

  std::cout << "Labelling Size : " << 1.*LabellingSize()/V << " "<<V<<std::endl;
}

void HighwayLabelling::SelectLandmarks_HD(int topk[]) {
  std::vector<std::pair<int, int> > deg(V );
  for (int v = 0; v < V; v++)
    deg[v] = std::make_pair(adj[v].size(), v);

  std::sort(deg.rbegin(), deg.rend());

  for (int v = 0; v < K; v++)
    topk[v] = deg[v].second;
}

int HighwayLabelling::min(int a, int b) {
  return (a < b) ? a : b;
}

int HighwayLabelling::QueryDistanceUB_naive(int s, int t) {	
  int m = inf; int i, j;
  for(i = 0; i < C[s]; i++) {
    for (j = 0; j < C[t]; j++) {
      m = min(m, distances[s][i] + highway[vertices[s][i]][vertices[t][j]] + distances[t][j]);
      }
  }
  return m;
}



int HighwayLabelling::QueryDistanceUB_opt(int s, int t) {
  /*for (int i=0;i<C[s];i++) printf("%d %d\n",vertices[s][i],distances[s][i]);
  puts("");
  for (int i=0;i<C[t];i++) printf("%d %d\n",vertices[t][i],distances[t][i]);*/
  int m = inf, uni1[C[s]], uni2[C[t]]; int i = 0, j = 0, i1 = 0, j1 = 0;
        for (int _i=0;_i<C[s];_i++) uni1[_i]=0;
        for (int _i=0;_i<C[t];_i++) uni2[_i]=0;

  while (i < C[s] && j < C[t]) {
    if (vertices[s][i] < vertices[t][j]) {
      uni1[i1] = i; i++; i1++;
    } else if (vertices[t][j] < vertices[s][i]) {
      uni2[j1] = j; j++; j1++;
    } else {
      m = min(m, distances[s][i] + distances[t][j]);
      i++; j++;
    }
  }

  while (i < C[s]) {
    uni1[i1] = i; i++; i1++;
  }

  while (j < C[t]) {
    uni2[j1] = j; j++; j1++;
  }
  i = 0; j = 0;
  while (i < i1) {
    while (j < j1) {
      m = min(m, distances[s][uni1[i]] + highway[vertices[s][uni1[i]]][vertices[t][uni2[j]]] + distances[t][uni2[j]]);
      j++;
    }
    i++;
    j=0;
  }

  return m;
}

int HighwayLabelling::QueryDistance(int s, int t) {
  if (s == t) {   return 0;  }
//
  std::priority_queue<dij>que;
  std::vector<int> qdist[2];
  qdist[0].resize(V, inf);
  qdist[1].resize(V, inf);
  for (int dir = 0; dir < 2; dir++){
    int v = dir == 0 ? s : t;
    que.push(dij{v,0,dir});
    qdist[dir][v] = 0;
  }
  int res = QueryDistanceUB_opt(s, t);
  //int res = QueryDistanceUB_naive(s,t);
  //if (res==inf) return inf;  
  while (!que.empty()) {
  	dij v=que.top();
  	que.pop();
  	if (qdist[v.flag][v.x]!=v.val) continue;
  	qdist[v.flag][v.x]=v.val;
  	res=min(res,qdist[v.flag][v.x]+qdist[v.flag^1][v.x]);
  	if (v.flag==0&&((qdist[v.flag][v.x]<<1)>=res)) continue;
  	if (v.val>=res) continue;
  	for (tmp_pa w:adj[v.x]) {
  		if (qdist[v.flag][w.x]<=qdist[v.flag][v.x]+w.val) continue;
  		qdist[v.flag][w.x]=qdist[v.flag][v.x]+w.val;
  		res=min(res,qdist[v.flag][w.x]+qdist[v.flag^1][w.x]);
  		if (v.flag==1&&(qdist[v.flag][w.x]<<1)>res) continue;
  		que.push(dij{w.x,qdist[v.flag][w.x],v.flag});
  	}
  }
  return res;
//
  /*std::vector<TwoLayerQueue> qque;
  std::vector<int> qdist[2];

  qdist[0].resize(V, inf);
  qdist[1].resize(V, inf);

  qque.push_back(TwoLayerQueue(V));
  qque.push_back(TwoLayerQueue(V));

  for (int dir = 0; dir < 2; dir++){
    int v = dir == 0 ? s : t;
    qque[dir].clear();
    qque[dir].push(v);
    qque[dir].next();
    qdist[dir][v] = 0;
  }

  int dist_upper = QueryDistanceUB_opt(s, t);
  int res = dist_upper, dis[2] = {0, 0};
  while (!qque[0].empty() && !qque[1].empty()) {
    int use = 0;
    use = (qque[0].size() <= qque[1].size()) ? 0 : 1;
    dis[use]++;

    if (dis[0] + dis[1] == dist_upper) {
      res = dis[0] + dis[1];
      goto LOOP_END;
    }

    while (!qque[use].empty()) {

      int v = qque[use].front();
      qque[use].pop();

      for (tmp_pa w : adj[v]) {

        int &src_d = qdist[use][w.x];
        int &dst_d = qdist[1 - use][w.x];
        if (src_d != inf) continue;
        if (dst_d != inf) {
          res = qdist[use][v] + 1 + dst_d;
          goto LOOP_END;
        } else {
          qque[use].push(w.x);
          qdist[use][w.x] = qdist[use][v] + 1;
        }
      }
    }
    qque[use].next();
  }
  LOOP_END:

  for (int dir = 0; dir < 2; dir++) {
    for (int v : qque[dir]) {
      qdist[dir][v] = inf;
    }
    qque[dir].clear();
  }

  return min(res, dist_upper);*/
}

void HighwayLabelling::LoadIndex(const char *filename) {
  std::ifstream ifs(std::string(filename) + std::string("_index"));
  
  C = new int[V];
  vertices = new int*[V];
  distances = new int*[V];

  long size = 0;
  for(int i = 0; i < V; i++) {
    ifs.read((char*)&C[i], sizeof(C[i]));
    size += C[i];
    vertices[i] = new int[C[i]];
    distances[i] = new int[C[i]];
    for(int j = 0; j < C[i]; j++) {
      ifs.read((char*)&vertices[i][j], sizeof(vertices[i][j]));
      ifs.read((char*)&distances[i][j], sizeof(distances[i][j]));
    }
  }
  std::cout << "Labelling Size : " << size << std::endl;
  ifs.close();

  ifs.open(std::string(filename) + std::string("_highway"));

  highway = new int*[K];
  for(int i = 0; i < K; i++) {
    highway[i] = new int[K];
    for(int j = 0; j < K; j++)
      ifs.read((char*)&highway[i][j], sizeof(highway[i][j]));
  }
  ifs.close();
}

void HighwayLabelling::StoreIndex(const char *filename) {
  std::ofstream ofs(std::string(filename) + std::string("_index"));

  for(int i = 0; i < V; i++) {
    int C = 0;
    for(int j = 0; j < K; j++) {
      if(distances[i][j] != inf)
        C++;
    }

    ofs.write((char*)&C, sizeof(C));
    for(int j = 0; j < K; j++) {
      if(distances[i][j] != inf) {
        ofs.write((char*)&j, sizeof(j));
        ofs.write((char*)&distances[i][j], sizeof(distances[i][j]));
      }
    }
  }
  ofs.close();

  ofs.open(std::string(filename) + std::string("_highway"));
  for(int i = 0; i < K; i++) {
    for(int j = 0; j < K; j++) {
      ofs.write((char*)&highway[i][j], sizeof(highway[i][j]));
    }
  }
  ofs.close();
}

void HighwayLabelling::printgraph(int topk[]) {
	freopen("graph","w",stdout);
	RemoveLandmarks(topk);
	for (int i=0;i<V;i++) {
		if (adj.find(i)==adj.end()) {
			puts("0");
			continue;
		}
	  	printf("%d\n",adj[i].size());
	  	for (auto p:adj[i]) printf("%d %d\n",p.x,p.val);
	}
}

#endif  // HGHWAY_LABELING_H_
