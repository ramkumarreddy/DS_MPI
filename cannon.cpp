#include "mpi.h"
#include <bits/stdc++.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#define  ARRAYSIZE	16
#define  MASTER	0

using namespace std;
int data[ARRAYSIZE];
int A[1000][1000] = {0};
int B[1000][1000] = {0};

int a[1000][1000] = {0};
int e[1000][1000] = {0};


void print(int z[][1000],int x){
    int i,j;
    cout << "hu " << x << endl;
    for(i=0;i<x;i++){
        for(j=0;j<x;j++){
            cout << z[i][j] << " ";
        }
        cout << "\n";
    }
}


int main (int argc, char *argv[])
{
    int numtasks, taskid, i, j, k, l, source, n, m; 
    int p, N, temp, c, temp1[1000][1000]={0};
    
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

        p = sqrt(numtasks-1);              // pxp grid of processorsa
        if(n%p==0)
            N = n;
        else
            N = (n/p)*p+p;               // new NxN such that it divides p
        c = N/p;


        for(i=0;i<p;i++){
            for(j=0;j<p;j++){
                for(k=0;k<c;k++){
                    for(l=0;l<c;l++){
                        a[(c*i)+k][(c*j)+l] = A[(c*i)+k][(c*((j+i)%p))+l];     // Shift(A, i, ’left’) skewing
                    }
                }
            }

            for(j=0;j<p;j++){
                for(k=0;k<c;k++){
                    for(l=0;l<c;l++){
                        A[(c*i)+k][(c*j)+l] = a[(c*i)+k][(c*j)+l];
                    }
                }
            }
        }


        for(j=0;j<p;j++){
            for(i=0;i<p;i++){
                for(k=0;k<c;k++){
                    for(l=0;l<c;l++){
                        a[(c*i)+k][(c*j)+l] = B[(c*((i+j)%p))+k][(c*j)+l];     // Shift(B, i, ’up’) skewing
                    }
                }
            }

            for(i=0;i<p;i++){
                for(k=0;k<c;k++){
                    for(l=0;l<c;l++){
                        B[(c*i)+k][(c*j)+l] = a[(c*i)+k][(c*j)+l];
                    }
                }
            }
        }


        for(i=0;i<p;i++){
            for(j=0;j<p;j++){
                for(k=0;k<c;k++){
                    for(l=0;l<c;l++){
                        a[k][l] = A[(i*c)+k][(j*c)+l];
                        e[k][l] = B[(i*c)+k][(j*c)+l];
                    }
                }
                // cout << "here" << endl;
                MPI_Send(&(a[0][0]), 1000000, MPI_INT, (i*p)+j+1, 101, MPI_COMM_WORLD);
                MPI_Send(&(e[0][0]), 1000000, MPI_INT, (i*p)+j+1, 102, MPI_COMM_WORLD);                 // sending skewed matrix
                MPI_Send(&N, 1, MPI_INT, (i*p)+j+1, 103, MPI_COMM_WORLD);
                MPI_Send(&p, 1, MPI_INT, (i*p)+j+1, 104, MPI_COMM_WORLD);
                MPI_Send(&c, 1, MPI_INT, (i*p)+j+1, 105, MPI_COMM_WORLD);
            }
        }


        for(i=0;i<p;i++){
            for(j=0;j<p;j++){
                MPI_Recv(&(a[0][0]), 1000000, MPI_INT, (i*p)+j+1, 201, MPI_COMM_WORLD,MPI_STATUS_IGNORE);       // recieving final matrix from all processors
                for(k=0;k<c;k++){
                    for(l=0;l<c;l++){
                        e[i*c+k][j*c+l] = a[k][l];
                    }
                }
            }
        }

        for(i=0;i<n;i++){
            for(j=0;j<n;j++){
                cout << e[i][j] << " ";
            }
            cout << "\n";
        }
    }


    /***** Non-master tasks only *****/
    if (taskid > MASTER)
    {
        source = MASTER;
        MPI_Recv(&(a[0][0]), 1000000, MPI_INT, 0, 101, MPI_COMM_WORLD, &status);
        MPI_Recv(&(e[0][0]), 1000000, MPI_INT, 0, 102, MPI_COMM_WORLD, &status);        
        MPI_Recv(&N, 1, MPI_INT, 0, 103, MPI_COMM_WORLD, &status);
        MPI_Recv(&p, 1, MPI_INT, 0, 104, MPI_COMM_WORLD, &status);
        MPI_Recv(&c, 1, MPI_INT, 0, 105, MPI_COMM_WORLD, &status);
        int left,right,top,below;
        int r_no,c_no;

        for(i=0;i<p;i++){
            r_no = (taskid-1)/p;
            c_no = (taskid-1)%p;

            left = (((c_no-1)%p)+p)%p;
            right = (((c_no+1)%p)+p)%p;
            top = (((r_no-1)%p)+p)%p;
            below = (((r_no+1)%p)+p)%p;

            if(i!=0){
                MPI_Recv(&(a[0][0]), 1000000, MPI_INT, (r_no*p+right)+1, 301, MPI_COMM_WORLD, &status);
                MPI_Recv(&(e[0][0]), 1000000, MPI_INT, (below*p+c_no)+1, 302, MPI_COMM_WORLD, &status);     // recieve matrix from right and below matrix
            }

            for(k=0;k<c;k++){
                for(l=0;l<c;l++){
                    temp = 0;
                    for(j=0;j<c;j++){
                        temp += a[k][j]*e[j][l];                  // Ai*Bi
                    }
                    temp1[k][l] += temp;
                }
            }
            MPI_Isend(&(a[0][0]), 1000000, MPI_INT, (r_no*p+left)+1, 301, MPI_COMM_WORLD, &send_request);
            MPI_Isend(&(e[0][0]), 1000000, MPI_INT, (top*p+c_no)+1, 302, MPI_COMM_WORLD, &send_request);              // send matrix to left and top matrices
        }
        MPI_Send(&(temp1[0][0]), 1000000, MPI_INT, 0, 201, MPI_COMM_WORLD);   // send final matrix to source
    }
    MPI_Finalize();
}