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
int B[1000][2];
int C[100][100];
int D[100][100];
int E[100][100] = {0};
int Z[100][100] = {0};

int a[100][100] = {0};
int e[100][100] = {0};
int an[100][100] = {0};
// int temp1[100][100] = {0};


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


int main (int argc, char *argv[])
{
    int numtasks, taskid, rc, dest, offset, i, j, k, l, tag1, tag2, tag, source, chunksize, n, m, v1, v2, poi, no_of_vertices,trace; 
    int mysum, sum, matrixsum, neighbourssum;
    int p, N, c, temp1[100][100]={0};
    int chunksize1, offset1, temp, mysum1,sum1, myarr[1000];

    for(i=0; i<100; i++){
        for(j=0; j<100; j++){
            A[i][j] = 0;
        }
    }
    
    // int update(int myoffset, int chunk, int myid);
    
    MPI_Status status;
    MPI_Request request,send_request;


    /***** Initializations *****/
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
    // printf ("MPI task %d has started...\n", taskid);
    
    chunksize = (ARRAYSIZE / numtasks);
    tag2 = 1;
    tag1 = 2;
    tag = 0;

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
                cin >> E[i][j];
            }
        }

        p = sqrt(numtasks-1);              // pxp grid of processors
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
                        a[(c*i)+k][(c*j)+l] = E[(c*((i+j)%p))+k][(c*j)+l];     // Shift(E, i, ’up’) skewing
                    }
                }
            }

            for(i=0;i<p;i++){
                for(k=0;k<c;k++){
                    for(l=0;l<c;l++){
                        E[(c*i)+k][(c*j)+l] = a[(c*i)+k][(c*j)+l];
                    }
                }
            }
        }

        cout << "skewing done" << "\n";


        for(i=0;i<p;i++){
            for(j=0;j<p;j++){
                for(k=0;k<c;k++){
                    for(l=0;l<c;l++){
                        a[k][l] = A[(i*c)+k][(j*c)+l];
                        e[k][l] = E[(i*c)+k][(j*c)+l];
                    }
                }
                cout << "here" << endl;
                MPI_Send(&(a[0][0]), 10000, MPI_INT, (i*p)+j+1, 101, MPI_COMM_WORLD);
                MPI_Send(&(e[0][0]), 10000, MPI_INT, (i*p)+j+1, 102, MPI_COMM_WORLD);                 // sending skewed matrix
                MPI_Send(&N, 1, MPI_INT, (i*p)+j+1, 103, MPI_COMM_WORLD);
                MPI_Send(&p, 1, MPI_INT, (i*p)+j+1, 104, MPI_COMM_WORLD);
                MPI_Send(&c, 1, MPI_INT, (i*p)+j+1, 105, MPI_COMM_WORLD);
            }
        }


        for(i=0;i<p;i++){
            for(j=0;j<p;j++){
                MPI_Recv(&(a[0][0]), 10000, MPI_INT, (i*c)+j+1, 201, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                for(k=0;k<c;k++){
                    for(l=0;l<c;l++){
                        e[i*c+k][j*c+l] = a[k][l];
                    }
                }
                // for(k=0;k<n;k++){
                //    for(l=0;l<n;l++){
                //         cout << e[k][l] << " ";
                //     }
                //     cout << "\n";
                // }
                // cout << "jj" << endl;
            }
        }

        // for(i=0;i<N;i++){
        //     for(j=0;j<N;j++){
        //         if(A[i][j]<10)
        //                 cout << "0";
        //         cout << A[i][j] << " ";
        //     }
        //     cout << "\n";
        // }


        // for(i=0;i<N;i++){
        //     for(j=0;j<N;j++){
        //         if(E[i][j]<10)
        //             cout << "0";
        //         cout << E[i][j] << " ";
        //     }
        //     cout << "\n";
        // }

        for(i=0;i<N;i++){
            for(j=0;j<N;j++){
                cout << e[i][j] << " ";
            }
            cout << "\n";
        }
        cout << "final" << endl;

    }



    /***** Non-master tasks only *****/
    if (taskid > MASTER)
    {
        source = MASTER;
        MPI_Recv(&(a[0][0]), 10000, MPI_INT, 0, 101, MPI_COMM_WORLD, &status);
        MPI_Recv(&(e[0][0]), 10000, MPI_INT, 0, 102, MPI_COMM_WORLD, &status);        
        MPI_Recv(&N, 1, MPI_INT, 0, 103, MPI_COMM_WORLD, &status);
        MPI_Recv(&p, 1, MPI_INT, 0, 104, MPI_COMM_WORLD, &status);
        MPI_Recv(&c, 1, MPI_INT, 0, 105, MPI_COMM_WORLD, &status);
        // cout << taskid << endl;
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
                MPI_Recv(&(a[0][0]), 10000, MPI_INT, (r_no*p+right)+1, 301, MPI_COMM_WORLD, &status);
                MPI_Recv(&(e[0][0]), 10000, MPI_INT, (below*p+c_no)+1, 302, MPI_COMM_WORLD, &status);
                // if(taskid==2){
                //     cout << taskid << " " << r_no << " " << c_no << " " << left << " " << right << " " << top << " " << below <<  endl;
                //     cout << "right " << (r_no*p+right)+1 << endl;
                //     cout << "left " << (below*p+c_no)+1 << endl;
                //     cout << "recieved\n";
                // }
            }

            cout << taskid << " recieve " << " right:" << (r_no*p+right)+1 << "  below:" << (below*p+c_no)+1 << endl;

            if(taskid==2){
                cout << "In 2 " << i <<  endl;
                print(a,c);
                print (e,c);
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
            //print(temp1,c);

            MPI_Isend(&(a[0][0]), 10000, MPI_INT, (r_no*c+left)+1, 301, MPI_COMM_WORLD, &send_request);
            MPI_Isend(&(e[0][0]), 10000, MPI_INT, (top*c+c_no)+1, 302, MPI_COMM_WORLD, &send_request);

            // cout << taskid << " send " << " left:" << (r_no*c+left)+1 << "  top:" << (top*c+c_no)+1 << endl;
            // cout << "Sent from " << taskid << "to " << (r_no*c+left)+1 << " and " << (top*c+c_no)+1 << endl;
        }
        // cout << "kkk " << endl;
        MPI_Send(&(temp1[0][0]), 10000, MPI_INT, 0, 201, MPI_COMM_WORLD);





        // MPI_Recv(&(A[0][0]), 10000, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
        // MPI_Recv(&(E[0][0]), 10000, MPI_INT, 0, 7, MPI_COMM_WORLD, &status);
        // MPI_Recv(&(B[0][0]), 2000, MPI_INT, 0, 3, MPI_COMM_WORLD, &status);
        // MPI_Recv(&offset1, 1, MPI_INT, source, 4, MPI_COMM_WORLD, &status);
        // MPI_Recv(&chunksize1, 1, MPI_INT, source, 5, MPI_COMM_WORLD, &status);
        // MPI_Recv(&no_of_vertices, 1, MPI_INT, source, 6, MPI_COMM_WORLD, &status);
        // poi = 0;
        // for(i=offset1;i<offset1+chunksize1;i++){
        //     mysum1 = 0;
        //     for(j=0;j<no_of_vertices;j++){
        //         mysum1 += A[B[i][0]][j]*E[j][B[i][1]];
        //     }
        //     myarr[poi] = mysum1;
        //     poi += 1;
        // }

        // MPI_Send(myarr, poi, MPI_INT, 0, 0, MPI_COMM_WORLD);
    } 


    MPI_Finalize();

}