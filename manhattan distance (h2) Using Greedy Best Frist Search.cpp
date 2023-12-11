// manhattan distance (h2) Using Greedy Best Frist Search


#include <iostream>
#include <queue>
#include <vector>
#include <unordered_set>
#include <sstream>

using namespace std;

// puzzle state structure
struct PuzzleState {
    vector<vector<int>> board; // puzzle board 2D vector
    int cost; // state reach cost
};

// in priority queue puzzle states comparison function
struct PuzzleStateCompare {
    bool operator()(const PuzzleState& state1, const PuzzleState& state2) const {
        return state1.cost > state2.cost;
    }
};

// 2D vector to string conversion function
string boardToString(const vector<vector<int>>& board) {
    stringstream ss;
    for (const auto& row : board) {
        for (int value : row) {
            ss << value << ' ';
        }
    }
    return ss.str();
}

// manhattan distance heuristic calcution
int manhattanDistance(const vector<vector<int>>& board, const vector<vector<int>>& goal) {
    int distance = 0;
    int size = board.size();

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            int value = board[i][j];
            if (value != 0) { // tile that is empty is skipped
                for (int k = 0; k < size; ++k) {
                    for (int l = 0; l < size; ++l) {
                        if (value == goal[k][l]) {
                            distance += abs(i - k) + abs(j - l);
                        }
                    }
                }
            }
        }
    }

    return distance;
}

// neighbor state generation and adding them in priority queue
void exploreNeighbors(const PuzzleState& current, const vector<vector<int>>& goal,
                      priority_queue<PuzzleState, vector<PuzzleState>, PuzzleStateCompare>& pq,
                      unordered_set<string>& visited) {
    int size = current.board.size();
    
    // empty tile (zero) position finding
    int emptyRow = -1, emptyCol = -1;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (current.board[i][j] == 0) {
                emptyRow = i;
                emptyCol = j;
                break;
            }
        }
        if (emptyRow != -1) break;
    }

    // moves that are possible
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    // neighbor states generation
    for (int i = 0; i < 4; ++i) {
        int newRow = emptyRow + dx[i];
        int newCol = emptyCol + dy[i];

        // new position is in bounds condition
        if (newRow >= 0 && newRow < size && newCol >= 0 && newCol < size) {
            // new state creation by swipping empty and neighnor tile.
            PuzzleState neighbor = current;
            swap(neighbor.board[emptyRow][emptyCol], neighbor.board[newRow][newCol]);

            // new state visited or not condition
            string neighborString = boardToString(neighbor.board);
            if (visited.find(neighborString) == visited.end()) {
               // // Update the cost and add the neighbor to the priority queue
                neighbor.cost = current.cost + manhattanDistance(neighbor.board, goal);
                pq.push(neighbor);
                visited.insert(neighborString);
            }
        }
    }
}

// Greedy Best-First Search function
void greedyBestFirstSearch(const vector<vector<int>>& initial, const vector<vector<int>>& goal) {
    int size = initial.size();

    // explored states priority queue
    priority_queue<PuzzleState, vector<PuzzleState>, PuzzleStateCompare> pq;

    // set of the states that are visited
    unordered_set<string> visited;

    // to priority queue initial state adding
    PuzzleState start;
    start.board = initial;
    start.cost = manhattanDistance(initial, goal);
    pq.push(start);
    visited.insert(boardToString(initial));

    while (!pq.empty()) {
        // lowest cost state of th puzzle
        PuzzleState current = pq.top();
        pq.pop();

        // current state is the goal state condition
        if (current.board == goal) {
            cout << "Goal state reached!" << endl;
            return;
        }

        // looking through neighbor states
        exploreNeighbors(current, goal, pq, visited);
    }

    cout << "Goal state not reachable!" << endl;
}



int main() {
    //// Example initial and goal states for the 8-puzzle
    cout<<"Enter 9 integer from 0-to-9 like 807612354 \n";
    int a[9];
    for(int i=0; i<9; i++) {
        cout<<"enter an integer and then press Enter: ";
        cin>>a[i];
    }


    vector<vector<int>> initial = {
        {a[0], a[1], a[2]},
        {a[3], a[4], a[5]},
        {a[6], a[7], a[8]}
    };
    
    // vector<vector<int>> initial = {
    //     {1, 2, 3},
    //     {7, 0, 8},
    //     {4, 5, 6}
    // };

    vector<vector<int>> goal = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 0}
    };

    // Execute Greedy BFS
    greedyBestFirstSearch(initial, goal);

    return 0;
}
