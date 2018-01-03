#include "/usr/include/uarm/libuarm.h"
#include "itoa.h"

void u_reverse(char str[], int in, int fin){


	if (in < fin){
		str[in] ^= str[fin] ; //xor swap algorithm
		str[fin] ^= str[in] ;
		str[in] ^= str[fin] ;
		u_reverse(str, ++in, --fin);
	}
}

int u_cifre(int num){


	if (num != 0){
		int cifre = u_cifre(num/10) + 1 ;
		return cifre ;
	}
	else 
		return 0 ;


}

int power(int num, int exp){

	if (exp != 0){
		int potenza = power(num,exp-1) * num ;
		return potenza ;
	}
	else 
		return 1 ;
}

int u_atoi(char str[], int length, int i){

	if (str[i] != 0){
		int num_stringa = u_atoi(str,length-1,i+1) + ((int)str[i] - 48) * power(10,length - 1); //length-1 è l'esponente posizionale della cifra i-esima
		return num_stringa ;
	}
	else 
		return 0 ;

}

void u_itoa(char str[], int num, int i){ 
	

	if (num != 0){
		int cifra = num % 10 ;
		str[i] = (char)cifra +'0' ; //tabella ascii assegna cifre decimali da 48 a 57 compresi
		u_itoa(str,num/10,++i);
	}

}

//per provare funzioni
int main(int argc, char* argv[]){

	char stringa[5] = {'m','i','n','k'};
	int n = 67857 ;
	int m = u_cifre(n);
	char stringa_di_cifre[m+1] ;
	u_reverse(stringa,0,3);
	u_itoa(stringa_di_cifre,n,0);
	tprint(stringa);
	tprint("\n");
	tprint(stringa_di_cifre);
	return 0 ;
}