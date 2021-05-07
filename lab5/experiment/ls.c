#include<stdio.h>
#include<limits.h>

int min();
int dijkstra();

int printPredecessors(int n,int pred[n],int j,int source){
	if(j<0) return 0;
	if(j==source) { 
		printf("%d ",source);
		return source;
	}
	printPredecessors(n,pred,pred[j],source);
	printf("%d ",j);
}
int main() {
	int n, m;
	scanf("%d%d", &n, &m);

	int adj_mat[n][n];
	int i, j, k, l;

	for(i = 0; i < n; i++)
		for(j = 0; j < n; j++)
			adj_mat[i][j] = INT_MAX >> 1;

	for(i = 0; i < m; i++) {
		scanf("%d%d%d", &j, &k, &l);
		adj_mat[j][k] = adj_mat[k][j] = l;
	}

	for(i = 0; i < n; i++)
		dijkstra(n, i, adj_mat);
}

int dijkstra(int n, int source, int adj_mat[n][n]) {
	int dist[n], boolean[n], pred[n];
	int i, j, k;

	for(i = 0; i < n; i++) {
		dist[i] = INT_MAX / 2;
		boolean[i] = 1;
		pred[i] = INT_MAX;
	}

	dist[source] = 0;
	pred[source] = source;

	for(i = 0; i < n; i++) {
		j = min(dist, boolean, n);
		boolean[j] = 0;

		for(k = 0; k < n; k++)
			if(boolean[k] && dist[k] > dist[j] + adj_mat[j][k]) {
				dist[k] = dist[j] + adj_mat[j][k];
				pred[k] = j;
			}
	}
	printf("Routing table of node: %d\n", source);	

	for(int i = 0; i < n; i++) {
		if(i==source)
		continue;
		printf("destination: %d - cost: %d, predecessor: %d, path :", i, dist[i], pred[i]);
		printPredecessors(n,pred,i,source);
		printf("\n");

	}
	printf("\n");


	
}

int min(int *dist, int *boolean, int n) {
	int min = INT_MAX, min_dist = INT_MAX;

	for(int i = 0; i < n; i++)
		if(boolean[i] && min_dist > dist[i]) {
			min = i;
			min_dist = dist[i];
		} 

	return min;
}