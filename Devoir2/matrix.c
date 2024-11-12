#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>


/********************************Déclaration des fonction*******************************/
int** creation_matrice(unsigned int d);

void libere_matrice(int**, unsigned int);

void affichage_matrice(int**, unsigned int);

void affichage_matrice_binaire(int fd, unsigned int d);
void affichage_matrice_texte(int fd, unsigned int);

void sauvegarde_matrice_binaire(int **mat, int fd, unsigned int d);
void sauvegarde_matrice_texte(int **mat, int fd, unsigned int d);



/********************************Fonction principale : le main********************************/

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		printf("Erreur: nombre d'argument insuffisant\n");
		printf("Usage: ./%s [-c] [-a] [-d dimension] [-b] [-t] [-f nomfichier]\n",argv[0]);
		exit(errno);
	}
	int opt;
	unsigned int dim = 10;
	int c_exist = 0;
	int a_exist = 0;
	int b_exist = 0;
	int t_exist = 0;

	char *nomfichier=NULL;
	

	while( (opt = getopt(argc, argv, ":cad:btf:")) != -1 )
	{
		switch(opt)
		{
			case 'c': c_exist = 1;
				break;
			case 'a': a_exist = 1;
				break;
			case 'd': dim = atoi(optarg);
				break;
			case 'b': b_exist = 1;
				break;
			case 't': t_exist = 1;
				break;
			case 'f':nomfichier = strdup(optarg);
				break;
			case ':': printf("EREUR: l'option %c requiere un argument\n",optopt);break;
			case '?': printf("ERREUR: option %c inconnues!!!\n",optopt);break;
		}
	}

	if(nomfichier)//si le fichier n'est pas spécifié, aucune opération n'est faite
	{
		if(t_exist && b_exist)
		{
			printf("ERREUR: le programme ne peut pas traiter binaire et texte simultanement\n");
		}
		else
		{
        int i,j;
        int fd;
		if(!c_exist)
		{
			fd = open(nomfichier, O_RDONLY);
			if(fd == -1)
			{
				perror("Erreur lors de l'ouverture du fichier:");
				exit(errno);
			}
        }
        else
        {
			printf("\nCreation de la matrice aleatoire matrix...\n");
			int **mat = creation_matrice(dim);
			printf("\nAffichage de la matrice matrix...\n\n");
			affichage_matrice(mat, dim);

            fd = open(nomfichier, O_RDWR|O_TRUNC|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
			if(fd == -1)
			{
				perror("Erreur d'ouverture du fichier");
				exit(errno);
			}
			if(b_exist)
			{
				sauvegarde_matrice_binaire(mat, fd, dim);
			}
			else
			{
				sauvegarde_matrice_texte(mat, fd, dim);
			}
        }
		if(a_exist)
		{
			lseek(fd, 0, SEEK_SET);//pour remettre le curseur au debut du fichier.
			printf("\nAffichage du contenu du fichier %s...\n\n",nomfichier);
			if(t_exist)
			{
				affichage_matrice_texte(fd, dim);//le chargement est suivi d'une affichage des éléments
			}
			else//par défaut traitement au format binaire.
			{
				affichage_matrice_binaire(fd, dim);//le chargement est suivi d'une affichage des éléments
			}
		}
		close(fd);
        }
    }
	else
		printf("ERREUR: aucun fichier spécifier!!\n");

	return 0;
}



/*****************************définition des fonctions************************************/

void affichage_matrice(int** mat, unsigned int d)
{
	if(mat)
	{
		int i,j;
		for(i = 0; i < d; i++)
		{
			for(j = 0; j < d; j++)
			{
				printf("%-10d   ",mat[i][j]);
			}
			printf("\n");
		}
	}
}

int** creation_matrice(unsigned int d)
{
	int **mat = NULL;
	int i,j;
	mat = (int**)malloc(d * sizeof(int*));
	for(i = 0; i < d; i++)
	{
		mat[i] = (int*)malloc(d * sizeof(int));
	}
	srand(time(NULL));
	for(i = 0; i < d; i++)
	{
		for(j = 0; j < d; j++)
		{
			mat[i][j] = rand();
		}
	}
	return mat;
}

void libere_matrice(int** mat, unsigned int d)
{
	if(mat)
	{
		int i, j;
		for(i = 0; i < d; i++)
		{
			if(mat[i]) free(mat[i]);
		}
		free(mat);
	}
}

void affichage_matrice_binaire(int fd, unsigned int d)
{
    int nombre, i, j;
    for(i = 0; i < d; i++)
    {
        for(j = 0; j < d; j++)
        {
            if( read(fd, &nombre, sizeof(int)) > 0 )
                printf("%-10d   ",nombre);
        }
        printf("\n");
    }
}
void affichage_matrice_texte(int fd, unsigned int d)
{
    int i,j;
    char character;
    char *buff = malloc(20 * sizeof(char));int nb = 1;
    for(i = 0; i < d; i++)
    {
        for(j = 0; j < d; j++)
        {

            while(read(fd, &character, sizeof(char)) > 0 )//recuperation caractère par caractére
            {		
                if(isdigit(character))//si c'est un chiffre, on prend
                {
                    sprintf(buff + strlen(buff), "%c",character);
                }
                else// espace ou \n => fin du nombre, on l'affiche donc
                {
                    printf("%-10s   ",buff);
                    memset(buff, 0, 20);//vide la chaine pour stocker le nombre suivant
                    char *buff = malloc(20 * sizeof(char));
                    break;
                }
            }
        }
        printf("\n");
    }
    free(buff);
}

void sauvegarde_matrice_binaire(int **mat, int fd, unsigned int d)
{
    int nombre, i, j;
    for(i = 0; i < d; i++)
    {
        for(j = 0; j < d; j++)
        {
            nombre = mat[i][j];
            if( write(fd, &nombre, sizeof(int)) == -1 )
            {
                perror("erreur ecriture");
                exit(errno);
            }
        }
    }
}
	
void sauvegarde_matrice_texte(int **mat, int fd, unsigned int d)
{
    int i,j;
    for(i = 0; i < d; i++)
    {
        for(j = 0; j < d; j++)
        {
            char buff[20]; int n;
            if(j != d-1)
                n = sprintf(buff, "%d ",mat[i][j]);
            else
                n = sprintf(buff, "%d\n",mat[i][j]);
            if( write(fd, buff, n) == -1 )
            {
                perror("erreur ecriture");
                exit(errno);
            }
        }
    }
}