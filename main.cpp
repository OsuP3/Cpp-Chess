#include <iostream>
#include <math.h>
#include <vector>
#include <string>
using namespace std;

void PrintBoard(vector<string> board){
    string num[] = {"1", "2", "3","4","5","6","7","8"};
    cout<<"  1 2 3 4 5 6 7 8"<<endl;
    for(int i = 0; i < 8; i++){
        cout<<num[i]<<" ";
        for(int k = 0; k < 8; k++){
            cout<<board.at(k + (8*i));
        }
        cout<<"\n";
    }
}

void GetDestination(string &selectedTile, int &destinationColumn, int &destinationRow, string player, vector<string> &board){
    cout<<"pick column of piece destination"<<endl;
    cin>>destinationColumn;

    cout<<"pick where row of piece destination"<<endl;
    cin>>destinationRow;

    selectedTile = board.at(((destinationRow-1)*8) + (destinationColumn-1));
    while(selectedTile.find(player) != string::npos){
        PrintBoard(board);
        cout<<"INVALID!\npick a column of piece destination"<<endl;
        cin>>destinationColumn;
        cout<<"pick row of piece destination:"<<endl;
        cin>>destinationRow;
        selectedTile = board.at(((destinationRow-1)*8) + (destinationColumn-1));
    }
}

void GetPiece(string &selectedPiece, int &pieceColumn, int &pieceRow, string player, vector<string> &board){
    cout<<"pick column of piece to move"<<endl;
    cin>>pieceColumn;
    cout<<"pick row of piece to move:"<<endl;
    cin>>pieceRow;

    selectedPiece = board.at(((pieceRow-1)*8) + (pieceColumn-1));

    while(selectedPiece.find(player) == string::npos){
        PrintBoard(board);
        cout<<"INVALID!\npick a column of piece to move"<<endl;
        cin>>pieceColumn;
        cout<<"pick row of piece to move:"<<endl;
        cin>>pieceRow;
        selectedPiece = board.at(((pieceRow-1)*8) + (pieceColumn-1));
    }

}

void CheckValid(string &selectedPiece,string &selectedTile, int &pieceColumn, int &pieceRow, int &destinationColumn, int &destinationRow, string player, vector<string> &board){
    string tempPiece;
    while(selectedPiece.find("P") != string::npos){//Pawn selected
        if((player == "1" && (destinationRow <= pieceRow))||(player == "2" && (destinationRow >= pieceRow))){
            PrintBoard(board);
            cout<<"Pawn cannot move backwards"<<endl;//cannot move backwards
            GetDestination(selectedTile, destinationColumn, destinationRow, player,  board);
            continue;
        }
        if((player == "1" && destinationColumn != pieceColumn && selectedTile.find("2")==string::npos)||(player == "2" && destinationColumn != pieceColumn && selectedTile.find("1")==string::npos)){
            PrintBoard(board);
            cout<<"Invalid Pawn move"<<endl;//cant leave column unless taking
            GetDestination(selectedTile, destinationColumn, destinationRow, player,  board);
            continue;
        }
        if((abs(destinationRow - pieceRow))>=2 && ((player == "1" && pieceRow != 2)||(player == "2" && pieceRow != 7))){
            PrintBoard(board);
            cout<<"Invalid Pawn move"<<endl;//no invalid double steps
            GetDestination(selectedTile, destinationColumn, destinationRow, player,  board);
            continue;
        }
        if(sqrt(pow((destinationColumn-pieceColumn),2) + pow((destinationRow-pieceRow), 2)) > 2){
            PrintBoard(board);
            cout<<"Invalid Pawn move"<<endl;//pawn moves too far
            GetDestination(selectedTile, destinationColumn, destinationRow, player,  board);
            continue;
        }
        break;
    }

    while(selectedPiece.find("R") != string::npos){//Rook selected
        if((pieceRow != destinationRow && pieceColumn!=destinationColumn)){
            PrintBoard(board);
            cout<<"Invalid Rook move"<<endl;//rook moves diagonally
            GetDestination(selectedTile, destinationColumn, destinationRow, player,  board);
            continue;
        }
    }

}


void MovePiece(int pieceRow, int pieceColumn, int destinationRow, int destinationColumn, string selectedPiece, vector<string> &board){
    board.at(((pieceRow-1)*8) + (pieceColumn-1)) = "XX";
    board.at(((destinationRow-1)*8) + (destinationColumn-1)) = selectedPiece;
}

string Result(vector<string> board){
    return "none";
}

int main()
{
    string selectedPiece, selectedTile;
    string player = "2";
    int pieceRow, pieceColumn, destinationRow, destinationColumn;
    vector<string> board = {"R1", "H1", "B1", "Q1", "K1","B1","H1","R1",
                            "P1", "P1", "P1", "P1", "P1","P1","P1","P1",
                            "XX", "XX", "XX", "XX", "XX","XX","XX","XX",
                            "XX", "XX", "XX", "XX", "XX","XX","XX","XX",
                            "XX", "XX", "XX", "XX", "XX","XX","XX","XX",
                            "XX", "XX", "XX", "XX", "XX","XX","XX","XX",
                            "P2", "P2", "P2", "P2", "P2","P2","P2","P2",
                            "R2", "H2", "B2", "Q2", "K2","B2","H2","R2"};


    while(Result(board) == "none"){
        PrintBoard(board);
        cout<<"Player "<<player<<"'s turn, ";

        GetPiece(selectedPiece, pieceColumn,pieceRow, player,board);

        GetDestination(selectedTile, destinationColumn, destinationRow, player,  board);

        CheckValid(selectedPiece,selectedTile, pieceColumn, pieceRow, destinationColumn,destinationRow, player, board);

        MovePiece(pieceRow, pieceColumn, destinationRow, destinationColumn, selectedPiece, board);


        if (player == "1")player = "2";
        else player = "1";
    }

    return 0;
}
