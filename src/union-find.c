#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX(a, b) (a > b ? a : b);
#define MIN(a, b) (a < b ? a : b);

typedef struct union_find_struct {
    int* graph;
    int graph_size;
} union_find;

int find(union_find* uf, int node, int* treeSize)
{   
    int curr = node;
    while (uf->graph[curr] > 0)
        curr = uf->graph[curr];

    *treeSize = -uf->graph[curr];
    return curr;
}

int merge(union_find* uf, int lNode, int rNode)
{
    int lSize, rSize;

    int lParent = find(uf, lNode, &lSize);
    int rParent = find(uf, rNode, &rSize);

    if (lParent == rParent)
        return -1;

    int newParent, newChild;
    if (lSize > rSize)
    {
        newParent = lParent;
        newChild = rParent;
    }
    else if (lSize < rSize)
    {
        newParent = rParent;
        newChild = lParent;
    }
    else
    {
        newParent = MAX(lParent, rParent);
        newChild = MIN(lParent, rParent);
    }

    uf->graph[newChild]= newParent;
    uf->graph[newParent] = -(lSize + rSize);
    return newParent;
}

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
bool hasCycle(int** edges, int edgesSize) {
    union_find* uf = (union_find*) malloc(sizeof(union_find));
    uf->graph_size = edgesSize;
    uf->graph = (int*) malloc(sizeof(int)*(uf->graph_size + 1)); // store from -> n
    for (int i_node = 0; i_node <= uf->graph_size; i_node++)
        uf->graph[i_node] = -1;

    bool res = false;

    // for (int i_edge = 0; i_edge < edgesSize; i_edge++)
    // {
    //     int* edge = edges[i_edge];
    //     printf("%d %d \n", edge[0], edge[1]);
    // }

    for (int i_edge = 0; i_edge < edgesSize; i_edge++)
    {
        int* edge = edges[i_edge];
        int lNode = edge[0], rNode = edge[1];

        int lSize, rSize;

        int lParent = find(uf, lNode, &lSize);
        int rParent = find(uf, rNode, &rSize);

        if (lParent == rParent)
        {
            res = true;
        }

        merge(uf, lParent, rParent);
    }

    free(uf->graph);
    free(uf);
    return res;
}

int main()
{
    int g1_data[][2] = {{1, 2}, {1, 3}, {2, 3}};
    int* g1[sizeof(g1_data) / sizeof(g1_data[0])];
    for (int i = 0; i < sizeof(g1) / sizeof(*g1); i++)
        g1[i] = g1_data[i];

    int edgesSize = sizeof(g1) / sizeof(g1[0]);
    bool res1 = hasCycle(g1, edgesSize);

    int g2_data[][2] = {{1,2},{2,3},{3,4},{1,5}};
    int g2_size = sizeof(g2_data) / sizeof(g2_data[0]);
    int* g2[g2_size];
    for (int i = 0; i < g2_size; i++)
        g2[i] = g2_data[i];
    bool res2 = hasCycle(g2, g2_size);

    printf("%d %d \n", res1, res2);
    return 0;
}