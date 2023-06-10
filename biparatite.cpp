#include <iostream>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <fstream>
#include <queue>

using namespace std;

#define V 50
#define INPUT  "bpGraph2.txt"			//Input File
#define OUTPUT "BIPARATITE.txt"	//Output File

//Functions


//Returns true if path exists from s to t in residual graph. Stores path in path[]
bool FindPath(int residual[V][V], int s, int t, int path[])//Used for path finding
{
    bool Visited[V];        //To mark vertices Visited using BFS
    memset(Visited, 0, sizeof(Visited));
 
    queue <int> q;          //Queue for maintaining all nodes to be Visited
    q.push(s);              //Add s to queue
    Visited[s] = true;      //Mark s as Visited
    path[s] = -1;
 
    //Loop for BFS
    while ( !q.empty() )
    {
        int u = q.front();
        q.pop();
 
        for (int v = 0; v < V; v++)
        {
            if (Visited[v] == false && residual[u][v] > 0)
            {
                q.push(v);
                path[v] = u;
                Visited[v] = true;
            }
        }
    }
    //If t has been Visited then path exists. Return true
    return (Visited[t] == true);
}



   
//DFS to find all vertices reachable from s
void Mincut(int residual[V][V], int s, bool Visit[]) //Used for min cut
{
    Visit[s] = true;
    for (int i = 0; i < V; i++)
       if (residual[s][i] && !Visit[i])
           Mincut(residual, i, Visit);
}

//Print edged from A (vertices reacchable from s) to B (vertices not reachable from s)
void Mincutout(int graph[V][V], bool Visit[], int s, int t)//Print edges of the min cut
{
     int mincut_size = 0, mincut_cap = 0;
     cout << "\n\n\t \t \t MINCUT\n";
     cout << "\t \t \t ______\n \n"; 
     cout << "Edges from S (set of vertices reachable from source) to T (set of vertices not reachable from source) are:" << endl; 
     for (int i = 0; i < V; i++)
      for (int j = 0; j < V; j++)
         if (Visit[i] && !Visit[j] && graph[i][j])
	 {    
	      mincut_size++; mincut_cap += graph[i][j];
              cout << i << " - " << j << endl;
	 }
    //  cout << "Min-cut capacity = " << mincut_cap << endl;
     cout << "Min-cut:" << endl << "Set S = {" << s;
     for (int i = 0; i < V; i++)
    	if ( Visit[i] && i != s)
          if(i<=t)
		    cout << ", " << i;
     
     cout << "}" << endl << "Set T = {" << t;
     for (int i = 0; i < V; i++)
    	if ( !Visit[i] && i != t)
         if(i<=t)
		    cout << ", " << i;
     cout << "}" << endl;
}


