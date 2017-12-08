#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdbool.h>

int inet_pton();
int read();
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
		} else if (boardState[1] == other_player_number) {
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
    struct sockaddr_in address;
    
    int id = *argv[1];
    int PORT = 22800 + id + 1;
    
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    
    char *hello = "5";
    char buffer[1];
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    char *ip_address = argv[2];
        
    int inet = inet_pton(AF_INET, ip_address, &serv_addr.sin_addr);
        
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    
    int connect_number = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  
    if (connect_number < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    
	int boardState[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	int my_player_number = 1;
	int other_player_number = 2;
  
	printf("Enter the number for the play you'd like to make: ");
	char first_play[2];
	scanf("%1s", first_play);
	strtok(first_play, "\n");
	first_play[2] = '\0';
	printf("%s\n", first_play);

	send(sock, first_play, strlen(first_play), 0);
	printf("\nSent your play: %s\n", first_play);
	
	int index = atoi(first_play);
	boardState[index] = my_player_number;
    
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
				printf("You are 'x's\n");
				while (current < 9) {
					if (boardState[current] == my_player_number) {
						printf("x");
					} else if (boardState[current] == other_player_number) {
						printf("o");
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
				back_to_main_menu = true;
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
