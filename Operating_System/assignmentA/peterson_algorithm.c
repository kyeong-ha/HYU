#include <iostream>
#include <pthread.h>
#include <atomic>

#define COUNTING_NUMBER 2000000

using namespace std;

int critical_section_variable = 0;

atomic_int turn(0);
atomic_int flag0(0), flag1(0);

void lock(int self) {
	turn = 1-self;
	if(self == 0) {
		flag0.store(1, memory_order_release);
		while(flag1 == 1 && turn == 1-self);
	}
	else if(self == 1) {
		flag1.store(1, memory_order_release);
		while(flag0 == 1 && turn == 1-self);
	}
}

void unlock(int self) {
	if(self == 0)
		flag0.store(false, memory_order_release);
	else if(self == 1)
		flag0.store(false, memory_order_release);
}

void* func(void *s) {
	int* thread_num = (int*)s;

	for(int i=0; i<COUNTING_NUMBER; i++) {
		lock(*thread_num);
		critical_section_variable++;
		unlock(*thread_num);
	}
}

int main(void) {
	
	pthread_t p1, p2;
	int parameter[2] = {0, 1};

	pthread_create(&p1, NULL, func, (void*)&parameter[0]);
	pthread_create(&p2, NULL, func, (void*)&parameter[1]);

	pthread_join(p1, NULL);
	pthread_join(p2, NULL);

	cout << "Actual Count : " << critical_section_variable << " | Expected Count : " << COUNTING_NUMBER*2 << endl;

	return 0;
}
