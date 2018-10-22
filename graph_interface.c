//File: graph_interface.c
//Here I will be defining everything I need in order to implement the graph

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mytypes.h"
#include "graph_interface.h"

graphptr createGraph(){
  graphptr newGraph;
  newGraph = (Graph*)malloc(sizeof(Graph));
  if (newGraph == NULL)
  {
    fprintf(stderr, "Unable to allocate space\n");
    exit(-1);
  }

  newGraph->nodes = NULL;

  return newGraph;
}

void insertNode(Tag nodeTag, graphptr mainGraph){
  //checking if the node already exists in the graph
  Node *tmp = mainGraph->nodes;
  while (tmp != NULL)
  {
    if (strcmp(tmp->name, nodeTag) == 0)
    {
      printf("Node |%s| Exists\n", nodeTag);
      return;
    }
    tmp = tmp->next;
  }

  Node *newNode = (Node*)malloc(sizeof(Node));
  if (newNode == NULL)
  {
    printf("Unable to allocate space\n");
    exit(-1);
  }

  newNode->name = malloc(strlen(nodeTag) + 1);
  if (newNode->name == NULL)
  {
    printf("Unable to allocate space\n");
    exit(-1);
  }
  strcpy(newNode->name, nodeTag);
  newNode->visited = 0;
  newNode->edges = NULL;
  newNode->next = NULL;

  //checking if the graph is empty
  if (mainGraph->nodes == NULL)
  {
    mainGraph->nodes = newNode;
  }
  else
  {
    //placing the new node at the end
    tmp = mainGraph->nodes;
    while ((tmp->next) != NULL)
    {
      tmp = tmp->next;
    }
    tmp->next = newNode;
  }

  printf("Inserted |%s|\n", nodeTag);
}

void insertEdge(graphptr mainGraph, Tag ni, Tag nj, edgeWeight w){
  //check if the nodes exist or not and add them to the graph if not
  Node *tmp = mainGraph->nodes;
  while ((tmp != NULL) && (strcmp(tmp->name, ni) != 0))
  {
    tmp = tmp->next;
  }
  if (tmp == NULL)
  {
    insertNode(ni, mainGraph);
  }
  tmp = mainGraph->nodes;
  while ((tmp != NULL) && (strcmp(tmp->name, nj) != 0))
  {
    tmp = tmp->next;
  }
  if (tmp == NULL)
  {
    insertNode(nj, mainGraph);
  }

  //finding the nodes in the graph
  Node *niNode = mainGraph->nodes;
  while ((niNode != NULL) && (strcmp(niNode->name, ni) != 0))
  {
    niNode = niNode->next;
  }
  Node *njNode = mainGraph->nodes;
  while ((njNode != NULL) && (strcmp(njNode->name, nj) != 0))
  {
    njNode = njNode->next;
  }

  //adding an edge from ni to nj
  Edge *newEdge= (Edge*)malloc(sizeof(Edge));
  if (newEdge == NULL)
  {
    printf("Unable to allocate space\n");
    exit(-1);
  }

  newEdge->next = NULL;
  newEdge->visited = 0;
  newEdge->weight = w;
  newEdge->dest = njNode;

  //connecting the edge to nj
  Edge *tmpEdge = niNode->edges;
  if (niNode->edges == NULL)
  {
    niNode->edges = newEdge;
  }
  else
  {
    //placing the new node at the end
    tmpEdge = niNode->edges;
    while ((tmpEdge->next) != NULL)
    {
      tmpEdge = tmpEdge->next;
    }
    tmpEdge->next = newEdge;
  }

  printf("Inserted |%s|->%d->|%s|\n", ni, w, nj);
}

//Ignore this function, I'm using it for debugging
void printGraph(graphptr mainGraph){
  Edge *currentEdge;
  Node *tmp;
  Node *current = mainGraph->nodes;
  while (current != NULL)
  {
    printf("%s", current->name);
    currentEdge = current->edges;
    while (currentEdge != NULL)
    {
      tmp = currentEdge->dest;
      printf(" --%d--> %s", currentEdge->weight, tmp->name);
      currentEdge = currentEdge->next;
    }
    current = current->next;
    printf("\n");
  }
}

