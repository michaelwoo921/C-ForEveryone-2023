#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;




class hex_graph {
    
    public:
        enum class color {BLANK, BLUE, RED};
        hex_graph(int hsize);  // n by n hex graph
        void reset();
        void draw_board();
        void setColor(int k, color c){board[k] = c; }
        void dijkstra(string str);
        void print_path(int pathindex);
        void print_path(int pathindex, int k);
        void print_longest_path(int pathindex);
        string mark(color c){
            if(c == color::BLANK)
                return ".";
            else if(c== color::BLUE)
                return "O";
            else 
                return "X";
        }


    private:
        int row;
        int size; // for 7 rows 7*7 nodes
        vector< vector<int> > edgelist;
        vector<color> board;
        // 0 is O blue path from top to bottom, 1 is X red path from left to right
        vector<int> mincost[2]; 
        vector<int> path[2]; 
        vector<int> longest_path[2];
        int longest_path_time[2];


};

int main(){
    hex_graph hex(5);
    hex.draw_board();
    hex.setColor(2, hex_graph::color::BLUE);
    hex.setColor(5, hex_graph::color::BLUE);
    hex.setColor(9, hex_graph::color::BLUE);
    hex.setColor(14, hex_graph::color::BLUE);
    // hex.setColor(4, color::RED);
    // hex.setColor(10, color::RED);
    // hex.setColor(6, color::RED);
    // hex.setColor(13, color::RED);
    // hex.setColor(7, color::RED);
    // hex.setColor(8, color::RED);

    hex.draw_board();
    hex.dijkstra("O");
    hex.print_path(0);
    hex.print_longest_path(0);
    hex.dijkstra("X");
 
    hex.print_path(1);
    
    hex.print_longest_path(1);


}



hex_graph::hex_graph(int hsize): row(hsize), size(hsize*hsize){
    edgelist.resize(size);
    board.resize(size);

    int i,j;
    // edge_list[k] contains nearest neighbor nodes
    // 6 nodes for internal, 2 or 3 nodes on the corner, 4 nodes on each side 
    for(int k=0; k<size; k++){
        i = k / row; j = k % row;
        // add six nearby nodes for internal node ith row and jth column
        if(i!=0 && i != row -1 && j!=0 && j!= row -1){
            edgelist[k].push_back(k-row);
            edgelist[k].push_back(k-row +1);
            edgelist[k].push_back(k-1);
            edgelist[k].push_back(k+1);
            edgelist[k].push_back(k + row);
            edgelist[k].push_back(k + row -1);
        }
        // for the first row
        if(i ==0 && j!=0 && j!= row -1 ){
            edgelist[k].push_back(k-1);
            edgelist[k].push_back(k+1);
            edgelist[k].push_back(k + row);
            edgelist[k].push_back(k + row -1);
        }
        if(i == 0 && j==0){ // k =0
            edgelist[k].push_back(k+1);
            edgelist[k].push_back(k + row);
        }
        if(i == 0 && j== row -1){ // k =row -1
            edgelist[k].push_back(k-1);
            edgelist[k].push_back(k + row);
            edgelist[k].push_back(k + row -1);
        }
        // for the last row
        if(i ==row-1 && j!=0 && j!= row -1 ){
            edgelist[k].push_back(k-1);
            edgelist[k].push_back(k+1);
            edgelist[k].push_back(k - row);
            edgelist[k].push_back(k - row + 1);
        }
        if(i == row-1 && j==0){ // k  left bottom corner
            edgelist[k].push_back(k+1);
            edgelist[k].push_back(k - row);
            edgelist[k].push_back(k - row+ 1);
        }
        if(i == row-1 && j== row -1){ // k right botom corner
            edgelist[k].push_back(k-1);
            edgelist[k].push_back(k - row);
        }
        // for the first column
        if(j==0 && i!=0 && i!= row -1){
            edgelist[k].push_back(k+1);
            edgelist[k].push_back(k - row);
            edgelist[k].push_back(k - row+ 1); 
            edgelist[k].push_back(k + row);
        }
            // for the last column
        if(j==row -1 && i!=0 && i!= row -1){
            edgelist[k].push_back(k-1);
            edgelist[k].push_back(k - row);
            edgelist[k].push_back(k + row -1); 
            edgelist[k].push_back(k + row);
        }   
    }
} // N: number of nodes

void hex_graph::reset(){
    for(int k=0; k<size;k++)
        board[k] = color::BLANK;
}

void hex_graph::draw_board(){
    string hedge=" _ ";
    string hblank = "";
    string vblank = "   ";
    string ledge = "/ ";
    string redge ="\\ ";


    cout << row << " by " << row << " hex board"<< endl;
    cout << "O represents the move by BLUE player and X represents the move by RED player" 
        << endl;

    // label of column node
    cout << "  ";
    for(int j=0;j<row;j++) // j column number row =col
        cout << (j % 10) << "   ";
    cout << endl;

    for(int i=0;i<row;i++){

        cout << hblank;
        // label of each row
        cout << (i% 10) << " ";
        // horizontal edges row: node followed by edge except the last column

        for(int j=0;j< row;j++){
            int k = i*row + j;
            if(j==row-1)
                cout << mark(board[k]);
            else
                cout << mark(board[k]) + hedge;

        }

        cout << endl;
        // vetical edges : \ followede by / except the last column
        cout << vblank;

        if(i!=row-1){
            for(int j=0;j<row;j++){
                if(j==row -1)
                    cout << redge;
                else
                    cout << redge + ledge;
                } 
                cout << endl;
        }

        // for next row move nodes and edges to the right
        hblank +="  ";
        vblank +="  ";    
    }

    cout << endl;
}

