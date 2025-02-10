#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <pthread.h>
#include <stdbool.h>

#define BUF_LEN 255

#include <iostream>
#include <map>
#include <string>
#include <set>
#include <functional>

class KeyValueCRDT
{
public:
    void set(std::string key, std::string value)
    {
        if (map.find(key) == map.end())
        {
            map[key] = value;
        }
        else {
            if (std::hash<std::string>{}(map[key]) <= std::hash<std::string>{}(value))
            {
                map[key] = value;
            }
        }
    }
    void del(std::string key)
    {
        tombstone.insert(key);
    }
    std::map<std::string, std::string> get_map() 
    {
        std::map<std::string, std::string> result;
        for (auto it = map.begin(); it != map.end(); it++)
        {
            if (tombstone.find(it->first) == tombstone.end())
            {
                result[it->first] = it->second;
            }
        }
        return result;
    }

private:
    std::map<std::string, std::string> map;
    std::set<std::string> tombstone;
};




KeyValueCRDT crdt;
int send_port[3] = {5000, 5001, 5002};


void *sendthread(void *arg)
{
    int sock;
    struct sockaddr_in addr[3];

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    for (int i = 0; i < 3; i++)
    {
        addr[i].sin_family = AF_INET;
        addr[i].sin_port = htons(send_port[i]);
        addr[i].sin_addr.s_addr = inet_addr("127.0.0.1");
    }
    while (1)
    {
        std::string op;
        std::cin >> op;
        char data[BUF_LEN];
        if (op == "set")
        {
            std::string key, value;
            std::cin >> key >> value;
            crdt.set(key, value);
            std::string msg = "set " + key + " " + value;
            strcpy(data, msg.c_str());
        }
        else if (op == "del")
        {
            std::string key;
            std::cin >> key;
            crdt.del(key);
            std::string msg = "del " + key;
            strcpy(data, msg.c_str());
        }
        else if (op == "get")
        {
            std::map<std::string, std::string> result = crdt.get_map();
            for (auto it = result.begin(); it != result.end(); it++)
            {
                std::cout << it->first << " " << it->second << std::endl;
            }
            continue;
        }
        else
        {
            printf("Invalid command\n");
            continue;
        }
        for (int i = 0; i < 3; i++)
        sendto(sock, data, BUF_LEN, 0, (struct sockaddr *)&addr[i], sizeof(addr[i]));
    }
    close(sock);
    return 0;
}
void *recvthread(void *arg)
{
    int sock;
    struct sockaddr_in addr;

    char buf[BUF_LEN];

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(*(int *)arg);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    while (1)
    {
        memset(buf, 0, sizeof(buf));
        if (recv(sock, buf, sizeof(buf), 0) < 0)
        {
            perror("recv");
            exit(1);
        }
        char *token = strtok(buf, " ");
        if (strcmp(token, "set") == 0)
        {
            token = strtok(NULL, " ");
            std::string key = token;
            token = strtok(NULL, " ");
            std::string value = token;
            crdt.set(key, value);
        }
        else if (strcmp(token, "del") == 0)
        {
            token = strtok(NULL, " ");
            std::string key = token;
            crdt.del(key);
        }
    }
}
int main(int argc, char *argv[])
{
    int recvport;
    if (argc != 2)
    {
        printf("Usage: %s <recvport>\n", argv[0]);
        return 1;
    }
    recvport = atoi(argv[1]);
    pthread_t pthread1, pthread2;
    pthread_create(&pthread1, NULL, &sendthread, NULL);
    pthread_create(&pthread2, NULL, &recvthread, &recvport);
    pthread_join(pthread1, NULL);
    pthread_join(pthread2, NULL);
    return 0;
}