void deleteNode(Tag nodeTag, graphptr mainGraph){
  Edge *currentEdge;
  Edge *tmp;
  Node *tmpNode;

  //finding where the node is located
  Node *niNode = mainGraph->nodes;
  while ((niNode != NULL) && (strcmp(niNode->name, nodeTag) != 0))
  {
    niNode = niNode->next;
  }
  if (niNode == NULL)
  {
    printf("Node %s does not exist - abort-d\n", nodeTag);
    return;
  }

  //First I delete all the edges that arrive to the node we want to delete
  Edge ** edges_ptr;
  Node *current = mainGraph->nodes;
  while (current != NULL)
  {
    edges_ptr = &(current->edges);
    while (*edges_ptr != NULL)
    {
      if (strcmp((**edges_ptr).dest->name, nodeTag) == 0)
      {
        tmp = *edges_ptr;
        *edges_ptr = tmp->next;
        free(tmp);
      }
      else
      {
        edges_ptr = &((**edges_ptr).next);
      }
    }

    current = current->next;
  }

  //now I'm going to delete all the edges that spawn from the node
  currentEdge = niNode->edges;
  while (currentEdge != NULL)
  {
    tmp = currentEdge;
    currentEdge = currentEdge->next;
    free(tmp);
  }
  niNode->edges = NULL;

  //Finally deleting the node from the graph
  Node ** nodes_ptr = &(mainGraph->nodes);
  while (*nodes_ptr != NULL)
  {
    if (strcmp((**nodes_ptr).name, nodeTag) == 0)
    {
      tmpNode = *nodes_ptr;
      *nodes_ptr = tmpNode->next;
      free(tmpNode);
    }
    else
    {
      nodes_ptr = &((**nodes_ptr).next);
    }
  }

  printf("Deleted |%s|\n", nodeTag);
}

void deleteEdge(graphptr mainGraph, Tag ni, Tag nj, edgeWeight w){
  Edge *njEdge;
  Edge *tmp = NULL;
  int flag;

  Node *niNode = mainGraph->nodes;
  while ((niNode != NULL) && (strcmp(niNode->name, ni) != 0))
  {
    niNode = niNode->next;
  }
  if (niNode == NULL)
  {
    printf("|%s| does not exist - abort-l\n", ni);
    return;
  }

  Node *njNode = mainGraph->nodes;
  while ((njNode != NULL) && (strcmp(njNode->name, nj) != 0))
  {
    njNode = njNode->next;
  }
  if (njNode == NULL)
  {
    printf("|%s| does not exist - abort-l\n", nj);
    return;
  }

  //In the case where Ni doesn't have any edges
  njEdge = niNode->edges;
  if (njEdge == NULL)
  {
    printf("|%s|->%d->|%s| does not exist - abort-l\n", ni, w, nj);
    return;
  }

  flag = 0;

  Edge ** edges_ptr;

  if (w != -1)
  {
    edges_ptr = &(niNode->edges);
    while (*edges_ptr != NULL)
    {
      if ((strcmp((**edges_ptr).dest->name, nj) == 0) && (w == (**edges_ptr).weight))
      {
        tmp = *edges_ptr;
        *edges_ptr = tmp->next;
        free(tmp);
        printf("Del-vertex |%s|->%d->|%s|\n", ni, w, nj);
        return;
      }
      else
      {
        edges_ptr = &((**edges_ptr).next);
      }
    }
  }
  else if (w == -1)
  {
    edges_ptr = &(niNode->edges);
    while (*edges_ptr != NULL)
    {
      if (strcmp((**edges_ptr).dest->name, nj) == 0)
      {
        printf("Del-vertex |%s|->%d->|%s|\n", ni, (**edges_ptr).weight, nj);
        flag = 1; //found at least one edge to delete
        tmp = *edges_ptr;
        *edges_ptr = tmp->next;
        free(tmp);
      }
      else
      {
        edges_ptr = &((**edges_ptr).next);
      }
    }
  }

  if (flag == 0)
  {
    printf("|%s|->%d->|%s| does not exist - abort-l\n", ni, w, nj);
  }
}

