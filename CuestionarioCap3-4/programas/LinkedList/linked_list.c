#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../timer.h"

#define RAND_RANGE 3000
#define RAND_RANGE_FUN 1000

#define INI_SIZE 200000
#define NUM_OP 100000
#define MEMBER_OP 99.9
#define INSERT_OP 0.05
#define DELETE_OP 0.05

#define MEMBER 0
#define INSERT 1
#define DELETE 2

pthread_mutex_t mutexAllList = PTHREAD_MUTEX_INITIALIZER;

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

pthread_mutex_t mutexRoot = PTHREAD_MUTEX_INITIALIZER;

typedef struct{
	int value;
	struct ListNode * next;
}ListNode;

typedef struct{
	int iterations[3];
	ListNode ** root;
}MethodArgs;

void  printList(ListNode ** root){
	ListNode * iter = *root;
	while(iter != NULL){
		printf("%d ",iter->value);
		iter = iter->next;
	}
	printf("\n");
}

int member(int value, ListNode ** root){
	ListNode * iter = NULL;
	iter = *root;
	while(iter != NULL && iter->value < value){
		iter = iter->next;
	}
	if(iter == NULL || iter->value > value){
		return 0;
	}
	else{
		return 1;
	}
}

int insert(int value, ListNode ** root){
	ListNode * iter = NULL;
	ListNode * pred = NULL;
	ListNode * temp = NULL;
	iter = *root;
	while(iter != NULL && iter->value < value){
		pred = iter;
		iter = iter->next;
	}
	if(iter == NULL || iter->value > value){
		temp = (ListNode *) malloc(sizeof(ListNode));
		temp->next = iter;
		temp->value = value;

		if(pred == NULL){
			*root = temp;
		}
		else{
			pred->next = temp;
		}
		return 1;
	}
	else{
		return 0;
	}
}


int delete(int value, ListNode ** root){
	ListNode * iter = NULL;
	ListNode * pred = NULL;
	iter = *root;
	while(iter != NULL && iter->value < value){
		pred = iter;
		iter = iter->next;
	}
	int * res = malloc(sizeof(int));
	if(iter != NULL && iter->value == value){
		if(pred == NULL){
			*root = iter->next;
		}
		else{
			pred->next = iter->next;
		}
		free(iter);
		return 1;
	}
	else{
		return 0;
	}
}

int getFunction(int iter[3]){
	//srand(time(NULL));
	int member = iter[MEMBER] != 0;
	int insert = iter[INSERT] != 0;
	int delete = iter[DELETE] != 0;
	if(member == 0 && insert == 0 && delete == 0) return -1;
	int i = rand() % RAND_RANGE_FUN;
	int res = i % 3;
	for(int j = 0; j < 2; j++){
		if((res == MEMBER && member == 0) || (res == INSERT && insert == 0) ||
				 (res == DELETE && delete == 0)){
			i++;
			res = i % 3;
		}
		else{
			//(*iter[res]) = (*iter[res]) - 1;
			return res;
		}
	}
}

void * thread_fun(void * args){
	srand(time(NULL));
	MethodArgs * m_args  = (MethodArgs *) args;
	int op = getFunction(m_args->iterations);
	m_args->iterations[op] = m_args->iterations[op] - 1;
	while(op != -1){
		int num = rand() % RAND_RANGE;
		//printf("A->%d\n",m_args->iterations[MEMBER]);
		//pthread_mutex_lock(&mutexAllList);
		switch(op){
			case MEMBER: 
				pthread_rwlock_rdlock(&rwlock);
				member(num,m_args->root); 
				pthread_rwlock_unlock(&rwlock);
				break;
			case INSERT:
				pthread_rwlock_wrlock(&rwlock);
				insert(num,m_args->root);
				pthread_rwlock_unlock(&rwlock);
				break;
			case DELETE: 
				pthread_rwlock_wrlock(&rwlock);
				delete(num,m_args->root);
				pthread_rwlock_unlock(&rwlock);
				break;
			default: break;
		}
		//pthread_mutex_unlock(&mutexAllList);
		op = getFunction(m_args->iterations);
		if(op != -1) m_args->iterations[op] = m_args->iterations[op] - 1;
	}
}

int main(int argv, char ** argc){
	if(argv != 2){
		printf("Faltan Argumentos <numThreads>\n");
		return 0;
	}
	//clock_t ini = clock();
	double ini, end;
	int numThreads = atoi(argc[1]);
	srand(time(NULL));
	ListNode ** root = (ListNode **) malloc(sizeof(void*));
	for(int i = 0; i < INI_SIZE; i++){
		int value = rand() % RAND_RANGE;
		insert(value, root);
	}
	GET_TIME(ini);
	//printList(root);
	//int member_op = NUM_OP * MEMBER_OP / 100;
	//int insert_op = NUM_OP * INSERT_OP / 100;
	//int delete_op = NUM_OP * DELETE_OP / 100;
	int member_op = 0;
	int insert_op = 0;
	int delete_op = NUM_OP;
	int iteraciones[3];
	iteraciones[MEMBER] = member_op / numThreads;
	iteraciones[INSERT] = insert_op / numThreads;
	iteraciones[DELETE] = delete_op / numThreads;
	pthread_t threads[numThreads];
	MethodArgs threads_args[numThreads];
	for(int i = 0; i < numThreads; i++){
		//memcpy(&(threads_args[i].iterations),&iteraciones,sizeof(int) * 3);
		for(int j = 0; j < 3; j++) threads_args[i].iterations[j] = iteraciones[j];
		//threads_args[i].iterations = iteraciones;
		threads_args[i].root = root;
		pthread_create(&threads[i],NULL,thread_fun,(void *) &threads_args[i]);
	}
	for(int i = 0; i < numThreads; i++){
		pthread_join(threads[i],NULL);
	}
	//clock_t end = clock();
	GET_TIME(end);
	double seconds = (double) (end - ini);
	//printList(root);
	printf("Time->%lf\n",seconds);	
	return 0;
}