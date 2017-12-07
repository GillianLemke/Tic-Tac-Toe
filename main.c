// Gillian Lemke
// COSC439
// Project 4 - Tic Tac Toe

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include "practical.h"
#include <sys/mman.h>

#define MAXPENDING 5    /* Maximum outstanding connection requests */

void HandleTCPClient(int clntSocket);

/**
 * Login
 * Who
 * send play request
 * receive play request
 * accept/decline play requests
 * send and receive play moves
 * send logout message to server
 * exit
 */

struct ClientToServerMessage {
  enum {login, who, play, lookup, logout} request_type;     /* same size as an unsigned int */
  unsigned int player_id;                             /* initiating player identifier */
  unsigned int id_to_play;
  unsigned short tcp_port;                            /* listening port*/
};

struct PlayerInfo {
		unsigned int player_id;
		char ip_address[15];
	};

int main(int argc, char *argv[]) {
  char *server = argv[1];
  char *port = argv[2];

  // Tell the system what kind(s) of address info we want
  struct addrinfo addrCriteria;                   // Criteria for address match
  memset(&addrCriteria, 0, sizeof(addrCriteria)); // Zero out structure
  addrCriteria.ai_family = AF_UNSPEC;             // Any address family
  // For the following fields, a zero value means "don't care"
  addrCriteria.ai_socktype = SOCK_DGRAM;          // Only datagram sockets
  addrCriteria.ai_protocol = IPPROTO_UDP;         // Only UDP protocol

  // Get address(es)
  struct addrinfo *servAddr; // List of server addresses
  int rtnVal = getaddrinfo(server, port, &addrCriteria, &servAddr);

  // Create a datagram/UDP socket
  int sock = socket(servAddr->ai_family, servAddr->ai_socktype,
                    servAddr->ai_protocol); // Socket descriptor for client
                    
  //TCP
   int servSock;                    /* Socket descriptor for server */
   int clntSock;                    /* Socket descriptor for client */
   struct sockaddr_in echoServAddr; /* Local address */
   struct sockaddr_in echoClntAddr; /* Client address */
   unsigned short echoServPort;     /* Server port */
   unsigned int clntLen;            /* Length of client address data structure */ 
   
    

  
  
  //END TCP

  bool quit = false;
  bool loggedIn = false;
  int userId;
  bool in_game = false;
  while (!quit) {
    if (loggedIn && !in_game) {
      int command;
      printf("\nUse the numbers preceding the options below to execute.");
      printf("\n1. See who is available to play");
      printf("\n2. Request to play");
      printf("\n3. Wait for request to play");
      printf("\n4. Logout");
      printf("\n5. Quit ");
      scanf("%d", &command);

      switch(command) {
        case 1: // see who is available to play
        {
		  struct sockaddr_storage fromAddr; // Source address of server
          // Set length of from address structure (in-out parameter)
          socklen_t fromAddrLen = sizeof(fromAddr);

          // TODO need tcp port
          struct ClientToServerMessage who_query = {who, userId, 100, 5};

          // send who message
		  ssize_t numBytesFollow = sendto(sock, (struct ClientToServerMessage*)&who_query, sizeof(who_query), 0, servAddr->ai_addr, servAddr->ai_addrlen);

          int *number_of_available_players = malloc(sizeof(int));

		  // get number of users available
          ssize_t numBytesRcvd = recvfrom(sock, number_of_available_players, MAXSTRINGLENGTH, 0,
			                                   (struct sockaddr *) &fromAddr, &fromAddrLen);

		  // get all of the users
		  if (*number_of_available_players <= 1) {
			  printf("There are no availalbe players");
		  } else {
			  printf("Ids of Players Available to Play:\n");
		      for(int i = 0; i < *number_of_available_players; i++) {
			      int *other_player_id = malloc(sizeof(int));
			      ssize_t numBytesRcvd = recvfrom(sock, other_player_id, MAXSTRINGLENGTH, 0,
			                                     (struct sockaddr *) &fromAddr, &fromAddrLen);
			                                     
			      if (*other_player_id != userId) {
			         printf("%i ", *other_player_id);
		          }
		      }
	      }
          break;
	    }
        case 2: // request to play
        {
		  struct sockaddr_storage fromAddr; // Source address of server
          // Set length of from address structure (in-out parameter)
          socklen_t fromAddrLen = sizeof(fromAddr);
          printf("Enter the id of the user you would like to play with: ");
          int user_to_play_with;
          scanf("%i", &user_to_play_with);

          // TODO tcp port
          struct ClientToServerMessage request_to_play = {play, userId, user_to_play_with, 5};

		  size_t numBytesUnfollow = sendto(sock, (struct ClientToServerMessage*)&request_to_play, sizeof(request_to_play), 0, servAddr->ai_addr, servAddr->ai_addrlen);
		  
		  struct PlayerInfo * temp = malloc(sizeof(struct PlayerInfo));
		  
		  ssize_t numBytesRcvd = recvfrom(sock, temp, MAXSTRINGLENGTH, 0, (struct sockaddr *) &fromAddr, &fromAddrLen);
		  
		  printf("(%s), (%i) ", temp->ip_address, temp->player_id);
		  
		  
		  #define PORT 8080

		    struct sockaddr_in address;
			int sock = 0, valread;
			struct sockaddr_in serv_addr;
			char *hello = "Hello from client";
			int buffer = -1;
			if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
				printf("\n Socket creation error \n");
				return -1;
			}
		  
			memset(&serv_addr, '0', sizeof(serv_addr));
		  
			serv_addr.sin_family = AF_INET;
			serv_addr.sin_port = htons(PORT);
			  
			// Convert IPv4 and IPv6 addresses from text to binary form
			if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
				printf("\nInvalid address/ Address not supported \n");
				return -1;
			}
		  
			if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
				printf("\nConnection Failed \n");
				return -1;
			}
			send(sock , buffer , strlen(hello) , 0 );
			printf("Hello message sent\n");
			int recvBuffer;
			
			valread = read( sock , recvBuffer, 1024);
			printf("%i\n",recvBuffer );
		  
		  /*TCP
		  
		  int sock;                       
			struct sockaddr_in echoServAddr; 
			unsigned short echoServPort;    
			char *servIP;                   
			char *echoString;                
			char echoBuffer[RCVBUFSIZE];    
			unsigned int echoStringLen;      
			int bytesRcvd, totalBytesRcvd;   
		  servIP = temp->ip_address;
		  echoString = "testingYOLO";
		  echoServPort = 22800 + temp->player_id + 1;
		  sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		  
		  memset(&echoServAddr, 0, sizeof(echoServAddr));    
			echoServAddr.sin_family      = AF_INET;            
			echoServAddr.sin_addr.s_addr = inet_addr(servIP);  
			echoServAddr.sin_port        = htons(echoServPort); 
			
		  connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr));
		  echoStringLen = strlen(echoString);
		  
		  send(sock, echoString, echoStringLen, 0);
		  
		  printf("Sent data");
		  
		  TCP END*/
		  
		  

          // get return message from server (other player agreed or not)
          //int *return_message = malloc(sizeof(int));

          // receive following number
          //ssize_t numBytesRcvd = recvfrom(sock, return_message, MAXSTRINGLENGTH, 0,
			//                                   (struct sockaddr *) &fromAddr, &fromAddrLen);

          //if (return_message == 0) {
          //  printf("Player %i has not agreed to play.\n", user_to_play_with);
          //} else {
           // printf("Player %i has agreed to play.\n", user_to_play_with);
          //  in_game = true;
            // TODO: get ip and port from server for other player
            // TODO: go to child program to play game
          //}

          break;
	    }
	    case 3:
	    {
			#define PORT 8080

		    int server_fd, new_socket, valread;
			struct sockaddr_in address;
			int opt = 1;
			int addrlen = sizeof(address);
			int buffer = 1;
			char *hello = "Hello from server";
      
			// Creating socket file descriptor
			if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
				perror("socket failed");
				exit(EXIT_FAILURE);
			}
      
			if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
				perror("setsockopt");
				exit(EXIT_FAILURE);
			}
			address.sin_family = AF_INET;
			address.sin_addr.s_addr = INADDR_ANY;
			address.sin_port = htons( PORT );
      
			// Forcefully attaching socket to the port 8080
			if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
				perror("bind failed");
				exit(EXIT_FAILURE);
			}
			if (listen(server_fd, 3) < 0) {
				perror("listen");
				exit(EXIT_FAILURE);
			}
			if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
				perror("accept");
				exit(EXIT_FAILURE);
			}
			printf("About to read");
			valread = read( new_socket , buffer, 1024);
			printf("%i\n",buffer );
			
			printf("Hello message sent\n");
		}
        case 4: // Logout
        {
          struct ClientToServerMessage logout_message = {logout, userId, 0};
		      ssize_t numBytes = sendto(sock, (struct ClientToServerMessage*)&logout_message, sizeof(logout_message), 0, servAddr->ai_addr, servAddr->ai_addrlen);
          printf("Goodbye!");
          loggedIn = false;
          break;
        }
        case 5: // Quit
          printf("Quitting...");
          loggedIn = false;
          quit = true;
          close(sock);
          exit(0);
          break;
        default:
          printf("Invalid option number.");
          break;
      }
    } else if (in_game) {
      printf("would play game now");
    } else {
      printf("\nTo log in, enter your username: ");
      char username[10];
      fgets(username,10,stdin);

      printf("Welcome, %s", username);

      // send login message
      struct ClientToServerMessage login_message = {login, 1, 0};
      ssize_t numBytes = sendto(sock, (struct ClientToServerMessage*)&login_message, sizeof(login_message), 0, servAddr->ai_addr, servAddr->ai_addrlen);

      struct sockaddr_storage fromAddr;
      socklen_t fromAddrLen = sizeof(fromAddr);
	  int * temp = malloc(sizeof(int));

	  //receive user id
	  ssize_t numBytesRcvd = recvfrom(sock, temp, MAXSTRINGLENGTH, 0,
		                                 (struct sockaddr *) &fromAddr, &fromAddrLen);

	  userId = *temp;
      loggedIn = true;
    }
  }

  close(sock);
  exit(0);
}

