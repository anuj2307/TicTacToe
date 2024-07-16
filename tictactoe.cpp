#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

#define LOSS -1000
#define WIN 1000
#define DRAW 0

#define AI_MARKER '0'
#define PLAYER_MARKER 'X'
#define EMPTY_SPACE '-'

#define START_DEPTH 0

vector<vector<pair<int, int>>>winningStates
{
    {{0,0}, {0,1}, {0,2}},
    {{1,0}, {1,1}, {1,2}},
    {{2,0}, {2,1}, {2,2}},

    {{0,0}, {1,0}, {2,0}},
    {{0,1}, {1,1}, {2,1}},
    {{0,2}, {1,2}, {2,2}},

    {{0,0}, {1,1}, {2,2}},
    {{0,2}, {1,1}, {2,0}}

};
void printBoard(vector<vector<char>>&board)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cout<<board[i][j];
            if(j!=2)
            {
                cout<<" | ";
            }
        }
        cout<<endl<< "----------" << endl;
        
    }
    
}
void printGameState(int state)
{
    if(state==WIN)
    {
        cout<<"PLAYER WON"<<endl;
    }
    else if(state==LOSS)
    {
        cout<<"PLAYER LOST"<<endl;
    }
    else
    {
        cout<<"DRAW"<<endl;
    }
}
vector<pair<int,int>>getLegalMoves(vector<vector<char>>&board)
{
    vector<pair<int,int>>ans;
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            if(board[i][j]==EMPTY_SPACE)
            {
                ans.push_back({i,j});
            }
        }
    }
    return ans;
}
bool isOccupied(vector<vector<char>>&board, pair<int,int>pos)
{
    if(board[pos.first][pos.second]!=EMPTY_SPACE)
    {
        return true;
    }
    return false;
}
vector<pair<int,int>>getOccupiedPositions(vector<vector<char>>&board, char marker)
{
    vector<pair<int,int>>ans;
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            if(board[i][j]==marker)
            {
                ans.push_back({i,j});
            }
        }
    }
    return ans;
}
bool isBoardFull(vector<vector<char>>&board)
{
    return getLegalMoves(board).empty();
    
}
bool isGameWon(vector<vector<char>>&board, char marker)
{
   
    vector<pair<int,int>>occupied_positions = getOccupiedPositions(board, marker);
    bool ans;
    for(int i=0;i<winningStates.size();i++)
    {
        ans = true;
        for(int j=0;j<3;j++)
        {
            if(find(occupied_positions.begin(), occupied_positions.end(), winningStates[i][j])==occupied_positions.end())
            {
                ans = false;
                break;
            }
        }
        if(ans)
        {
            break;
        }
    }
    return ans;
}
char getOpponentMarker(char marker)
{
    if(marker==AI_MARKER)
    {
        return PLAYER_MARKER;

    }
    return AI_MARKER;
}
int getGameState(vector<vector<char>>&board, char marker)
{
    char opponentMarker = getOpponentMarker(marker);
    if(isGameWon(board, marker))
    {
        return WIN;
    }
    
    if(isGameWon(board, opponentMarker))
    {
        return LOSS;
    }
    return DRAW;
}
pair<int, pair<int,int>> miniMaxOptimization(vector<vector<char>>&board, int alpha, int beta, int depth, char marker)
{
    int gameState = getGameState(board, AI_MARKER);
    pair<int,int>bestMove = {-1, -1};
    int bestScore = (marker==AI_MARKER)?LOSS:WIN;
    if(isBoardFull(board)||gameState!=DRAW)
    {
        bestScore = gameState;
        return {bestScore, bestMove};
    }
    vector<pair<int,int>>legalMoves = getLegalMoves(board);
    for(int i=0;i<legalMoves.size();i++)
    {
        pair<int,int>currMove = legalMoves[i];
        board[currMove.first][currMove.second] = marker;
        if(marker==AI_MARKER)
        {
            int score = miniMaxOptimization(board, alpha, beta, depth+1, PLAYER_MARKER).first;
            if(score>bestScore)
            {
                bestScore = score - depth*10;
                bestMove = currMove;
                alpha = max(alpha, bestScore);
                
            }       
        }
        else
        {
            int score = miniMaxOptimization(board, alpha, beta, depth+1, AI_MARKER).first;
            if(score<bestScore)
            {
                bestScore = score + depth*10;
                bestMove = currMove;
                beta = min(beta, bestScore);
                
            }
        }
        board[currMove.first][currMove.second] = EMPTY_SPACE;
        if(beta<=alpha)
        {
            break;  
        }
        
    }
    return {bestScore, bestMove};
}
bool isGameDone(vector<vector<char>>&board)
{
    if(isBoardFull(board)||getGameState(board, AI_MARKER)!=DRAW)
    {
        return true;
    }
    return false;
}
int main()
{
    vector<vector<char>>board(3, vector<char>(3, EMPTY_SPACE));
    cout << "\n\n\tTic Tac Toe AI\n\n**" << endl << endl;
	cout << "Player = X\t AI Computer = O" << endl << endl;
    printBoard(board);
	
    while(!isGameDone(board))
    {
        int row, col;
		cout << "Row play: ";
		cin >> row;
		cout << "Col play: ";
		cin >> col;
		cout << endl << endl;
        if(row>2||row<0||col>2||col<0)
        {
            cout<<"Invalid Move. Please enter Values between 0 and 2."<<endl;
            continue;
        }

		if (isOccupied(board, {row,col}))
		{
			cout << "The position (" << row << ", " << col << ") is occupied. Try another one..." << endl;
			continue;
		}
		else
		{
			board[row][col] = PLAYER_MARKER;
		}
        if(isGameDone(board))
        {
            break;
        }
        pair<int,int>aiMove = miniMaxOptimization(board, LOSS, WIN, START_DEPTH, AI_MARKER).second;
        //cout<<aiMove.first<<aiMove.second;
        board[aiMove.first][aiMove.second] = AI_MARKER;
        printBoard(board);

    }
    cout << "********** GAME OVER **********" << endl << endl;

	int playerState = getGameState(board, PLAYER_MARKER);
    printGameState(playerState);

    return -1;
}
