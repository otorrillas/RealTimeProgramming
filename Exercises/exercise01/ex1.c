//gcc -std=gnu99 -Wall -g -o ex1 ex1.c -lpthread



#include <pthread.h>
#include <stdio.h>



int i = 0;

void* Thread_func1(){
	
	for(int j = 0; j<1000000; j++){
		
		i+=1;
	}
	
	 printf("Value of i - Thread1: %d \n", i);
	
	return NULL;
}

void* Thread_func2(){
	
	for(int j = 0; j<1000000; j++){
		
		i-=1;
	}
	
	 printf("Value of i - Thread2: %d \n", i);

	return NULL;
}



int main(){
	
	pthread_t Thread1;
    pthread_create(&Thread1, NULL, Thread_func1, NULL);
    
    pthread_join(Thread1, NULL);
    
    pthread_t Thread2;
    pthread_create(&Thread2, NULL, Thread_func2, NULL);
    
    pthread_join(Thread2, NULL);
    
	
	return 0;
}
