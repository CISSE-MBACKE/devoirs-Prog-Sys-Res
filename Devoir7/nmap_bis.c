#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		printf("Erreur: argument insuffisant!!\n");
		printf("Usage: %s ADRESSE_IP\n", argv[0]);
		return 0;
	}
	
	int port = 0;
	int resultat;
	
	while(port <= 3000)
	{
		int socketClient;
		socketClient = socket(AF_INET, SOCK_STREAM, 0);
		if(socketClient == -1)
		{
			perror("Erreur de creation de la socket");
			return 0;
		}
		
		struct sockaddr_in addrClient;
		
		addrClient.sin_family = AF_INET;
		
		addrClient.sin_addr.s_addr = inet_addr(argv[1]);
		
		memset(addrClient.sin_zero, 0, sizeof(addrClient.sin_zero));
		
		addrClient.sin_port = htons(port);
		
		resultat = connect(socketClient, (struct sockaddr*)&addrClient, sizeof(struct sockaddr_in));
		
		if(resultat != -1)
		{
			printf("### PORT %d ouvert\n\n", port);
		}
		
		close(socketClient);
		port++;
	}
	return 0;
}

