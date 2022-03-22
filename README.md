# The-Ultimate-Chess-Battle
Computer Networks project

### Installation

Raylib must be installed first in order for the graphical interface to work. The steps for using the app are the following:

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
