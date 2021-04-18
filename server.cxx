
// Socket ish
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

// Handy helpers
#include <utils.hxx>
#include <iostream>
#include <string>
#include <map>

// Alpha-Master-Scrabble-Bot objects
#include "Game.hxx"
#include "Tile.hxx"
#include "Letter.hxx"
#include "Request.hxx"
#include "PlayerInfo.hxx"
#include "InitializeRequest.hxx"

using namespace std;


// Creates HTTP response from JSON formatted string body
string createResponse(string body) {

    // Construct raw header
    string response = "HTTP/1.1 200 OK \n"
                    "Server: Alpha-Master-Scrabble-Bot \n"
                    "Content-Type: application/json \n";

    // Add content length and spacer between header/body
    string contentLength = "Content-Length: " + to_string(body.length()) + "\n\n";

    // Add everything together
    response.append(contentLength);
    response.append(body);
    
    return response;
};


#define PORT 8080
int main(int argc, char const *argv[])
{
    int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    // Creating socket file descriptor, reuse if already bound
    int option = 1;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));    

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    // Bind socket to port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    };

    // Init game
    auto game = new Game();

    // Listen for incoming requests
    while(1)
    {
        cout << "\n+++++++ Waiting for new connection ++++++++\n\n";
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            cout << "In accept" << endl;
            exit(EXIT_FAILURE);
        }
        
        char buffer[30000] = {0};
        valread = read( new_socket , buffer, 30000);
        auto req = Request(buffer);

        // Either update game with initialization request or generate next move
        string resBody;
        if (req.isInitializationRequest)    
            resBody = game->update(req);
        else 
            resBody = game->makeMove(req);

        // Add response body to a valid response string
        string res = createResponse(resBody);

        // Write response over socket back to the client and close the socket
        write(new_socket, &res[0], res.length());
        close(new_socket);
    }

    return 0;
}