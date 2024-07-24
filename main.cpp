#include <iostream>
#include <math.h>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

bool InBoard(int row, int col){
    if((row*8 + col) > 64){return false;}
    if((row*8 + col) < 0){return false;}
    return true;
}

bool finds(string searchfor, string astring){
    if(astring.find(searchfor) != string::npos){
            return true;
    }
    else{return false;}
}

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
void GetDestination(string &selectedPiece, string &selectedTile,int &pieceColumn, int &pieceRow, int &destinationColumn, int &destinationRow, string player, vector<string> &board){
    cout<<"pick column of piece destination"<<endl;
    cin>>destinationColumn;

    cout<<"pick where row of piece destination"<<endl;
    cin>>destinationRow;

    selectedTile = board.at(((destinationRow-1)*8) + (destinationColumn-1));
    while(selectedTile.find(player) != string::npos){
        PrintBoard(board);
        cout<<"INVALID!"<<endl;
        GetPiece(selectedPiece, pieceColumn,pieceRow, player,board);
        cout<<"pick column of piece destination"<<endl;
        cin>>destinationColumn;
        cout<<"pick row of piece destination:"<<endl;
        cin>>destinationRow;
        selectedTile = board.at(((destinationRow-1)*8) + (destinationColumn-1));
    }
}
void Retry(string name, string &selectedPiece,string &selectedTile, int &pieceColumn, int &pieceRow, int &destinationColumn, int &destinationRow, string player, vector<string> &board){
    PrintBoard(board);
    cout<<"Invalid "<<name<<" move"<<endl;
    GetPiece(selectedPiece, pieceColumn,pieceRow, player,board);
    GetDestination(selectedPiece,selectedTile,pieceColumn,pieceRow,destinationColumn,destinationRow,player,board);
}

string CheckSafety(int destinationRow,int destinationColumn, string player, vector<string> &board, int &attackerRow, int &attackerCol){
    string tempPiece, enemy = "2";
    if(player == "2"){enemy = "1";}

    cout<<"begin knight check"<<endl;
    int knights[8] = {6, 10, 15, 17, -6, -10, -15, -17};//possible knight tiles
    for(int i = 0; i < 8; i++){
        if(destinationRow*8 + destinationColumn + knights[i] < 0){continue;}
        if(destinationRow*8 + destinationColumn + knights[i] > 64){continue;}
        tempPiece = board.at(destinationRow*8 + destinationColumn + knights[i]);
        //cout<<tempPiece<<endl;
        if(finds(enemy, tempPiece) && finds("H", tempPiece)){return "unsafe";}
    }
    cout<<"passed knight check"<<endl;


    cout<<"begin perp check"<<endl;
    int perps[2] = {1, -1};
    for(int i = 0; i < 2; i++){
        int add = perps[i];
        while(InBoard(destinationRow, destinationColumn + add)){
            tempPiece = board.at((destinationRow)*8 + (destinationColumn + add));
            cout<<tempPiece<<endl;
            if(finds(player, tempPiece)){break;}
            if(finds(enemy, tempPiece) && (finds("R",tempPiece)||finds("Q", tempPiece))){return "unsafe";}
            add+=add;
        }
    }
    for(int i = 0; i < 2; i++){
        int add = perps[i];
        while(InBoard(destinationRow + add, destinationColumn)){
            tempPiece = board.at((destinationRow + add)*8 + (destinationColumn));
            cout<<tempPiece<<endl;
            if(finds(player, tempPiece)){break;}
            if(finds(enemy, tempPiece) && (finds("R",tempPiece)||finds("Q", tempPiece))){return "unsafe";}
            add+=add;
        }
    }
    cout<<"passed perp check"<<endl;


    cout<<"begin diag check"<<endl;
    int diags[2] = {-1, 1};
    for(int i = 0; i < 2; i++){
        for(int k = 0; k <2; k++){
            int addRow = diags[i];
            int addCol = diags[k];
            while(InBoard(destinationRow + addRow, destinationColumn+ addCol )){
                tempPiece = board.at((destinationRow + addRow)*8 + (destinationColumn + addCol));
                cout<<tempPiece<<endl;
                if(finds(player, tempPiece)){break;}
                if(finds(enemy, tempPiece) && (finds("B",tempPiece)||finds("Q", tempPiece))){return "unsafe";}
                addRow+=addRow;
                addCol+=addCol;
            }
        }
    }
    cout<<"passed diag check"<<endl;

    return "safe";
}

