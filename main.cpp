/* Author: M. Magdiyev
 * Implementation and testing of a function that
 * takes a maze in form of a matrix and outputs
 * whether there exists a valid path from the upper left corner
 * to the bottom right corner.
 */ 
#include <iostream>
#include <stdexcept>
#include <vector>
#include <map>

using namespace std;

#define vi vector<int>

/* Represent each element of the matrix as a graph node */
struct node {
  int row;
  int col;
  vector<node> neighbours = {};
};

// declare functions
bool checkMaze(vector<vector<int>> matrix);
void testFunction(vector<vector<vi>> cases, map<int, int> outputs);
vector<struct node> getAdj(vector<vector<int>> matrix, struct node *n);
void delNode(vector<vector<int>> *matrix, struct node *n);
void DFS(vector<vector<int>> *matrix, struct node *n, vector<struct node> *tree);

/* Driver program - test the function */
int main() {
    vector<vi> mat = {{1, 0, 1, 0, 0}, 
                      {1, 0, 1, 0, 0}, 
                      {1, 0, 1, 0, 1}, 
                      {1, 0, 1, 0, 1}, 
                      {1, 1, 1, 1, 1}};

    vector<vi> mat2 = { {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0},
                        {0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0},
                        {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0},
                        {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0},
                        {0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1},
                        {1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                        {1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                        {1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
                        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

    vector <vi> mat3 = {{1, 1, 0},
                        {1, 0, 1},
                        {1, 1, 1},
                        {1, 1, 0},
                        {1, 1, 1},
                        {0, 1, 0},
                        {1, 1, 0},
                        {1, 0, 0},
                        {1, 0, 0},
                        {1, 0, 0},
                        {1, 0, 1},
                        {1, 1, 1} };

    vector <vi> mat4 = {{0, 0, 0, 0},
                        {0, 0, 0, 0},
                        {0, 0, 0, 0},
                        {0, 0, 0, 0}};
    
    vector <vi> mat5 = {{1, 1, 1, 1, 1},
                        {1, 1, 1, 1, 1},
                        {1, 1, 1, 1, 1},
                        {1, 1, 1, 1, 1},
                        {1, 1, 1, 1, 0}};

    vector <vi> mat6 = {{1, 0, 0, 0, 0},
                        {1, 1, 0, 0, 0},
                        {1, 0, 1, 0, 0},
                        {0, 0, 0, 1, 0},
                        {0, 0, 0, 0, 1}};

    map<int,int> testOutputs {{0, 1}, {1, 1}, {2, 1}, {3, 0}, {4, 0}, {5, 0}};
    vector<vector<vi>> cases = {mat, mat2, mat3, mat4, mat5, mat6};
    testFunction(cases, testOutputs);

    return 0;
}


/* Helper function to get node's neighbours */
vector<struct node> getAdj(vector<vector<int>> matrix, struct node *n) {
  // up
  if (n->row - 1 >= 0) {
    if (matrix[n->row - 1][n->col] > 0) {
      struct node adj = {n->row - 1, n->col};
      n->neighbours.push_back(adj);
    }
  }
  // down
  if (n->row + 1 < matrix.size()) {
    if (matrix[n->row + 1][n->col] > 0) {
      struct node adj = {n->row + 1, n->col};
      (n->neighbours).push_back(adj);
    }
  }
  // left
  if (n->col - 1 >= 0) {
    if (matrix[n->row][n->col - 1] > 0) {
      struct node adj = {n->row, n->col - 1};
      n->neighbours.push_back(adj);
    }
  }
  // right
  if (n->col + 1 < matrix[n->row].size()) {
    if (matrix[n->row][n->col + 1] > 0) {
      struct node adj = {n->row, n->col + 1};
      n->neighbours.push_back(adj);
    }
  }
  return n->neighbours;
}

/* This effectively marks a node as visited */
void delNode(vector<vector<int>> *matrix, struct node *n) {
  (*matrix)[n->row][n->col] = 0;
}

/* Find if there's a valid path by viewing the matrix as graph
 * and performing a DFS on the top left corner node. If the node
 * from the bottom left corner occurs in the obtained subtree, return true.
 */
bool checkMaze(vector<vector<int>> matrix) {
    // validate input maze
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            if (matrix[i][j] != 0 && matrix[i][j] != 1) {
                throw std::invalid_argument("maze should contain only 1's and 0's");
            }
        }
    }
    // look for a valid path
    int lastRow = matrix.size() - 1;
    int lastCol = matrix[0].size() - 1;
    struct node first = {0, 0};
    vector<struct node> tree = {}; // store resuling subtree of the graph
    DFS(&matrix, &first, &tree);
    for (struct node x : tree) {
        if (x.row == lastRow && x.col == lastCol) {
            return true;
        }
    }
    return false;
}


/* Do a depth-first search */
void DFS(vector<vector<int>> *matrix, struct node *n, vector<struct node> *tree) {
  delNode(matrix, n); // mark visited
  vector<struct node> adj = getAdj(*matrix, n);
  for (int i = 0; i < adj.size(); i++) {
    if ((*matrix)[adj[i].row][adj[i].col] == 1) { //i.e. if not visited
      DFS(matrix, &adj[i],tree);
      tree->push_back(adj[i]);
    } 
  }
}

/* Function for unit testing */
void testFunction(vector<vector<vi>> cases, map<int, int> outputs) {
    int valid = 0;
    for (int i = 0; i<cases.size(); i++) {
        if (checkMaze(cases[i]) == outputs[i]) {
            valid++;
        }
    }
    cout << valid << " cases passed, " << cases.size() - valid << " failed" << "\n";
}