void hex_graph::print_path(int pathindex){
    int k, inc;
    if(pathindex == 0){
        k= size - row; inc = 1;
    }else{
        k= row -1; inc = row;
    }
    for(k;k<size;k += inc){
        if(mincost[pathindex][k]>=0){
            int p=k, q = path[pathindex][k];
            while(q !=p){
                cout << p << " <-- ";
                p=path[pathindex][p]; q = path[pathindex][q];
            }
            cout << p << "\tmincost: " << mincost[pathindex][k] << endl;
        }

    }
}

void hex_graph::print_path(int pathindex, int k){


    int p=k, q = path[pathindex][k];
    while(q !=p){
        cout << p << " <-- ";
        p=path[pathindex][p]; q = path[pathindex][q];
    }
    cout << p << "\tmincost: " << mincost[pathindex][k] << endl;
  

}
 
void hex_graph::print_longest_path(int pathindex){
    for(auto k: longest_path[pathindex]){
        cout << k << " <--";
    }
    cout << "longest_path_time " << longest_path_time[pathindex] << endl;
}
 
void hex_graph::dijkstra(string str){ // str : "O" or "X"
    std::cout << "running dijkstra algorithm for " + str<< endl;
    // shortest blue O path from top to bottom algorithm

    // reset
    bool cSet[size], oSet[size];
    int cost[size];
    int c_cnt =0, o_cnt =0;
    int temp;
    vector<int> oneSide;
    vector<int> oppositeSide;


    int pathindex; // 0 means O path index 1 means X path index
    color c; 
    if(str == "O"){
        pathindex =0; c = color::BLUE;
        for(int k=0;k<row;k++){
            oneSide.push_back(k);  // top row 
            oppositeSide.push_back(size-1-k); // bottom row
        }
    }
    else{
        pathindex =1; c = color::RED;
        for(int k=0;k<size;k +=row){
            oneSide.push_back(k); // left column
            oppositeSide.push_back(size-1-k); // right column
        }
    }

    for(int k=0;k<size;k++){
        mincost[pathindex].push_back(-1);
        path[pathindex].push_back(k);
    }
    for(int k=0;k<size;k++){
        cSet[k] =oSet[k] = false; cost[k] =-1;
    }

    for(auto k : oneSide){
        if(board[k] == c || board[k] == color::BLANK){
            cSet[k] =true; c_cnt++;
            path[pathindex][k]=k;
            cost[k] =(c == board[k] ? 0 : 1);
            mincost[pathindex][k]= (c == board[k] ? 0 : 1);

        } 
    }


    // initially update oSet 
    for(int k=0; k<size;k++){
        for(auto n: edgelist[k]){
            if(cSet[k] && !cSet[n] && !oSet[n] && (board[n] == c  || board[n] == color::BLANK)){
                oSet[n] = true; o_cnt++;
                if(cost[n] <0){
                    cost[n] =  mincost[pathindex][k] + (c == board[n] ? 0 : 1);

                    path[pathindex][n]=k;
                }
                if(cost[n] > mincost[pathindex][k] + (c == board[n] ? 0 : 1) ){
                    cost[n] =  mincost[pathindex][k] + (c == board[n] ? 0 : 1);
                    path[pathindex][n]=k;
                }
                mincost[pathindex][n] = cost[n];
                
                
            }
        }
    }



   // while loop until c_cnt ==N or o_cnt == 0
    while(c_cnt<size){
        // pick elements in Oset with smallest cost;
        temp =-1;
        for(int k=0;k<size;k++){
            if(temp<0 && oSet[k]){
                temp = cost[k];      
            }
            if(oSet[k] && cost[k] < temp){
                temp = cost[k]; 
            }  
        }
        // temp>0 means min cost elements are picked temp<0 means oSet is empty
        if(o_cnt>0){
            // update ix to cSet remove from oSet
            // cout << "min cost element is " << ix << " o_cnt is: " << o_cnt << endl;
            for(int k=0;k<size;k++){
                if(oSet[k] && temp == cost[k]){
                    cSet[k] = true; c_cnt++; mincost[pathindex][k] = cost[k];
                    oSet[k] = false; o_cnt--;
                    // update oSet to include sites reachable from k
                    for(auto n: edgelist[k]){
                        if(!cSet[n] && !oSet[n] && (board[n] == c || board[n] == color::BLANK) ){
                            oSet[n] = true; o_cnt++; 
                            cost[n] = mincost[pathindex][k] + (c == board[n] ? 0 : 1);
                            mincost[pathindex][n] = cost[n];
                            path[pathindex][n] = k;
                        }
                    }

                }
            }

        }



        // test if Blue player O won the game

        for(auto k: oppositeSide){
            if(mincost[pathindex][k] ==0){
                cout << str + " won the game and shorthest path is: \n";
                print_path(pathindex, k);
            
            }
        }

  
        

        if(o_cnt==0){
            cout << "oset empty now ..."<< endl;
            break;
        }


        // no one won the game yet and continue the loop


   


    }

    // find the longest path and longest_path_time
    int temp_time = -1; 
    int kx;
    cout <<"jjjjjj"<< endl;
    for(auto k: oppositeSide){
        if(temp_time <0 && mincost[pathindex][k] >=0){
            temp_time = mincost[pathindex][k];
            kx = k;
        }
        if(temp_time > mincost[pathindex][k] && mincost[pathindex][k] >=0){
            temp_time = mincost[pathindex][k];
            kx =k;
        }
        cout << "temp time: " << temp_time;
    }
   
    if(temp_time>=0){
        longest_path_time[pathindex] = temp_time;
        

        longest_path[pathindex].push_back(kx);
        int p = kx; 
        int q = path[pathindex][kx];
        while(p !=q){
            longest_path[pathindex].push_back(q);
            p = q;
            q = path[pathindex][q];
        }
    }





}









  
 
