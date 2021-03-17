# project4
Starter repo for Project 4

Algorithm as follows:
- Start and Terminal nodes are added to given adjacency matrix  as:
	- Start node has distance of 0 to neighbours [1, 2, .., n]
	- Terminal node has distance of 0 to neighbours [n+1, n+2, ..., 2n]
- An augmenting path(flow) is found by SPFA algorithm
- Residual graph is constructed with reversing the edges along the path.
- Loop terminates until a negative cycle is found or a path is not found