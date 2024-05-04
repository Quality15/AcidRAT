// client.h
#pragma once
#include<WinSock2.h>
#include<Windows.h>
#include<WS2tcpip.h>
#include<iostream>
#include<string>

#pragma comment(lib, "Ws2_32.lib")

#define BUFF_SIZE 1024

class TCP_Client {
private:
  SOCKET client_socket;

public:
  TCP_Client();
  ~TCP_Client();
  int connect_to_server(const char* ip, int port);
  int send_data(const std::string& data);
  std::string receive_data();

  void process_command(const std::string& command);
};
