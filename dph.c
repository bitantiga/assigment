#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

pthread_mutex_t chos[5];
void *run(void *arg){
	int id = *(int *)arg;
	int left, right;
	switch (id){
		case 0:
			left=0;
			right=1;
			break;
		case 1:
			left=1;
			right=2;
			break;
		case 2:
			left=2;
			right=3;
			break;
		case 3:
			left=3;
			right=4;
			break;
		case 4:
			left=4;
			right=0;
			break;
	}
	printf("%d",id);
	while(1){
		usleep(100);
		pthread_mutex_lock(&chos[left]);
		printf("%d is using left \n",id);
		if(pthread_mutex_trylock(&chos[right])==EBUSY){	
			printf("%d's right is being used \n",id);
		}
		//pthread_mutex_lock(&chos[right]);
		printf("%d is using right \n",id);
		usleep(100);
		pthread_mutex_unlock(&chos[left]);
		printf("%d put down his left \n",id);
		pthread_mutex_unlock(&chos[right]);
		printf("%d put down his right \n",id);
	}

}
int main(){
	pthread_t tid[5];
	for(int i=0; i<5; i++){
		printf("%d",i);
		pthread_mutex_init(&chos[i],NULL);
		//if(pthread_mutex_trylock(&chos[i])==EBUSY){
			//pthread_mutex_unlock(&chos[i]);
		//}
		
	}
	int a=0,s=1,d=2,f=3,g=4;
	pthread_create(&tid[0],NULL,run,&a);
pthread_create(&tid[1],NULL,run,&s);
pthread_create(&tid[2],NULL,run,&d);
pthread_create(&tid[3],NULL,run,&f);
pthread_create(&tid[4],NULL,run,&g);
	for(int i=0; i<5; i++){
		pthread_join(tid[i],NULL);
	}
}
