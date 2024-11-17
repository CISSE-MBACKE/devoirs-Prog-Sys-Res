#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/stat.h>

#define MAX_BUFFER_SIZE 256

void join(int argcbis, char *argvbis[]);//déclaration de la fonction join

int getNbChiffre(int n);//fonction supplémentaire juste pour savoir le nombre de chiffre d'un nombre donné en paramètre

int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		printf("ERREUR: nombre d'argument insuffisant!!\n");
		printf("Usage: %s [ taille_en_octet | part_1 part_2 part_3 ... part_n ]\n",argv[0]);
		exit(errno);
	}
	char *fin;
	strtol(argv[2], &fin, 10);
	if(*fin == '\0')//vérification si l'argument est un nombre ou pas
	{
		int taille = atoi(argv[2]);
		int fd = open(argv[1], O_RDONLY);
		struct stat info_file;
		if(fd == -1)
		{
			perror("Erreur lors de l'ouverture du fichier");
			exit(errno);
		}

		char *buff = NULL;
		buff = malloc(taille * sizeof(char) + 1);
		if(!buff)
		{
			perror("erreur allocation");
			exit(errno);
		}
		int index = 1, N;
		int n, fdFils;
		int nbRead = 0;
		int nbChiffre = 0, v = 0;
		printf("découpage du fichier %s en fichiers de taille %d maximum...\n\n",argv[1], taille);
		while( (nbRead = read(fd, buff, taille)) > 0)
		{
			//Création du nom du nouveau fichier => "nomFichierOrigine + _index + .txt"
			//(exemple d'un premier partition d'un fichier file1.txt : son nom sera file1_1.txt)
		    nbChiffre =	getNbChiffre(index);
			N = strlen(argv[1]) + nbChiffre + 2; //On gère d'abord sa taille avec exactitude
			char nomNouvFichier[N];
			v = strlen(argv[1]) - 4;// pour ne pas copier le ".txt"
			strncpy(nomNouvFichier, argv[1], v);
			n = sprintf(nomNouvFichier + v,"_%d.txt", index);

			fdFils = open(nomNouvFichier, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
			if(fdFils == -1)
			{
				perror("Erreur lors de la creation de l'un des fichiers");
				exit(errno);
			}
			if( write(fdFils, buff, nbRead) == -1)
			{
				perror("erreur d'ecriture dans l'un des fichiers");
				exit(errno);
			}
			memset(buff,0,taille);
			
			if( fstat(fdFils, &info_file) == -1)
			{
				perror("erreur de recuperation des informations du fichier");
				exit(errno);
			}
			printf("#%d => %s\n",index, nomNouvFichier);
			printf("Taille => %.4ld octets\n",info_file.st_size);
			printf("-----------------------------\n");
			index++;
			close(fdFils);
		}
		close(fd);
	}
	else
	{
		join(argc, argv);
	}
	return 0;
}

void join(int argcbis, char *argvbis[])
{
	int fd = open(argvbis[1], O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
	if(fd == -1 )
	{
		perror("erreur d'ouverture du fichier");
		exit(errno);
	}
	int fdFils;
	for(int i = 2; i < argcbis; i++)
	{
		fdFils = open(argvbis[i], O_RDONLY);
		if(fdFils == -1)
		{
			perror("erreur d'ouverture de l'un des fichiers à joindre");
			exit(errno);
		}
		int nbRead = 0;
		char *buff = malloc(MAX_BUFFER_SIZE * sizeof(char));
		if(!buff) exit(-1);
		while( (nbRead = read(fdFils, buff, MAX_BUFFER_SIZE)) > 0)
		{
			if(write(fd, buff, nbRead) != nbRead)
			{
				perror("erreur de d'écriture");
				exit(errno);
			}
		}
		close(fdFils);
	}
	close(fd);
}

int getNbChiffre(int n)
{
	int nb = 0;
	while(n > 0)
	{
		n = n / 10;
		nb++;
	}
	return nb;
}