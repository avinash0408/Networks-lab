#include<stdio.h>
#include<limits.h>

int printPredecessors(int n,int pred[n],int j,int source){
	if(j<0) return 0;
	if(j==source) { 
		printf("%d ",source);
		return source;
	}
	printPredecessors(n,pred,pred[j],source);
	printf("%d ",j);
}
int bellman_ford(int n, int source, int adj_mat[n][n]) {
	int dist[n], pred[n];
	int i, j, k;

	for(i = 0; i < n; i++) {
		dist[i] = INT_MAX / 2;
		pred[i] = INT_MAX;
	}

	dist[source] = 0;
	pred[source] = source;
	

	for(i = 0; i < n; i++)
		for(j = 0; j < n; j++)
			for(k = 0; k < n; k++)
				if(dist[j] > dist[k] + adj_mat[j][k]) {
					dist[j] = dist[k] + adj_mat[j][k];
					pred[j] = k;
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

int main() {
	int n; 			//Number of node/routers (vertices)
	int m;			//Number of links (edges)

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
		bellman_ford(n, i, adj_mat);
}
