
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <semaphore.h>

struct STU {
	int rear;
	int front;
    	char buffer[20];
};
void *mm;
sem_t *empty;
pthread_t tid1;
pthread_t tid2;
pthread_t tid3;
void *run(void *a){
	pthread_t tid=pthread_self();
	printf("well\n");
	while (1) {
        	usleep(rand() % 100 + 1);
        	int num = rand() % 10 +1;
        	struct STU *shm_ptr = (struct STU *)mm;
        	sem_wait(empty);
			usleep(rand() % 100 + 1);
        	printf("Producing the data %d to buffer[] by id%lu  \n",num,tid);//,shm_ptr->rear,(unsigned long)tid);
        	//shm_ptr->buffer[shm_ptr->rear] = num;
        	//shm_ptr->rear = (shm_ptr->rear+1) % 20;
        	//sem_post(mutex);//Unlock the binary-mutex
        	sem_post(empty);
	}
	pthread_exit(0);
}

int main(int argc, char *argv[])
{
	struct STU share;
    memset(&share,0,sizeof(struct STU));
	struct STU *shm_fd;
    int shm_fd = open("share",O_RDWR,0666);
    empty = sem_open("theempty",O_CREAT,0777,0);
	printf("ss");
	sem_post(empty);
	mm = mmap(0, sizeof(struct STU), PROT_WRITE, MAP_SHARED, shm_fd, 0);
	shm_fd=(struct STU *)mm;
	pthread_create(&tid1,NULL,run,NULL);
	pthread_create(&tid2,NULL,run,NULL);
	pthread_create(&tid3,NULL,run,NULL);
	pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);

	if (argc < 2) {
		printf("./a.out file_shared\n");
		exit(-1);
	}
    return 0;

}
