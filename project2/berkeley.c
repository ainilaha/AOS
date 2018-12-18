/***
 * 
 * name: Laha Ale
 *
 */
#include<mpi.h>
#include "simple_time.c"
#include <assert.h>


int init_clock(char *filename,int rank_size);
int check_initial_clock_number(char *filename,int rank_size);

int CLOCK_LEN=6;
int main(int argc, char *argv[]) 
{
   
   char clock_str[CLOCK_LEN];
   memset(clock_str, 0, sizeof(clock_str)); 
   struct Simple_Time local_clock,coord_clock,diff_clock,adjust_time,new_time;
   


   int rank,rank_size;
   int coordinator=-1;
   int threshold= 40;
   MPI_Status status;
   MPI_Datatype time_type;
   int actual_recv_cnt;
   MPI_Init(&argc,&argv);
   MPI_Comm_rank(MPI_COMM_WORLD,&rank);
   MPI_Comm_size(MPI_COMM_WORLD,&rank_size);
   MPI_Type_contiguous(2,MPI_INT,&time_type);
   MPI_Type_commit(&time_type);


   if(rank==0)
   {
     check_initial_clock_number(argv[1],rank_size);
     init_clock(argv[1],rank_size);
       
   }
   MPI_Recv(&coordinator,1,MPI_INT,0,1,MPI_COMM_WORLD,&status);
   MPI_Get_count(&status, MPI_INT, &actual_recv_cnt);
   assert(1==actual_recv_cnt);

   MPI_Recv(clock_str,CLOCK_LEN,MPI_CHAR,0,0,MPI_COMM_WORLD,&status);
   MPI_Get_count(&status, MPI_CHAR, &actual_recv_cnt);
   assert(CLOCK_LEN==actual_recv_cnt);
   MPI_Barrier(MPI_COMM_WORLD);  

   local_clock = parse_time(clock_str);
   printf("rank %d initial clock %d:%d\n",rank, local_clock.hour,local_clock.minute);



  if(rank==coordinator)
   {
       printf("I am process with rank %d acting as the coordinator process\n", rank);
       for(int i=0; i<rank_size;i++)
       {
           MPI_Send(&local_clock, 1, time_type, i,3, MPI_COMM_WORLD);
       }
   } 
   MPI_Recv(&coord_clock,1,time_type,coordinator,3,MPI_COMM_WORLD,&status);
   MPI_Get_count(&status, time_type, &actual_recv_cnt);
   assert(1==actual_recv_cnt);   


   diff_clock = time_minus(local_clock,coord_clock);
   MPI_Send(&diff_clock, 1, time_type,coordinator, 4, MPI_COMM_WORLD);
   printf( "Process %d is sending time differential value of %d:%d to process %d\n", 
            rank, diff_clock.hour,diff_clock.minute, coordinator); 

   if(rank==coordinator)
   {
       struct Simple_Time diff_clock_list[rank_size];
       struct Simple_Time recev_clock,avg_time,adj_time;
       int sum = 0;
       int avg = 0;
       for(int i=0;i<rank_size;i++)
       {
            MPI_Recv(&recev_clock,1,time_type,i,4,MPI_COMM_WORLD,&status); 
            MPI_Get_count(&status, time_type, &actual_recv_cnt);
            assert(1==actual_recv_cnt);
            printf("Process %d has received time differential value of %d:%d\n",
                                rank, recev_clock.hour,recev_clock.minute);
            sum =  +(recev_clock.hour*60+recev_clock.minute);   
            diff_clock_list[i] = recev_clock;                 
       }
       avg_time = time_avg(diff_clock_list,rank_size,rank,threshold); //


     for(int i=0; i<rank_size;i++)
       {
           adj_time = time_minus(avg_time,diff_clock_list[i]);
  
           MPI_Send(&adj_time, 1, time_type, i,5, MPI_COMM_WORLD);
       }

   }


     MPI_Recv(&adjust_time,1,time_type,coordinator,5,MPI_COMM_WORLD,&status); 
             
    MPI_Get_count(&status, time_type, &actual_recv_cnt);
    assert(1==actual_recv_cnt);
     printf("Process %d has received time adjust value of %d:%d\n",
                        rank, adjust_time.hour,adjust_time.minute);  
    new_time = time_addition(adjust_time,local_clock);                    
    printf("Adjusted local time at process %d is %d:%d \n", rank, new_time.hour,new_time.minute); 


   MPI_Finalize();

}

// read time from config file and distribute them
int init_clock(char *filename, int rank_size){
    FILE *fp = fopen(filename, "r");
    int bufSize=6;
    int coord,count;
    int i=-1;
    char line[bufSize];
    char c;
    if (fp == NULL) {
        printf("can open file :%s", filename);
        return 1;
    }
    while ((fgets(line, sizeof(line), fp) != NULL) && (i <rank_size))
    {

        line[strcspn(line, "\r\n")] = 0; 
        if(i==-1 && strlen(line)!=0){
           coord = atoi(line);
           for(int j=0;j<rank_size;j++)
           {
               MPI_Send(&coord, 1, MPI_INT, j, 1, MPI_COMM_WORLD);
           }
           i++;
        }
        
        else if(strlen(line)!=0)
        {
            MPI_Send(line, bufSize, MPI_CHAR, i, 0, MPI_COMM_WORLD);
            i++;
        }
        
    }

    
    
    fclose(fp);
    // free(line);
    return 0; 
}

//check wether the number of config clock are match rank size
int check_initial_clock_number(char *filename,int rank_size)
{
    int count=0;
    char line[CLOCK_LEN];
    FILE *fp = fopen(filename, "r");
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        line[strcspn(line, "\r\n")] = 0;
        if(strlen(line)!=0)
        {
            count++;
        } 
    }
    fclose(fp);
    if(rank_size > count-1)
    {
        printf("Please add %d initial clock in: %s\n",rank_size,filename);
        return MPI_Abort(MPI_COMM_WORLD,1);
    }
    return 0;
}
