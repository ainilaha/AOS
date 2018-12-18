/***
 * 
 * name: Laha Ale
 *
 */
#include<mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>


struct Event{
    char type[5];
    int pid1;
    int pid2;
    int vector_time[100];
    char msg[100];
};

int read_and_dristr_events(char *filename,
                           int rank_size,
                           MPI_Datatype event_type,
                           int total_event_num);
int distribute_event(struct Event event_list[], 
                     MPI_Datatype event_type,
                     int total_event_num);
int check_events_number(char *filename);
void print_vector(int vector_time[],int rank,int rank_size);

int max(int local_vec_time, int global_vet_time);

int main(int argc, char *argv[])  
{

    int rank,rank_size,event_num;
    int local_d=1;
    int global_d=0;
    bool events_exist=false;
    MPI_Status status;
    MPI_Datatype event_type;
    struct Event event;
    
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&rank_size);
    MPI_Datatype types[5] = { MPI_CHAR, MPI_INT, MPI_INT,MPI_INT,MPI_CHAR };
    int blocklengths[5] = { 5, 1, 1, 100, 100 };
    MPI_Aint displacements[5],address,start_address;

 
    MPI_Get_address(&event, &start_address);
    displacements[0]  = (MPI_Aint) 0;

    MPI_Get_address(&event.pid1, &address);
    displacements[1] = address - start_address;
    MPI_Get_address(&event.pid2, &address);
    displacements[2] = address - start_address;
    MPI_Get_address(&event.vector_time, &address);
    displacements[3] = address - start_address;
    MPI_Get_address(&event.msg, &address);
    displacements[4] = address - start_address;    
    MPI_Type_create_struct( 5, blocklengths, displacements, types, &event_type);
    MPI_Type_commit(&event_type);  

    // create intialize local vector
    int vector_time[rank_size-1];
    for(int i=0;i<rank_size-1;i++)
    {
        vector_time[i]=0;
    }


   if(0==rank)
   {
       printf("There are %d processes in the system\n",rank_size);
       printf("The manager process 0 will manage the simulation\n");
       int total_event_num = check_events_number(argv[1]);
       read_and_dristr_events(argv[1],rank_size,event_type,total_event_num);
   }else
   {
       MPI_Recv(&event_num,1, MPI_INT, 0, 1, MPI_COMM_WORLD,&status);
       for(int i=0;i<event_num;i++)
       {
           
           MPI_Recv(&event,1, event_type, 0, 0, MPI_COMM_WORLD,&status);
           
           if(strstr(event.type,"send") != NULL && event.pid1==rank)
           {   
               events_exist = true;
               vector_time[rank-1] += local_d; // Rule 1
                for(int n=1;n<rank_size;n++)
                    {
                        event.vector_time[n-1] = vector_time[n-1];
                    }
               MPI_Send(&event, 1, event_type, event.pid2, 2, MPI_COMM_WORLD);
               printf( "Message sent from process %d to process %d: %s\n", rank, event.pid2, event.msg);
               print_vector(vector_time,rank,rank_size);
           }else if (strstr(event.type,"send") != NULL && event.pid2==rank)
           {
               events_exist = true;
               MPI_Recv(&event,1, event_type, event.pid1, 2, MPI_COMM_WORLD,&status);
               //Rule2
               vector_time[rank-1] += local_d;

               for(int j=1; j<rank_size;j++)
               {
                 vector_time[j-1] = max(vector_time[j-1],event.vector_time[j-1] + global_d);
               }
               print_vector(vector_time,rank,rank_size);
               printf( "Message received from process %d by process %d: %s\n", event.pid1, rank, event.msg );
           }else if(strstr(event.type,"exec") != NULL){
                events_exist = true;
                printf( "Execution event in process %d\n", rank );
                vector_time[rank-1] += local_d; // Rule 1
                print_vector(vector_time,rank,rank_size);
                for(int n=1;n<rank_size;n++)
                {
                    event.vector_time[n-1] = vector_time[n-1];
                }
           }else{
              print_vector(vector_time,rank,rank_size); 
           }
       }
 
   }
    MPI_Barrier(MPI_COMM_WORLD);
   if(!events_exist && rank!=0)
   {
       print_vector(vector_time,rank,rank_size); 
   }

   MPI_Finalize();

}


int read_and_dristr_events(char *filename,
                           int rank_size,
                           MPI_Datatype event_type,
                           int total_event_num)
{
    FILE *fp = fopen(filename, "r");
    int bufSize=255;
    char line[bufSize];
    int pid1, pid2,rank_range;
    struct Event events_list[total_event_num];
    int event_num_list[rank_size];
    struct Event event;
    if (fp == NULL) {
        printf("can open file :%s", filename);
        return 1;
    }
    for(int i=0;i<rank_size;i++){
        event_num_list[i] = 0;
    }
    int i=0;
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        
        line[strcspn(line, "\r\n")] = 0; 
        if(strstr(line,"end") != NULL && strstr(line,"send") == NULL)
        {
            break;
        }
        if(i>0)
        {
            strcpy(event.type, strtok (line," "));
            event.pid1 = atoi(strtok (NULL, " "));
            event_num_list[event.pid1] +=1; 
            if(strstr(event.type,"send") != NULL) 
            {
                event.pid2 = atoi(strtok (NULL, " "));
                event_num_list[event.pid2] +=1; 
                strcpy(event.msg, strtok (NULL,"\""));
            } else{
                event.pid2 = 0;
            }
            if(event.pid1>rank_size || event.pid2>rank_size)
            {
                
               printf("Both PID1=%d and PID2=%d have to smaller than rank size = %d \n",event.pid1,event.pid2,rank_size);
               return MPI_Abort(MPI_COMM_WORLD,1);
            }
            events_list[i-1] = event;
        }else
        {
            rank_range = atoi(strtok (line," "));
            if((rank_range+1) !=rank_size)
            {
                
                printf("Rank size =%d is NOT match in configure file rank range=%d! \n",rank_size,rank_range);
                return MPI_Abort(MPI_COMM_WORLD,1);
            } 
        }
   
        i++;
        
    }

   for(int j=1;j<rank_size;j++)
   {
       MPI_Send(&event_num_list[j], 1, MPI_INT,j, 1, MPI_COMM_WORLD);
   }

   distribute_event(events_list,event_type,total_event_num);
    fclose(fp);
    // free(line);
    return 0; 
}

int distribute_event(struct Event event_list[], MPI_Datatype event_type,int total_event_num)
{
    for(int i=0;i<total_event_num;i++)
    {
        MPI_Send(&event_list[i], 1, event_type, event_list[i].pid1, 0, MPI_COMM_WORLD);
        if(event_list[i].pid2!=0){
            MPI_Send(&event_list[i], 1, event_type, event_list[i].pid2, 0, MPI_COMM_WORLD);
        }
    }

}


int check_events_number(char *filename)
{
    int count=0;
    char line[150];
    FILE *fp = fopen(filename, "r");
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        line[strcspn(line, "\r\n")] = 0;
        if(strlen(line)!=0)
        {
            count++;
        } 
    }
    return count-2; // first and last line are not events;
}



int max(int local_time, int global_time)
{
    if(local_time>global_time)
    {
        return local_time;
    }else
    {
        return global_time;
    }
}


void print_vector(int vector_time[],int rank,int rank_size)
{
    char buf[255];
    char time[5];
    sprintf(buf, "Vector time at process %d is:[", rank);

    for(int m=0;m<rank_size-1;m++)
        {
            sprintf(time," %d", vector_time[m]);
            strcat(buf,time);
        }
    strcat(buf,"]\n");
    printf("%s", buf);
}


