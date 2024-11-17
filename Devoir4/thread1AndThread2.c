#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

int plusN = 0;
int moinsN = 0;
int i = 0;
int j = 0;
sem_t sem1;
sem_t sem2;
sem_t mutex;

void* threadSecondaire(void *args)
{
	while(1)
	{
		sem_wait(&sem1);//sem1 = +n au début;
		sem_wait(&mutex);
		printf("%d %d\n", ++j, ++i);
		if(i == plusN)
		{
			for(int k = 0; k < 2*plusN; k++)
			{
				sem_post(&sem2);
			}
		}
		sem_post(&mutex);
	}
		
}

void* threadTertiaire(void *args)
{
	while(1)
	{
		sem_wait(&sem2); //sem2 = 0 au début;
		sem_wait(&mutex);
		printf("%d %d\n", ++j, --i);
		if(i == moinsN)
		{
			for(int k = 0; k < 2*plusN; k++)
			{
				sem_post(&sem1);
			}
		}
		sem_post(&mutex);
	}
}



int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("Syntaxe incorrecte\n");
		printf("Usage: %s nombre\n", argv[0]);
		exit(-1);
	}
	
	int n = atoi(argv[1]);
	plusN = n; 
	moinsN = (-1) * n;
	int resultat;
	resultat = sem_init(&mutex, 0, 1);
	if(resultat == -1)
	{
		perror("erreur sem_init");
		exit(-1);
	}
	resultat = sem_init(&sem1, 0, plusN);
	resultat = sem_init(&sem2, 0, 0);
	
	pthread_t th2, th3;
	resultat = pthread_create(&th2, NULL, threadSecondaire, NULL);
	if(resultat == -1)
	{
		perror("erreur pthread_create");
		exit(-1);
	}
	resultat = pthread_create(&th3, NULL, threadTertiaire, NULL);
	if(resultat == -1)
	{
		perror("erreur pthread_create");
		exit(-1);
	}
	
	
	pthread_join(th2, NULL);
	pthread_join(th3, NULL);
	
	return EXIT_SUCCESS;
}