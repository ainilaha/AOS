/***
 * 
 * name: Laha Ale
 *
 */

#include "laha_mpi.h"

int Laha_Scatter(
    void* send_buff,
    int send_count,
    MPI_Datatype send_type,
    void* recv_data,
    int recv_count,
    MPI_Datatype recv_type,
    int root,
    MPI_Comm communicator);

int main(int argc, char *argv[]){
   int rank,size,i,j;
   double param[4],mine;
   int sndcnt,revcnt;


  double time_spent = 0.0;
  

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD,&rank);
   MPI_Comm_size(MPI_COMM_WORLD,&size);
   double begin = MPI_Wtime();
   revcnt=1;

   if(rank==3)
   {
       for(i=0;i<size;i++) 
	      param[i]=23.0+i;
       sndcnt=1;
   }
   Laha_Scatter(param,sndcnt,MPI_DOUBLE,&mine,revcnt,MPI_DOUBLE,3,MPI_COMM_WORLD);
   //MPI_Scatter(param,sndcnt,MPI_DOUBLE,&mine,revcnt,MPI_DOUBLE,3,MPI_COMM_WORLD);
   printf("P:%d mine is %f\n",rank,mine);
   double end = MPI_Wtime();
   time_spent = end - begin;
   printf("%f\n", time_spent);
   MPI_Finalize();

   return 0;
}

