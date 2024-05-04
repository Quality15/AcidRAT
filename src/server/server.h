// server.h
#pragma once
#include<WinSock2.h>
#include<Windows.h>
#include<WS2tcpip.h>
#include<iostream>
#include<string>

// Assuming "../colors.h" has its own header guard
#include "../colors.h"

#pragma comment(lib, "Ws2_32.lib")

#define BUFF_SIZE 1024

class TCP_Server {
private:
  SOCKET server_socket;
  SOCKADDR_IN server_addr;
  int sizeofaddr;
public:
  TCP_Server(int port);
  ~TCP_Server();
  SOCKET accept_client();
  int send_data(SOCKET s, const std::string& data);
  std::string receive_data(SOCKET s);

  void process_command(SOCKET s, const std::string& command);
};