void modifyWeight(graphptr mainGraph, Tag ni, Tag nj, edgeWeight oldW, edgeWeight newW){
  Edge *njEdge;
  Node *njNode;

  //finding where Ni is located in the graph
  Node *niNode = mainGraph->nodes;
  while ((niNode != NULL) && (strcmp(niNode->name, ni) != 0))
  {
    niNode = niNode->next;
  }

  //finding where Nj is located in the graph
  njNode = mainGraph->nodes;
  while ((njNode != NULL) && (strcmp(njNode->name, nj) != 0))
  {
    njNode = njNode->next;
  }
  if (njNode == NULL)
  {
    printf("|%s| does not exist - abort-m\n", nj);
    return;
  }

  if (niNode == NULL)
  {
    printf("|%s| does not exist - abort-m\n", ni);
    return;
  }
  else
  {
    //searching Ni's edges to find Nj and replace the weight
    njEdge = niNode->edges;
    while (njEdge != NULL)
    {
      njNode = njEdge->dest;
      if ((strcmp(njNode->name, nj) == 0) && (njEdge->weight == oldW))
      {
        //finding the first correct edge, modifying it and returning
        njEdge->weight = newW;
        printf("Mod-vertex |%s|->%d->|%s|\n", ni, newW, nj);
        return;
      }

      njEdge = njEdge->next;
    }
    //if the while loop finished the edges does not exist
    printf("|%s|->%d->|%s| does not exist - abort-m\n", ni, oldW, nj);
  }
}

void receiving(graphptr mainGraph, Tag nodeTag){
  Edge *currentEdge;
  Node *current = mainGraph->nodes;
  int flag = 0;

  //checking if the node exists
  while ((current != NULL) && (strcmp(current->name, nodeTag) != 0))
  {
    current = current->next;
  }
  if (current == NULL)
  {
    printf("|%s| does not exist - abort-r\n", nodeTag);
    return;
  }

  //starting from the first node of the graph and searching all of them
  current = mainGraph->nodes;
  while (current != NULL)
  {
    //since he can give money to himself I will search every node
    //searching all the edges to find the ones where the destination is the Node the user gave
    currentEdge = current->edges;
    while (currentEdge != NULL)
    {
      if ((strcmp(currentEdge->dest->name, nodeTag) == 0))
      {
        if (flag == 0)
        {
          printf("Rec-edges\n");
        }
        flag = 1;
        printf("|%s|-%d->|%s|\n", current->name, currentEdge->weight, nodeTag);
      }

      currentEdge = currentEdge->next;
    }

    current = current->next;
  }
  if (flag == 0)
  {
    printf("No-rec-edges %s\n", nodeTag);
  }
}

void simpleCycles(graphptr mainGraph, Tag ni){
  Node *current;
  int x, num_circ;
  edgeWeight *weight;
  Node **path;

  //checking if the node exists
  Node *niNode = mainGraph->nodes;
  while ((niNode != NULL) && (strcmp(niNode->name, ni) != 0))
  {
    niNode = niNode->next;
  }
  if (niNode == NULL)
  {
    printf("|%s| does not exist - abort-c\n", ni);
    return;
  }

  //setting every node as unvisited
  current = mainGraph->nodes;
  x = 0;
  while (current != NULL)
  {
    x++;
    current->visited = 0;

    current = current->next;
  }

  //I will need arrays that can hold at most all of the nodes plus the starting node one more time
  x++;

  path = malloc(x * sizeof(Node *));
  weight = malloc(x * sizeof(edgeWeight));
  num_circ = 0;

  simpleCyclesUtil(niNode, ni, path, weight, 0, 0, &num_circ);

  if (num_circ == 0)
  {
    printf("No-circle-found |%s|\n", ni);
  }

  free(path);
  free(weight);
}

