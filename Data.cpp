#ifndef DATA_CPP
#define DATA_CPP

#include <bits/stdc++.h>

#define MAX_SIZE 1024
#define DEBUGG 0

using namespace std;
#define MAX_SIZE 1024

struct node {
    int vertex;
    node* next;
    int capacity;
    int flow;
    int MaxCapacity;
    int Zvalue;
};

struct linkedList {
    node *self;
    node *head;
};

struct graph {
    int totalVertices;
    linkedList *adjacencyList; 

    int extentX;
	int extentY;
	int extentZ;
};

struct tree {
    node* data;
    tree* next;
    tree* children;
    tree* parent;
};

struct queueElement {
    tree* data;
    queueElement* next;
};

struct Dataqueue {
    queueElement *head;
    queueElement *tail;
};
struct stackItem {
	node * data;
	stackItem * next;
};
struct Datastack {
	stackItem* head;
};
struct isQueue {
	struct isItem *head;
	struct isItem *tail;
};

struct isItem {
	struct isItem * next;
	struct node * data;
};

node* newNode(int vertex, int capacity, int flow);
linkedList* newList();
graph* newGraph(int Vertices);
tree* newTree(tree* parent);
Dataqueue* newQueue();

void enqueue(Dataqueue* thisQueue, tree* thisBranch);
tree* dequeue(Dataqueue* thisQueue);
void treeFree(tree * thisBranch);

Datastack* newDataStack();
void push(Datastack * thisStack,node * newItem);
node* queuePop (Dataqueue * thisStack);

void addEdge(graph* thisGraph, int source, int sink, int cost, int Z);
void addEdgeFromLine(graph* thisGraph, char* line, int start);
graph* addEdgeFromFile(char* filename);

isQueue *newISQueue();
void isEnqueue(struct isQueue *thisQueue, struct node * thisBranch);
node * isDequeue(struct isQueue *thisQueue);



node* newNode(int vertex, int capacity, int flow) {
    node* myNode = new node;
    myNode->vertex = vertex;
    myNode->capacity = capacity;
    myNode->MaxCapacity = capacity;
    myNode->flow = flow;
    myNode->Zvalue = 0;
    myNode->next = nullptr;
    return myNode;
}

linkedList* newList(){
    linkedList* myList = new linkedList;
    myList->head = nullptr;
    myList->self = nullptr;
    return myList;
}

graph* newGraph(int Vertices) {
    graph* myGraph = new graph;
    myGraph->totalVertices = Vertices;
    myGraph->adjacencyList = new linkedList[Vertices];
    for(int i = 0; i < Vertices; i++){
        myGraph->adjacencyList[i].head = nullptr;
        myGraph->adjacencyList[i].self = newNode(i, 0, 0);
    }
    return myGraph;
}

tree* newTree(tree* parent) {
    tree* myTree = new tree;
    myTree->data = nullptr;
    myTree->parent = parent;
    myTree->children = nullptr;
    myTree->next = nullptr;
    return myTree;
}

Dataqueue* newQueue() {
    Dataqueue* myQueue = new Dataqueue;
    myQueue->head = nullptr;
    myQueue->tail = nullptr;
    return myQueue;
}

void enqueue(Dataqueue* thisQueue, tree* thisBranch) {
    queueElement* thisItem = new queueElement;
    thisItem->data = thisBranch;
    if (thisQueue->head == nullptr) {
        thisQueue->head = thisItem;
        thisQueue->tail = thisItem;
    } else {
        thisQueue->tail->next = thisItem;
        thisQueue->tail = thisItem;
    }
    thisItem->next = nullptr;
}

tree* dequeue(Dataqueue* thisQueue) {
    queueElement* thisNode = thisQueue->head;
    tree* result = nullptr;
    if (thisNode != nullptr) {
        if (thisNode->next != nullptr) {
            thisQueue->head = thisNode->next;
        } else {
            thisQueue->head = nullptr;
        }
        result = thisNode->data;
        delete thisNode;
    }
    return result;
}

