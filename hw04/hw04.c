// EE3980 HW04 Network Connectivity Problem
// 105061110, 周柏宇
// 2020/04/04

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int V;	// number of vertices
int E;	// number of edges
int *G;	// set of edges
int N = 100;	// number of repetitions
int NS;	// number of disjoint connected sets

void readGraph(void);	// read a network from stdin
void Connect1(void);	// find disjoint connected sets for a network, method 1
void Connect2(void);	// find disjoint connected sets for a network, method 2
void Connect3(void);	// find disjoint connected sets for a network, method 3
double GetTime(void);	// get local time in seconds

int main(void)
{
	int i;				// array index
	double t0, t1, t2, t3;	// timestamp
	int Ns1, Ns2, Ns3;	// number of disjoint connected sets found

	readGraph();	// read a network from stdin
	t0 = GetTime();	// record time

	for (i = 0; i < N; i++) Connect1();
	t1 = GetTime(); Ns1 = NS;	// record time and number of sets found

	for (i = 0; i < N; i++) Connect2();
	t2 = GetTime(); Ns2 = NS;	// record time and number of sets found

	for (i = 0; i < N; i++) Connect3();
	t3 = GetTime(); Ns3 = NS;	// record time and number of sets found

	printf("Connect1:\n  ");	// print out the algorithm name
	printf("CPU time = %.5e seconds", (t1 - t0) / N); // print out avg CPU time
	printf(", NS = %d\n", Ns1);	// print out the number of sets found
	printf("Connect2:\n  ");	// print out the algorithm name
	printf("CPU time = %.5e seconds", (t2 - t1) / N); // print out avg CPU time
	printf(", NS = %d\n", Ns2);	// print out the number of sets found
	printf("Connect3:\n  ");	// print out the algorithm name
	printf("CPU time = %.5e seconds", (t3 - t2) / N); // print out avg CPU time
	printf(", NS = %d\n", Ns3);	// print out the number of sets found
	free(G);	// free memory

	return 0;
}

void readGraph(void)	// read a network from stdin
{
	int i;			// loop index
	int vi, vj;		// vertices

	scanf("%d %d", &V, &E);	// input number of vertices and edges
	G = (int *)malloc(sizeof(int) * 2 * E);	// allocate memory for edges
	for (i = 0; i < E; i++) {
		scanf("%d %d", &vi, &vj);	// input the connected vertices
		G[2 * i] = vi - 1;		// store the vertices to array
		G[2 * i + 1] = vj - 1;
	}
}

double GetTime(void)						// get local time in seconds
{
	struct timeval tv;						// variable to store time

	gettimeofday(&tv, NULL);				// get local time

	return tv.tv_sec + 1e-6 * tv.tv_usec;	// return local time in seconds
}

void Connect1(void)
{
	int i;		// loop index
	int vi, vj;	// vertices
	int *S;		// array for disjoint sets
	int *R;		// final result for disjoint sets

	S = (int *)malloc(sizeof(int) * V);	// allocate memory for array
	R = (int *)malloc(sizeof(int) * V);	// allocate memory for array
	NS = V;		// initiate the number of disjoint sets to be V
	for (i = 0; i < V; i++) S[i] = -1;	// initiate every vertex to be root
	for (i = 0; i < E; i++) {	// enumerate every edge
		vi = G[2 * i];			// vertices connected by the edge
		vj = G[2 * i + 1];
		while (S[vi] >= 0) vi = S[vi];	// find the root for vi
		while (S[vj] >= 0) vj = S[vj];	// find the root for vj
		if (vi != vj) {	// if the roots are different
			NS--;		// decrease the number of disjoint sets found by 1
			S[vj] = vi;	// merge the sets
		}
	}
	for (i = 0; i < V; i++) {	// enumerate every vertex
		vi = i;	// initiate the root as i
		while (S[vi] >= 0) vi = S[vi];	// find the root for i
		R[i] = vi;	// store the root for vertex i in array R
	}
	free(S);	// free memory
	free(R);	// free memory
}

void Connect2(void)
{
	int i;		// loop index
	int vi, vj;	// vertices
	int *S;		// array for disjoint sets
	int *R;		// final result for disjoint sets
	int temp;	// number of elements

	S = (int *)malloc(sizeof(int) * V);	// allocate memory for array
	R = (int *)malloc(sizeof(int) * V);	// allocate memory for array
	NS = V;		// initiate the number of disjoint sets to be V
	for (i = 0; i < V; i++) S[i] = -1;	// initiate every vertex to be root
	for (i = 0; i < E; i++) {	// enumerate every edge
		vi = G[2 * i];			// vertices connected by the edge
		vj = G[2 * i + 1];
		while (S[vi] >= 0) vi = S[vi];	// find the root for vi
		while (S[vj] >= 0) vj = S[vj];	// find the root for vj
		if (vi != vj) {	// if the roots are different
			NS--;		// decrease the number of disjoint sets found by 1
			temp = S[vi] + S[vj];	// (number of elements of two trees) x -1
			if (S[vi] > S[vj]) {	// vi has fewer elements
				S[vi] = vj;			// connect vi to vj
				S[vj] = temp;		// update the number of elements
			}
			else {					// vj has fewer elements
				S[vj] = vi;			// connect vj to vi
				S[vi] = temp;		// update the number of elements
			}
		}
	}
	for (i = 0; i < V; i++) {	// enumerate every vertex
		vi = i;	// initiate the root as i
		while (S[vi] >= 0) vi = S[vi];	// find the root for i
		R[i] = vi;	// store the root for vertex i in array R
	}
	free(S);	// free memory
	free(R);	// free memory
}

void Connect3(void)
{
	int i;		// loop index
	int vi, vj;	// vertices
	int ri, rj;	// roots
	int s;		// temporary variable
	int *S;		// array for disjoint sets
	int *R;		// final result for disjoint sets
	int temp;	// number of elements

	S = (int *)malloc(sizeof(int) * V);	// allocate memory for array
	R = (int *)malloc(sizeof(int) * V);	// allocate memory for array
	NS = V;		// initiate the number of disjoint sets to be V
	for (i = 0; i < V; i++) S[i] = -1;	// initiate every vertex to be root
	for (i = 0; i < E; i++) {	// enumerate every edge
		vi = G[2 * i];			// vertices connected by the edge
		vj = G[2 * i + 1];
		ri = vi;				// initiate root for vi to be vi
		rj = vj;				// initiate root for vj to be vj
		while (S[ri] >= 0) ri = S[ri];	// find the root for vi
		while (vi != ri) {		// update the root for vi to be ri
			s = S[vi];
			S[vi] = ri;
			vi = s;
		}
		while (S[rj] >= 0) rj = S[rj];	// find the root for vj
		while (vj != rj) {		// update the root for vj to be rj
			s = S[vj];
			S[vj] = rj;
			vj = s;
		}
		if (ri != rj) {	// if the roots are different
			NS--;		// decrease the number of disjoint sets found by 1
			temp = S[ri] + S[rj];	// (number of elements of two trees) x -1
			if (S[ri] > S[rj]) {	// ri has fewer elements
				S[ri] = rj;			// connect ri to rj
				S[rj] = temp;		// update the number of elements
			}
			else {					// rj has fewer elements
				S[rj] = ri;			// connect rj to ri
				S[ri] = temp;		// update the number of elements
			}
		}
	}
	for (i = 0; i < V; i++) {	// enumerate every vertex
		vi = i;	// initiate the root as i
		while (S[vi] >= 0) vi = S[vi];	// find the root for i
		R[i] = vi;	// store the root for vertex i in array R
	}
	free(S);	// free memory
	free(R);	// free memory
}
