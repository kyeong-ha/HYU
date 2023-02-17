#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define COUNTING_NUMBER 200000

sem_t rw_mutex; //id of current writer
sem_t mutex; 

int cur_writer;
int cur_count[5] = {0, 0, 0, 0, 0};

void writer(void* pid) {
	for(int i=0; i<COUNTING_NUMBER; i++) {
		usleep(100000);
		sem_wait(&rw_mutex);
		int* idx = (int*) pid;

		cur_writer = *idx;
		cur_count[*idx]++;

		sem_post(&rw_mutex);
	}
}

void reader() {
	for(int i=0; i<COUNTING_NUMBER; i++) {
		usleep(30000);

		sem_wait(&mutex);

		printf("The most recent writer id:\t[%d], count:\t[%d]\n", cur_writer, cur_count[cur_writer]);

		sem_post(&mutex);
	}
}

int main(void) {
	sem_init(&rw_mutex, 0, 1);
	sem_init(&mutex, 0, 1);

	int pid[5] = {0, 1, 2, 3, 4};
	pthread_t thread_writer[5];
	pthread_t thread_reader[2];
	
	//create
	for(int i=0; i<5; i++)
		pthread_create(&thread_writer[i], NULL, (void *)&writer, (void *)&pid[i]); //thread for producer
	for(int i=0; i<2; i++)
		pthread_create(&thread_reader[i], NULL, (void *)&reader, NULL); //thread for consumer

	//join
	for(int i=0; i<5; i++)
		pthread_join(thread_writer[i], NULL);
	for(int i=0; i<2; i++)
		pthread_join(thread_reader[i], NULL);


	sem_destroy(&rw_mutex);
	sem_destroy(&mutex);

	return 0;
}

