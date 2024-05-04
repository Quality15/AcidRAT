// client.cpp
#include "client.h"

TCP_Client::TCP_Client() {
  WSADATA wsa_data;
  int result = WSAStartup(MAKEWORD(2,2), &wsa_data);
  if(result != 0) {
    std::cerr << "WSAStartup failed\n\t" << WSAGetLastError() << '\n';
    return;
  }

  client_socket = socket(AF_INET, SOCK_STREAM, NULL);
  if(client_socket == INVALID_SOCKET) {
    std::cerr << "Failed to create socket\n\t" << WSAGetLastError() << '\n';
    WSACleanup();
    return;
  }
}

TCP_Client::~TCP_Client() {
  closesocket(client_socket);
  WSACleanup();
}

int TCP_Client::connect_to_server(const char* ip, int port) {
  SOCKADDR_IN server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr(ip);

  int result = connect(client_socket, (SOCKADDR*)&server_addr, sizeof(server_addr));
  if(result == SOCKET_ERROR) {
    std::cerr << "Failed to connect to the server\n\t" << WSAGetLastError() << '\n';
    return 1;
  }
  std::cout << "Connected!\n";
  return 0;
}

int TCP_Client::send_data(const std::string& data) {
  int result = send(client_socket, data.c_str(), data.size(), 0);
  if(result == SOCKET_ERROR) {
    std::cerr << "Failed to send data\n\t" << WSAGetLastError() << '\n';
    return 1;
  }
  return 0;
}

std::string TCP_Client::receive_data() {
  char buff[BUFF_SIZE];
  int result = recv(client_socket, buff, sizeof(buff), 0);
  if(result == SOCKET_ERROR) {
    std::cerr << "Failed to receive data\n\t" << WSAGetLastError() << '\n';
    return "";
  }
  buff[result] = '\0';  // Ensure null-termination
  return std::string(buff);
}

void TCP_Client::process_command(const std::string& command)
{
  if(command == "exit")
    exit(0);

  else if(command == "msgbox") 
  {
    std::string caption;
    std::string text;

    caption = this->receive_data();
    text = this->receive_data();

    MessageBox(NULL, caption.c_str(), text.c_str(), MB_OK);

    this->send_data("MessageBox summoned");
  }

  else if(command == "msgbeep")
  {
    std::string dur_str;
    int dur;

    dur_str = this->receive_data();
    dur = std::stoi(dur_str);

    std::string serv_callback = "Beep is playing for " + std::to_string(dur) + " seconds... Please wait";
    this->send_data(serv_callback);

    Beep(1500, dur * 1000);

    this->send_data("Beep played");
  }

  /* File transfering */
  else if(command == "upload")
  {
    std::string filename;

    filename = this->receive_data();

    UpDown::ReceiveFile(client_socket, filename);
  }
}