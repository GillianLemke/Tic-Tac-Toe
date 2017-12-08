// Server side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdbool.h>

int read();
char getChar();
int close();
int determineWin(int boardState[], int my_player_number, int other_player_number) {
	if (boardState[0] == boardState[1] &&
		boardState[1] == boardState[2]) {
		if (boardState[0] == my_player_number) {
			printf("YOU WIN!");
			return 1;
		} else if (boardState[0] == other_player_number) {
			printf("YOU LOSE!");
			return 1;
		}
	} else if (boardState[0] == boardState[4] &&
		boardState[4] == boardState[8]) {
		if (boardState[0] == my_player_number) {
			printf("YOU WIN!");
			return 1;
		} else if (boardState[0] == other_player_number) {
			printf("YOU LOSE!");
			return 1;
		}
	} else if (boardState[0] == boardState[3] &&
		boardState[3] == boardState[6]) {
		if (boardState[0] == my_player_number) {
			printf("YOU WIN!");
			return 1;
		} else if (boardState[0] == other_player_number) {
			printf("YOU LOSE!");
			return 1;
		}
	} else if (boardState[1] == boardState[4] &&
		boardState[4] == boardState[7]) {
		if (boardState[1] == my_player_number) {
			printf("YOU WIN!");
			return 1;
		} else if (boardState[4] == other_player_number) {
			printf("YOU LOSE!");
			return 1;
		}
	} else if (boardState[2] == boardState[5] &&
		boardState[5] == boardState[8]) {
		if (boardState[2] == my_player_number) {
			printf("YOU WIN!");
			return 1;
		} else if (boardState[2] == other_player_number) {
			printf("YOU LOSE!");
			return 1;
		}
	} else if (boardState[2] == boardState[4] &&
		boardState[4] == boardState[6]) {
		if (boardState[2] == my_player_number) {
			printf("YOU WIN!");
			return 1;
		} else if (boardState[2] == other_player_number) {
			printf("YOU LOSE!");
			return 1;
		}
	} else if (boardState[3] == boardState[4] &&
		boardState[4] == boardState[5]) {
		if (boardState[3] == my_player_number) {
			printf("YOU WIN!");
			return 1;
		} else if (boardState[3] == other_player_number) {
			printf("YOU LOSE!");
			return 1;
		}
	} else if (boardState[6] == boardState[7] &&
		boardState[7] == boardState[8]) {
		if (boardState[6] == my_player_number) {
			printf("YOU WIN!");
			return 1;
		} else if (boardState[6] == other_player_number) {
			printf("YOU LOSE!");
			return 1;
		}
	} else {
		int i = 0;
		bool contains_empty_slots = false;
		while (i < 9) {
			if (boardState[i] == 0) {
				contains_empty_slots = true;
			}
			i+=1;
		}
		if (contains_empty_slots == true) {
			return 0;
		} else {
			printf("IT'S A DRAW!");
			return 1;
		}
		return 0;
	}
}

int main(int argc, char const *argv[]) {
    int server_fd, sock, valread;
    
    int id = *argv[1];
    int PORT = 22800 + id + 1;
    
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
          
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed\n");
        exit(EXIT_FAILURE);
    }
          
    // Forcefully attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt\n");
        exit(EXIT_FAILURE);
    }
        
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
            
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed\n");
        exit(EXIT_FAILURE);
    }
        
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
        
    if ((sock = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept\n");
        exit(EXIT_FAILURE);
    }
    
    int boardState[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	int my_player_number = 2;
	int other_player_number = 1;
    bool back_to_main_menu = false;
    while (!back_to_main_menu) {
		int bytesReceived = 0;
		int stringLength = 1;
		char other_play[2];
		while (bytesReceived < stringLength) {
			valread = read(sock, other_play, 1);
			bytesReceived+=1;
			other_play[bytesReceived] = '\0';
		}
		
		printf("Other player played: %s\n", other_play);
		int board_index = atoi(other_play);
		boardState[board_index] = other_player_number;
		
		int win_now = determineWin(boardState, my_player_number, other_player_number);
		if (win_now > 0) {
			back_to_main_menu = true;
			close(sock);
			break;
		}
		
		int command;
		printf("\nUse the numbers preceding the options below to execute.");
		printf("\n1. View board state.");
		printf("\n2. Enter your play.");
		printf("\n3. Back to main menu.");
		scanf("%d", &command);
		
		switch(command) {
			case 1: 
			{
				// print board state
				int current = 0;
				printf("You are 'o's\n");
				while (current < 9) {
					if (boardState[current] == my_player_number) {
						printf("o");
					} else if (boardState[current] == other_player_number) {
						printf("x");
					} else {
						printf(" ");
					}
					switch(current) {
						case 0:
						case 1:
						case 3:
						case 4:
						case 6:
						case 7:
							printf("|");
							break;
						case 2:
						case 5:
							printf("\n-----\n");
					}
					current+=1;
				}
				printf("\n");
			}
			case 2:
			{	
				printf("Enter the number for the play you'd like to make: ");
				char play[2];
	
				scanf("%1s", play);
				strtok(play, "\n");
				play[2] = '\0';
				printf("%s\n", play);
				
				int index = atoi(play);
				boardState[index] = my_player_number;
								
				int win = determineWin(boardState, my_player_number, other_player_number);
				
				send(sock, play, strlen(play), 0);
				printf("\nSent your play: %s\n", play);
				
				if (win > 0) {
					back_to_main_menu = true;
					close(sock);
				}
				
				break;
			}
			case 3:
			{
				close(sock);
				break;
			}
			default:
			{
				break;
			}
		}
      
    }
    
}
