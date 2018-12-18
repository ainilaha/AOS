#include <stdio.h>
#include <mpi.h>
/* Run with 16 processes */
int main (int argc, char *argv[]) 
{
   int rank;
   struct {
      double value;
      int rank;
   } in, out;

   int root;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD,&rank);
   in.value = rank+1;
   in.rank = rank;
   root = 7;
   
   MPI_Reduce(&in,&out,1,MPI_DOUBLE_INT,MPI_MAXLOC,root,MPI_COMM_WORLD);
   if(rank==root) 
      printf("PE:%d max=%lf at rank %d\n",rank,out.value,out.rank);

   MPI_Reduce(&in,&out,1,MPI_DOUBLE_INT,MPI_MINLOC,root,MPI_COMM_WORLD);
   if(rank == root) 
      printf("PE:%d min=%lf at rank %d\n",rank,out.value,out.rank);

	MPI_Finalize();
}