void simpleCyclesUtil(Node *curr, Tag ni, Node **path, edgeWeight *weight, int index, edgeWeight w, int *num_circ){
  Edge *currentEdge;
  int i;

  //Each time I am at a node I set it as visited
  curr->visited = 1;

  //I also add it to the path array I am using
  path[index] = curr;
  weight[index] = w;
  index++;

  //I need to skip the first time we meet him because it's when the function gets first called
  if ((strcmp(curr->name, ni) == 0) && (index > 1))
  {
    //If I found ni I have found a cycle, so print it
    printf("Cir-found ");
    printf("|%s|", path[0]->name);
    for (i = 1; i < index; i++)
    {
      printf("->|%d|->|%s|", weight[i], path[i]->name);
    }
    printf("\n");
    (*num_circ)++;
  }
  else
  {
    //performing a DFS
    currentEdge = curr->edges;
    while (currentEdge != NULL)
    {
      if ((strcmp(currentEdge->dest->name, ni) == 0) || (currentEdge->dest->visited != 1))
      {
        simpleCyclesUtil(currentEdge->dest, ni, path, weight, index, currentEdge->weight, num_circ);
      }

      currentEdge = currentEdge->next;
    }
  }

  index--;
  curr->visited = 0;
}

void findCircles(graphptr mainGraph, Tag ni, edgeWeight k){
  Node *current;
  Edge *currentEdge;
  int x, num_circ;
  edgeWeight *weight;
  Node **path;

  //checking if the node exists
  Node *niNode = mainGraph->nodes;
  while ((niNode != NULL) && (strcmp(niNode->name, ni) != 0))
  {
    niNode = niNode->next;
  }
  if (niNode == NULL)
  {
    printf("|%s| does not exist - abort-f\n", ni);
    return;
  }

  //setting every node as unvisited
  current = mainGraph->nodes;
  x = 0;
  while (current != NULL)
  {
    currentEdge = current->edges;
    while (currentEdge != NULL)
    {
      x++;
      currentEdge->visited = 0;

      currentEdge = currentEdge->next;
    }

    current = current->next;
  }

  //I will need arrays that can hold at most all of the edges plus the starting node one more time
  x++;

  path = malloc(x * sizeof(Node *));
  weight = malloc(x * sizeof(edgeWeight));
  num_circ = 0;

  findCirclesUtil(niNode, ni, path, weight, 0, 0, k, &num_circ);

  if (num_circ == 0)
  {
    printf("No-circle-found involving |%s| %d\n", ni, k);
  }

  free(path);
  free(weight);
}

void findCirclesUtil(Node *curr, Tag ni, Node **path, edgeWeight *weight, int index, edgeWeight w, edgeWeight k, int *num_circ){
  Edge *currentEdge;
  int i;

  //I also add it to the path array I am using
  path[index] = curr;
  weight[index] = w;
  index++;

  //I need to skip the first time we meet him because it's when the function gets first called
  if ((strcmp(curr->name, ni) == 0) && (index > 1))
  {
    //If I found ni I have found a cycle, so print it
    printf("Cir-found ");
    printf("|%s|", path[0]->name);
    for (i = 1; i < index; i++)
    {
      printf("->|%d|->|%s|", weight[i], path[i]->name);
    }
    printf("\n");
    (*num_circ)++;
  }
  else
  {
    //performing a DFS
    currentEdge = curr->edges;
    while (currentEdge != NULL)
    {

      if ((strcmp(currentEdge->dest->name, ni) == 0) || ((currentEdge->visited != 1) && (currentEdge->weight >= k)))
      {
        currentEdge->visited = 1;

        findCirclesUtil(currentEdge->dest, ni, path, weight, index, currentEdge->weight, k, num_circ);

        currentEdge->visited = 0;
      }

      currentEdge = currentEdge->next;
    }
  }

  index--;
}

void traceflow(graphptr mainGraph, Tag ni, Tag nj, int length){
  Node *current;
  Edge *currentEdge;
  int x, num_trc;
  edgeWeight *weight;
  Node **path;

  //checking if the node exists
  Node *niNode = mainGraph->nodes;
  while ((niNode != NULL) && (strcmp(niNode->name, ni) != 0))
  {
    niNode = niNode->next;
  }
  if (niNode == NULL)
  {
    printf("|%s| does not exist - abort-l\n", ni);
    return;
  }

  Node *njNode = mainGraph->nodes;
  while ((njNode != NULL) && (strcmp(njNode->name, nj) != 0))
  {
    njNode = njNode->next;
  }
  if (njNode == NULL)
  {
    printf("|%s| does not exist - abort-l\n", nj);
    return;
  }

  //setting every node as unvisited
  current = mainGraph->nodes;
  x = 0;
  while (current != NULL)
  {
    currentEdge = current->edges;
    while (currentEdge != NULL)
    {
      x++;
      currentEdge->visited = 0;

      currentEdge = currentEdge->next;
    }

    current = current->next;
  }

  //I will need arrays that can hold at most all of the edges plus the starting node one more time
  x++;

  path = malloc(x * sizeof(Node *));
  weight = malloc(x * sizeof(edgeWeight));
  num_trc = 0;

  traceflowUtil(niNode, nj, path, weight, 0, length, 0, &num_trc);

  if (num_trc == 0)
  {
    printf("No-trace from |%s| |%s|\n", ni, nj);
  }

  free(path);
  free(weight);
}

