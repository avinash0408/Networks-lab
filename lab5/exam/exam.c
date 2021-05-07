#include<stdio.h>
#include<limits.h>

int min(int *dist, int *boolean, int n) {
	int min = INT_MAX, min_dist = INT_MAX;

	for(int i = 0; i < n; i++)
		if(boolean[i] && min_dist > dist[i]) {
			min = i;
			min_dist = dist[i];
		} 

	return min;
}
int dijkstra(int n, int source, int adj_mat[n][n]) {
	int dist[n], boolean[n], pred[n];
	int i, j, k;

	for(i = 0; i < n; i++) {
        dist[i] = INT_MAX / 2;
		pred[i] = INT_MAX;
        boolean[i]=1;
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
    if(dist[n-1]!=INT_MAX >>1)
    printf("%d\n",dist[n-1]);
    else
    printf("-1\n");
	
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
    int rp;
    scanf("%d",&rp);
    for(int i=0;i<n;i++){
            adj_mat[i][rp] =adj_mat[rp][i]=INT_MAX >>1;
        }
    

		dijkstra(n, 0, adj_mat);
}


