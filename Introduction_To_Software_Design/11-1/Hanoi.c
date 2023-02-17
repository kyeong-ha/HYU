#include<stdio.h>
#include<stdlib.h>

int hanoi(int n, int a, int b, int c){
	if(n==1){
		printf("%d %d\n", a, c);}
	else {
		hanoi(n-1, a, c, b);
		printf("%d %d\n", a, c);
		hanoi(n-1, b, a, c);
	}
}

int main(void){
	int n, i;
	int power=1;
	scanf("%d", &n);
	for (i=0; i<n; i++){
		power*=2;
	}
	printf("%d\n", power-1);
	hanoi(n, 1, 2, 3);
}
