//File: mytypes.h
//I'm defining here all the types I need

#ifndef MYTYPES_H
#define MYTYPES_H

typedef char* Tag;

typedef int edgeWeight;

typedef struct Graph *graphptr;

typedef struct Node{
  Tag name;
  int visited;  //I use this variable when searching for cycles
  struct Edge *edges; //the list of edges spawning from this Node
  struct Node *next;
} Node;

typedef struct Edge{
  Node *dest;
  int visited;
  struct Edge *next;
  edgeWeight weight;
} Edge;

typedef struct Graph{
  Node *nodes;  //a list with all the nodes in the graph
} Graph;

#endif
