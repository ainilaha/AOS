/***
 * 
 * name: Laha Ale
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <mpi.h>
#include <time.h>
/***
 * This is a mpi function implemation header file.
 * This file can be use differen files

/***
 * 
 * declare of scatter function
 * 
 * */
int Laha_Scatter(
    void* send_buff,
    int send_count,
    MPI_Datatype send_type,
    void* recv_data,
    int recv_count,
    MPI_Datatype recv_type,
    int root,
    MPI_Comm communicator);
/***
 * 
 * declare of gather function
 * 
 * */
int Laha_Gather(void* sendbuf, 
                int send_count ,
                MPI_Datatype send_type ,
                void* recvbuf ,
                int recv_count , 
                MPI_Datatype recv_type , 
                int root , MPI_Comm communicator);
/***
 * 
 * declare of Allgather function
 * 
 * */
 int Laha_Allgather(void* sendbuf, 
                int send_count ,
                MPI_Datatype send_type ,
                void* recvbuf ,
                int recv_count , 
                MPI_Datatype recv_type , 
                MPI_Comm communicator);              

/***
 * 
 * implemetation of Scatter function
 * 
 * */
int Laha_Scatter(
    void* send_buff,
    int send_count,
    MPI_Datatype send_type,
    void* recv_data,
    int recv_count,
    MPI_Datatype recv_type,
    int root,
    MPI_Comm communicator)    
    { 
        int rank_size,rank;
        void *send_address;
        int tag=0;
        MPI_Status status;
        MPI_Comm_size(communicator,&rank_size);
        MPI_Comm_rank(communicator,&rank);
        if(rank==root)
        {
            for(int i=0;i<rank_size;i++)
                {
                    send_address = send_buff+sizeof(send_type)*i;
                    assert(rank_size != 0);
                    MPI_Send(send_address,send_count, send_type,i,tag,communicator);
                }
        }

        MPI_Recv(recv_data,recv_count,recv_type,root,tag,communicator, &status);
        int actual_recv_cnt;
        MPI_Get_count(&status, recv_type, &actual_recv_cnt);
        assert(recv_count==actual_recv_cnt);
        return 0;    

    }
/***
 * 
 * implemetation of gather function
 * 
 * */
int Laha_Gather(void* sendbuf, 
                int send_count ,
                MPI_Datatype send_type ,
                void* recvbuf ,
                int recv_count , 
                MPI_Datatype recv_type , 
                int root , MPI_Comm communicator)
    {
        int rank_size,rank;
        void *recv_address;
        int tag=0;
        MPI_Status status;
        MPI_Comm_size(communicator,&rank_size);
        MPI_Comm_rank(communicator,&rank);

        MPI_Send(sendbuf,send_count, send_type,root,tag,communicator);

        if(rank==root)
        {
            for(int i=0;i<rank_size;i++)
                {
                    recv_address = recvbuf+sizeof(send_type)*i;
                    MPI_Recv(recv_address,recv_count,recv_type,i,tag,communicator, &status); 
                    //error check goes heres
                    int actual_recv_cnt;
                    MPI_Get_count(&status, recv_type, &actual_recv_cnt);
                    assert(recv_count==actual_recv_cnt);
                }
        }
        return 0;           
    }
/***
 * 
 * implemetation of allgather function
 * 
 * */
int Laha_Allgather(void* sendbuf, 
                int send_count ,
                MPI_Datatype send_type ,
                void* recvbuf ,
                int recv_count , 
                MPI_Datatype recv_type , 
                MPI_Comm communicator)
    {
        int rank_size;  
        MPI_Comm_size(communicator,&rank_size);    
        for(int i=0;i<rank_size;i++)
            {
                Laha_Gather(sendbuf,send_count, send_type, recvbuf, recv_count,recv_type,i,communicator);
            }
        return 0;    
    }
