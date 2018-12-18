/***
 * 
 * name: Laha Ale
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct Simple_Time
{
    int hour;
    int minute;
};
struct Simple_Time parse_time(char str[]);
struct Simple_Time time_minus(struct Simple_Time time1, struct Simple_Time time2);
struct Simple_Time time_addition(struct Simple_Time time1, struct Simple_Time time2);
struct Simple_Time time_avg(struct Simple_Time times[],
                            int size, int coord, int threshold_minutes);

struct Simple_Time parse_time(char str[])
{
  struct Simple_Time stime;     
  char * ch_hour;
  char * ch_minute;
  ch_hour = strtok (str,":");
  if (ch_hour != NULL){
    stime.hour = atoi(ch_hour);
  }
  ch_minute = strtok (NULL, ":");
  if (ch_minute != NULL){
       stime.minute = atoi(ch_minute);
  }


 return stime;
}

struct Simple_Time time_minus(struct Simple_Time time1, struct Simple_Time time2){
    struct Simple_Time interval;
    int minutes_time1 = time1.hour*60 + time1.minute;
    int minutes_time2 = time2.hour*60 + time2.minute;
    interval.minute = (minutes_time1 - minutes_time2)%60;
    interval.hour = (minutes_time1 - minutes_time2)/60;
    return interval;
}

struct Simple_Time time_addition(struct Simple_Time time1, struct Simple_Time time2){
    struct Simple_Time interval;
    int minutes;
    int minutes_time1 = time1.hour*60 + time1.minute;
    int minutes_time2 = time2.hour*60 + time2.minute;
    minutes = minutes_time1 + minutes_time2;

    interval.minute = minutes % 60;
    interval.hour = minutes/60;
    interval.minute = minutes - interval.hour*60;
    return interval;
}


struct Simple_Time time_avg(struct Simple_Time times[],int size, int coord, int threshold_minutes){
    int sum=0,tmp=0,diff=0;
    int final_size=0;
    struct Simple_Time avg_time;
    float avg;
    
    for(int i =0;i<size;i++) 
    {   
        tmp = times[i].hour*60 + times[i].minute;
        diff = abs((times[coord].hour*60 + times[coord].minute) - tmp);
        
        if(diff <= threshold_minutes)
        {
            sum = sum+ tmp;
            final_size++;
        }
        
    }
    avg = sum/final_size;
    printf("Time differential average is %f \n", avg); 
    avg_time.minute = (int)avg%60;
    avg_time.hour = (int)avg/60;
    return avg_time;
}



