//File: graph_interface.h

#include "mytypes.h"

graphptr createGraph();
void insertNode(Tag, graphptr);
void insertEdge(graphptr, Tag, Tag, edgeWeight);
void printGraph(graphptr);
void deleteNode(Tag, graphptr);
void deleteEdge(graphptr, Tag, Tag, edgeWeight);
void modifyWeight(graphptr, Tag, Tag, edgeWeight, edgeWeight);
void receiving(graphptr, Tag);
void simpleCycles(graphptr, Tag);
void simpleCyclesUtil(Node *, Tag, Node **, int *, int, int, int *);
void findCircles(graphptr, Tag, edgeWeight);
void findCirclesUtil(Node *, Tag, Node **, edgeWeight *, int, edgeWeight, edgeWeight, int *);
void traceflow(graphptr, Tag, Tag, int);
void traceflowUtil(Node *, Tag, Node **, edgeWeight *, int, int, edgeWeight, int *);
void readCSVFile(FILE *, graphptr);
void outputGraph(FILE *, graphptr);
void exitProgram(graphptr);
