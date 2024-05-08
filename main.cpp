#include <iostream>
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
string Result(vector<string> board){
    return "none";
}

int main()
{
    string selectedPiece, selectedTile;
    string player = "1";
    int pick1, pick2, pick3, pick4;
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

        cout<<"Player "<<player<<"'s turn, pick column of piece to move"<<endl;
        cin>>pick2;
        cout<<"pick row of piece to move:"<<endl;
        cin>>pick1;

        selectedPiece = board.at(((pick1-1)*8) + (pick2-1));

        while(selectedPiece.find(player) == string::npos){
            PrintBoard(board);
            cout<<"INVALID!\npick a column of piece to move"<<endl;
            cin>>pick2;
            cout<<"pick row of piece to move:"<<endl;
            cin>>pick1;
            selectedPiece = board.at(((pick1-1)*8) + (pick2-1));
        }

        cout<<"pick column of piece destination"<<endl;
        cin>>pick4;

        cout<<"pick where row of piece destination"<<endl;
        cin>>pick3;

        selectedTile = board.at(((pick3-1)*8) + (pick4-1));
        while(selectedTile.find(player) != string::npos){
            PrintBoard(board);
            cout<<"INVALID!\npick a column of piece destination"<<endl;
            cin>>pick4;
            cout<<"pick row of piece destination:"<<endl;
            cin>>pick3;
            selectedTile = board.at(((pick3-1)*8) + (pick4-1));
        }


        board.at(((pick1-1)*8) + (pick2-1)) = "XX";

        board.at(((pick3-1)*8) + (pick4-1)) = selectedPiece;

        if (player == "1")player = "2";
        else player = "1";
    }

    return 0;
}
