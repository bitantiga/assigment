#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#define NUM_THREADS 3
#define BUFFER_SIZE 20


struct STU
{			
	int rear;			
	int front;				
	int buffer[20];			
};

struct STU mm;
sem_t *empty;
void *mm;



void *consumer(void *a)
{
    	pthread_t tid = pthread_self();
    	while(1){
            int random_time = rand() %100 +1;
	        usleep(random_time);
            struct buf *shm_ptr = ((struct buf *)mm);
            sem_wait(empty);//Wait for a full buffer
            int item = shm_ptr->buffer[shm_ptr->front];
            shm_ptr->front = (shm_ptr->front+1) % BUFFER_SIZE;
            sem_post(empty);//Add a empty buffer
            printf("Consuming the data %d by pid %lu\n",item,(unsigned long)tid);
    	}
    pthread_exit(0);


}

int main(int argc, char** argv)
{
    empty = sem_open("empty",O_CREAT);
    int shm_fd = shm_open("buffer",O_RDWR,0666);
    mm = mmap(0,sizeof(struct buf),PROT_READ | PROT_WRITE,MAP_SHARED,shm_fd,0); 
    
    pthread_t tid1,tid2,tid3;/*The thread identifier*/
    pthread_create(&tid1,NULL,consumer,NULL);
    pthread_create(&tid2,NULL,consumer,NULL);
    pthread_create(&tid3,NULL,consumer,NULL);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);

    munmap(ptr, sizeof(struct buf));
    shm_unlink("buffer");
    sem_close(full);
    sem_close(empty);

    return 0;
}
