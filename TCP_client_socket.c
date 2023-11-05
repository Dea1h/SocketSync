#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
int main(int argc, char **argv)
{
	struct sockaddr_in server_info = {0};
	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = htonl(0x7f000001);
	server_info.sin_port = htons(2300);

	socklen_t server_info_len = sizeof(server_info);

	int sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd < 0)
	{
		perror("Socket");
		return -1;
	}
	perror("Socket");
	
	if(0 > connect(sfd,(struct sockaddr*)&server_info,server_info_len))
	{
		perror("Connect");
		return -1;
	}
	perror("Connect");

	char buffer[1024];
	while (1) {

	ssize_t recvd = recv(sfd, buffer, sizeof(buffer) - 1, 0);
	if( recvd == -1)
	{
		perror("Recv");
		return -1;
	}
	else {
		buffer[recvd] = '\0';
		if(strncmp(buffer,"exit",4) == 0){
				close(sfd);
				break;
			}
			printf("Server:%s",buffer);
	}
		printf("Client:");
		fgets(buffer,sizeof(buffer),stdin);
		send(sfd,buffer,sizeof(buffer),0);

}
	return 0;

}

