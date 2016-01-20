//gcc -std=gnu99 -Wall -g -o main main.c -lpthread



#include <pthread.h>
#include <stdio.h>



int i = 0;

void* Thread_func1(){
	int j;
	for(j = 0; j<1000000; j++){
		
		i+=1;
	}
	
	
	return NULL;
}

void* Thread_func2(){
	int j;
	for(j = 0; j<1000000; j++){
		
		i-=1;
	}
	 

	return NULL;
}



int main(){
	
    pthread_t Thread1, Thread2;
    pthread_create(&Thread1, NULL, Thread_func1, NULL);
    pthread_create(&Thread2, NULL, Thread_func2, NULL);
    pthread_join(Thread1, NULL);
    pthread_join(Thread2, NULL);
    printf("Value of i: %d \n", i);
	
	return 0;
}
