#include<stdio.h>
#include<stdlib.h>

int num;

int user(int a){
	if(a==1)
		printf("Beef\n");

	else if(a==2)
		printf("Chicken\n");

	else if(a==3)
		printf("Wine\n");

	else if(a==4)
		printf("Lamb\n");

	else if(a==5)
		exit(0);
	
	else
		printf("Invalid Input\n");
}

int main(void){
	while(1){
		scanf("%d", &num);
		user(num);
	}
}