void traceflowUtil(Node *curr, Tag nj, Node **path, edgeWeight *weight, int index, int length, edgeWeight w, int *num_trc){
  Edge *currentEdge;
  int i;

  //I also add it to the path array I am using
  path[index] = curr;
  weight[index] = w;
  index++;

  if (strcmp(curr->name, nj) == 0)
  {
    //If I found nj I have found a path, so print it
    printf("Tra-found ");
    printf("|%s|", path[0]->name);
    for (i = 1; i < index; i++)
    {
      printf("->|%d|->|%s|", weight[i], path[i]->name);
    }
    printf("\n");
    (*num_trc)++;
  }

  //performing a DFS
  currentEdge = curr->edges;
  while (currentEdge != NULL)
  {

    if ((currentEdge->visited != 1) && (index <= length))
    {
      currentEdge->visited = 1;

      traceflowUtil(currentEdge->dest, nj, path, weight, index, length, currentEdge->weight, num_trc);

      currentEdge->visited = 0;
    }

    currentEdge = currentEdge->next;
  }

  index--;
}

void readCSVFile(FILE *input, graphptr mainGraph){
  char whole_line[100], Ni[40], Nj[40];
  edgeWeight weight = 0;

  if (input == NULL)
  {
    //no input to read from
    return;
  }

  //reading from input until we reach the end
  while (fgets(whole_line, 100, input))
  {
    if ((strlen(whole_line) > 0) && (whole_line[strlen(whole_line) - 1] == '\n'))
    {
      whole_line[strlen(whole_line) - 1] = '\0';
    }

    //I will be using the insertEdge function because it enters the edges and also enters the nodes when they don't exist, all at once
    sscanf(whole_line, "%s\t%s\t%d", Ni, Nj, &weight);
    if (weight < 1)
    {
      printf("Weight should be a positive integer\n");
      continue;
    }
    insertEdge(mainGraph, Ni, Nj, weight);
  }

  //I won't be needing the input file any further
  fclose(input);
}

void outputGraph(FILE *output, graphptr mainGraph){
  Edge *currentEdge;
  Node *tmp;
  Node *current = mainGraph->nodes;

  if (output == NULL)
  {
    //no output file to print the graph
    return;
  }

  while (current != NULL)
  {
    fprintf(output, "\n|%s|\n", current->name);
    //fprintf(f, "Some text: %s\n", text);
    currentEdge = current->edges;
    while (currentEdge != NULL)
    {
      tmp = currentEdge->dest;
      fprintf(output, " -%d-> |%s|\n", currentEdge->weight, tmp->name);
      currentEdge = currentEdge->next;
    }
    current = current->next;
  }

  //closing the output file before exiting
  fclose(output);
}

void exitProgram(graphptr mainGraph){
  Edge *currentEdge;
  Edge *tmpEdge;
  Node *tmpNode;
  Node *currentNode;
  Tag nodeName;

  currentNode = mainGraph->nodes;
  while (currentNode != NULL)
  {
    //I start with freeing the edges of every node
    currentEdge = currentNode->edges;
    while (currentEdge != NULL)
    {
      tmpEdge = currentEdge;
      currentEdge = currentEdge->next;
      free(tmpEdge);
    }
    currentNode->edges = NULL;

    //then I free all the memory I used to store the names of the nodes
    nodeName = currentNode->name;
    free(nodeName);

    //next step is to free the node itself
    tmpNode = currentNode;
    currentNode = currentNode->next;
    free(tmpNode);
  }

  //finally I'm freeing the graph
  free(mainGraph);

  printf("Exit program\n");
}
