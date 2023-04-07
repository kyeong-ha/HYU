#include <stdio.h>
#include <pthread.h>
#include <atomic>

#define COUNTING_NUMBER 2000000

using namespace std;

int critical_section_variable = 0;

atomic_int turn(0);
atomic_int flag0(0), flag1(0);

void lock(int self) {
	if (self == 0) {
		flag0.store(1, memory_order_seq_cst);
		turn.store(1-self, memory_order_seq_cst);
		while (flag1 == 1 && turn == 1-self);
	}
	else if (self == 1) {
		flag1.store(1, memory_order_seq_cst);
		turn.store(1-self, memory_order_seq_cst);
		while (flag0 == 1 && turn == 1-self);
	}
}

void unlock(int self) {
	if (self == 0)
		flag0.store(0, memory_order_seq_cst);
	else if (self == 1)
		flag1.store(0, memory_order_seq_cst);
}

void* func(void* s) {
	int* thread_num = (int*)s;

	for (int i = 0; i < COUNTING_NUMBER; i++) {
		lock(*thread_num);
		critical_section_variable++;
		unlock(*thread_num);
	}
}

int main(void) {

	pthread_t p1, p2;
	int num[2] = { 0 , 1 };
	pthread_create(&p1, NULL, func, (void*)&num[0]);
	pthread_create(&p2, NULL, func, (void*)&num[1]);

	pthread_join(p1, NULL);
	pthread_join(p2, NULL);

	printf("Actual Count : %d Expected Count : %d\n", 	critical_section_variable, COUNTING_NUMBER * 2);

	return 0;
}
