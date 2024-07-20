#include <bits/stdc++.h>
#include "BFS.c++"

using namespace std;

/**
 * Calculate the maximum flow along a path
 */
int pathFlow(linkedList *thisPath, int sink) {
	//go to the first node after the s, path starts at s and has no cost/capacity
	node *currentNode = thisPath->head->next;
	node *nextNode = NULL;
    int flow = INT_MAX;
    while (currentNode) {
        if (currentNode->capacity < flow) {
        	flow = currentNode->capacity;
        }
        nextNode = currentNode->next;
        currentNode = nextNode;
    }
    return flow;
}

/**
 * Determine residual graph
 */
void residual(graph *thisGraph, linkedList *thisPath, int flow) {
    node *currentPathNode = thisPath->head;
    node *nextPathNode = currentPathNode->next;
    node *currentNode = thisGraph->adjacencyList[currentPathNode->vertex].head;
    node *nextNode = NULL;
    
    // for each node in the path
    while (currentNode && (nextPathNode != NULL)) {
    	// find the sink node in the graph
    	if (currentNode->vertex == nextPathNode->vertex) {
    		//
    		// TODO: Fix the math here, 
    		//
    		currentNode->flow = flow;
    		currentNode->capacity = currentNode->capacity - flow;
        	// if(DEBUGG) cout << currentPathNode->vertex<< " -> " <<currentNode->vertex << " ( x - " << currentNode->flow<<" = " << currentNode->capacity<<endl;

        	// advance to the next item in the path
            if (nextPathNode->next) {
	            currentPathNode = nextPathNode;
	    		nextPathNode = currentPathNode->next;
	    		currentNode = thisGraph->adjacencyList[currentPathNode->vertex].head;
	    	} else {
	    		currentNode = NULL;
	    	}
    	
    	// advance to the next item in the linked list
    	} else {
    		if (currentNode->next) {
	    		nextNode = currentNode->next;
	    		currentNode = nextNode;
	    	}
    	}
    }
    if(DEBUGG) cout<<"res Done"<<endl;
}

/**
 * Print the residual graph
 * For debuging purposes
 */
void printResidualGraph(graph* currentGraph) {
    int v;
    for (v = 0; v < currentGraph->totalVertices; ++v) {
        node *currentNode = currentGraph->adjacencyList[v].head;
        
        cout<<"Adjacency list of vertex "<< v <<" head "<<endl;
        while (currentNode) {
            cout<<" -> "<< currentNode->vertex <<"("<< currentNode->capacity <<")" << endl;
            currentNode = currentNode->next;
        }
        cout<< "print Graph Done"<<endl;
    }
}

/**
 * Use Ford-Folkerson to calulate max-flow / min-cut
 */
int maxFlow(graph *thisGraph, int source, int sink) {
	int flow = 0;
	int maximumFlow = 0;
	
	if(DEBUGG) cout<<" BFS 0 ... "<<endl;
	// initial bfs graph traversal
	linkedList*path = BFS(thisGraph, source, sink);
	if(DEBUGG) cout<<"DONE "<<endl;

	// while there are still paths
	while (path != NULL) {
		// determine max flow for this path
		flow = pathFlow(path, sink);
		maximumFlow += flow;
		if(DEBUGG) cout<<"FLOW: "<< flow <<"/ MAXFLOW: " << maximumFlow<<endl;

		// determine the residual graph
		residual(thisGraph, path, flow);
		// if(DEBUGG) printResidualGraph(thisGraph);
		
		if(DEBUGG) cout<<" BFS n ... "<<endl;
		// find path in residual
		path = BFS(thisGraph, source, sink);
		if(DEBUGG) cout<<"DONE "<<endl;
	}

	return maximumFlow;
}


