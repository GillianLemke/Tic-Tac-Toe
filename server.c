// Gillian Lemke
// COSC439
// Project 4 - Tic Tac Toe

#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include "practical.h"

int main(int argc, char *argv[]) {

	struct ClientToServerMessage {
	  enum {login, who, play, lookup, logout} request_type; /* same size as an unsigned int */
	  unsigned int player_id;                               /* initiating player identifier */
	  unsigned int id_to_play;
	  unsigned short tcp_port;                              /* listening port*/
	};
	
	struct RequestToPlay {
		unsigned int id_who_requested;
		struct sockaddr_storage clntAddr;
	};

	struct ClientToServerMessage * temp = malloc(sizeof(struct ClientToServerMessage));
	
	struct PlayerInfo {
		unsigned int player_id;
		char ip_address[15];
	};

  char *service = "22800";

  // Construct the server address structure
  struct addrinfo addrCriteria;                   // Criteria for address
  memset(&addrCriteria, 0, sizeof(addrCriteria)); // Zero out structure
  addrCriteria.ai_family = AF_UNSPEC;             // Any address family
  addrCriteria.ai_flags = AI_PASSIVE;             // Accept on any address/port
  addrCriteria.ai_socktype = SOCK_DGRAM;          // Only datagram socket
  addrCriteria.ai_protocol = IPPROTO_UDP;         // Only UDP socket

  struct addrinfo *servAddr; // List of server addresses
  int rtnVal = getaddrinfo(NULL, service, &addrCriteria, &servAddr);

  // Create socket for incoming connections
  int sock = socket(servAddr->ai_family, servAddr->ai_socktype,
      servAddr->ai_protocol);

  // Bind to the local address
  if (bind(sock, servAddr->ai_addr, servAddr->ai_addrlen) < 0)
    fputs("dead ", stdout);

  // Free address list allocated by getaddrinfo()
  freeaddrinfo(servAddr);

  struct SavedUser {
	  unsigned int player_id;
	  bool in_game;
	  struct sockaddr_storage clntAddr;
  };

  struct SavedUser users[100];

  int next_user = 0;

  for (;;) { // Run forever
    struct sockaddr_storage clntAddr; // Client address
    // Set Length of client address structure (in-out parameter)
    socklen_t clntAddrLen = sizeof(clntAddr);

    // Block until receive message from a client
    char buffer[MAXSTRINGLENGTH]; // I/O buffer
    // Size of received message
    ssize_t numBytesRcvd = recvfrom(sock, temp, MAXSTRINGLENGTH, 0,
        (struct sockaddr *) &clntAddr, &clntAddrLen);

    setbuf(stdout, NULL);
    fflush( stdout );

    switch(temp->request_type) {
	  case login: // Login
      {
		printf("New user logging in.\n");
        struct SavedUser newUser;
		printf("created new user\n");
        newUser.player_id = next_user;
		newUser.in_game = false;
		// TODO: this should be a socket for the other clients to interact with
        newUser.clntAddr = clntAddr;
        users[next_user] = newUser;

        ssize_t numBytesSentLogin = sendto(sock, (int*)&next_user, sizeof(next_user), 0,
          (struct sockaddr *) &clntAddr, sizeof(clntAddr));
		next_user+=1;
        break;
      }
      case who: // who
      {
		printf("sending number of people who are logged in to play\n");
		ssize_t numBytesSent = sendto(sock, (int*)&next_user, sizeof(next_user), 0, 
		                             (struct sockaddr *)&clntAddr, sizeof(clntAddr));
		printf("sending ids of users logged in\n");
		for (int i = 0; i < next_user; i++) {
			ssize_t numBytesSent = sendto(sock, (int*)&users[i].player_id, sizeof(next_user), 0,
                                         (struct sockaddr *) &clntAddr, sizeof(clntAddr));
		}
        break;
      }
      case play: // play
      {
		printf("getting id of user to play\n");
		int user_id = temp->player_id;
		int id_to_play = temp->id_to_play;
		
		printf("user %i has requested to play with user %i\n", user_id, id_to_play);
		
		struct PlayerInfo playerInfo = {id_to_play, "127.0.0.1"};
		

		
		ssize_t numBytesSent = sendto(sock, (struct PlayerInfo*)&playerInfo, sizeof(playerInfo), 0, (struct sockaddr *) &clntAddr, sizeof(clntAddr));
		
		
		struct RequestToPlay new_request = {user_id, users[user_id].clntAddr};
		
		ssize_t numBytesSent2 = sendto(sock, (struct RequestToPlay*)&new_request, sizeof(new_request), 0, 
		                             (struct sockaddr *)&users[id_to_play].clntAddr, sizeof(users[id_to_play].clntAddr));
		            
		                             
		int reply;
		ssize_t numBytesRcvd = recvfrom(sock, (int*)&reply, MAXSTRINGLENGTH, 0,
			(struct sockaddr *) &users[id_to_play].clntAddr, sizeof(&users[id_to_play].clntAddr));
		
		printf("user %i replyed %i", id_to_play, reply);
		
		if (reply == 1) {
			users[id_to_play].in_game = true;
			users[user_id].in_game = true;
			ssize_t numBytesSent = sendto(sock, (int*)&reply, sizeof(reply), 0, 
		                             (struct sockaddr *)&clntAddr, sizeof(clntAddr));
		}
		
		printf("sent to other user\n");
        break;
	  }
      case lookup: // lookup
      {

        break;
	    }
      case logout: // logout
      {
        int currentUser = temp->player_id;
        printf("user %i logging out\n", currentUser);
        break;
      }
      default:
        break;
	}

  }
  // NOT REACHED
}

