#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
int main(int argc,char **argv)
{
	struct sockaddr_in server_info;
	struct stat fs;

	int fd = open("wordlist.txt", O_RDONLY);
	if(0 > fd)
	{
		return -1;
	}
	perror("File Opening");

	server_info.sin_family = AF_INET;
	server_info.sin_port = htons(2300);
	server_info.sin_addr.s_addr = INADDR_ANY;

	socklen_t server_info_len = sizeof(server_info);

	struct sockaddr client_info = {0};
	socklen_t client_info_len = sizeof(client_info);

	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sfd < 0){
		return -1;
	}
	perror("Socket");
	
	if(bind(sfd , (struct sockaddr*)&server_info, server_info_len) < 0)
	{
		return -1;
	}
	perror("Bind");

	if (0 > listen(sfd, 5))
	{
		perror("Listen");
		return -1;
	}
	printf("Listening On Port 2300\n");

	int st = fstat(fd,&fs);
	if(st < 0)
	{
		perror("Stat");
		return -1;
	}
	
	off_t offset = 0;
	off_t remaining = fs.st_size;

	while (1) {

	int cfd = accept(sfd, &client_info, &client_info_len);
	if(0 > cfd)
	{
		continue;
	}
	perror("Accept");
	char *string = "Connection Estabilished Successfully\n";
	ssize_t sent = send(cfd,string,strlen(string),0);
	if (sent < 0)
	{
		continue;
	}
	perror("Send");
	char buffer[1024];
	while (1) {
			recv(cfd,buffer,sizeof(buffer),0);
			printf("Client:%s",buffer);

			printf("Server:");
			fgets(buffer,sizeof(buffer),stdin);
			send(cfd,buffer,strlen(buffer),0);
			if(strncmp(buffer,"File_Transfer",13) == 0){
				while(remaining > 0){
					sent = sendfile(cfd, fd, &offset, remaining);
					if(sent < 0){
						perror("Send");
						break;
					}
					remaining = remaining - sent; 
				}
			}
			else if (strncmp(buffer,"exit",4) == 0) {
				close(sfd);
				close(fd);
				break;
			}
	
	}
	}
	return 0;

}
