#ifndef UPDOWN_HPP
#define UPDOWN_HPP

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <tchar.h>
#include <chrono>

#define FILE_BUFF_SIZE              100'000'000

namespace UpDown {
  void ReceiveFile(SOCKET s, const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
      printf("[! | UpDown.cpp] Failed to create file: %s\n", filename.c_str());
      return;
    }

    std::streampos fileSize;
    int bytesReceived = recv(s, reinterpret_cast<char*>(&fileSize), sizeof(fileSize), 0);
    if (bytesReceived <= 0) {
      printf("[! | UpDown.cpp] Failed to receive file size\n");
      file.close();
      std::remove(filename.c_str());
      return;
    }

    char* buffer = new char[FILE_BUFF_SIZE];

    while (fileSize > 0) {
      int bytesReceived = recv(s, buffer, FILE_BUFF_SIZE, 0);
      if (bytesReceived <= 0) {
        printf("[! | UpDown.cpp] Failed to receive file\n");
        break;
      }

      file.write(buffer, bytesReceived);
      fileSize -= bytesReceived;
    }

    delete[] buffer;
    file.close();

    if (fileSize <= 0) {
      printf("[* | UpDown.cpp] File received: %s\n", filename.c_str());
    } else {
      printf("[! | UpDown.cpp] Failed to receive complete file: %s\n", filename.c_str());
      std::remove(filename.c_str());
    }
  }

  void SendFile(SOCKET s, const char* filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
      printf("[! | UpDown.cpp] Failed to open file: %s\n", filename);
      return;
    }

    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    char* buffer = new char[FILE_BUFF_SIZE];

    send(s, reinterpret_cast<char*>(&fileSize), sizeof(fileSize), 0);

    while (fileSize > 0) {
      std::streamsize bytesRead = file.readsome(buffer, FILE_BUFF_SIZE);
      if (bytesRead <= 0) {
        printf("[! | UpDown.cpp] Failed to read file: %s\n", filename);
        break;
      }

      int bytesSent = send(s, buffer, bytesRead, 0);
      if (bytesSent <= 0) {
        printf("[! | UpDown.cpp] Failed to send file: %s\n", filename);
        break;
      }

      fileSize -= bytesSent;
    }

    delete[] buffer;
    file.close();
  }
}

#endif // UPDOWN_HPP
