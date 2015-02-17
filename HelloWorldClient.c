#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
//#include <time.h>

int getRand(){
    srand( time(NULL) );
    int r = rand();
    return r%10;
}

int main( int argc, char *argv[] )
{
int sock, length;
struct sockaddr_in server_addr;
struct hostent *host;
char send_data[1024];
char recv_data[1024];

int PORT = atoi( argv[2] );

int guessedValue;

length = sizeof(struct sockaddr);

host = (struct hostent *) gethostbyname((char *)argv[1]);
//host= (struct hostent *) gethostbyname((char *)"54.149.166.205");

if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
{
perror("socket");
exit(1);
}

server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(PORT);
server_addr.sin_addr = *((struct in_addr *)host->h_addr);
bzero(&(server_addr.sin_zero),8);

   while (1)
   {

    guessedValue = getRand();
    sprintf( send_data, "%d", guessedValue);
    //gets(send_data);
    printf("Send Data:- %s\n", send_data );
    
    sendto(sock, send_data, strlen(send_data), 0,
          (struct sockaddr *)&server_addr, sizeof(struct sockaddr));

    recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&server_addr, &length);
    //printf("%s\n", recv_data);

    if( atoi(recv_data) == 0 ){  // 0 means value is guessed correctly
      printf( "You Have Guessed Successfully");
      break;
    }
    else{
      printf( "Not matched. Keep trying");
      continue;
    }
    printf("%s\n", recv_data);
      
        
   }

}