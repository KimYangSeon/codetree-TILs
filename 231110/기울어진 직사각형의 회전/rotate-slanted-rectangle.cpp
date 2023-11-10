#include <iostream>
using namespace std;

int n;
int board[101][101];
int dx[4] = {-1,-1,1,1};
int dy[4] = {1,-1,-1,1};

void solve(int r,int c,int m1,int m2,int m3,int m4,int dir){

    int temp = board[r][c];

    if(dir==0){
        int x=r;
        int y=c;

        for(int i=0; i<m4; i++){
            board[x][y] = board[x-1][y-1]; 
            x += -1;
            y += -1;
        }

        for(int i=0; i<m3; i++){
            board[x][y] = board[x-1][y+1]; 
            x += -1;
            y += 1;
        }

        for(int i=0; i<m2; i++){
            board[x][y] = board[x+1][y+1]; 
            x += 1;
            y += 1;
        }

        for(int i=0; i<m1; i++){
            board[x][y] = board[x+1][y-1]; 
            x += 1;
            y += -1;
        }

        board[r-1][c+1] = temp;

    }else{
        int x=r;
        int y=c;

        for(int i=0; i<m1; i++){
            board[x][y] = board[x-1][y+1]; 
            x += -1;
            y += 1;
        }

        for(int i=0; i<m2; i++){
            board[x][y] = board[x-1][y-1]; 
            x += -1;
            y += -1;
        }

        
        for(int i=0; i<m3; i++){
            board[x][y] = board[x+1][y-1]; 
            x += 1;
            y += -1;
        }

        for(int i=0; i<m4; i++){
            board[x][y] = board[x+1][y+1]; 
            x += 1;
            y += 1;
        }


        board[r-1][c-1] = temp;
    }

    
}

int main() {

    cin>>n;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            cin>>board[i][j];
        }
    }

    int r,c,m1,m2,m3,m4,dir;
    cin>>r>>c>>m1>>m2>>m3>>m4>>dir;
    solve(r-1,c-1,m1,m2,m3,m4,dir);

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            cout<<board[i][j]<< ' ';
        }
        cout << '\n';
    }
    
    return 0;
}