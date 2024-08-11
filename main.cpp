#include <iostream>
#include <math.h>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
class piece{
public:
    piece();
    piece(int inputrow, int inputcolumn, string inputpiece){
        row = inputrow;
        column = inputcolumn;
        pieceType = inputpiece[0];
        pieceName = inputpiece;
    }
    piece(int inputrow, int inputcolumn, string inputpiece, int inputRowOp, int inputColOp){
        row = inputrow;
        column = inputcolumn;
        pieceType = inputpiece[0];
        pieceName = inputpiece;
        RowOp = inputRowOp;
        ColumnOp = inputColOp;
    }
    void printCoords(){cout<<"vector row = "<<row<<"|| vector column = "<<column<<endl;}
    int Row(){return row;}
    int Col(){return column;}
    string Type(){return pieceType;}
    int GetColOp(){return ColumnOp;}
    int GetRowOp(){return RowOp;}
    string GetName(){return pieceName;}
    void SetName(string inputName){pieceName = inputName;}
    void SetRow(int inputRow){row = inputRow;}
    void SetCol(int inputCol){column = inputCol;}
private:
    int row;
    int column;
    string pieceType;
    int RowOp;//what row operation led to attack (for keeping track of direction of attack)
    int ColumnOp;//what column operation led to attack
    string pieceName;
};

