#include <stdio.h>
#include<mpi.h>

int main(int argc, char *argv[]) 
{
	int rank,i;
	MPI_Status status;
	struct {
		int num;
		float x;
		double data[4];
	}a;
   
	int blocklengths[3]={1,1,4};
	MPI_Datatype types[3]={MPI_INT, MPI_FLOAT, MPI_DOUBLE};
	MPI_Aint displacements[3];
	MPI_Datatype restype;
	MPI_Aint intex, floatex;
	
	MPI_Init(&argc, &argv);
	
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Type_extent(MPI_INT, &intex);
	MPI_Type_extent(MPI_FLOAT, &floatex);
	displacements[0] = (MPI_Aint) 0; 
	displacements[1] = intex;
	displacements[2] = intex+floatex;
	MPI_Type_create_struct(3, blocklengths, displacements, types, &restype);
	MPI_Type_commit(&restype);
	if (rank==3)
	{
		a.num = 6; 
		a.x = 3.14; 
		for(i=0;i < 4;++i) 
			a.data[i] = (double) i;

		MPI_Send(&a,1,restype,1,52,MPI_COMM_WORLD);
	} 
	else 
		if(rank==1) 
		{
			MPI_Recv(&a,1,restype,3,52,MPI_COMM_WORLD,&status);
			printf("P:%d my a is %d %f %lf %lf %lf %lf\n", rank,a.num,a.x,a.data[0],a.data[1],a.data[2],a.data[3]);
		}
	  
	MPI_Finalize();
}