#include <iostream>
#include <cstring>
#include <ctime>
#include <chrono>
#include <stdio.h>
#include <mpi.h>
#include <fstream> 

#include "constansts.h"
#include "main.h"

int main(int argc, char* argv[]) {
    int        comm_sz;               /* Number of processes    */
    int        my_rank;               /* My process rank        */
    char       greeting[42];

    MPI_Init(NULL,NULL);

    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if(my_rank == 0){
        sprintf(greeting, "hi");
        std::cout<<"Sending"<<std::endl;
        MPI_Send(greeting, strlen(greeting)+1, MPI_CHAR, 1, 0,MPI_COMM_WORLD); 
        std::cout<<"Sending"<<std::endl;
        MPI_Send(greeting, strlen(greeting)+1, MPI_CHAR, 2, 0,MPI_COMM_WORLD); 
        std::cout<<"Sending"<<std::endl;
        MPI_Send(greeting, strlen(greeting)+1, MPI_CHAR, 3, 0,MPI_COMM_WORLD); 
        std::cout<<"Sent"<<std::endl;
    }else{
        int thiss;
        std::cout<<"Process "<<my_rank<<" ready to recieve"<<std::endl;
        MPI_Recv(greeting, 42, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout<<"Message recieved:"<<greeting<<" On process "<<my_rank<<std::endl;
    }

    MPI_Finalize();
}
