#include <cstdio>
#include <map>
using namespace std;
const int INF = 1 << 29, NN = 20048;

map < int, int > m;
struct edge { int to, w, next; } e[NN];
int val[NN][2], first[NN], tree[NN], rval[NN];
int cnt, ecnt;

void add_edge(int from, int to, int cost){
	edge &E = e[ecnt];
	E.to = to;
	E.w = cost;
	E.next = first[from];
	first[from] = ecnt++;
}

int get_mapping(int x){
	int &z = m[x];
	if(!z) z = ++cnt;
	return z;
}

void dfs(int u, int value, bool d, int tn){
	tree[u] = tn;
	val[u][d] = value;
	for(int x = first[u]; x != -1; x = e[x].next){
		int v = e[x].to, z;
		if(v == u) continue;
		if(val[v][!d] == INF){
			if(d) z = value - e[x].w;
			else z = value + e[x].w;
			dfs(v, z, !d, tn);
		}
	}
}

void dfsv(int u){
	for(int at = first[u]; at != -1; at = e[at].next){
		int v = e[at].to;
		if(rval[v] == INF){
			rval[v] = e[at].w - rval[u];
			dfsv(v);
		}
	}
}

int query(int a, int b){
	int r = INF;
	if(a == b && rval[a] != INF) return rval[a];
	if(val[a][0] != INF && val[b][1] != INF)
		r = val[b][1] - val[a][0];
	if(val[a][1] != INF && val[b][0] != INF)
		r = val[a][1] - val[b][0];
	return r >>= (r != INF && a == b);
}

int main(void){
	val[0][0] = val[1][0] = INF;
	rval[0] = INF;
	for(int n, z; scanf("%d %d", &n, &z) == 2 && n; ){
		cnt = ecnt = 0;
		m.clear();
		for(int i = 1; i <= n << 1; ++i){
			val[i][0] = val[i][1] = INF;
			first[i] = -1;
			tree[i] = i;
			rval[i] = INF;
		}
		for(int a, b, c, i = 0; i < n; ++i){
			scanf("%d %d %d", &a, &b, &c);
			a = get_mapping(a);
			b = get_mapping(b);
			if(a == b) rval[a] = c;
			add_edge(a, b, c);
			add_edge(b, a, c);
		}
		for(int i = 1; i <= cnt; ++i) if(rval[i] != INF) dfsv(i);
		for(int i = 1; i <= cnt; ++i)
			if(rval[i] == INF && val[i][0] == INF && val[i][1] == INF)
				dfs(i, 0, 0, i);
		for(int a, b, d, i = 0; i < z; ++i){
			scanf("%d %d", &a, &b);
			a = m[a]; b = m[b]; d = INF;
			if(tree[a] == tree[b]) d = query(a, b);
			else if((a = query(a, a)) != INF && (b = query(b, b)) != INF) d = a + b;
			if(d != INF) printf("%d\n", d);
			else puts("*");
		}
		puts("-");
	}
	return 0;
}

