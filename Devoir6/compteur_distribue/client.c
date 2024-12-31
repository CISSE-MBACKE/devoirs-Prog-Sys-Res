#include "common.h"


int main(int argc, char* argv[])
{
	if(argc < 3)
	{
		printf("Erreur: argument insuffisant\n");
		printf("Usage: %s ADRESSE_IP PORT\n", argv[0]);
		return 0;
	}
	
	//création du socket serveur
	int socketClient = socket(AF_INET, SOCK_STREAM, 0);
	if(socketClient == -1)
	{
		perror("erreur de creation de la socket");
		return 0;
	}
	
	printf("[#Client #] Creaion de la socket .......... OK\n\n");
	
	//initialisation de la structure
	struct sockaddr_in addrClient;
	addrClient.sin_family = AF_INET;
	addrClient.sin_port = htons(atoi(argv[2]));
	addrClient.sin_addr.s_addr = inet_addr(argv[1]);
	memset(addrClient.sin_zero, 0, sizeof(addrClient.sin_zero));
	
	printf("[#Client #] initialisation de la structure ............. OK\n\n");
	printf("Contenu de la structure :\n");
	printf("----------------\n");
	printf("ADRESSE IP: %s\n", inet_ntoa(addrClient.sin_addr));
	printf("PORT: %d\n", ntohs(addrClient.sin_port));
	printf("----------------\n\n");
	
	//preparation de l'appel de connect
	int taille = sizeof(struct sockaddr_in);
	
	//appel de la accept
	int resultat = connect(socketClient, (struct sockaddr*)&addrClient, taille);
	if(resultat == 1)
	{
		perror("Erreur lors de l'appel de accept");
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
		
		printf("[#Client #]: compteur = %d\n\n", compteur);
		
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

