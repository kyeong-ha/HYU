#include<stdio.h>
#include<stdlib.h>

int comparison();

int main(void){
	int i;
	char *ptr1, *ptr2;
	char input_char[100];
	char cmp_char[100];
	scanf("%s %s", input_char, cmp_char);
	
	ptr1=input_char;
	ptr2=cmp_char;

	comparison(ptr1, ptr2);
		
}

int comparison(char *ptr1, char *ptr2){
	int i;
	
	for(i=0; ptr1[i]; i++){
		if(ptr1[i]!=ptr2[i])
		printf("%d ", i);
	}
	printf("\n");
}
