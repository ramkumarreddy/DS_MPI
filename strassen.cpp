#include "mpi.h"
#include <bits/stdc++.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#define  ARRAYSIZE	16
#define  MASTER	0

using namespace std;
int data[ARRAYSIZE];
int A[100][100] = {0};
int B[100][100] = {0};

int A1[100][100] = {0};
int B1[100][100] = {0};

int p1[100][100] = {0};
int p2[100][100] = {0};
int p3[100][100] = {0};
int p4[100][100] = {0};
int p5[100][100] = {0};
int p6[100][100] = {0};
int p7[100][100] = {0};

int a[100][100] = {0};
int b[100][100] = {0};
int c[100][100] = {0};
int d[100][100] = {0};

int e[100][100] = {0};
int f[100][100] = {0};
int g[100][100] = {0};
int h[100][100] = {0};

int C[100][100] = {0};

void print(int z[][100],int x){
    int i,j;
    cout << "hu " << x << endl;
    for(i=0;i<x;i++){
        for(j=0;j<x;j++){
            cout << z[i][j] << " ";
        }
        cout << "\n";
    }
}


void add(int x[][100], int y[][100], int n){
    int i,j;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            x[i][j] += y[i][j];
        }
    }
}

void subtract(int x[][100], int y[][100], int n){
    int i,j;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            x[i][j] -= y[i][j];
        }
    }
}


void multiply(int x[][100],int y[][100], int n){
    int i,j,k;
    int z[100][100] = {0};
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            for(k=0;k<n;k++){
                z[i][j] = x[i][k]*y[k][j];
            }
        }
    }
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            x[i][j] = z[i][j];
        }
    }
}

