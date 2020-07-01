#include<stdio.h>
void bin(unsigned n) 
{ 
    /* step 1 */
    if (n > 1) 
        bin(n/2); 
  
    /* step 2 */
    //cout << n % 2; 
    printf("%d",n%2);
} 
void main(){
	int *a;
	//*a=5;
	unsigned long b=(unsigned long)a;
	unsigned long c=b & 4294901760LL;  //2^32-2^16
	c=c>>16;
	unsigned long d=b & ~4294901760LL;
	
	printf("*a: %d\n",*a);
	bin(b);
	printf("\n");
	bin(c);
	printf("\n");
	bin(d);
}