void CheckValid(string &selectedPiece,string &selectedTile, int &pieceColumn, int &pieceRow, int &destinationColumn, int &destinationRow, string player, vector<string> &board){
start:
    string tempPiece;
    int tempHIGH, tempLOW, row ,column;
    while(true){

        while(selectedPiece.find("P") != string::npos){//Pawn selected
            if((player == "1" && (destinationRow <= pieceRow))||(player == "2" && (destinationRow >= pieceRow))){
                Retry("Pawn", selectedPiece,selectedTile, pieceColumn, pieceRow, destinationColumn,destinationRow, player, board);
                goto start;
            }
            if((player == "1" && destinationColumn != pieceColumn && selectedTile.find("2")==string::npos)||(player == "2" && destinationColumn != pieceColumn && selectedTile.find("1")==string::npos)){
                Retry("Pawn", selectedPiece,selectedTile, pieceColumn, pieceRow, destinationColumn,destinationRow, player, board);
                goto start;
            }
            if((abs(destinationRow - pieceRow))>=2 && ((player == "1" && pieceRow != 2)||(player == "2" && pieceRow != 7))){
                Retry("Pawn", selectedPiece,selectedTile, pieceColumn, pieceRow, destinationColumn,destinationRow, player, board);
                goto start;
            }
            if(sqrt(pow((destinationColumn-pieceColumn),2) + pow((destinationRow-pieceRow), 2)) > 2){
                Retry("Pawn", selectedPiece,selectedTile, pieceColumn, pieceRow, destinationColumn,destinationRow, player, board);
                goto start;
            }
            break;
        }
        while(selectedPiece.find("R") != string::npos){//Rook selected
            //cout<<"enters while"<<endl;
            if((pieceRow != destinationRow && pieceColumn!=destinationColumn)){
                Retry("Rook", selectedPiece,selectedTile, pieceColumn, pieceRow, destinationColumn,destinationRow, player, board);
                goto start;
            }
            if(pieceColumn==destinationColumn){
                if(pieceRow>destinationRow){
                    tempHIGH = pieceRow;
                    tempLOW = destinationRow;
                }
                else{
                    tempHIGH = destinationRow;
                    tempLOW = pieceRow+1;
                }
                for(;tempLOW < tempHIGH; tempLOW++){
                    if(board.at(((tempLOW-1)*8) + (pieceColumn-1)) != "XX"){
                        Retry("Rook", selectedPiece,selectedTile, pieceColumn, pieceRow, destinationColumn,destinationRow, player, board);
                        goto start;
                    }
                }
            }
            else{
                if(pieceColumn>destinationColumn){
                    tempHIGH = pieceColumn;
                    tempLOW = destinationColumn;
                }
                else{
                    tempHIGH = destinationColumn;
                    tempLOW = pieceColumn+1;
                }
                for(;tempLOW < tempHIGH; tempLOW++){//1 5
                    //cout<<tempLOW<<endl;
                    if(board.at(((pieceRow-1)*8) + (tempLOW-1)) != "XX"){
                        Retry("Rook", selectedPiece,selectedTile, pieceColumn, pieceRow, destinationColumn,destinationRow, player, board);
                        goto start;

                    }
                }
            }
            break;
        }
        while(selectedPiece.find("H") != string::npos){//Knight selected
            if(((abs(pieceColumn-destinationColumn) == 1)&&(abs(pieceRow-destinationRow)==2))||((abs(pieceColumn-destinationColumn) == 2)&&(abs(pieceRow-destinationRow)==1))){
                return;
            }
            else{
                Retry("Knight", selectedPiece,selectedTile, pieceColumn, pieceRow, destinationColumn,destinationRow, player, board);
                goto start;
            }


        }
        while(selectedPiece.find("B") != string::npos){//Bishop selected
            if(abs(pieceColumn-destinationColumn) != abs(pieceRow-destinationRow)){
                Retry("Rook", selectedPiece,selectedTile, pieceColumn, pieceRow, destinationColumn,destinationRow, player, board);
                goto start;
            }
            else {return;}
        }
        while(selectedPiece.find("Q") != string::npos){//Queen selected
            if(abs(pieceColumn-destinationColumn) == abs(pieceRow-destinationRow)){
                return;
            }
            if((sqrt(pow((destinationColumn-pieceColumn),2) + pow((destinationRow-pieceRow), 2)) > 1) && (pieceColumn != destinationColumn) && (pieceRow != destinationRow)){
                Retry("Queen", selectedPiece,selectedTile, pieceColumn, pieceRow, destinationColumn,destinationRow, player, board);
                goto start;
            }
            return;
        }
        while(selectedPiece.find("K") != string::npos){//King selected
            if((sqrt(pow((destinationColumn-pieceColumn),2) + pow((destinationRow-pieceRow), 2)) > 1)) {
                Retry("King", selectedPiece,selectedTile, pieceColumn, pieceRow, destinationColumn,destinationRow, player, board);
                goto start;
            }
            if((player == "2" && ((board.at(((destinationRow-1-1)*8) + (destinationColumn-1-1)).find("P") != string::npos)||(board.at(((destinationRow-1-1)*8) + (destinationColumn)).find("P") != string::npos)))||(player == "1" && ((board.at(((destinationRow)*8) + (destinationColumn-1-1)).find("P") != string::npos)||(board.at(((destinationRow)*8) + (destinationColumn)).find("P") != string::npos)))){
                Retry("King", selectedPiece,selectedTile, pieceColumn, pieceRow, destinationColumn,destinationRow, player, board);
                goto start;

            }
            for(column = 0; column < 3; column++){
                for(row = 0; row< 3; row++){
                    tempPiece = (board.at((destinationRow+row-1-1)*8 + (destinationColumn+column-1-1)));
                    if((tempPiece.find(player) == string::npos && tempPiece.find("Q") != string::npos)||(tempPiece.find(player) == string::npos && tempPiece.find("K") != string::npos)){
                        Retry("King", selectedPiece,selectedTile, pieceColumn, pieceRow, destinationColumn,destinationRow, player, board);
                        goto start;
                    }
                }
            }
            ////Knight check
            int knightColumns[8] = {-1, -2, 1, 2, -1 , 2,  1, -2 };
            int knightRows[8] =    {-2, -1, 2, 1,  2, -1, -2,  1};
            //cout<<"brah"<<endl;
            for(int k = 0; k < 8; k++){
                //cout<<board.at((destinationRow-1+knightRows[k])*8 + (destinationColumn-1+knightColumns[k]))<<endl;
                tempPiece = board.at((destinationRow-1+knightRows[k])*8 + (destinationColumn-1+knightColumns[k]));
                if(tempPiece.find("H") != string::npos && tempPiece.find(player) == string::npos){
                    Retry("King", selectedPiece,selectedTile, pieceColumn, pieceRow, destinationColumn,destinationRow, player, board);
                    //cout<<"pluh"<<endl;
                    goto start;
                }
            }
            /////////////////
            int rows[8]    = {0, 0, 2, 2, -1, 0, 0, 1};
            int columns[8] = {0, 2, 0, 2,  0, 1, -1, 0 };

            int rowsop[8]    = {-1, -1, 1, 1, -1, 0, 1, 0};
            int columnsop[8] = {-1, 1, -1, 1, 0, 1, 0, -1};

            for(int k = 4; k < 8; k++){// PERPENDICULAR
                row = rows[k];
                column = columns[k];
                while(true){
                    tempPiece = board.at((destinationRow-1+row)*8 + (destinationColumn-1+column));//temp piece to check validity
                    if(tempPiece.find(player) != string::npos){break;continue;}
                    if(tempPiece.find(player) == string::npos && tempPiece.find("Q")==string::npos && tempPiece.find("R")==string::npos && tempPiece.find("XX")==string::npos){break;continue;}

                    if(tempPiece.find("Q")!=string::npos||tempPiece.find("R")!=string::npos){
                        Retry("King", selectedPiece,selectedTile, pieceColumn, pieceRow, destinationColumn,destinationRow, player, board);
                        goto start;
                    }
                    //cout<<(destinationColumn+column-1)<<" "<< (destinationRow+row-1)<<endl;
                    row += rowsop[k];
                    column += columnsop[k];
                }

            }

            for(int k = 0; k < 4; k++){//DIAGONALS
                row = rows[k];
                column = columns[k];
                while(true){
                    tempPiece = board.at((destinationRow-1-1+row)*8 + (destinationColumn-1-1+column));
                    if(tempPiece.find(player) != string::npos){break;continue;}
                    if(tempPiece.find(player) == string::npos && tempPiece.find("Q")==string::npos && tempPiece.find("B")==string::npos && tempPiece.find("XX")==string::npos){break;continue;}

                    if(tempPiece.find("Q")!=string::npos||tempPiece.find("B")!=string::npos){
                        Retry("King", selectedPiece,selectedTile, pieceColumn, pieceRow, destinationColumn,destinationRow, player, board);
                        goto start;
                    }
                    //cout<<(destinationColumn+column-1)<<" "<< (destinationRow+row-1)<<endl;
                    row += rowsop[k];
                    column += columnsop[k];
                }
            }

        }

        return;
    }
}

