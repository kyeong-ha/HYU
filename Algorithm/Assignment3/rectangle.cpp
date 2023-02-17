#include <stdio.h>
#include <string.h>

class Goods {
	public:
	char name[50];
	char maker[50];
	int price, rate;

	Goods(char* n, char* m, int p, int r);
	void updateRate();
	int getSellingPrice();
};

Goods::Goods(char* n, char* m, int p, int r) {
	strcpy(name, n);
	strcpy(maker, m);
	price = p;
	rate = r;
}

int Goods::getSellingPrice() {
	return (price - price*rate);
}	

void Goods::updateRate() {
	rate = rate/100;
}

int main() {
	int n;
	char name[50];
	char maker[50];
	int price = 0;
	int rate = 0;
	
	scanf("%d", &n);
	
	for(int i=0; i<n; i++) {
		scanf("%s %s %d %d", name, maker, &price, &rate);

	Goods g(name, maker, price, rate);
	g.updateRate();

	printf("%d", g.getSellingPrice());
	}
}