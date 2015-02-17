/* udpserver.c */ 

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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

int main(int argc, char *argv[])
{
    int sock, length, randNumber ;
    int recvdFromClient;
    int addr_len, bytes_read;
    char recv_data[1024];
    struct sockaddr_in server_addr , client_addr;
    int PORT = atoi( argv[1] );
    char randString[1];

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if ( sock == -1 ) {
        perror("Socket");
        exit(1);
    }

    length = sizeof( server_addr );
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_addr.sin_zero),length);


    if (bind(sock,(struct sockaddr *)&server_addr,
        sizeof(struct sockaddr)) == -1){
        perror("Bind");
        exit(1);
    }

    addr_len = sizeof(struct sockaddr);

    if( argc == 3 ){
        randNumber = atoi( argv[2] )%10;
    }
    else{
        randNumber = getRand();
    }
	sprintf( randString, "%d", randNumber);

    printf("\n UDP Server Waiting for client on port %d\n", PORT);
    fflush(stdout);

    char m[] = "0";

	while (1)
	{

        bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr, &addr_len);
        printf("%s\n", recv_data );
        
        recvdFromClient = atoi( recv_data);
        printf("%d\n", recvdFromClient );
        printf("%d\n", randNumber );
        if( randNumber == recvdFromClient ){
            printf( "Successfully Guessed ");
            //m = "0";
            sendto( sock, "0" , sizeof(m), 0, (struct sockaddr *)&client_addr, addr_len );
            break;
        }
        else{
            // god knows what
            //m = "1";
            printf("Not matched\n");
            sendto( sock, "1" , sizeof(m), 0, (struct sockaddr *)&client_addr, addr_len );
        }
	    
        // if( bytes_read > 0){
        //     sendto( sock, m , sizeof(m), 0, (struct sockaddr *)&client_addr, addr_len );
        // }
        
          
	    fflush(stdout);

    }
    return 0;
}