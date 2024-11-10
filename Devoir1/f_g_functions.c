#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

extern char* optarg;
extern int optopt;

int f(int n)
{
	return pow(2, n);
}

int g(int n)
{
	return 2 * n;
}

int main(int argc, char *argv[])
{
	if(argc == 3 || argc == 4)
	{
		int opt;
		int i = 0;//pour savoir si c'est fog (i.e i = 1) ou gof (i.e i = 2)
		int argument_f = 0, argument_g = 0; //récupère l'argument de l'option f ou g
		int f_exist = 0, g_exist = 0; //vérifie que les options f ou g sont sont fournies ou pas(0 --> non fournie et 1 -- > fournie
		while( (opt = getopt(argc, argv, ":f:g:")) != -1)
		{
			switch(opt)
			{
				case 'f': f_exist = 1;
					  if(optarg)
						argument_f = atoi(optarg);
					  i = 1;
				          break;
				case 'g': g_exist = 1;
					  if(optarg)
						argument_g = atoi(optarg);
					  i = 2;
					  break;
				case ':':if(f_exist && optopt != 'f')//pour ne pas qu'il donne deux fois l'option f ou autre chose inconnue
						g_exist = 1;
					    if(g_exist && optopt != 'g')//pour ne pas qu'il donne deux fois l'option g ou autre chose inconnue
					 	f_exist = 1;
					 break;
				case '?':printf("ERREUR: option -%c inconnue\n",optopt);exit(-1);
			}
		}
		if(argc == 3)
		{
			if(f_exist)// f(n)
				printf(" f(n) = 2 ^ n ==> f(%d) = %d\n",argument_f, f(argument_f));
			else// g(n)
				printf(" g(n) = 2 * n  ==> g(%d) = %d\n",argument_g, g(argument_g));
		}
		else
		{
			if(f_exist && g_exist)
			{
				if(i == 1)// cas ou l'utilisateur veut calculer fog
					printf(" f(n) = 2 ^ n et g(n) = 2 * n ==> fog(%d) = %d\n",argument_f, f(g(argument_f)));
				else// cas ou l'utilisateur veut calculer gof
					printf(" f(n) = 2 ^ n et g(n) = 2 * n ==> gof(%d) = %d\n",argument_g, g(f(argument_g)));
			}
			else
				printf("Erreur : option ou argument inconnu(e)!!\n");
		}
	}
	else
		printf("ERREUR: nombre d'argument inssuffisant\nUsage : %s [-f entier] => f(n)\n        %s [-g entier] => g(n)\n        %s [-f entier -g] => fog(n)\n        %s [-g entier -f] => gof(n)\n",argv[0], argv[0], argv[0], argv[0]);
	return 0;
}