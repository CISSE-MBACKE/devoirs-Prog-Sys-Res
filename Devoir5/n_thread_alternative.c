#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <errno.h>
#include <semaphore.h>

int j = 1;
int n;

void* n_thread_function(void *args)
{
	while(1)
	{
		sem_wait( (sem_t*)args );
		printf("[ %d ] ==> ", j);
		if(j != n)
		{
			j++;
			sem_post( (sem_t*)args + 1 );
		}
		else
		{
			j = 1;
			printf("\n\n");
			sem_post( (sem_t*)args - n + 1);
		}
	}
}


int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("Erreur syntaxe!!\n");
		printf("Usage: %s entier\n",argv[0]);
		exit(errno);
	}

	
	n = atoi(argv[1]);
	
	sem_t tabSem[n];
	
	int resultat;
	int i;
	for(i = 0; i < n; i++)
	{
		if(i == 0)
		{
			sem_init(&tabSem[0], 0, 1);
		}
		else
		{
			sem_init(&tabSem[i], 0, 0);
		}
		if(resultat == -1)
		{
			perror("erreur sem_init");
			exit(errno);
		}
	}
	
	pthread_t tabThread[n];

	for(i = 0; i < n; i++)
	{
		resultat = pthread_create(&tabThread[i], NULL, n_thread_function, (void*)&tabSem[i]);
		if(resultat == -1)
		{
			perror("erreur pthread");
			exit(errno);
		}
	}
	
	return EXIT_SUCCESS;
}
