#include <bits/stdc++.h>
using namespace std;

char board[8][8];
int X1,Y1;                                             /// Save start position of piece
int X2,Y2;                                             /// Save end position of piece
bool white=true,Valid_Move;
char pieceName,currentPlace[2],nextPlace[2];
int W_king_x,W_king_y,W,B_king_x,B_king_y;
vector <int> Saved_moves;
vector <int> Undo_moves;
vector <char> W_captured;
vector <char> B_captured;

void printLogo()
{
    cout << "    ======================================  \n";
    cout << "       _____ _    _ ______  _____ _____     \n";
    cout << "      / ____| |  | |  ____|/ ____/ ____|    \n";
    cout << "     | |    | |__| | |__  | (___| (___      \n";
    cout << "     | |    |  __  |  __|  \\___ \\\\___ \\ \n";
    cout << "     | |____| |  | | |____ ____) |___) |    \n";
    cout << "      \\_____|_|  |_|______|_____/_____/\n  \n";
    cout << "    ======================================\n\n";
}
void init()
{
    Saved_moves.clear(),Undo_moves.clear();
    white=true;
    for (int i=0; i<8; i++)
        for (int j=0; j<8; j++)
           board[i][j]= ( (i+j)%2!=0 ) ? '*' : ' ' ;
    for (int i=0; i<8; i++)
        board[1][i]='P',board[6][i]='p';
    board[0][0]='R',board[0][7]='R';
    board[0][1]='N',board[0][6]='N';
    board[0][2]='B',board[0][5]='B';
    board[0][3]='K',board[0][4]='Q';

    board[7][0]='r',board[7][7]='r';
    board[7][1]='n',board[7][6]='n';
    board[7][2]='b',board[7][5]='b';
    board[7][3]='k',board[7][4]='q';
}
void Board_Print()
{
    cout<<"          __";
    for (int i=0; i<8; i++)
        printf("%c__",'a'+i);
    cout<<"\n          __________________________";
    cout<<endl;
    int i=0;
    if(white)
        i=7;
    for (; i>=0 && i<8;)
    {
        if (i%2!=0)
            cout<<"          |***   ***   ***   ***   |\n";
        else
            cout<<"          |   ***   ***   ***   ***|\n";
        for (int j=0; j<8; j++)
        {
            if (j==0)
                cout<<"        "<<i+1<<" |";
            if((i+j)%2!=0)
                cout<<"*"<<board[i][j]<<"*";
            else
                cout<<" "<<board[i][j]<<" ";
            if (j==7)
                cout<<"|";

            if (j==7 && i==5)
                cout<<"     Enter the piece symbol     The current place       the next place   ";
        }
        cout<<endl;
        if (i%2!=0)
            cout<<"          |***   ***   ***   ***   |\n";
        else
            cout<<"          |   ***   ***   ***   ***|\n";

        if(white)
            i--;
        else
            i++;
    }
    cout<<"          --------------------------     \n\n";
}
void to_coordinates(char currentPlace[], char nextPlace[]) /// feel free to right position with lower or upper letters
{
    X1 = currentPlace[0]>=97 ? currentPlace[0]- 97 : currentPlace[0]-65 ;
    X2 = nextPlace[0]   >=97 ? nextPlace[0]-97     : nextPlace[0]-65    ;
    Y1 = currentPlace[1]-49  , Y2=nextPlace[1]-49  ;

}
bool is_black_piece_there(int x,int y)
{
    return board[y][x]>'a' && board[y][x]<'z';
}
bool is_white_piece_there(int x,int y)
{
    return board[y][x]>'A' && board[y][x]<'Z';
}
bool is_emptyy(int x, int y)
{
    return  board[y][x]=='*' || board[y][x]==' ';
}
bool Destination_place_check()
{
    if ( white && ( is_emptyy(X2,Y2) || is_black_piece_there(X2,Y2) ))
        return true;
    else if (!white &&(is_emptyy(X2,Y2) || is_white_piece_there(X2,Y2)))
        return true;
    return false;
}
void find_king()
{
    for (int i=0 ; i<8; i++)
        for(int j=0; j<8; j++)
        {
            if(board[i][j]=='K')
                W_king_x=j,W_king_y=i;
            if (board[i][j]=='k')
                B_king_x=j,B_king_y=i;
        }
}
bool Pawn_Valid ( )
{
    if ( white && board[Y1][X1]=='P' )
    {
        if (is_black_piece_there(X2,Y2) && abs(X1-X2)==1 && Y2-Y1==1 )
            return true;
        else if ( is_emptyy(X1,Y1+1) && X2==X1 && ( (Y1==1 && Y2==3 && is_emptyy(1,3))  || Y2-Y1==1))
            return true;
    }
    else if ( !white && board[Y1][X1]=='p' )
    {
        if (is_white_piece_there(X2,Y2) && abs(X1-X2)==1 && Y1-Y2==1)
            return true;
        else if ( is_emptyy(X1,Y1-1) && X2==X1 && ((Y1==6 && Y2==4 && is_emptyy(6,4)) || Y1-Y2==1 ))
            return true;
    }
    return false;
}
bool Rook_Valid( )
{
    if ( X1==X2  )
    {
        int steps= max(Y1,Y2)-min(Y1,Y2),mn= min(Y1,Y2);
        for( int i = 1 ; i < steps && i!=Y2 ; i++ )
            if ( !is_emptyy(X1,mn+i))
                return false;
    }
    else if ( Y1==Y2 )
    {
        int steps=  max(X1,X2)-min(X1,X2),mn= min(X1,X2);
        for( int i = 1 ; i < steps ; i++ )
            if ( !is_emptyy(mn+i,Y1))
                return false;
    }
    if (X1==X2 || Y1==Y2)
        return Destination_place_check();

    return false;
}
bool Bishop_Valid( )
{
    if (X1+Y1==X2+Y2)
    {
        int minX=min(X1,X2),maxY=max(Y1,Y2),steps=abs(X1-X2);
        for (int i=1 ; i < steps ; i++)
            if(!is_emptyy(i+minX,maxY-i) )
                return false;
    }
    else if (X1-X2==Y1-Y2)
    {
        int minX=min(X1,X2),minY=min(Y1,Y2),steps=abs(X1-X2);
        for (int i=1 ; i < steps  ; i++)
            if(!is_emptyy(minX+i,minY+i))
                return false;
    }
    if ( X1+Y1==X2+Y2 || X1-X2==Y1-Y2 )
        return Destination_place_check();
    return false;
}
bool Knight_Valid()
{
    if (( abs(X1-X2)==2 && abs(Y1-Y2)==1) ||( abs(X1-X2)==1 && abs(Y1-Y2)==2))
        return Destination_place_check();
    return false;
}
bool King_Valid()
{
    if (abs(X1-X2)<=1 && abs(Y1-Y2)<=1)
        return Destination_place_check();
    return false;
}
bool Queen_Valid()
{
    return Bishop_Valid() || Rook_Valid();
}
void pawn_promotion()
{
    int choise=0;
    char Knight='n',Queen='q',Rook='r',Bishop='b';
    if (white)
        Knight='N',Queen='Q',Rook='R',Bishop='B';

    while(choise<1 || choise>4)
    {
        if((pieceName=='P'&&Y2==7) || (pieceName=='p'&&Y2==0))
            cout<<"What  Do You Want The Pawn To Be?   \nPress 1 for Queen 2 for Rook 3 for Knight 4 for Bishop       ",cin>>choise;
        if(choise==1)
            board[Y2][X2]=Queen;
        else if(choise==2)
            board[Y2][X2]=Rook;
        else if(choise==3)
            board[Y2][X2]=Knight;
        else if(choise==4)
            board[Y2][X2]=Bishop;
        else
            cout<<"     INVALED CHOISE      \n";
    }
}
void Save_Moves()
{
    int Move=X1*1000000+Y1*100000+X2*10000+Y2*1000+(int)(board[Y2][X2]);
    Saved_moves.push_back(Move);
}
void Move_Piece()
{

    board[Y2][X2]=board[Y1][X1];///
    if ((X1+Y1)%2==0)
        board[Y1][X1]=' ';
    else
        board[Y1][X1]='*';
}
void undo()
{

    if(Saved_moves[Saved_moves.size()-1]==1)             ///castling undo castling
    {
        Saved_moves.pop_back(),undo(),undo(),Undo_moves.push_back(1);
        return;
    }

    int last_index=Saved_moves.size()-1;
    Undo_moves.push_back((Saved_moves[last_index]));


    /// moving back to  ..
    X2=(Saved_moves[last_index]/1000000)%10;
    Y2=(Saved_moves[last_index]/100000)%10;
    /// moving back from..
    X1=(Saved_moves[last_index]/10000)%10;
    Y1=(Saved_moves[last_index]/1000)%10;
    Move_Piece();
    board[Y1][X1]=(char)((Saved_moves[last_index])%1000);

    if( W_captured.size()>0 && board[Y1][X1] == W_captured[W_captured.size()-1])
        W_captured.pop_back();
    else if ( B_captured.size()>0 && board[Y1][X1] == B_captured[B_captured.size()-1])
        B_captured.pop_back();

    X1=X2,Y1=Y2;
    Saved_moves.pop_back();

}
void Redo()
{

    if(Undo_moves[Undo_moves.size()-1]==1)
    {
        Undo_moves.pop_back(),Redo(),Redo(),Saved_moves.push_back(1);
        return;
    }

    int last_index=Undo_moves.size()-1;
    Saved_moves.push_back((Undo_moves[last_index]));


    /// moving back to  ..
    X1=(Undo_moves[last_index]/1000000)%10;
    Y1=(Undo_moves[last_index]/100000)%10;
    /// moving back from..
    X2=(Undo_moves[last_index]/10000)%10;
    Y2=(Undo_moves[last_index]/1000)%10;
    Move_Piece();
    Undo_moves.pop_back();
}
void Save()
{
    ofstream myfile;
    myfile.open ("board.txt");
    for (int i=0; i<8; i++)
        for (int j=0; j<8; j++)
            myfile<<board[i][j];
    myfile.close();
    cout<<"\n_______________________________________________________ \n\n";
    cout<<"                 GAME IS SAVED SUCCESSGULLY               \n";
    cout<<"_______________________________________________________   \n\n\n";
}
void Load()
{
    cout<<"\n_______________________________________________________ \n\n";
    cout<<"                 GAME IS Loaded SUCCESSGULLY              \n";
    cout<<"_______________________________________________________   \n\n\n";
    ifstream myfile;
    myfile.open("board.txt");

    for (int i=0; i<8; i++)
        for (int j=0; j<8; j++)
            myfile>>noskipws>>board[i][j];
    myfile.close();
}
bool check(int x, int y, int by_what)
{
    ///by what=0, by white/// by what =1 by black
    ///x,y is position to be check
    char Knight='n',Queen='q',Rook='r',Bishop='b',King='k',Pawn='p';
    if(by_what==0)
        Knight='N',Queen='Q',Rook='R',Bishop='B',King='K',Pawn='P';
    ///knight check
    if (board[y+2][x+1]==Knight||board[y+2][x-1]==Knight||board[y-2][x+1]==Knight||board[y-2][x-1]==Knight||board[y+1][x+2]==Knight||board[y-1][x+2]==Knight||board[y+1][x-2]==Knight||board[y-1][x-2]==Knight)
        return true;

    ///Rook check
    for(int i=x+1; i<8; i++)
        if(board[y][i]==Rook || board[y][i]==Queen)
            return true;
        else if (!is_emptyy(i,y))
            break;
    for(int i=x-1; i>=0; i--)
        if(board[y][i]==Rook || board[y][i]==Queen)
            return true;
        else if (!is_emptyy(i,y))
            break;
    for(int i=y+1; i<8; i++)
        if(board[i][x]==Rook || board[i][x]==Queen)
            return true;
        else if (!is_emptyy(x,i))
            break;
    for(int i=y-1; i>=0; i--)
        if(board[i][x]==Rook || board[i][x]==Queen)
            return true;
        else if (!is_emptyy(x,i))
            break;

    ///Bishop check
    int Max_Steps=8-max(x,y);
    for(int i=1; i<Max_Steps; i++)                          /// down right diagonal
        if(board[y+i][x+i]==Bishop || board[y+i][x+i]==Queen)
            return true;
        else if (!is_emptyy(x+i,y+i))
            break;
    Max_Steps=8-min(x,y);
    for(int i=1; i<Max_Steps; i++)                          /// up left diagonal
        if(board[y-i][x-i]==Bishop || board[y-i][x-i]==Queen)
            return true;
        else if (!is_emptyy(x-i,y-i))
            break;
    Max_Steps= min(8-x,y);
    for(int i=1; i<Max_Steps ; i++)               /// up right diagonal
        if(board[y-i][x+i]==Bishop || board[y-i][x+i]==Queen)
            return true;
        else if (!is_emptyy(x+i,y-i))
            break;
    Max_Steps= min(x,8-y);
    for(int i=1; i< Max_Steps; i++)              /// down left diagonal
        if(board[y+i][x-i]==Bishop || board[y+i][x-i]==Queen)
            return true;
        else if (!is_emptyy(x-i,y+i))
            break;
    ///Pawn check
    if (is_white_piece_there(x,y) &&(board[y+1][x+1]=='p' || board[y+1][x-1]=='p'))
        return true;
    else if (is_black_piece_there(x,y) &&(board[y-1][x+1]=='P' || board[y-1][x-1]=='P'))
        return true;

    return false;
}
bool move_try_success(int x,int y)
{
    Save_Moves();
    Move_Piece();
    if(!check(x,y,is_white_piece_there(x,y)))
    {
        undo();
        return true;
    }
    undo();
    return false;
}
bool can_king_avoid_check(int x, int y)
{
    ///x,y position of the king
    int possible_displacements[8][2]= {{1,1},{1,0},{1,-1},{-1,1},{-1,0},{1,-1},{0,1},{0,-1}};
    for (int i=0; i<8; i++)
    {
        X2=x+possible_displacements[i][0];
        Y2=y+possible_displacements[i][1];

        if(X2>=0&& X2<8 &&Y2>=0 && Y2<0 )
            return move_try_success(X2,Y2);
    }
    return false;
}
bool Check_mate()
{
    int xx_king=B_king_x,yy_king=B_king_y;

    char Knight='n',Queen='q',Rook='r',Bishop='b',King='k',Pawn='p';
    if (white)
        Knight='N',Queen='Q',Rook='R',Bishop='B',King='K',Pawn='P',xx_king=W_king_x,yy_king=W_king_y;

    if(can_king_avoid_check(xx_king,yy_king))
        return false;

    for (X1=0; X1<8; X1++)
        for(Y1=0; Y1<8; Y1++)
        {
            if(board[Y1][X1]==Knight)
            {
                int possible_displacement[8][2]= {{1,2},{1,-2},{-1,2},{-1,-2},{2,1},{2,-1},{-2,1},{-2,-1}};
                for (int i=0; i<8; i++)
                {
                    X2=X1+possible_displacement[i][0],Y2=Y1+possible_displacement[i][1];
                    if((X2>=0 && X2<8 &&Y2>=0&&Y2<8 ) && Knight_Valid() && move_try_success(xx_king,yy_king) )
                        return false;
                }
            }
            else if (board[Y1][X1]==Rook || board[Y1][X1]==Queen )///XXXX
            {
                int possible_displacement[28][2]= {{7,0},{6,0},{5,0},{4,0},{3,0},{2,0},{1,0},{-7,0},{-6,0},{-5,0},{-4,0},{-3,0},{-2,0},{-1,0},{0,7},{0,6},{0,5},{0,4},{0,3},{0,2},{0,1},{0,-7},{0,-6},{0,-5},{0,-4},{0,-3},{0,-2},{0,-1}};
                for(int i=0 ; i<28; i++)
                {
                    X2=X1+possible_displacement[i][0],Y2=Y1+possible_displacement[i][1];
                    if (X2>=0 && X2<8 &&Y2>=0&&Y2<8 )
                        if(( board[Y1][X1]==Rook  && Rook_Valid() &&move_try_success(xx_king,yy_king) ) || ( board[Y1][X1]==Queen  && Queen_Valid() &&move_try_success(xx_king,yy_king) ))
                            return false;
                }
            }
            else if (board[Y1][X1]==Bishop || board[Y1][X1]==Queen )
            {
                int possible_displacement[28][2]= {{7,7},{6,6},{5,5},{4,4},{3,3},{2,2},{1,1},{-7,-7},{-6,-6},{-5,-5},{-4,-4},{-3,-3},{-2,-2},{-1,-1},{-7,7},{-6,6},{-5,5},{-4,4},{-3,3},{-2,2},{-1,1},{7,-7},{6,-6},{5,-5},{4,-4},{3,-3},{2,-2},{1,-1}};
                for(int i=0; i<28; i++)
                {
                    X2=X1+possible_displacement[i][0],Y2=Y1+possible_displacement[i][1];
                    if (X2>=0 && X2<8 &&Y2>=0&&Y2<8 )
                        if(( board[Y1][X1]==Bishop  && Bishop_Valid() &&move_try_success(xx_king,yy_king) ) || ( board[Y1][X1]==Queen  && Queen_Valid() &&move_try_success(xx_king,yy_king) ))
                            return false;
                }
            }
            else if (board[Y1][X1]==Pawn)
            {
                int possible_displacement[8][2]= {{1,1},{1,-1},{-1,1},{-1,-1},{0,2},{0,1},{0,-2},{0,-1}};
                for (int i=0; i<8; i++)
                {
                    X2=X1+possible_displacement[i][0], Y2=Y1+possible_displacement[i][1];
                    if ((X2>=0 && X2<8 &&Y2>=0&&Y2<8 ) && Pawn_Valid() && move_try_success(xx_king,yy_king))
                        return false;
                }
            }
        }
    return true;
}
bool Castling_Valid()
{
    /*   check if any of these pieces moved
         White right Rook=00         White left Rook==70      White king=30
         black right Rook==07        black left Rook==77      Black king=37     */
    int moved=0,king=30,RightRook=00,LeftRook=70,byWhom=1;
    if(!white)
        king=37,RightRook=07,LeftRook=77,byWhom=0;

    if (X2==1 && X1==3 )
        for(int i=0; i<Saved_moves.size(); i++)
            if((Saved_moves[i]/100000)%100==RightRook || (Saved_moves[i]/100000)%100==king )
                moved=1;
    if ( X2==5 && X1==3 )
        for(int i=0; i<Saved_moves.size(); i++)
            if((Saved_moves[i]/100000)%100==LeftRook || (Saved_moves[i]/100000)%100==king )
                moved=1;

    if(!moved)
        for(int i=min(X1,X2); i<=max(X1,X2); i++)
            if((!is_emptyy(i,Y1) && (board[Y1][i]!='K' && board[Y1][i]!='k'))  || check(i,Y1,byWhom) )
                return false;

    Save_Moves(),Move_Piece();
    if(X2>X1)
        X1=7,X2=4;
    else
        X1=0,X2=2;
    Save_Moves(),Move_Piece();

    Saved_moves.push_back(1);/// if found in undo >> undo 3 times

    return true;
}
void Captured_Pieces()
{
    char xxx=board[Y2][X2];
    if(board[Y2][X2]!=' ' && board[Y2][X2]!='*' && white)
        W_captured.push_back(board[Y2][X2]);
    else if (board[Y2][X2]!=' ' && board[Y2][X2]!='*' && !white)
        B_captured.push_back(board[Y2][X2]);
}
void Print_Captured_Pieces()
{
    cout<<"WHITE CAPTURED :     ";
    for(int i=0; i<W_captured.size(); i++)
        cout<<W_captured[i]<<"  ";
    cout<<"\nBLACK CAPTURED :     ";
    for(int i=0; i<B_captured.size(); i++)
        cout<<B_captured[i]<<"  ";
    cout<<endl;
}
int main()
{
    printLogo();
    init(),Board_Print();
    white=true;
    while(true)
    {
        Valid_Move=0;
        char option;
        char Knight,Queen,Rook,Bishop,King,Pawn;
        cout<<"Commands: New Game(N) Move(M) Save game(S) Load game(L) Undo(U) Redo(R) View Captured Pieces(C)\nType Here       ";
        scanf(" %c",&option);
        if(option=='N')
            init(),Board_Print(),Undo_moves.clear(),Saved_moves.clear();
        else if(option=='U')
            white=!white,undo();
        else if (option=='R')
            white=!white,Redo();
        else if (option=='S')
            Save(),Undo_moves.clear(),Saved_moves.clear();
        else if (option=='L')
            Load(),Undo_moves.clear(),Saved_moves.clear();
        else if (option=='C')
            Print_Captured_Pieces();
        else
            while (Valid_Move==0)
            {
                Undo_moves.clear();
                if (white)
                    Knight='N',Queen='Q',Rook='R',Bishop='B',King='K',Pawn='P',cout<<"White Turn:      ";
                else
                    Knight='n',Queen='q',Rook='r',Bishop='b',King='k',Pawn='p',cout<<"Black Turn:      ";


                scanf(" %c %c%c %c%c",&pieceName,&currentPlace[0],&currentPlace[1],&nextPlace[0],&nextPlace[1]);
                to_coordinates(currentPlace,nextPlace);

                if((pieceName==Pawn && Pawn_Valid()) ||(pieceName==Queen && Queen_Valid()) || (pieceName==Knight && Knight_Valid()) || (pieceName==Rook && Rook_Valid())||  (pieceName==Bishop && Bishop_Valid()) || (pieceName==King && King_Valid()))
                    Valid_Move=1;
                Captured_Pieces();
                if (Valid_Move)
                    Save_Moves(),Move_Piece();
                find_king();
                if ((white && check(W_king_x,W_king_y,1)) ||(!white && check(B_king_x,B_king_y,0)) )
                    undo(),Valid_Move=0 ;

                if (Valid_Move==0 || (is_black_piece_there(X1,Y1) && white ) || ( is_white_piece_there(X1,Y1) && !white) || board[X1][Y1]!=pieceName )
                    cout<<"     INVALED MOVE      \n";


            }
       if (option=='M')
        {
            Board_Print();
            white=!white;
            int checkB=check(B_king_x,B_king_y,0);
            int checkW=check(W_king_x,W_king_y,1);
            int check_matee= Check_mate();

            if (!white && checkB && check_matee)
                cout<<"CHECK MATE\n   WHITE WINS\n";
            else if (white && checkW&&check_matee)
                cout<<"CHECK MATE\n   BLACK WINS\n";

            if ((!white &&checkB) ||(white && checkW) )
                cout<<"     Check \n        You Must Move Your King";

        }
        else
            Board_Print();
    }
    return 0;
}
