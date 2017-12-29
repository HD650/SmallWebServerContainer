#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

bool return_file(int client, char* file_name);
int get_line(int sock, char *buf, int size);
bool parse_url(char* method,char* path,int sock_id);
#endif