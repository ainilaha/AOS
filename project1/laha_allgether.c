/***
 * 
 * name: Laha Ale
 *
 */


#include"laha_mpi.h"


int main(int argc, char *argv[]){
   int rank,size,i,j;
   
   int sndcnt,revcnt;
   double time_spent = 0.0;
   clock_t begin = clock();

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD,&rank);
   MPI_Comm_size(MPI_COMM_WORLD,&size);
   double recv_address[size],send_data;
   sndcnt=1;
   revcnt=1;
   send_data = rank+20.5; 

    if(sizeof(recv_address)/sizeof(recv_address[0])< size)
    {
        printf("The receive size is must geater or equal to the rank size!\n");
        return MPI_Abort(MPI_COMM_WORLD,1);
    }

    Laha_Allgather(&send_data,sndcnt,MPI_DOUBLE,recv_address,revcnt,MPI_DOUBLE,MPI_COMM_WORLD);
    //MPI_Allgather(&send_data,sndcnt,MPI_DOUBLE,recv_address,revcnt,MPI_DOUBLE,MPI_COMM_WORLD);
    for(int i=0;i<size;i++)
    {
        printf("P:%d from :%f\n",rank,recv_address[i]);
    }
 
   MPI_Finalize();
   clock_t end = clock();
   time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
   printf("%f\n", time_spent);
   return 0;
}