void MovePiece(int pieceRow, int pieceColumn, int destinationRow, int destinationColumn, string selectedPiece, vector<string> &board){
    board.at(((pieceRow-1)*8) + (pieceColumn-1)) = "XX";
    board.at(((destinationRow-1)*8) + (destinationColumn-1)) = selectedPiece;
}


void DefensePerp(){

}

void DefenseDiag(){

}



string Result(string player, vector<string> &board){
    bool underAttack = false;
    int attackerRow, attackerCol,attackerRow2, attackerCol2;
    string safety, tempPiece, enemyPlayer = "1";
    if(player == "1"){enemyPlayer = "2";}

    for(int kingCol = 0; kingCol < 8; kingCol++){
        for(int kingRow = 0; kingRow < 8; kingRow++){
            tempPiece = board.at((kingRow*8)+kingCol);
            //cout<<kingRow<<" "<<kingCol<<endl;

            if(finds("K", tempPiece) && finds(player, tempPiece)){//finds a king
                cout<<"king found at "<<kingRow+1<<" "<<kingCol+1<<endl;
                cout<<"king vector row = "<<kingRow<<", king vector column = "<<kingCol<<endl;

                underAttack = CheckSafety(kingRow, kingCol, player, board, attackerRow, attackerCol) != "safe";//check if king is under attack

                if(underAttack){

                    for(int row = 0; row < 3; row++){//check possible escape tiles
                        for(int col = 0; col < 3; col++){
                            if(!InBoard(kingRow+row - 1, kingCol+col - 1)){continue;}
                            tempPiece = board.at((kingRow+row -1)*8 + (kingCol+col - 1));
                            if(finds(player, tempPiece)){continue;}
                            if(CheckSafety(kingRow+row-1, kingCol+col-1, player, board, attackerRow2, attackerCol2) == "safe"){
                                return "none";
                            }

                        }
                    }

                    //no escape tiles, check if attacker is vulnerable, and if they are, that defending yourself actually saves you
                    //archive last diagonal/perpendicular row/col they attacked, and check whether or not you can move one of your pieces to block it, also checking that doing so doesnt leave you vulnerable
                    //I need a list of all possible defenders and then evaluate if any can defend, maybe can be done by taking the checksafety and instead of returning, appending each attacker into a list where I then check if any can defend safely, To "theoretically" check we can add a conditional to continue if it reaches our proposed defender

                    safety = CheckSafety(attackerRow, attackerCol, enemyPlayer, board, attackerRow2, attackerCol2);
                    if(safety != "safe"){

                    }

                    return"checkamte i guess";


                }

            }
        }

    }

    return "none";
}


