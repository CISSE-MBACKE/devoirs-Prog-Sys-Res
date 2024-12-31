#include "common.h"


int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		printf("Erreur: argument insuffisant\n");
		printf("Usage: %s PORT\n", argv[0]);
		return 0;
	}
	
	//création du socket serveur
	int socketServeur = socket(AF_INET, SOCK_STREAM, 0);
	if(socketServeur == -1)
	{
		perror("erreur de creation de la socket");
		return 0;
	}
	
	printf("[#Serveur #] Crearion de la socket .......... OK\n\n");
	
	//initialisation de la structure
	struct sockaddr_in addrServeur;
	addrServeur.sin_family = AF_INET;
	addrServeur.sin_port = htons(atoi(argv[1]));
	addrServeur.sin_addr.s_addr = inet_addr("0.0.0.0");
	memset(addrServeur.sin_zero, 0, sizeof(addrServeur.sin_zero));
	
	printf("[#Serveur #] initialisation de la structure ............. OK\n\n");
	printf("Contenu de la structure :\n");
	printf("[#Client #] initialisation de la structure ............. OK\n\n");
	printf("Contenu de la structure :\n");
	printf("----------------\n");
	printf("ADRESSE IP: %s\n", inet_ntoa(addrServeur.sin_addr));
	printf("PORT: %d\n", ntohs(addrServeur.sin_port));
	printf("----------------\n\n");
	
	//Nommage de la structure
	int resultat = bind(socketServeur, (struct sockaddr*)&addrServeur, sizeof(struct sockaddr_in));
	if(resultat == -1)
	{
		perror("erreur lors du nommage de la structure");
		return 0;
	}
	
	printf("[#Serveur #] Nommage de la structure ............. OK\n\n");
	
	//appel de listen
	resultat = listen(socketServeur, 1);
	if(resultat == -1)
	{
		perror("Erreur à l'appel de listen");
		return 0;
	}
	
	//preparation de l'appel de accept
	int socketClient;
	struct sockaddr_in addrClient;
	memset(&addrClient, 0, sizeof(addrClient));
	int taille = sizeof(struct sockaddr_in);
	
	//appel de la accept
	socketClient = accept(socketServeur, (struct sockaddr*)&addrClient, &taille);
	if(socketClient == 1)
	{
		perror("Erreur lors de l'appel de accept");
		return 0;
	}
	
	//initialisation du compteur à 0
	compteur = 0;
	printf("[#Serveur #]: compteur = %d\n\n", compteur);
	
	//envoi de la valeur du compteur
	if( send(socketClient, &compteur, sizeof(int), 0) == -1)
	{
		perror("erreur d'envoi de la donnee");
		close(socketClient);
		return 0;
	}
	
	while(1)
	{
		//reception de la valeur du compteur
		if( recv(socketClient, &compteur, sizeof(int), 0) == -1)
		{
			perror("erreur de reception de la donnee");
			close(socketClient);
			return 0;
		}
		
		printf("[#Serveur #]: compteur = %d\n\n", compteur);
		
		//attente pendant 1 seconde
		sleep(1);
		
		compteur++;
		//renvoi de la valeur du compteur
		if( send(socketClient, &compteur, sizeof(int), 0) == -1)
		{
			perror("erreur d'envoi de la donnee");
			close(socketClient);
			return 0;
		}
	}
	
	close(socketClient);
	
	return 0;
}