int main (int argc, char *argv[])
{
    int numtasks, taskid, i, j, k, l, source, n, m; 
    int p, N;
    int cur_level, parent_id, task, temp, temp1, levelby7, max_level;
    
    MPI_Status status;
    MPI_Request request,send_request;

    /***** Initializations *****/
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&taskid);

    /***** Master task only ******/
    if (taskid == MASTER)
    {
        cin >> n;
        for(i=0;i<n;i++){
            for(j=0;j<n;j++){
                cin >> A[i][j];
            }
        }

        for(i=0;i<n;i++){
            for(j=0;j<n;j++){
                cin >> B[i][j];
            }
        }


        N = pow(2,ceil(log2(n)));   // matrix size after padding
        temp1 = 1;
        temp = 0;
        levelby7 = 0;
        for(i=0;i<n;i++){
            temp += temp1;
            if(temp>numtasks){
                break;
            }
            levelby7 += 1;
            temp1 *= 7; 
        }
        max_level = min(int(log2(N)),levelby7);           // maxlevels we can divide matrix
        cur_level = 1;


        for(i=1;i<=7;i++){
            MPI_Send(&(A[0][0]), 10000, MPI_INT, i, 101, MPI_COMM_WORLD);
            MPI_Send(&(B[0][0]), 10000, MPI_INT, i, 102, MPI_COMM_WORLD);
            MPI_Send(&N, 1, MPI_INT, i, 103, MPI_COMM_WORLD);
            MPI_Send(&i, 1, MPI_INT, i, 104, MPI_COMM_WORLD);
            MPI_Send(&cur_level, 1, MPI_INT, i, 105, MPI_COMM_WORLD);
            MPI_Send(&max_level, 1, MPI_INT, i, 106, MPI_COMM_WORLD);
            MPI_Send(&parent_id, 1, MPI_INT, i, 107, MPI_COMM_WORLD);
        }

        MPI_Recv(&(p1[0][0]), 10000, MPI_INT, 1, 201, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&(p2[0][0]), 10000, MPI_INT, 2, 201, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&(p3[0][0]), 10000, MPI_INT, 3, 201, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&(p4[0][0]), 10000, MPI_INT, 4, 201, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&(p5[0][0]), 10000, MPI_INT, 5, 201, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&(p6[0][0]), 10000, MPI_INT, 6, 201, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&(p7[0][0]), 10000, MPI_INT, 7, 201, MPI_COMM_WORLD, MPI_STATUS_IGNORE);



        a = subtract(add(add(p5,p4,N/2),p6,N/2),p2,N/2);
        b = add(p1,p2,N/2);
        c = add(p3,p4,N/2);
        d = subtract(add(p1,p5,N/2),add(p3,p7,N/2),N/2);

        while(1){                       // Combining a,b,c,d to get final matrix
            for(i=0;i<N/2;i++){
                for(j=0;j<N/2;j++){
                    C[i][j] = a[i][j];
                }
            }
            for(i=0;i<N/2;i++){ 
                for(j=0;j<N/2;j++){
                    C[i][j+N/2] = b[i][j];
                }
            }
            for(i=0;i<N/2;i++){
                for(j=0;j<N/2;j++){
                    C[i+N/2][j] = c[i][j];
                }
            }
            for(i=0;i<N/2;i++){
                for(j=0;j<N/2;j++){
                    C[i+N/2][j+N/2] = d[i][j];
                }
            }
        }

        for(i=0;i<n;i++){
            for(j=0;j<n;j++){
                cout << C[i][j] << " ";     // Printing final matrix
            }
            cout << "\n";
        }
    }


    /***** Non-master tasks only *****/
    if (taskid > MASTER)
    {
        source = MASTER;
        MPI_Recv(&(A[0][0]), 10000, MPI_INT, 0, 101, MPI_COMM_WORLD, &status);
        MPI_Recv(&(B[0][0]), 10000, MPI_INT, 0, 102, MPI_COMM_WORLD, &status);
        MPI_Recv(&N, 1, MPI_INT, 0, 103, MPI_COMM_WORLD, &status);
        MPI_Recv(&task, 1, MPI_INT, 0, 104, MPI_COMM_WORLD, &status);
        MPI_Recv(&cur_level, 1, MPI_INT, 0, 105, MPI_COMM_WORLD, &status);
        MPI_Recv(&max_level, 1, MPI_INT, 0, 106, MPI_COMM_WORLD, &status);
        MPI_Recv(&parent_id, 1, MPI_INT, 0, 107, MPI_COMM_WORLD, &status);

        while(1){                     // Dividing matrix A,B into a,b,c,d,e,f,g,h
            for(i=0;i<N/2;i++){
                for(j=0;j<N/2;j++){
                    a[i][j] = A[i][j];
                    e[i][j] = B[i][j];
                }
            }
            for(i=0;i<N/2;i++){
                for(j=0;j<N/2;j++){
                    b[i][j] = A[i][j+N/2];
                    f[i][j] = B[i][j+N/2];
                }
            }
            for(i=0;i<N/2;i++){
                for(j=0;j<N/2;j++){
                    c[i][j] = A[i+N/2][j];
                    g[i][j] = B[i+N/2][j];
                }
            }
            for(i=0;i<N/2;i++){
                for(j=0;j<N/2;j++){
                    d[i][j] = A[i+N/2][j+N/2];
                    h[i][j] = B[i+N/2][j+N/2];
                }
            }
        }

        if(task==1){
            memcpy(A1, a, sizeof(int)*100*100);
            B1 = subtract(f,h,N/2);
        }
        else if(task==2){
            A1 = add(a,b,N/2);
            memcpy(B1, h, sizeof(int)*100*100);
        }
        else if(task==3){
            A1 = add(c,d,N/2);
            memcpy(B1, e, sizeof(int)*100*100);
        }
        else if(task==4){
            memcpy(A1, d, sizeof(int)*100*100);
            B1 = subtract(g,e,N/2);
        }
        else if(task==5){
            A1 = add(a,d,N/2);
            B1 = add(e,h,N/2);
        }
        else if(task==6){
            A1 = subtract(b,d,N/2);
            B1 = add(g,h,N/2);
        }
        else if(task==7){
            A1 = subtract(a,c,N/2);
            B1 = add(e,f,N/2);
        }


        if(cur_level>=max_level){
            C = multiply(A1,B1,N/2);
            MPI_Send(&(C[0][0]), 10000, MPI_INT, parent_id, 201, MPI_COMM_WORLD);
        }
        else{
            int new_N = N/2;
            cur_level += 1;

            int t1,t2,t3;
            t1 = 7;
            t2 = 0;
            t3 = 0;
            while(1){
                if(t2>=taskid)
                    break;
                t3 = t2;
                t2 += t1;
                t1 *= 7;
            }

            for(i=1;i<=7;i++){
                MPI_Send(&(A1[0][0]), 10000, MPI_INT, t2+(taskid-t3-1)*7+i, 101, MPI_COMM_WORLD);
                MPI_Send(&(B1[0][0]), 10000, MPI_INT, t2+(taskid-t3-1)*7+i, 102, MPI_COMM_WORLD);
                MPI_Send(&new_N, 1, MPI_INT, t2+(taskid-t3-1)*7+i, 103, MPI_COMM_WORLD);
                MPI_Send(&i, 1, MPI_INT, t2+(taskid-t3-1)*7+i, 104, MPI_COMM_WORLD);
                MPI_Send(&cur_level, 1, MPI_INT, t2+(taskid-t3-1)*7+i, 105, MPI_COMM_WORLD);
                MPI_Send(&max_level, 1, MPI_INT, t2+(taskid-t3-1)*7+i, 106, MPI_COMM_WORLD);
                MPI_Send(&taskid, 1, MPI_INT, t2+(taskid-t3-1)*7+i, 107, MPI_COMM_WORLD);
            }

            MPI_Recv(&(p1[0][0]), 10000, MPI_INT, t2+(taskid-t3-1)*7+1, 201, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&(p2[0][0]), 10000, MPI_INT, t2+(taskid-t3-1)*7+2, 201, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&(p3[0][0]), 10000, MPI_INT, t2+(taskid-t3-1)*7+3, 201, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&(p4[0][0]), 10000, MPI_INT, t2+(taskid-t3-1)*7+4, 201, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&(p5[0][0]), 10000, MPI_INT, t2+(taskid-t3-1)*7+5, 201, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&(p6[0][0]), 10000, MPI_INT, t2+(taskid-t3-1)*7+6, 201, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&(p7[0][0]), 10000, MPI_INT, t2+(taskid-t3-1)*7+7, 201, MPI_COMM_WORLD, MPI_STATUS_IGNORE);



            a = subtract(add(add(p5,p4,N/2),p6,N/2),p2,N/2);
            b = add(p1,p2,N/2);
            c = add(p3,p4,N/2);
            d = subtract(add(p1,p5,N/2),add(p3,p7,N/2),N/2);

            while(1){                       // Combining a,b,c,d to get final matrix
                for(i=0;i<N/2;i++){
                    for(j=0;j<N/2;j++){
                        C[i][j] = a[i][j];
                    }
                }
                for(i=0;i<N/2;i++){ 
                    for(j=0;j<N/2;j++){
                        C[i][j+N/2] = b[i][j];
                    }
                }
                for(i=0;i<N/2;i++){
                    for(j=0;j<N/2;j++){
                        C[i+N/2][j] = c[i][j];
                    }
                }
                for(i=0;i<N/2;i++){
                    for(j=0;j<N/2;j++){
                        C[i+N/2][j+N/2] = d[i][j];
                    }
                }
            }
            MPI_Send(&(C[0][0]), 10000, MPI_INT, parent_id, 201, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
}