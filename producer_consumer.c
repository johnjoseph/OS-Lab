/* producer consumer problem */

#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

#define BUFFER_SIZE 10

sem_t full_count;
sem_t empty_count;
sem_t mutex;

int counter=1 , flag=0 , size=0;
int queue[BUFFER_SIZE];


void *producer()
	{ int i , item;

	while(counter<1000) {
	   sem_getvalue(&empty_count , &item);
	   sem_getvalue(&full_count , &i);
	  sem_wait(&empty_count);

	  sem_wait(&mutex);
	  queue[size] = ++counter;
	  size++;
	  printf("\nsizep: %d  e_c: %d  f_c: %d\n" , size , item , i);
	  for(i=0; i<size ; i++)
	     printf("\tp%d" , queue[i]);
	  printf("\n");
	  sem_post(&mutex);
	  sem_post(&full_count);
	  
   }
    }

void *consumer()
	{ int i , item;
	
	while(counter<1000){
	  sem_getvalue(&empty_count , &item);
	  sem_getvalue(&full_count , &i);
	sem_wait(&full_count);
	sem_wait(&mutex);
	  size--;
	  printf("\nsizec: %d  e_c: %d  f_c: %d\n" , size , item , i);
	
	  for(i=0; i<size ; i++)
	     printf("\tc%d" , queue[i]);
	  printf("\n");
	  sem_post(&mutex);
	  sem_post(&empty_count);
	 
   }
    }	   

int main(int argc, char **argv)
	{ pthread_t thread_a , thread_b;

	  sem_init(&full_count , 0 , 0);
	  sem_init(&empty_count , 0 , BUFFER_SIZE-1);
	  sem_init(&mutex , 0 , 1);

	  int i;
	 
	  pthread_create(&thread_a , NULL , &producer , NULL);
	  pthread_create(&thread_b , NULL , &consumer , NULL);

	  pthread_join(thread_a , NULL);
	  pthread_join(thread_b , NULL);
	  return 0;
	}
