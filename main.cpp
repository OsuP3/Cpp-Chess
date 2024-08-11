#include <iostream>
#include <math.h>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
class piece{
public:
    piece(){
        row =0;
        column=0;
        pieceType= "X";
        RowOp= 0;
        ColumnOp = 0;
        pieceName ="XX";
        player = "X";
    }
    piece(int inputrow, int inputcolumn, string inputName){
        row = inputrow;
        column = inputcolumn;
        pieceType = inputName[0];
        pieceName = inputName;
        player = inputName[1];
    }
    piece(int inputrow, int inputcolumn, string inputName, int inputRowOp, int inputColOp){
        row = inputrow;
        column = inputcolumn;
        pieceType = inputName[0];
        pieceName = inputName;
        player = inputName[1];
        RowOp = inputRowOp;
        ColumnOp = inputColOp;
    }
    void printCoords(){cout<<"vector row = "<<row<<"|| vector column = "<<column<<endl;}
    int Row(){return row;}
    int Col(){return column;}
    string Type(){return pieceType;}
    string Player(){return player;}
    int GetColOp(){return ColumnOp;}
    int GetRowOp(){return RowOp;}
    string GetName(){return pieceName;}
    void SetName(string inputName){pieceName = inputName; pieceType = inputName[0]; player = inputName[1];}
    void SetRow(int inputRow){row = inputRow;}
    void SetCol(int inputCol){column = inputCol;}
private:
    int row;
    int column;
    string pieceType;
    int RowOp;//what row operation led to attack (for keeping track of direction of attack)
    int ColumnOp;//what column operation led to attack
    string pieceName;
    string player;
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
void GetPiece(piece &Selected, string player, vector<string> &board){//Prompts player for piece
    int pieceRow, pieceColumn;
    cout<<"pick row of piece to move:"<<endl;
    cin>>pieceRow; Selected.SetRow(pieceRow);
    cout<<"pick column of piece to move"<<endl;
    cin>>pieceColumn; Selected.SetCol(pieceColumn);


    Selected.SetName(board.at(((pieceRow-1)*8) + (pieceColumn-1)));

    while(Selected.GetName()[1] != player[0]){
        PrintBoard(board);
        cout<<"INVALID!\npick row of piece to move:"<<endl;
        cin>>pieceRow; Selected.SetRow(pieceRow);
        cout<<"pick a column of piece to move"<<endl;
        cin>>pieceColumn; Selected.SetCol(pieceColumn);

        Selected.SetName(board.at(((pieceRow-1)*8) + (pieceColumn-1)));
    }

}
void GetDestination(piece &Selected, piece &Destination, string player, vector<string> &board){//Prompts player for destination
    int destinationColumn, destinationRow;

    cout<<"pick where row of piece destination"<<endl;
    cin>>destinationRow; Destination.SetRow(destinationRow);
    cout<<"pick column of piece destination"<<endl;
    cin>>destinationColumn;Destination.SetCol(destinationColumn);

    Destination.SetName(board.at(((destinationRow-1)*8) + (destinationColumn-1)));

    while(Destination.Player() == player){
        PrintBoard(board);
        cout<<"INVALID!"<<endl;
        GetPiece(Selected, player,board);
        cout<<"pick row of piece destination:"<<endl;
        cin>>destinationRow; Destination.SetRow(destinationRow);
        cout<<"pick column of piece destination"<<endl;
        cin>>destinationColumn; Destination.SetCol(destinationColumn);

        Destination.SetName(board.at(((destinationRow-1)*8) + (destinationColumn-1)));
    }
}
void Retry(string pieceFullName, piece &Selected, piece &Destination, string player, vector<string> &board){//RePrompts player for piece and destination
    PrintBoard(board);
    cout<<"Invalid "<<pieceFullName<<" move"<<endl;
    GetPiece(Selected, player,board);
    GetDestination(Selected, Destination,player,board);
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
string TheoricalSafety(piece InDanger, string player, vector<string> board, piece Attacker, piece attackerDestination){//checks if a theoretical move will be safe
    vector<piece> tempAttackers;
    string enemy = "2";
    if(player == "2"){enemy = "1";}

    board.at(attackerDestination.Row()*8 + attackerDestination.Col()) = board.at((Attacker.Row() *8) + Attacker.Col());//make switch on theoretical board
    board.at((Attacker.Row() *8) + Attacker.Col()) = "XX";

    return CheckSafety(InDanger, player, board, tempAttackers);//check safety of theoretical board

}
void CheckValid(piece Selected, piece Destination, string player, vector<string> &board){//check that a pieces destination is valid
start:
    string tempPiece;
    int tempHIGH, tempLOW, row ,column;
    while(true){

        while(Selected.Type() == "P"){//Pawn selected
            if((player == "1" && (Destination.Row() <= Selected.Row()))||(player == "2" && (Destination.Row() >= Selected.Row()))){
                Retry("Pawn", Selected, Destination, player, board);
                goto start;
            }
            if((player == "1" && Destination.Col() != Selected.Col() && Destination.GetName().find("2")==string::npos)||(player == "2" && Destination.Col() != Selected.Col() && Destination.GetName().find("1")==string::npos)){
                Retry("Pawn", Selected, Destination, player, board);
                goto start;
            }
            if((abs(Destination.Row() - Selected.Row()))>=2 && ((player == "1" && Selected.Row() != 2)||(player == "2" && Selected.Row() != 7))){
                Retry("Pawn", Selected, Destination, player, board);
                goto start;
            }
            if(sqrt(pow((Destination.Col()-Selected.Col()),2) + pow((Destination.Row()-Selected.Row()), 2)) > 2){
                Retry("Pawn", Selected, Destination, player, board);
                goto start;
            }
            break;
        }
        while(Selected.Type() == "R"){//Rook selected
            //cout<<"enters while"<<endl;
            if((Selected.Row() != Destination.Row() && Selected.Col()!=Destination.Col())){
                Retry("Rook", Selected, Destination, player, board);
                goto start;
            }
            if(Selected.Col()==Destination.Col()){
                if(Selected.Row()>Destination.Row()){
                    tempHIGH = Selected.Row();
                    tempLOW = Destination.Row();
                }
                else{
                    tempHIGH = Destination.Row();
                    tempLOW = Selected.Row()+1;
                }
                for(;tempLOW < tempHIGH; tempLOW++){
                    if(board.at(((tempLOW-1)*8) + (Selected.Col()-1)) != "XX"){
                        Retry("Rook", Selected, Destination, player, board);
                        goto start;
                    }
                }
            }
            else{
                if(Selected.Col()>Destination.Col()){
                    tempHIGH = Selected.Col();
                    tempLOW = Destination.Col();
                }
                else{
                    tempHIGH = Destination.Col();
                    tempLOW = Selected.Col()+1;
                }
                for(;tempLOW < tempHIGH; tempLOW++){//1 5
                    //cout<<tempLOW<<endl;
                    if(board.at(((Selected.Row()-1)*8) + (tempLOW-1)) != "XX"){
                        Retry("Rook", Selected, Destination, player, board);
                        goto start;

                    }
                }
            }
            break;
        }
        while(Selected.Type() == "H"){//Knight selected
            if(((abs(Selected.Col()-Destination.Col()) == 1)&&(abs(Selected.Row()-Destination.Row())==2))||((abs(Selected.Col()-Destination.Col()) == 2)&&(abs(Selected.Row()-Destination.Row())==1))){
                return;
            }
            else{
                Retry("Knight", Selected, Destination, player, board);
                goto start;
            }


        }
        while(Selected.Type() == "B"){//Bishop selected
            if(abs(Selected.Col()-Destination.Col()) != abs(Selected.Row()-Destination.Row())){
                Retry("Bishop", Selected, Destination, player, board);
                goto start;
            }
            else {return;}
        }
        while(Selected.Type() == "Q"){//Queen selected
            if(abs(Selected.Col()-Destination.Col()) == abs(Selected.Row()-Destination.Row())){
                return;
            }
            if((sqrt(pow((Destination.Col()-Selected.Col()),2) + pow((Destination.Row()-Selected.Row()), 2)) > 1) && (Selected.Col() != Destination.Col()) && (Selected.Row() != Destination.Row())){
                Retry("Queen", Selected, Destination, player, board);
                goto start;
            }
            return;
        }
        while(Selected.Type() == "K"){//King selected
            if((sqrt(pow((Destination.Col()-Selected.Col()),2) + pow((Destination.Row()-Selected.Row()), 2)) > 1)) {
                Retry("King", Selected, Destination, player, board);
                goto start;
            }
            if((player == "2" && ((board.at(((Destination.Row()-1-1)*8) + (Destination.Col()-1-1)).find("P") != string::npos)||(board.at(((Destination.Row()-1-1)*8) + (Destination.Col())).find("P") != string::npos)))||(player == "1" && ((board.at(((Destination.Row())*8) + (Destination.Col()-1-1)).find("P") != string::npos)||(board.at(((Destination.Row())*8) + (Destination.Col())).find("P") != string::npos)))){
                Retry("King", Selected, Destination, player, board);
                goto start;

            }
            for(column = 0; column < 3; column++){
                for(row = 0; row< 3; row++){
                    tempPiece = (board.at((Destination.Row()+row-1-1)*8 + (Destination.Col()+column-1-1)));
                    if((tempPiece.find(player) == string::npos && tempPiece.find("Q") != string::npos)||(tempPiece.find(player) == string::npos && tempPiece.find("K") != string::npos)){
                        Retry("King", Selected, Destination, player, board);
                        goto start;
                    }
                }
            }
            ////Knight check
            int knightColumns[8] = {-1, -2, 1, 2, -1 , 2,  1, -2 };
            int knightRows[8] =    {-2, -1, 2, 1,  2, -1, -2,  1};
            //cout<<"brah"<<endl;
            for(int k = 0; k < 8; k++){
                //cout<<board.at((Destination.Row()-1+knightRows[k])*8 + (Destination.Col()-1+knightColumns[k]))<<endl;
                tempPiece = board.at((Destination.Row()-1+knightRows[k])*8 + (Destination.Col()-1+knightColumns[k]));
                if(tempPiece.find("H") != string::npos && tempPiece.find(player) == string::npos){
                    Retry("King", Selected, Destination, player, board);
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
                    tempPiece = board.at((Destination.Row()-1+row)*8 + (Destination.Col()-1+column));//temp piece to check validity
                    if(tempPiece.find(player) != string::npos){break;continue;}
                    if(tempPiece.find(player) == string::npos && tempPiece.find("Q")==string::npos && tempPiece.find("R")==string::npos && tempPiece.find("XX")==string::npos){break;continue;}

                    if(tempPiece.find("Q")!=string::npos||tempPiece.find("R")!=string::npos){
                        Retry("King", Selected, Destination, player, board);
                        goto start;
                    }
                    //cout<<(Destination.Col()+column-1)<<" "<< (Destination.Row()+row-1)<<endl;
                    row += rowsop[k];
                    column += columnsop[k];
                }

            }

            for(int k = 0; k < 4; k++){//DIAGONALS
                row = rows[k];
                column = columns[k];
                while(true){
                    tempPiece = board.at((Destination.Row()-1-1+row)*8 + (Destination.Col()-1-1+column));
                    if(tempPiece.find(player) != string::npos){break;continue;}
                    if(tempPiece.find(player) == string::npos && tempPiece.find("Q")==string::npos && tempPiece.find("B")==string::npos && tempPiece.find("XX")==string::npos){break;continue;}

                    if(tempPiece.find("Q")!=string::npos||tempPiece.find("B")!=string::npos){
                        Retry("King", Selected, Destination, player, board);
                        goto start;
                    }
                    //cout<<(Destination.Col()+column-1)<<" "<< (Destination.Row()+row-1)<<endl;
                    row += rowsop[k];
                    column += columnsop[k];
                }
            }

        }

        return;
    }
}
void MovePiece(piece Selected, piece Destination, vector<string> &board){//moves piece on board
    board.at(((Selected.Row()-1)*8) + (Selected.Col()-1)) = "XX";
    board.at(((Destination.Row()-1)*8) + (Destination.Col()-1)) = Selected.GetName();
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
                if(TheoricalSafety(King ,player, board, Pawn, Tile) == "safe"){

                    return true;
                }
            }
        }
        if(CheckSafety(Tile, enemyPlayer, board, blockers) != "safe"){//if you can attack the tile //might not account for some pawn scenarios, not sure though
            for(piece blocker : blockers){//check if any of the blockers can succesfully attack the spot
                if(blocker.Type() != "P" && TheoricalSafety(King,player, board, blocker, Tile) == "safe"){
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
                            if(TheoricalSafety(King ,player,board,Defender, kingsAttackers.at(0)) == "safe"){//check the safety of doing that move
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


int main()//Chess
{

    string selectedPiece, selectedTile;
    string player = "1";
    piece Selected, Destination;
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

        GetPiece(Selected, player ,board);

        GetDestination(Selected, Destination,player,board);

        CheckValid(Selected, Destination, player, board);

        //cout<<"check valid done"<<endl;
        MovePiece(Selected, Destination, board);
        if (player == "1")player = "2";
        else player = "1";

    }



    cout<<Result(player, board)<<endl;

    return 0;
}
