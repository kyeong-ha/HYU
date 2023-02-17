#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<windows.h>

int main(void){
	char id[10]="abc123";
	char pw[10]="1234";
	char idInput[10];
	char pwInput[10];
	int count=0;
	int choice;
	int account=0;
	int deposit;
	int withdraw;
	int lotto=0;

	while(1){
		printf("Login\n");
		printf("ID: ");
		scanf("%s", idInput);
		printf("PassWord: ");
		scanf("%s", pwInput);

		if(strcmp(id, idInput)||strcmp(pw,pwInput)){
			printf("ERROR :ID ! Login\n");
			sleep(2000);
			count=count+1;
			if(count==3){
				exit(1);}}

		else if(strcmp(id,idInput)==0 && strcmp(pw,pwInput)==0){
			break;}
		}
	

	while(1){
		printf("Menu\n");
		printf("1. Check Account 2. Deposit 3. withdraw 4. Lotto 5. Logout\n");
		scanf("%d", &choice);
		
		if(choice==1){
			printf("%d\n", account);}

		if(choice==2){
			printf("Input your deposit money\n");
			scanf("%d", &deposit);
			account=account+deposit;}

		if(choice==3){
			printf("Input your withdraw money\n");
			printf("Your Account : %d\n", account);
			scanf("%d", &withdraw);
			if(account>=withdraw){
				account=account-withdraw;}
			else{
				printf("Low balance\n");}}

		if(choice==4){
			if(account>=0){
				srand(time(NULL));
				lotto=(rand()%1001)-500;
				account=account-lotto;}
			else{
				printf("Low balance\n");}}

		if(choice==5){
			printf("Bye~!\n");
			exit(0);}
	}
}



