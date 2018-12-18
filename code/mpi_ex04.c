#include <mpi.h>
#include <math.h>
#include <stdio.h>
int main(int argc, char *argv[]) 
{
  int rank,i,j;
  MPI_Status status;
  double x[4][8];
  MPI_Datatype coltype;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Type_vector(4,1,8,MPI_DOUBLE,&coltype);
  MPI_Type_commit(&coltype);
  if(rank == 3)
  {
     for(i=0;i<4;++i)
        for(j=0;j<8;++j) 
		   x[i][j] = pow(10.0,i+1) + j;
        MPI_Send(&x[0][6],1,coltype,1,52,MPI_COMM_WORLD);
  } 
  else 
     if(rank==1) 
	 {
        MPI_Recv(&x[0][2],1,coltype,3,52,MPI_COMM_WORLD,&status);
        printf("P:%d my x[%d][2]=%1f\n",rank,i,x[i][2]);
     }

	 MPI_Finalize();
}