void Deltaupdate(int deltares[V][V], int residual[V][V], int delta)//update for delta scaling implementation
{
     for (int i = 0; i < V; i++)
      for (int j = 0; j < V; j++)
	if( residual[i][j] >= delta)
	   deltares[i][j] = residual[i][j];
	else	
	   deltares[i][j] = 0;
}

                       
//Returns max flow from s to t in given graph
void FordFulkerson(int graph[V][V], int s, int t)//Ford Fulkerson implementation
{
    int u, v, it = 0, mC = INT_MIN, delta = 1;
    
    //Finding maximum capacity coming out of source
    for( int i = 0; i < V; i++ )
    	mC = max(mC, graph[s][i]);

    //Getting the value of Delta
    while( delta < mC )
    	delta <<= 1;
    if( delta > mC )
	delta >>= 1;
    
    // cout << "\t \t \t ALGORITHM\n";
    // cout << "\t \t \t _________\n \n"; 
    
    int residual[V][V]; // Residual graph where residual[i][j] indicates residual capacity of edge from i to j
    int deltares[V][V];

    //Fill residual graph with intial capacities in directed graph
    for (u = 0; u < V; u++)
        for (v = 0; v < V; v++)
             residual[u][v] = graph[u][v];

    //Fill delta residual graph intiall as residual graph
    for (u = 0; u < V; u++)
        for (v = 0; v < V; v++)
             deltares[u][v] = residual[u][v];
 
    int path[V];  //Array used to fill augment path in residual graph
 
    int maxflow = 0;  //Intially no flow
    
    Deltaupdate(deltares, residual, delta);
    
    //Augment flow as long as there is a path from s to t
    while(delta >= 1)
    {
	// cout << endl << "With Delta = " << delta << ":";
    	while (FindPath(deltares, s, t, path))
    	{
		// cout << endl << "Iteration #" << ++it << ":" << endl << endl;
            ++it;
        	int path_flow = INT_MAX;
		// cout << "Path = ";
        	for (v = t; v != s; v = path[v])
        	{
            		u = path[v];
            		//Bottleneck for augment path
            		path_flow = min(path_flow, residual[u][v]);
	    		// if(u != s)
	    		// 	cout << v << " <-(" << residual[u][v] << ")- ";
	    		// else if( v != s)
				// cout << s << endl;
        	}
 
        	//Update residual capacities
        	for (v = t; v != s; v = path[v])
        	{
	    		u = path[v];
            		residual[u][v] -= path_flow;
            		residual[v][u] += path_flow;
        	}
	 
        	// Adding bottleneck to overall flow
        	maxflow += path_flow;
		// cout << "Bottleneck of path, b = " << path_flow << endl;
		// cout << "New flow, f = " << maxflow << endl;
		Deltaupdate(deltares, residual, delta);
	}
        delta = delta/2;
	Deltaupdate(deltares, residual, delta);
    }
    
    //Find vertices reachable from s after flow is maximum
    bool Visit[V];
    memset(Visit, false, sizeof(Visit));
    Mincut(residual, s, Visit);
    
    Mincutout(graph, Visit, s, t);

    cout << "\n\n\t\t \t MAX MATCH\n";
    cout << "\t \t \t ___ ____\n \n";
    cout << "Max-biparatite matching = " << maxflow << endl;
    // cout << "Final Residual graph: (Adjacency Matrix representation)" << endl;
    // for (u = 0; u <=t; u++)
    // {
	// cout << endl;
    //     for (v = 0; v <=t; v++)
    //          cout << residual[u][v] << "\t";
    // }
    //****************************************************************************************************************************************************
}



			    
int main()
{
    freopen(OUTPUT, "w", stdout);
     std::ifstream filein(INPUT, std::ifstream::in);
    int M,N;//number of rows and columns
    //  eg:bool bpGraph[M][N] = {{0, 1, 1, 0, 0, 0},
    //                       {1, 0, 0, 1, 0, 0},
    //                       {0, 0, 1, 0, 0, 0},
    //                       {0, 0, 1, 1, 0, 0},
    //                       {0, 0, 0, 0, 0, 0},
    //                       {0, 0, 0, 0, 0, 1}}; if (m,n)=1 means person m is intrested in job n
    if ( filein.is_open() )
    {
	filein >> M; filein >> N;
    }
    bool bpGraph[M][N];
    for(int row = 0; row < M; row++) {  // stop loops if nothing to read
    for(int column = 0; column < N; column++){
            filein>> bpGraph[row][column];
            if ( ! filein ) {
            cout << "Error reading file for element " << row << "," << column << endl; 
            return 1; 
            }
        }
    }

    int graph[V][V];
    for(int i = 0; i < V; i++)
	for( int j = 0; j < V; j++)
		graph[i][j] = 0;

    int s, t, v1, v2, c;
    s=0,t=M+N+1;
    for(int i=0;i<M;i++){
        for(int j=0;j<N;j++){
            if(bpGraph[i][j]==1){
                graph[i+1][j+M+1]=1;
            }
        }
    }
    //connecting source to all people
    for(int i=1;i<=M;i++){
        graph[0][i]=1;
    }
    //connecting all jobs to sink t which is the M+N+2'th vertex
    for(int i=M+1;i<=M+N;i++){
        graph[i][t]=1;
    }
    // to print adjecency matrix of biparatite graph
    // cout << "real graph  "<< endl;
    // for (int u = 0; u <=t; u++)
    // {
	// cout << endl;
    //     for (int v = 0; v <=t; v++)
    //          cout << graph[u][v] << "\t";
    // }
    

    
    //struct timeval tstart, tend;
    // gettimeofday (&tstart, NULL);

    FordFulkerson(graph, s, t); 
    

    // gettimeofday (&tend, NULL);
    // cout << "\n\n\t \t \t TIME TAKEN\n";
    // cout << "\t \t \t ____ _____\n \n"; 
    // std::cout << "Max flow with Min Cut but with Delta Scaling took " << ((tend.tv_sec - tstart.tv_sec)*1000000L+tend.tv_usec)-tstart.tv_usec << " microseconds\n";       
    return 0;
}

