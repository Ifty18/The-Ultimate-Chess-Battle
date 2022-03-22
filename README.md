# The-Ultimate-Chess-Battle
A Computer Networks project written in <a href="https://en.wikipedia.org/wiki/C_(programming_language)">C</a> that uses <a href="https://www.raylib.com/">RayLib</a> for the graphical design. The game supports multiple users being constructed in a concurrent manner. 

### Installation

Raylib must be installed first in order for the graphical interface to work. The steps for using the app after installing the library are the following:

1. Build and run the server in the Networking directory:
 ```sh
 gcc -o server server.c
 ```
 ```sh
 ./server
 ```
2. Build and run the client side of the application in the App directory;
 ```sh
 cc -o main main.c start.c login.c logo.c menu.c lobby.c options.c game.c wwin.c bwin.c exit.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
 ```
   
 ```sh
 ./main 127.0.0.1 1818
 ```
It is worth mentioning that the first argument (in this case 127.0.0.1) can be replaced with the user's IP address, whereas the second argument must alwways be 1818 which is the port where the server can be accessed.
