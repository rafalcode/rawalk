#include <Rcpp.h>
using namespace Rcpp;
using namespace std;

//The way I have structured my program is to create the RandomWalk algorithm which returns a matrix of positions
// Then to answer questions 1,2 and 4 I've written a separate function that calls the algorthmic walk function
// and then the outputs is specific for answering questions 1,2 and 4.
//Just thought it was a nicer/ clearer way of organise the code rather than have than copying and pasting the algorithm twice
//also allows for some flexibility, ie. I can graph the positions of the algorithmic walk

//the line below has to present before function other wise won't be able to call the function in R

//Algorithmic random walk function - not used for question 1 and 4

// [[Rcpp::export]] 
IntegerMatrix walk0( int n, int seed)
{
    //n walks, n different position, mat[n,] = final position
    IntegerMatrix mat(n,2);//rows is each walk, col0=coordinate x, col1=coordinate y

    //  start at (0,0) origin
    mat(0,0)=0; // row zero, col idx 0 =0
    mat(0,1)=0;

    srand(seed);//for reproducibility set seed
    for(int i = 1; i<n;i++){
        int ran1=rand()%2;
        int ran2=rand()%2;
        //cout<<ran1<<endl;
        if(ran1==0){//50:50 chance to go x or y axis
            mat(i,0) = mat(i-1,0) + 1; // right
        } else if(ran2==0) {
            mat(i,1) = mat(i-1,1) - 1;// down
        } else if(ran2==1) {
            mat(i,1) = mat(i-1,0) - 1;// left
        } else if(ran1==1) {
            mat(i,1) = mat(i-1,1) + 1;// up
        }
    }
    return(mat);
}

// [[Rcpp::export]] 
IntegerMatrix walk1( int n, int seed)
{
    //n walks, n different position, mat[n,] = final position
    IntegerMatrix mat(n,2);//rows is each walk, col0=coordinate x, col1=coordinate y

    //  start at (0,0) origin
    mat(0,0)=0; // row zero, col idx 0 =0
    mat(0,1)=0;

    srand(seed);//for reproducibility set seed
    for(int i = 1; i<n;i++){
        int ran1=rand()%4;
        //cout<<ran1<<endl;
        if(ran1==0){//50:50 chance to go x or y axis
            mat(i,0) = mat(i-1,0) + 1; // right
        } else if(ran1==1) {
            mat(i,1) = mat(i-1,1) - 1;// down
        } else if(ran1==2) {
            mat(i,1) = mat(i-1,0) - 1;// left
        } else if(ran1==3) {
            mat(i,1) = mat(i-1,1) + 1;// up
        }
    }
    return(mat);
}

//------------------------
//My second and improved algorithmic function which is used for  questions and 1,2 and 4
// [[Rcpp::export]] 
IntegerMatrix walk2( int n, int seed)
{
    //n walks, n different postion, mat[n,] = final position
    IntegerMatrix mat(n,2);//rows is each walk, col0=coordinate x, col1=coordinate y

    //IntegerVector possib = {0,1,0,-1,1,0,-1,0}; //possibility matrix
    //possib.attr("dim") = Dimension(4,2);//4 possibility = 4 rows, 2 axis = 2 cols
    //IntegerMatrix pm = as<IntegerMatrix>(possib);//change vector to matrix

    //  start at (0,0) origin
    mat(0,0)=0;
    mat(0,1)=0;
    srand(seed);//for reproducibility set seed
    int i, j = 0;
    int ii, x;
    int starti;
    
    //IntegerVector ps(n);
    do{
        int ran = rand();
        if(j==0) {
            starti=1;
        } else {
            starti=0;
        }

        for(i = starti; i<16;i++){
            //int p;//p can take 4 possibilities 
            ii = i*2;
            x = (ran>>ii)&0x03;
            //cout<<ran<<endl;
            switch(x){
                case 0:
                    mat(i,0)=mat(i-1,0)+1;mat(i,1)=mat(i-1,1);break; //move 1 in x direction, +(1,0)
                case 2:
                    mat(i,0)=mat(i-1,0)-1;mat(i,1)=mat(i-1,1);break;//move -1 in x direction -(1,)
                case 3:
                    mat(i,0)=mat(i-1,0);mat(i,1)=mat(i-1,1)+1;break;//move 1 in y direction
                case 1:
                    mat(i,0)=mat(i-1,0);mat(i,1)=mat(i-1,1)-1;break; //move -1 in y direction
            }//end of switch
            if(j>=n)
                break;
            j++;
        }//end of inner loop
    } while(j < n);

    return(mat);
}//end of walk2 function

//--------------------
//--------------------


//**QUESTION 1 and QUESTION 2**

//this function will look at to see if the destination has been reached and how many goes it takes to reach the destination
//input is destination and the umber of roads taken
// so if input is (1,-3) and N =100 roads.
//output will say whether the destination has been reached at any point during the 100 roads taken

//the line below has to present before function other wise won't be able to call the function in R
// [[Rcpp::export]] 
LogicalVector Destination(IntegerVector dest, int N, int seed){
    //cout<<dest[0]<<dest[1]<<endl;//for debugging
    IntegerMatrix m = walk2(N, seed); 
    LogicalVector reached(N,false);
    int road;
    for(int i =0;i<N;i++){
        //cout<<m(i,0)<<m(i,1)<<endl;//for debugging
        if((m(i,0)==dest[0]&&m(i,1)==dest[1])){
            reached[i]=true;
            road = i;
        }
    }
    //cout<<road<<endl; shows index, ie after how many roads do they find the destination
    return(reached);
}

// [[Rcpp::export]] 
//**QUESTION 4**
//manhattan distance is defined as the min number of "roads" taken to get to a coordinate destination
int ManhattanD(int n, int seed){  //finds the Manhatan distance after n amount of moves
    IntegerMatrix m = walk2(n, seed); 
    int dist =  abs(m(n-1,0))+abs(m(n-1,1));
    return(dist);
}
