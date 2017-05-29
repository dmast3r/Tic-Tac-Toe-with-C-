#include <bits/stdc++.h>
using namespace std;
// A utility function to check if there is a three consecutive match in row, column or diagonal.
// row represents row number, column represents column number, board is a 3 x 3 matrix that stores the current states of the game. Symbol is a move,
bool ismatch(int row, int column, int (*board)[3], char symbol) { // symbol represents O or X.
	int value = symbol - '0'; // get the value from symbol. X is set to the value 1 and O is set to 0.
	bool row_match = true, column_match = true, diagonal_match = true;
	// check for a row match
	for(int i = 0; i < 3; ++i) {
		if(board[row][i] != value) {
			row_match = false;
			break;
		}
	}
	// check for a column match
	for(int i = 0; i < 3; ++i) {
		if(board[i][column] != value) {
			column_match = false;
			break;
		}
	}
	// check for a diagonal match, either the principal diagonal or the other.
	if(row == column or (row == 2 and column == 0) or (column == 2 and row == 0)) {
        if(row == column) { // check principal diagonal
            if(row != 1 and column != 1) {
                for(int i = 0; i < 3; ++i) {
                    if(board[i][i] != value) {
                        diagonal_match = false;
                        break;
                    }
                }
            }
            else { // check for principal and other diagonal at same time(when the cell being filled is 1, 1 then we will have to check both the diagonals
                for(int i = 0; i < 3; ++i) {
                    if(board[i][i] != value) {
                        diagonal_match = false;
                        break;
                    }
                }
                if(board[0][2] == board[1][1] and board[2][0] == board[1][1] and board[1][1] == value)
                    diagonal_match = true;
            }
        }
        else { // check for the other diagonal
            for(int i = 0; i < 3; ++i) {
                if(board[i][2-i] != value) {
                    diagonal_match = false;
                    break;
                }
            }
        }
	}
	else
        diagonal_match = false; // if the current cell does not lie in any of the diagonal then there is no role of diagonal hence set it to false.
    return row_match || column_match || diagonal_match; // if any one of them is true than return true(i.e. there is a match)
}
// This is just for printing a particular cell
void printpattern(int val) {
    if(val == -1)
        cout<<" "; // if -1 then it means there is no move in this cell so print nothing
    else if(val == 1)
        cout<<"X"; // 1 means X
    else
        cout<<"O"; // 0 means O
}
// This is the utility function for printing the entire board.
void printTicTacToe(int (*board)[3]) {
	for(int i = 0; i < 2; ++i) {
		for(int j = 0; j < 2; ++j) {
            printpattern(board[i][j]);
            cout << " | ";
		}
		printpattern(board[i][2]);
		cout << "\n----------" << endl;
	}
	for(int i = 0; i < 2; ++i) {
        printpattern(board[2][i]);
        cout << " | ";
	}
	printpattern(board[2][2]);
    cout << endl << endl << endl;
}
// A utility function that applies Minimax algorithm
int minimax(int moves, int (*board)[3], bool computer_turn) {
	if(moves == 9)
		return 0; // if all the moves have been exhausted then return 0;
 	if(computer_turn) { // if it is Computer's turn to play then act as Maximizer.
		int max_score = -10; // start with the least possible score.
		for(int i = 0; i < 3; ++i) {
			for(int j = 0; j < 3; ++j) {
				if(board[i][j] == -1) { // if there is an unfilled cell
					board[i][j] = 0; // let a move be made on the i row and j column
					if(ismatch(i, j, board, '0')) { // check if this move leads to a three consecutive match.
                        board[i][j] = -1; // reset the cell
						return 10; // and return 10;
					}
					int score = minimax(moves + 1, board, false); // if not a consecutive match than recursively try the Minimax algorithm
					if(score == 10) { // check if the current move fetch a win
                        board[i][j] = -1;
                        return score;  // if yes then make this move(as maximizer and return)
					}
					else if(score == 0) // if draw then update the maximum score(it will be either 0 or -10 before this as the functions returns on getting 10)
                        max_score = 0; // update the max score
					board[i][j] = -1;
				}
			}
		}
		return max_score; // return the Max Score predicted from all possible moves.
	}
	else { // else act as a minimizer
        // rest of the code is similar as for maximizer except some minor changes like min in place of max, 1 in place of 0, etc.
		int min_score = 10; // start with least possible score.
		for(int i = 0; i < 3; ++i) {
			for(int j = 0; j < 3; ++j) {
				if(board[i][j] == -1) {
					board[i][j] = 1;
					if(ismatch(i, j, board, '1')) {
                        board[i][j] = -1;
						return -10;
					}
					int score = minimax(moves + 1, board, true);
					if(score == -10) {
                        board[i][j] = -1;
                        return score;
					}
					else if(score == 0)
                        min_score = 0;
					board[i][j] = -1;
				}
			}
		}
		return min_score;
	}
}
// A utility function to find the next move for Computer.
void nextmove(int moves, int (*board)[3]) {
	pair<int, int> Move; // this store row and column of the next move.
	for(int i = 0; i < 3; ++i) {
	    for(int j = 0; j < 3; ++j) {
            if(board[i][j] == -1) { // if there is an unfilled cell
                board[i][j] = 0;
                if(ismatch(i, j, board, '0')) { // again check for a consecutive match.
                    cout<< "\n\nMy Play :-\n\n";
                    printTicTacToe(board);
                    cout << endl << "You Lose!"; // if found then end the game with a win for Computer(lose for player)
                    exit(0); // terminate the game.
                }
                int score = minimax((moves+1), board, false); // else search for best move using minimax.
                if(score == 10 or score == 0) { // if current move fetches a win or draw
                    if(score == 10) {// if it is a win then return 10(max score)
                       board[i][j] = 0; // make this move (that leads to win)
                        return;
                    }
                    // else if the score is zero store the current move as the next move
                    Move.first = i;
                    Move.second = j;
                }
                board[i][j] = -1; // reset the cell.
            }
	    }
	}
	board[Move.first][Move.second] = 0; // make the move that leads to draw.
}
// A utility function to play the game.
void play() {
    // total move stores the number of moves made so far, row_move and column move hold the row and column of user's move.
	int total_moves = 0, row_move, column_move, board[3][3] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
	while(total_moves < 9) { // while there are still some moves left.
		if(total_moves % 2 == 0) { // even moves(0, 2, 4, 6 and 8) are played by user.
			cout <<"Tell the position of your next move: ";
			cin >> row_move >> column_move;
			while(board[row_move - 1][column_move - 1] != -1) { // while the user not enter a valid move keep asking
                cout << "Sorry! Invalid Move. Try Again...\n";
                cout <<"Tell the position of your next move: ";
                cin >> row_move >> column_move;
			}
			board[row_move - 1][column_move - 1] = 1; // make a move when it is valid
			cout<< "\n\nYour Play :-\n\n";
			printTicTacToe(board); // print the TicTacToe board after the player's move
			if(ismatch(row_move - 1, column_move - 1, board, '1')) { // check if the current move made a match
				cout << endl << "You Win!"; // if yes make the player win (ofcourse it won't happen if the algorithm is correct)
				exit(0); // and end the game.
			}
		}
		else { // odd moves are played by computer
			nextmove(total_moves, board); // play the best move using minimax.
			cout<< "\n\nMy Play :-\n\n";
			printTicTacToe(board); // print the TicTacToe board after computer's move
		}
		++total_moves; // increment the number of moves by 1.
	}
	cout << "\n\nGame Over!\nGame ends in a Draw."; // if no match found by either player or computer than its a draw.
}

int main() {
	cout << "----------------------------------------------- Tic Tac Toe By Sourabh ------------------------------------------\n\n | |\n-----\n | |\n-----\n | |\n\n\n";
	play(); // play the game.
}
