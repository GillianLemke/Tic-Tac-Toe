# Setup
Run on ubuntu 16.04

## Compile 
Compile tcpclient.c to game_client_requester<br/>
`gcc tcpclient.c -o game_client_requester`<br/>

Compile tcpserver.c to game_client_requested<br/>
`gcc tcpserver.c -o game_client_requested`<br/>

Compile main.c to client<br/>
`gcc main.c -o client`<br/>

Compile server.c to server<br/>
`gcc server.c -o server`<br/>

## Run
Run `./server`

Open another terminal window

Run `./client 127.0.0.1 22800`

Open another terminal window
 
Run `./client 127.0.0.1 22800`

# Client Directions
Must login before options become available

Username can be any string of length 10 or less

### Client Options
1. Request Available Users from Server
2. Request to Play with User by ID
3. Wait for Request to Play
4. Logout
5. Quit

### Client Options in Game
1. View board state then enter location of desired play
2. Enter location of desired play
3. Back to main menu (close connection)

## Board Layout
Numbers indicate location integer to be entered to play


`x | x | x` * `0 | 1 | 2`<br/>
`- - - - -` * `- - - - -`<br/>
`x | x | x` * `3 | 4 | 5`<br/>
`- - - - -` * `- - - - -`<br/>
`x | x | x` * `6 | 7 | 8`<br/>
