#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph_interface.h"

#define MAX_INPUT 100

int main(int argc, char *argv[])
{
  FILE *input = NULL;
  FILE *output = NULL;
  int i, flag, weight, nweight, k, l;
  char Ni[40], Nj[40], command[1];
  char *user_input = malloc(MAX_INPUT);
  if (user_input == NULL)
  {
    printf("Out of memory\n");
    exit(-1);
  }

  //checking the input from the command line
  if (argc > 5)
  {
    printf("Wrong arguments\n");
    exit(-1);
  }
  for (i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "-i") == 0)
    {
      if (input != NULL)
      {
        printf("Specify only one input\n");
        exit(-1);
      }
      input = fopen(argv[i + 1], "r");
      if (input == NULL)
      {
        printf("Couldn't open input file\n");
        exit(-1);
      }
      i++;
    }
    else if (strcmp(argv[i], "-o") == 0)
    {
      if (output != NULL)
      {
        printf("Specify only one output\n");
        exit(-1);
      }
      output = fopen(argv[i + 1], "w");
      if (output == NULL)
      {
        printf("Couldn't create output file\n");
        exit(-1);
      }
      i++;
    }
    else
    {
      printf("Wrong argument, use only -i or -o\n");
      exit(-1);
    }
  }

  //creating the graph for this application
  graphptr mainGraph;
  mainGraph = createGraph();

  //entering the input file to the graph
  readCSVFile(input, mainGraph);

  //using a prompt to get input from the user
  flag = -1;
  while (flag != 0)
  {
    //printGraph(mainGraph);

    printf("Enter a command:\n");

    fgets(user_input, MAX_INPUT, stdin);
    if ((strlen(user_input) > 0) && (user_input[strlen(user_input) - 1] == '\n'))
    {
      user_input[strlen(user_input) - 1] = '\0';
    }

    //using a switch statement to call the right function
    switch (user_input[0])
    {
      case 'i':
        //insert new node
        sscanf(user_input, "%s %s", command, Ni);
        insertNode(Ni, mainGraph);
        break;
      case 'n':
        //insert new edge
        sscanf(user_input, "%s %s %s %d", command, Ni, Nj, &weight);
        if (weight < 1)
        {
          printf("Weight should be a positive integer\n");
          break;
        }
        insertEdge(mainGraph, Ni, Nj, weight);
        break;
      case 'd':
        //delete node
        sscanf(user_input, "%s %s", command, Ni);
        deleteNode(Ni, mainGraph);
        break;
      case 'l':
        //delete edge
        weight = -1;  //in the case the user doesn't give an input
        sscanf(user_input, "%s %s %s %d", command, Ni, Nj, &weight);
        deleteEdge(mainGraph, Ni, Nj, weight);
        break;
      case 'm':
        //modify edge
        sscanf(user_input, "%s %s %s %d %d", command, Ni, Nj, &weight, &nweight);
        if (nweight < 1)
        {
          printf("New weight should be a positive integer\n");
          break;
        }
        modifyWeight(mainGraph, Ni, Nj, weight, nweight);
        break;
      case 'r':
        //receives
        sscanf(user_input, "%s %s", command, Ni);
        receiving(mainGraph, Ni);
        break;
      case 'c':
        //find simple cycles
        sscanf(user_input, "%s %s", command, Ni);
        simpleCycles(mainGraph, Ni);
        break;
      case 'f':
        //find circles
        sscanf(user_input, "%s %s %d", command, Ni, &k);
        findCircles(mainGraph, Ni, k);
        break;
      case 't':
        //trace
        sscanf(user_input, "%s %s %s %d", command, Ni, Nj, &l);
        traceflow(mainGraph, Ni, Nj, l);
        break;
      case 'e':
        //exit
        outputGraph(output, mainGraph); //before exiting I print the graph
        exitProgram(mainGraph); //freeing everything I used
        flag = 0; //changing the flag in order to end the program
        free(user_input);
        break;
      default:
        printf("Invalid input\n");
        break;
    }
  }

  return 0;
}
