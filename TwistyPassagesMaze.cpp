// Name: Michael Lee
// Prog 4 - Twisty Passages

#include <iostream>
#include <vector>
#include <stack>
#include <time.h>
#include <stdlib.h>

using namespace std;

struct Node{
    bool visited;
    bool northWall;
    bool eastWall;
    bool southWall;
    bool westWall;
};

vector<vector<Node*> > maze;

void generateGraph(int size){
    for (int i = 0; i < size; i++){
        vector<Node*> row;
        for (int j = 0; j < size; j++){ // if wall is present (there is an 'X') then bool value is true
            Node* room = new Node;
            room->northWall = true;
            room->eastWall = true;
            room->southWall = true;
            room->westWall = true;
            room->visited = false;
            row.push_back(room);
        }
        maze.push_back(row);
    }
}

void printMaze(int size){
    int gridDims = size * 2 + 1; // user input * 2 + 1 to include borders
    char grid[gridDims][gridDims]; // char 2D array to display the maze

    for (int i = 0; i < gridDims; i++){ // walls
        for (int j = 0; j < gridDims; j++){
            grid[i][j] = 'X';
        }
    }

    for (int i = 1; i < gridDims; i+=2){ // rooms
        for (int j = 1; j < gridDims; j+=2){
            grid[i][j] = '.';
        }
    }

    for (int i = 0; i < gridDims; i++){ // print the maze
        for (int j = 0; j < gridDims; j++){
            cout << grid[i][j];
        }
        cout << endl;
    }
}

int bounds(int dir[], int x, int y, int size){
    int gridDims = size * 2 + 1; // user input * 2 + 1 to include borders
    char gridArr[gridDims][gridDims];
    int coord = 0;
    Node* room = new Node;

    if (y > 0 && gridArr[y-1][x] == 0) dir[coord++] = room->northWall;
    if (x < gridDims - 1 && gridArr[y][x+1] == 0) dir[coord++] = room->eastWall;
    if (y < gridDims - 1 && gridArr[y+1][x] == 0) dir[coord++] = room->southWall;
    if (x > 0 && gridArr[y][x - 1] == 0) dir[coord++] = room->westWall;

    return coord;
}

void DFS() {
    stack<Node *> roomStack;
    roomStack.push(maze[1][1]);

    while (!roomStack.empty()) {
        Node *currNode = roomStack.top();
        roomStack.pop();

        if (currNode->visited == true) {
            continue;
        } else { // load all the current node's unvisited neighbors into the stack of work to do
            for (auto& n : maze){
                if(!currNode->visited) roomStack.push(currNode);

                srand(time(NULL));
                int randDir = rand() % 4 + 1;

                if (randDir == 1){ // NORTH
                    currNode->northWall = false;
                }
                else if (randDir == 2){ // EAST
                    currNode->eastWall = false;
                }
                else if (randDir == 3){ // SOUTH
                    currNode->southWall = false;
                }
                else if (randDir == 4){ // WEST
                    currNode->westWall = false;
                }
                currNode->visited = true;
            }
        }
    }
}

bool check(int size, int i, int j ){
    // Boundary Conditions
    if(i == size || j == size || i < 0 || j < 0) {
        return false;
    } else{
        return true;
    }
}
bool northHas(int size, int i, int j ){
    return check(size, i-1, j );
}
bool southHas( int size, int i, int j ){
    return check(size, i+1, j );
}
bool eastHas( int size, int i, int j ){
    return check(size, i, j+1 );
}
bool westHas( int size, int i, int j )
{
    return check(size, i, j-1 );
}

void getNeighbors(int size){
    // use pairs to mark current room and for possible rooms
    pair<Node*, bool> markRooms;
    // 1. in bounds
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            //if (maze[i][j] >= 0 && maze < size){

            //}
        }
    }

    // 2. unvisited
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            //if (maze[i][j] != ){ // not in 2D array

            //}
        }
    }
}

int main(int argc) {
    if (argc >= 1) {
        generateGraph(argc);
        printMaze(argc);
        DFS();
    } else {
        int size = 40; // default size is 40 if no command line arguments are provided
        generateGraph(size);
        printMaze(size);
        DFS();
    }
    return 0;
}