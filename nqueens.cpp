#include <iostream>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <deque>
using namespace std;

struct treeNode {
    int queens;
    bool visited;
    vector<treeNode*> children;
    vector<int> columnCount;
    
    treeNode() : queens(0), visited(false) {}
};

void findSolutions(deque<treeNode*>, ofstream&, int);
void createTree(int, treeNode*, int&);

int main(int argc, const char* argv[])
{
    if (argc !=3) {
        cout << "Sorry, did not pass the correct number of arguments." << endl;
        return 0;
    }
    
    int n = atoi(argv[1]);
    ofstream fout;
    int numSolutions = 0;
    treeNode* startNode = new treeNode;
    deque<treeNode*> treeStack;
    
    treeStack.push_back(startNode);
    createTree(n, startNode, numSolutions);
    
    fout.open(argv[2]);
    fout << "Number of Super Queens: " << n << endl;
    
    findSolutions(treeStack, fout, n);
    
    fout << "Total number of solutions: " << numSolutions << endl;
    
    
    
    return 0;
}


void findSolutions(deque<treeNode*> treeStack, ofstream& fout, int n) {
    
    treeNode* current;
    int count = 0;
    
    while (treeStack.size() > 0) {
        current = treeStack[0];
        treeStack.pop_front();
        
        if (current->visited == false) {
            
            //mark the current node as visited
            current->visited = true;
            
            if (n!= 0 && current->queens == n) {
                
                //write the solution information to the file
                count++;
                fout << "Solution " << count << ": ";
                
                for (int i = 0; i < current->columnCount.size(); i++) {
                    fout << "(" << current->columnCount[i] + 1 << ", " << i + 1 << ")";
                    
                    if (i != current->columnCount.size() - 1) {
                        fout << ", ";
                    }
                    else {
                        fout << endl;
                    }
                }
            }
            
            for (int i = 0; i < current->children.size(); i++) {
                
                //add the current node's children to the stack
                treeStack.push_back(current->children[i]);
            }
        }
    }
    
}

void createTree(int n, treeNode* current, int& numSolutions) {
    
    if (n != 0 && current->queens == n) {
        //increase the number of solutions
        numSolutions++;
        return;
    }
    
    int board[n][n];
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            board[i][j] = 0;
        }
    }
    
    for (int i = 0; i < current->columnCount.size(); i++) {
        
        int x = i;
        int y = current->columnCount[i];
        
        //kill vertical squares
        for (int i = 0; i < n; i++) {
            board[x][i] = 1;
        }
        
        //kill horizontal squares
        for (int i = 0; i < n; i++) {
            board[i][y] = 1;
        }
        
        //kill diagonal squares
        int diagX = x, diagY = y;
        
        while (diagX >= 0 && diagY >= 0) {
            board[diagX][diagY] = 1;
            diagX--;
            diagY--;
        }
        
        diagX = x;
        diagY = y;
        
        while (diagX >= 0 && diagY < n) {
            board[diagX][diagY] = 1;
            diagX--;
            diagY++;
        }
        
        diagX = x;
        diagY = y;
        
        while (diagX < n && diagY < n) {
            board[diagX][diagY] = 1;
            diagX++;
            diagY++;
        }
        
        diagX = x;
        diagY = y;
        
        while (diagX < n && diagY >= 0) {
            board[diagX][diagY] = 1;
            diagX++;
            diagY--;
        }
        
        //kill knight moves
        if (x + 2 < n) {
            if (y + 1 < n) {
                board[x+2][y+1] = 1;
            }
            if (y - 1 >= 0) {
                board[x+2][y-1] = 1;
            }
        }
        
        if (x - 2 >= 0) {
            if (y + 1 < n) {
                board[x-2][y+1] = 1;
            }
            if (y - 1 >= 0) {
                board[x-2][y-1] = 1;
            }
        }
        
        if (x + 1 < n) {
            if (y + 2 < n) {
                board[x+1][y+2] = 1;
            }
            if (y - 2 >= 0) {
                board[x+1][y-2] = 1;
            }
        }
        
        if (x - 1 >= 0) {
            if (y + 2 < n) {
                board[x-1][y+2] = 1;
            }
            if (y - 2 >= 0) {
                board[x-1][y-2] = 1;
            }
        }
    }
    
    int nextColumn = current->queens;
    
    for (int i = 0; i < n; i++) {
        if (board[nextColumn][i] == 0) {
            treeNode* next = new treeNode;
            vector<int> temp = current->columnCount;
            
            temp.push_back(i);
            next->columnCount = temp;
            next->queens = current->queens + 1;
            current->children.push_back(next);
            
            //expand the tree by calling the same function on the current node's children
            createTree(n, next, numSolutions);
        }
    }
}