void treeFree(tree * thisBranch){
	if (thisBranch == nullptr) return;
	//start with all siblings, if any
	treeFree(thisBranch->next);
	//then onto children
	treeFree(thisBranch->children);
	//we won't be killing the data because these nodes belong to the graph ure
	free(thisBranch);
}

void addEdge(graph* thisGraph, int source, int sink, int cost, int Z) {
    node* thisNode = newNode(sink, cost, 0);
    thisNode->next = thisGraph->adjacencyList[source].head;
    thisNode->Zvalue = Z;
    thisGraph->adjacencyList[source].head = thisNode;
}

void addEdgeFromLine(graph* thisGraph, char* line, int start) {
    int vertex = INT16_MAX;
    int cost = INT16_MAX;

    int nextDigit = 0, currentNumber = 0, loopCount = 0, charCount = 0;
    while (line[loopCount]) {
        if (isspace(line[loopCount]) || line[loopCount] == '\n') {
            if (vertex == INT16_MAX) {
                vertex = currentNumber;
            } else if (cost == INT16_MAX) {
                cost = currentNumber;
                addEdge(thisGraph, start, vertex, cost, 0);

                vertex = INT16_MAX;
                cost = INT16_MAX;
            }
            currentNumber = 0;
            charCount = 0;
        } else {
            nextDigit = line[loopCount] - '0';
            currentNumber *= 10;
            currentNumber += nextDigit;
            ++charCount;
        }
        ++loopCount;
    }
}

graph* addEdgeFromFile(char* filename) {
    FILE* pFile;
    char line[MAX_SIZE];

    int loopCount = 0;
    pFile = fopen(filename, "r");
    if (pFile != nullptr) {
        while (fgets(line, MAX_SIZE, pFile) != nullptr) {
            if (line[0] != '#') {
                ++loopCount;
            }
        }
    }
    fclose(pFile);

    graph* thisGraph = newGraph(loopCount + 1);
    loopCount = 0;
    pFile = fopen(filename, "r");
    if (pFile != nullptr) {
        while (fgets(line, MAX_SIZE, pFile) != nullptr) {
            if (line[0] != '#') {
                addEdgeFromLine(thisGraph, line, loopCount);
                ++loopCount;
            }
        }
    }
    fclose(pFile);

    return thisGraph;
}

Datastack* newDataStack(){
	Datastack* result = new Datastack;
	result->head = nullptr;
	return result;
}
 
void push(Datastack * thisDatastack,node * newItem) {
	 stackItem * result = new stackItem;
	 result->next = thisDatastack->head;
	 result->data = newItem;
	 thisDatastack->head = result;
}
 
node* queuePop (Datastack * thisstack) {
	 if (thisstack == nullptr || thisstack->head == nullptr) return nullptr;
	 node * result = thisstack->head->data;
	 stackItem* temp = thisstack->head;
	 thisstack->head = thisstack->head->next;
	 free(temp);
	 return result;
}

isQueue *newISQueue() {
	isQueue *thisQueue = new isQueue;

	thisQueue->head = nullptr;
	thisQueue->tail = nullptr;

	return thisQueue;
}

/**
 * Enqueue a node
 */
void isEnqueue( isQueue *thisQueue,node* thisBranch) {
	isItem * thisItem = new isItem;
	thisItem->data = thisBranch;
	if (thisQueue->head == nullptr) {
		thisQueue->head = thisItem;
		thisQueue->tail = thisItem;
	} else {
		thisQueue->tail->next = thisItem;
		thisQueue->tail = thisItem;
	}
	thisItem->next = nullptr;
}

/**
 * Dequeue a node
 */
node* isDequeue( isQueue *thisQueue){
	 isItem * thisNode = thisQueue->head;
	 node * result = nullptr;
	if (thisNode != nullptr) {
		if (thisNode->next != nullptr) {
			thisQueue->head = thisNode->next;
		} else {
			thisQueue->head = nullptr;
		}
		result = thisNode->data;
		free(thisNode);
	}
	return result;
}

#endif // DATA_CPP

