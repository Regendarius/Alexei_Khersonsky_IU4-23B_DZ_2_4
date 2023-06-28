#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define memzero(base, size)        \
    memset(base,0,size)

#define EXIT_PROCESS(how)        \
    exit(how)

#define VISITED            1
#define NOT_VISITED        0

#define TRUE            1
#define FALSE           0

#define OK                0
#define BAD               1

typedef unsigned char byte_t;

typedef enum _boolean {
    _false,
    _true
} boolean;


void create_graph(byte_t **graph, int nodes);

boolean is_connected(byte_t **graph, int nodes);


boolean is_connected_node(byte_t **graph, int target_node, int nodes);


void print_graph(byte_t **graph, int nodes);


void *my_malloc(size_t bytes);


void usage(char *process);

void fatal(char *what);

int main(int argc, char **argv) {
    byte_t **graph = NULL;
    int nodes = 0;
    int x = 0;

    srand(time(NULL));

    if (argc <= 1)
        usage(argv[0]);

    nodes = atoi(argv[1]);

    if (nodes <= 0)
        usage(argv[0]);

    graph = (byte_t **) my_malloc((nodes + sizeof(byte_t *)) * sizeof(byte_t *));

    for (x = 0; x < nodes; ++x)
        graph[x] = (byte_t *) my_malloc(nodes * sizeof(byte_t) + sizeof(byte_t));

    printf("[!!]Generating a graph of %d nodes..\n\n", nodes);

    create_graph(graph, nodes);
    print_graph(graph, nodes);

    if (is_connected(graph, nodes) == _true)
        printf("\n[+]Your graph is connected!\n");
    else
        printf("\n[+]Your graph is not connected!\n");

    for (x = 0; x < nodes; ++x)
        free(graph[x]);

    free(graph);

    EXIT_PROCESS(OK);
}

void create_graph(byte_t **graph, int nodes) {
    for (int i = 0; i < nodes; ++i)
        for (int j = 0; j < nodes; ++j)
            graph[i][j] = rand() % 2;
}

boolean is_connected(byte_t **graph, int nodes) {
    int i = 0;
    boolean bit = _true;

    for (i = 0; i < nodes && bit == _true; ++i)
        bit = is_connected_node(graph, i, nodes);

    return bit;
}

boolean is_connected_node(byte_t **graph, int target_node, int nodes) {
    byte_t *stack = NULL;
    byte_t *visited = NULL;
    byte_t *stack_interrupt = NULL;
    int n_visited = 0;
    int n_stack = 0;
    int i = 0;
    int top_of_stack = 0;

    stack = (byte_t *) my_malloc(nodes * sizeof(char) + sizeof(char));
    visited = (byte_t *) my_malloc(nodes * sizeof(char) + sizeof(char));
    stack_interrupt = (byte_t *) my_malloc(nodes * sizeof(char) + sizeof(char));

    stack[n_stack++] = target_node;

    while (n_stack > 0) {
        if (visited[stack[--n_stack]] != VISITED) {
            ++n_visited;
            visited[stack[n_stack]] = VISITED;
        }

        top_of_stack = stack[n_stack];
        for (i = 0; i < nodes; ++i) {
            if (graph[top_of_stack][i] && visited[i] == NOT_VISITED) {
                if (stack_interrupt[i] == FALSE) {
                    stack[n_stack++] = i;
                    stack_interrupt[i] = TRUE;
                }
            }
        }
    }

    free(stack);
    free(visited);
    free(stack_interrupt);

    return (n_visited == nodes) ? _true : _false;
}

void print_graph(byte_t **graph, int nodes) {
    for (int i = 0; i < nodes; ++i) {
        for (int j = 0; j < nodes; ++j) {
            printf("%d", graph[i][j]);
        }
        printf("\n");
    }
}

void *my_malloc(size_t bytes) {
    void *mem_block = malloc(bytes);
    if (mem_block == NULL)
        fatal("malloc");

    memzero(mem_block, bytes);
    return mem_block;
}

void usage(char *process) {
    fprintf(stdout, "<usage> <%s> <number of nodes in graph>\n", process);
    EXIT_PROCESS(BAD);
}

void fatal(char *what) {
    fprintf(stderr, "fatal in %s\n", what);
    EXIT_PROCESS(BAD);
}