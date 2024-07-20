#include <bits/stdc++.h>
#include "imageSegmentation.c++"

/**
 * Respond to the -b command line switch by running
 * breadth first search and reporting back the path found
 */
void runBFS(char* fileName, int source, int sink) {
    if (!fileName && (source != sink)) {
        // TODO: Make sure that source and sink are within the graph
        graph* thisGraph = addEdgeFromFile(fileName);
        linkedList* thisPath = BFS(thisGraph, source, sink);

        node* currentNode = thisPath->head;
        node* nextNode = nullptr;

        std::cout << "Path: ";
        while (currentNode) {
            std::cout << "=> " << currentNode->vertex << " ";
            nextNode = currentNode->next;
            currentNode = nextNode;
        }
        std::cout << std::endl;
    }
}

/**
 * Respond to the -f command line switch by running
 * Ford-Fulkerson and reporting back the max-flow
 */
void runFF(char* fileName) {
    if (fileName == nullptr){
        return;
    }

    int graphStart = 0;
    int graphEnd = 0;
    bool graphViable = true;
    int maximumFlow = 0;

    graph* thisGraph = addEdgeFromFile(fileName);
    node* thisNode = nullptr;

    // find the first graph node
    thisNode = thisGraph->adjacencyList[graphStart].head;
    while ((thisNode == nullptr) && graphViable) {
        ++graphStart;
        if (thisGraph->adjacencyList[graphStart].head) {
            thisNode = thisGraph->adjacencyList[graphStart].head;
        } else {
            graphViable = false;
        }
    }

    // find the last graph node
    graphEnd = thisGraph->totalVertices - (graphStart + 1);

    if (thisNode) {
        maximumFlow = maxFlow(thisGraph, graphStart, graphEnd);
        std::cout << "Max flow (from " << graphStart << " to " << graphEnd << ") is " << maximumFlow << std::endl;
    } else {
        std::cout << "Error creating or reading graph." << std::endl;
    }
}

/**
 * Respond to the -i command line switch by running
 * the image segmentation routines
 */
void runIS(char* imageFileName, char* cutFileName) {
    for(int i=0;i<strlen(imageFileName);i++){
        cout<<imageFileName[i];
    }
    cout<<endl;
    for(int i=0;i<strlen(cutFileName);i++){
        cout<<cutFileName[i];
    }
    cout<<endl;
    if (imageFileName != nullptr && cutFileName != nullptr) {
        int mFlow;
        // create a graph complete with source 0 and sink at totalVertices
        graph* thisGraph = readPgmFile(imageFileName);
        if (thisGraph == nullptr) {
            std::cout << "Error: invalid PGM file!" << std::endl;
            return;
        }
        // printResidualGraph(thisGraph);
        mFlow = maxFlow(thisGraph, thisGraph->totalVertices - 2, thisGraph->totalVertices - 1);
        // std::cout << "Maximum Flow: " << mFlow << std::endl;
        // now we have a maximum flow, so run image segmentation to build a minimum cut
        if (mFlow != 0) {
            imageSegmentation(thisGraph, cutFileName, thisGraph->totalVertices - 2);
        }
    }
}

/**
 * ff
 * Run breadth first search, Ford-Fulkerson, and Image Segmentation
 */
int main(int argc, char* argv[]) {
    if (argc > 2) {
        if (argv[1][1] == 'b') {
            if (argc > 4) {
                runBFS(argv[2], std::atoi(argv[3]), std::atoi(argv[4]));
            } else {
                std::cout << "Usage (pick one):\n";
                std::cout << " ff –b graph.txt source_node sink_node\n";
            }
        } else if (argv[1][1] == 'f') {
            runFF(argv[2]);
        } else if (argv[1][1] == 'i') {
            if (argc > 3) {
                runIS(argv[2], argv[3]);
            } else {
                std::cout << "Usage (pick one):\n";
                std::cout << " ff –i image.pgm imageWcut.pgm\n";
            }
        }
    } else {
        std::cout << "Usage (pick one):\n";
        std::cout << " ff –b graph.txt source_node sink_node\n";
        std::cout << " ff –f graph.txt\n";
        std::cout << " ff –i image.pgm imageWcut.pgm\n";
    }

    return 0;
}