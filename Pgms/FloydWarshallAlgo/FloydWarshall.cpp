#include <limits.h>
#include <iostream>
using namespace std;

/*
 * http://www.geeksforgeeks.org/archives/19772
 */

int** Get2DArray(int rows, int columns) {
    int** array = new int*[rows];

    for (int i=0; i<rows; i++) {
        array[i] = new int[columns];
    }

    return array;
}

/* A utility function to print solution */
void PrintSolution(int** solution, int vertex) {
    for (int i = 0; i < vertex; i++) {
        for (int j = 0; j < vertex; j++) {
            if (solution[i][j] == INT_MAX) {
                cout<<"INF  ";
            }
            else {
                cout<<solution[i][j]<<"    ";
            }
        }
        cout<<endl;
    }
}

void PrintAllPaths(int** pShortestPathList, int** solution, int vertex) {
    int numOfPaths = vertex*vertex;

    for (int i=0; i<numOfPaths; i++) {
        int source = i/vertex;
        int destination = i % vertex;

        if (pShortestPathList[i][0] == -1 && solution[source][destination] == INT_MAX) {
            cout<<"There is no path between "<<source<<" and "<<destination<<endl;
        }
        else if (pShortestPathList[i][0] == -1) {
            if (source == destination) {
                //Both source and destination are the same, no path needed
            }
            else {
                cout<<"Path between "<<source<<" and "<<destination<<"(weight - "<<solution[source][destination] \
                    <<"): "<<source<<" "<<destination<<endl;
            }
        }
        else if (pShortestPathList[i][0] != 0) {
            cout<<"Path between "<<source<<" and "<<destination<<"(weight - "<<solution[source][destination] \
                    <<"): "<<source;
            for (int j=0; j<vertex; j++){
                if (pShortestPathList[i][j] == -1) {
                    break;
                }
                cout<<" "<<pShortestPathList[i][j];
            }
            cout<<" "<<destination<<endl;
        }
    }
}

int** CreateShortestPathList(int vertex) {
    int numOfPaths = vertex*vertex;
    int** pShortestPathList = Get2DArray(numOfPaths, vertex);
    for (int i=0; i<numOfPaths; i++) {
        for (int j=0; j<vertex; j++) {
            pShortestPathList[i][j] = -1;
        }
    }
    return pShortestPathList;
}

void DeleteShortestPathList(int**pShortestPathList, int vertex) {
    int numOfPaths = vertex*vertex;

    for (int i=0; i<numOfPaths; i++) {
        delete [] pShortestPathList[i];
    }

    delete []pShortestPathList;
}

void UpdatePath(int** pShortestPathList, int vertex, int begin, int intermediate, int end) {
    int numOfPaths = vertex*vertex;
    int pathIndex = begin * vertex + end;
    int firstHalfIndex = begin * vertex + intermediate;
    int secondHalfIndex = intermediate * vertex + end;
    int index = 0;

    if (pathIndex < numOfPaths) {
        //copy the first half of the path, path(begin, intermediae)
        for (int i=0; i<vertex && index<vertex; i++) {
            if (pShortestPathList[firstHalfIndex][i] == -1) {
                break;
            }
            pShortestPathList[pathIndex][index] = pShortestPathList[firstHalfIndex][i];
            index++;
        }
        pShortestPathList[pathIndex][index] = intermediate;
        index++;
        //copy the second half of the path, path(intermediae, end)
        for (int j=0; j<vertex && index<vertex; j++) {
            if (pShortestPathList[secondHalfIndex][j] == -1) {
                break;
            }
            pShortestPathList[pathIndex][index] = pShortestPathList[secondHalfIndex][j];
            index++;
        }
    }
}


/*
 * The Floyd Warshall Algorithm is for solving the All Pairs Shortest Path problem. 
 * The problem is to find shortest distances between every pair of vertices in a given edge weighted directed Graph.
 *
 * Input:
 *       graph[][] = { {0,   5,  INF, 10},
 *                    {INF,  0,  3,  INF},
 *                    {INF, INF, 0,   1},
 *                    {INF, INF, INF, 0} }
 * which represents the following graph
 *             10
 *       (0)------->(3)
 *        |         /|\
 *      5 |          |
 *        |          | 1
 *       \|/         |
 *       (1)------->(2)
 *            3       
 * Note that the value of graph[i][j] is 0 if i is equal to j 
 * And graph[i][j] is INF (infinite) if there is no edge from vertex i to j.
 *
 * Output:
 * Shortest distance matrix
 *      0      5      8      9
 *    INF      0      3      4
 *    INF    INF      0      1
 *    INF    INF    INF      0 
 *
 */
void FloydWarshallAlgo(int graph[][4], int vertex) {
    int i,j,k;
    int** solution = Get2DArray(vertex, vertex);
    int** pShortestPathList = CreateShortestPathList(vertex);

    for (i=0; i<vertex; i++) {
        for (j=0; j<vertex; j++) {
            solution[i][j] = graph[i][j];
        }
    }

    for (k=0; k<vertex; k++){
        for (i=0; i<vertex; i++) {
            for (j=0; j<vertex; j++) {
                // If vertex k is on the shortest path from
                // i to j, then update the value of solution[i][j]
                if (solution[i][k] != INT_MAX && solution[k][j] != INT_MAX &&
                    solution[i][k] + solution[k][j] < solution[i][j]) {
                        //Update the shortest path and the weight
                        solution[i][j] = solution[i][k] + solution[k][j];
                        UpdatePath(pShortestPathList, vertex, i, k, j);
                }
            }
        }
    }

    PrintSolution(solution, vertex);
    PrintAllPaths(pShortestPathList, solution, vertex);

    for (i=0; i<vertex; i++) {
        delete[] solution[i];
    }
    delete[] solution;

    DeleteShortestPathList(pShortestPathList, vertex);
}

int main() {
    int graph[4][4] = { {0,       5,       INT_MAX, 10},
                        {INT_MAX, 0,       3,       INT_MAX},
                        {INT_MAX, INT_MAX, 0,       1},
                        {INT_MAX, INT_MAX, INT_MAX, 0}
                      };
    int vertex = 4;
    FloydWarshallAlgo(graph, vertex);
    return 0;
}
