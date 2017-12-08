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

          int port = 22800 + userId + 1;
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
			int id;
			printf("Enter the id of the player you'd like to play with: ");
			scanf("%d", &id);
						
			char run_command[100];
			snprintf(run_command, 100, "./game_client_requester %i", id);
			
            int status = system(run_command);
            break;
	    }
	    case 3:
	    {
			char run_command[100];
			snprintf(run_command, 100, "./game_client_requested %i", userId);
			
			int status = system(run_command);
			break;
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