int main()
{

    string selectedPiece, selectedTile;
    string player = "1";
    int pieceRow, pieceColumn, destinationRow, destinationColumn;
    vector<string> board = {"H1", "XX", "Q1", "B1", "XX","R1","R1","R1",//17 10 horse
                            "P1", "P1", "K1", "Q2", "XX","P1","P1","P1",
                            "XX", "Q1", "Q1", "Q1", "R2","H2","XX","XX",
                            "XX", "B2", "XX", "XX", "XX","XX","XX","B2",
                            "XX", "XX", "XX", "XX", "XX","XX","XX","XX",
                            "XX", "XX", "XX", "XX", "XX","XX","XX","XX",
                            "P2", "P2", "P2", "P2", "P2","P2","P2","P2",
                            "R2", "H2", "B2", "Q2", "K2","B2","H2","R2"};


    while(Result(player, board) == "none"){

        PrintBoard(board);
        cout<<"Player "<<player<<"'s turn, ";

        GetPiece(selectedPiece, pieceColumn,pieceRow, player,board);

        GetDestination(selectedPiece,selectedTile,pieceColumn,pieceRow,destinationColumn,destinationRow,player,board);

        CheckValid(selectedPiece,selectedTile, pieceColumn, pieceRow, destinationColumn,destinationRow, player, board);

        cout<<"check valid done"<<endl;
        MovePiece(pieceRow, pieceColumn, destinationRow, destinationColumn, selectedPiece, board);
        if (player == "1")player = "2";
        else player = "1";

    }

    cout<<Result(player, board)<<player<<endl;
    return 0;
}
