#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM 6

pthread_mutex_t mutex[NUM];
pthread_cond_t cond[NUM]; //condition of thread
pthread_t tid[NUM]; //id of thread

int permit[NUM]; //availability of fork


void get_fork(int cur_tid) {
	usleep(10000);
	pthread_mutex_lock(&mutex[cur_tid%NUM]);

	//critical section
	while(permit[cur_tid%NUM] == 0) {
		pthread_cond_wait(&cond[cur_tid%NUM], &mutex[cur_tid%NUM]);
	}
	permit[cur_tid%NUM] = 0;

	pthread_mutex_unlock(&mutex[cur_tid%NUM]);
}

void return_fork(int cur_tid) {
	usleep(10000);
	pthread_mutex_lock(&mutex[cur_tid%NUM]);
	
	//critical section
	permit[cur_tid%NUM] = 1;
	pthread_cond_signal(&cond[cur_tid%NUM]);

	pthread_mutex_unlock(&mutex[cur_tid%NUM]);
}

void* odd_philosopher(void* n) {
	int* id = (int*)n;
	
	while(1){
		//right--->left
		get_fork(*id+1);
		printf("%d, get the right fork(%d)\n", *id, (*id+1)%NUM);

		get_fork(*id);
		printf("%d, get the left fork(%d)\n", *id, *id);
		usleep(100000); //eating
		printf("%d, finished eating\n", *id);
		

		//left--->right
		return_fork(*id);
		printf("%d, put down the left fork(%d)\n", *id, *id);

		return_fork(*id+1);
		printf("%d, put down the right fork(%d)\n", *id, (*id+1)%NUM);

	}
}

void* even_philosopher(void* n) {
	int* id = (int*)n;
	while(1) {
		//left--->right
		get_fork(*id);
		printf("%d, get the left fork(%d)\n", *id, *id);
		
		get_fork(*id+1);
		printf("%d, get the right fork(%d)\n", *id, (*id+1)%NUM);
		
		usleep(100000);
		printf("%d, finished eating\n", *id);
		
		//right--->left
		return_fork(*id+1);
		printf("%d, put down the right fork(%d)\n", *id, (*id+1)%NUM);
		
		return_fork(*id);
		printf("%d, put down the left fork(%d)\n", *id, *id);
	}
}

int main() {
	int n[NUM] = {0, 1, 2, 3, 4, 5};
	for(int i=0; i<NUM; i++){
		pthread_mutex_init(&mutex[i], NULL);
		pthread_cond_init(&cond[i], NULL);
		permit[i] = 1;
	}

	for(int i=0; i<NUM; i++) {
		if(i%2) //odd case
			pthread_create(&tid[i], NULL, odd_philosopher, (void *)&n[i]);
		else //even case
			pthread_create(&tid[i], NULL, even_philosopher, (void *)&n[i]);
	}

	for(int i=0; i<NUM; i++){
		pthread_join(tid[i], NULL);
	}
	
	for(int i=0; i<NUM; i++){
		pthread_mutex_destroy(&mutex[i]);
		pthread_cond_destroy(&cond[i]);
	}

	return 0;
}