bool InBoard(int row, int col){//check coordinates are on board
    if(row >= 8 || row < 0 || col >= 8 || col < 0){
        cout<<"out of board"<<endl;
        return false;
    }
    return true;
}
bool finds(string phrase, string astring){//find a phrase in a string
    if(astring.find(phrase) != string::npos){
            return true;
    }
    else{return false;}
}
void PrintBoard(vector<string> board){//Prints the board
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
void GetPiece(string &selectedPiece, int &pieceColumn, int &pieceRow, string player, vector<string> &board){//Prompts player for piece
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
void GetDestination(string &selectedPiece, string &selectedTile,int &pieceColumn, int &pieceRow, int &destinationColumn, int &destinationRow, string player, vector<string> &board){//Prompts player for destination
    cout<<"pick column of piece destination"<<endl;
    cin>>destinationColumn;

    cout<<"pick where row of piece destination"<<endl;
    cin>>destinationRow;

    selectedTile = board.at(((destinationRow-1)*8) + (destinationColumn-1));

    while(finds(player, selectedTile)){
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
void Retry(string name, string &selectedPiece,string &selectedTile, int &pieceColumn, int &pieceRow, int &destinationColumn, int &destinationRow, string player, vector<string> &board){//RePrompts player for piece and destination
    PrintBoard(board);
    cout<<"Invalid "<<name<<" move"<<endl;
    GetPiece(selectedPiece, pieceColumn,pieceRow, player,board);
    GetDestination(selectedPiece,selectedTile,pieceColumn,pieceRow,destinationColumn,destinationRow,player,board);
}
string CheckSafety(piece InDanger, string player, vector<string> &board, vector<piece> &attackers){//Checks If A tile is In Danger
    string tempPiece, enemy = "2";
    if(player == "2"){enemy = "1";}
    bool safe = true;
    cout<<"begin knight check"<<endl;
    int knights[8] = {6, 10, 15, 17, -6, -10, -15, -17};//possible knight tiles
    for(int i = 0; i < 8; i++){
        if(InDanger.Row()*8 + InDanger.Col() + knights[i] < 0){continue;}
        if(InDanger.Row()*8 + InDanger.Col() + knights[i] > 64){continue;}//check bounds
        tempPiece = board.at(InDanger.Row()*8 + InDanger.Col() + knights[i]);
        //cout<<tempPiece<<endl;
        if(finds(enemy, tempPiece) && finds("H", tempPiece)){
            safe = false;
            //cout<<"fail"<<endl;
            attackers.push_back(piece((InDanger.Row()*8 + InDanger.Col() + knights[i])/8, (InDanger.Row()*8 + InDanger.Col() + knights[i])%8, tempPiece));//add to list of attackers
            continue;
        }
    }
    cout<<"passed knight check"<<endl;


    cout<<"begin perp check"<<endl;
    int perps[2] = {1, -1};
    for(int i = 0; i < 2; i++){//columns
        int add = perps[i];
        while(InBoard(InDanger.Row(), InDanger.Col() + add)){
            tempPiece = board.at((InDanger.Row()*8) + (InDanger.Col() + add));
            //cout<<tempPiece<<" "<<InDanger.Row()<<" "<<InDanger.Col() + add<<endl;
            if(finds(player, tempPiece)){break;}
            if(finds(enemy, tempPiece) && (finds("R",tempPiece)||finds("Q", tempPiece))){
                safe = false;
                //cout<<"fail 1"<<tempPiece<<endl;
                attackers.push_back(piece(InDanger.Row(), InDanger.Col() + add, tempPiece, 0, perps[i]));//add to list of attackers
                break;
            }
            if(!finds("XX", tempPiece)){break;}//enemy neutral piece
            add+=perps[i];
        }
    }
    for(int i = 0; i < 2; i++){//rows
        int add = perps[i];
        while(InBoard(InDanger.Row() + add, InDanger.Col())){
            tempPiece = board.at((InDanger.Row() + add)*8 + (InDanger.Col()));
            //cout<<tempPiece<<endl;
            if(finds(player, tempPiece)){break;}
            if(finds(enemy, tempPiece) && (finds("R",tempPiece)||finds("Q", tempPiece))){
                safe = false;
                //cout<<"fail 2"<<tempPiece<<endl;
                attackers.push_back(piece(InDanger.Row() +add, InDanger.Col(), tempPiece, perps[i], 0));//add to list of attackers
                break;
            }
            if(!finds("XX", tempPiece)){break;}//enemy neutral piece
            add+=perps[i];
        }
    }
    cout<<"passed perp check"<<endl;


    cout<<"begin diag check"<<endl;
    int diags[2] = {-1, 1};
    for(int i = 0; i < 2; i++){
        for(int k = 0; k <2; k++){
            int addRow = diags[i];
            int addCol = diags[k];
            while(InBoard(InDanger.Row() + addRow, InDanger.Col()+ addCol )){
                tempPiece = board.at((InDanger.Row() + addRow)*8 + (InDanger.Col() + addCol));
                //cout<<tempPiece<<endl;
                if(finds(player, tempPiece)){break;}
                if(finds(enemy, tempPiece) && (finds("B",tempPiece)||finds("Q", tempPiece))){
                    safe = false;
                    //cout<<"fail"<<endl;
                    attackers.push_back(piece(InDanger.Row()+addRow, InDanger.Col() + addCol, tempPiece, diags[i], diags[k]));//add to list of attackers
                    break;
                }
                if(!finds("XX", tempPiece)){break;}//enemy neutral piece
                addRow+=diags[i];
                addCol+=diags[k];
            }
        }
    }
    cout<<"passed diag check"<<endl;

    //cout<<"list of attacker tiles="<<endl;
    //for(piece attacker : attackers){
    //    attacker.printCoords();
    //}

    if(!safe){return "unsafe";}
    return "safe";
}
string TheoricalSafety(piece InDanger, string player, vector<string> board, int attackerRow, int attackerCol, int attackerRow2, int attackerCol2){//checks if a theoretical move will be safe
    vector<piece> tempattackers;
    string enemy = "2";
    if(player == "2"){enemy = "1";}

    board.at(attackerRow2*8 + attackerCol2) = board.at((attackerRow *8) + attackerCol);//make switch on theoretical board
    board.at((attackerRow *8) + attackerCol) = "XX";

    return CheckSafety(InDanger, player, board, tempattackers);//check safety of theoretical board

}
void CheckValid(string &selectedPiece,string &selectedTile, int &pieceColumn, int &pieceRow, int &destinationColumn, int &destinationRow, string player, vector<string> &board){//check that a pieces destination is valid
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
void MovePiece(int pieceRow, int pieceColumn, int destinationRow, int destinationColumn, string selectedPiece, vector<string> &board){//moves piece on board
    board.at(((pieceRow-1)*8) + (pieceColumn-1)) = "XX";
    board.at(((destinationRow-1)*8) + (destinationColumn-1)) = selectedPiece;
}
bool AbleToBlock(piece King, string player,vector<string> board, piece Attacker){//Checks if you can block the path of the attacker
    int row=Attacker.GetRowOp(), column = Attacker.GetColOp();
    vector<piece> blockers;
    string enemyPlayer = "2";int direction = -1;
    if(player == "2"){enemyPlayer=1;direction = 1;}

    while(board.at(((King.Row()+row)*8)+King.Col()+column) != Attacker.GetName()){//make less messy<<<<<<<<
        piece Tile(King.Row() +row, King.Col()+column, "XX");//set up Tile

        if(InBoard(King.Row()+row + (1*direction) , King.Col()+column)){
            cout<<"go"<<endl;
            piece Pawn(King.Row()+row +(1*direction), King.Col()+column, board.at(((King.Row()+row+(1*direction))*8)+King.Col()+column)); // gross setting up of pieces
            // gross setting up of pieces
            if(Pawn.GetName() == "P"+player){
                cout<<"pawn check!"<<endl;
                if(TheoricalSafety(King ,player, board, Pawn.Row(), Pawn.Col(), Tile.Row(), Tile.Col()) == "safe"){

                    return true;
                }
            }
        }
        if(CheckSafety(Tile, enemyPlayer, board, blockers) != "safe"){//if you can attack the tile //might not account for some pawn scenarios, not sure though
            for(piece blocker : blockers){//check if any of the blockers can succesfully attack the spot
                if(blocker.Type() != "P" && TheoricalSafety(King,player, board, blocker.Row(), blocker.Col(), King.Row()+row,King.Col()+column) == "safe"){
                    cout<<"CAN BLOCK!!!!!!!!!"<<endl;
                    return true;
                }
            }
        }
        row += Attacker.GetRowOp();
        column+= Attacker.GetColOp();
    }

    return false;
}
string Result(string player, vector<string> &board){
    bool underAttack = false;
    string attackerSafety, tempPiece, enemyPlayer = "1", direction;
    vector<piece> defenders, kingsAttackers, attackers2;

    if(player == "1"){enemyPlayer = "2";}

    for(int kingCol = 0; kingCol < 8; kingCol++){
        for(int kingRow = 0; kingRow < 8; kingRow++){
            tempPiece = board.at((kingRow*8)+kingCol);
            //cout<<kingRow<<" "<<kingCol<<endl;

            if(finds("K", tempPiece) && finds(player, tempPiece)){//finds a king
                piece King(kingRow, kingCol, "K"+player);
                //cout<<"king found at "<<King.Row()+1<<" "<<King.Col()+1<<endl;
                //cout<<"king vector row = "<<King.Row()<<", king vector column = "<<King.Col()<<endl;

                underAttack = CheckSafety(King, player, board, kingsAttackers) != "safe";//check if king is under attack

                if(underAttack){
                    for(int row = 0; row < 3; row++){//check possible escape tiles
                        for(int col = 0; col < 3; col++){
                            if(!InBoard(kingRow+row - 1, kingCol+col - 1)){continue;}
                            tempPiece = board.at((kingRow+row -1)*8 + (kingCol+col - 1));
                            if(finds(player, tempPiece)){continue;}
                            if(CheckSafety(piece(kingRow+row-1, kingCol+col-1,tempPiece), player, board, attackers2) == "safe"){
                                return "none";
                            }

                        }
                    }//under attack, no escape tiles
                    //cout<<"attacker vector row = "<<attackerRow<<"\nattacker vector column = "<<attackerCol<<endl;

                    attackerSafety = CheckSafety(kingsAttackers.at(0), enemyPlayer, board, defenders);//check if your attacker is in danger
                    if(attackerSafety != "safe"){//CAN ATTACK YOUR ATTACKER
                        for(piece Defender : defenders){//every possible defender
                            //cout<<"step1"<<endl;
                            if(TheoricalSafety(King ,player,board,Defender.Row(), Defender.Col(), kingsAttackers.at(0).Row(), kingsAttackers.at(0).Col()) == "safe"){//check the safety of doing that move
                                return "none";
                            }

                        }

                    }
                    //try to block your attacker //if its a horse you're cooked
                    if(kingsAttackers.at(0).Type() == "H"){return "checkmate";}//if attacker is a knight and you cant move and you cant attack him, theres no point in trying to block
                    if(AbleToBlock(King ,player,board, kingsAttackers.at(0))){continue;}

                    PrintBoard(board);
                    return"checkmate, player " + enemyPlayer + " wins";


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
    piece Selected, Destination;
    int pieceRow, pieceColumn, destinationRow, destinationColumn;
    vector<string> board = {"P1", "H1", "B1", "P1", "K1","P1","R2","XX",
                            "P1", "P1", "XX", "P1", "P1","XX","XX","XX",
                            "XX", "XX", "XX", "XX", "XX","XX","B2","XX",
                            "XX", "XX", "XX", "XX", "XX","XX","XX","XX",
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

        //cout<<"check valid done"<<endl;
        MovePiece(pieceRow, pieceColumn, destinationRow, destinationColumn, selectedPiece, board);
        if (player == "1")player = "2";
        else player = "1";

    }



    cout<<Result(player, board)<<endl;

    return 0;
}
