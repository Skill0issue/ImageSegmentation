#include <bits/stdc++.h>
#include "Data.cpp"

linkedList* BFS(graph* thisGraph,int start,int end){
    if(thisGraph == nullptr || thisGraph->adjacencyList[start].head == nullptr){ 
        cout<<"nullptr"<<endl;
        return nullptr;
    }
    if(DEBUGG) cout<<" " <<start<<" "<<end<<endl;
    Dataqueue* BFSqueue = newQueue();
    int sinks;
    int *visited = new int[thisGraph->totalVertices];
    for(int i=0;i< thisGraph->totalVertices;i++) visited[i] = 0;
    
    linkedList* bfsPath =nullptr;
    tree* bfsTree = nullptr,*currentBranch = nullptr,*currentChild = nullptr;
    node* startNode = thisGraph->adjacencyList[start].self;
    node* currentNode = nullptr;
    node* adjacentNode = nullptr;
    Datastack* traceStack = nullptr;
    if(DEBUGG) cout<< "start at: "<< startNode->vertex<<endl;
    bfsTree = newTree(nullptr); // init with nullptr
    bfsTree->data= startNode;

    enqueue(BFSqueue,bfsTree);
    visited[start] = true;
    sinks = 0;
    while(sinks != 1 && (currentBranch = dequeue(BFSqueue)) != nullptr){
        currentNode = currentBranch->data;
        // if(DEBUGG) cout<<"AT: " <<currentNode->vertex << endl;
        if(currentNode->vertex == end){
            sinks = 1;
            if(DEBUGG) cout<<"END."<<endl;
        }else if(currentNode->vertex >= thisGraph->totalVertices){
            //nothing
        }else{
            currentChild = nullptr;
            adjacentNode = thisGraph->adjacencyList[currentNode->vertex].head;

            while(adjacentNode != nullptr){
                // if(DEBUGG) cout<<"ADJ while instance: " << adjacentNode->vertex<<endl;
                if((visited[adjacentNode->vertex] == 0) && adjacentNode->capacity >0){
                    // if(DEBUGG) cout << "UNSEEN "<< adjacentNode->vertex<<"QUEUEING"<<endl;
                    if(currentChild == nullptr){
                        currentBranch->children = newTree(currentBranch);
                        currentChild = currentBranch->children;
                    }else{
                        currentChild->next = newTree(currentBranch);
						currentChild = currentChild->next;  
                    }
                    currentChild->data = adjacentNode;
                    enqueue(BFSqueue,currentChild);
                    if(adjacentNode->vertex < thisGraph->totalVertices) visited[adjacentNode->vertex] = 1;
                }
                adjacentNode = adjacentNode->next;
            }
        }
    }
    if(sinks == 1){
        //found our path in queue in reverse order use stack to correct the path in the tree using parent pointers
        Datastack* traceStack = newDataStack();
        push(traceStack,currentBranch->data);
        do{
            // if(DEBUGG) cout<< " TRACKING NODE: "<< currentBranch->data->vertex;
            currentBranch = currentBranch->parent;
            push(traceStack,currentBranch->data);
        }while(currentBranch != bfsTree);
        // if(DEBUGG) cout <<"STACK OUT"<<endl; 
        //stored in stack push to list
        bfsPath = newList();
        if(DEBUGG) cout << "LIST CREATED" <<endl;
        currentNode = queuePop(traceStack);
        // if(DEBUGG) cout<< "POPPED"<<endl;
        // if(DEBUGG) cout<< " ";
        bfsPath->head = newNode(currentNode->vertex,currentNode->capacity,0);
        // if(DEBUGG) cout<<"LIST TRACE NODE: "<<currentNode->vertex;
        adjacentNode = bfsPath->head;
        while(currentNode->vertex != end){
            currentNode = queuePop(traceStack);
			adjacentNode->next = newNode(currentNode->vertex, currentNode->capacity,0);
			// if(DEBUGG)cout<< "ADDING NODE: "<< currentNode->vertex<<"TO LIST WITH CAPACITY: "<<currentNode->capacity<<endl;
			adjacentNode = adjacentNode->next;
        }
    }
    //clean up memory allocated by BFS
	treeFree(bfsTree);
	while (dequeue(BFSqueue) != NULL); //clear any nodes still in queue
	free(BFSqueue);
	//stack will by definition be out of elements, so we just need to free the data for the structure
	if (traceStack != NULL) free(traceStack);
	free(visited);
    if(DEBUGG) cout<<"BFS DONE ..."<<endl;
	return bfsPath;
}