#include <signal.h>
#include <fstream>
#include <variant>
#include <sstream>

#include "../include/main.hpp"

void proccess_data(const int &connfd);
std::variant<std::string, int> parse(std::stringstream &ssBuff);

namespace msgs
{
    char succ[]{"Success"};
    char unk[]{"Unknown command. Enter -help for mote info"};
    char failW[]{"Failed! Use command: write -a <text>\n"};
}

int main(int argc, char *argv[])
{
    int fd = Socket(AF_INET, SOCK_STREAM, 0);

    const int trueFlg = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &trueFlg, sizeof(int));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(6666);

    Bind(fd, addr, sizeof(addr));

    Listen(fd, SOMAXCONN);

    struct sockaddr_in cliAddr;
    socklen_t cliAddrSz{sizeof(cliAddr)};
    memset(&cliAddr, 0, cliAddrSz);

    pid_t pid;
    bool isRun{true};
    while (isRun)
    {
        int connfd = Accept(fd, cliAddr, cliAddrSz);
        Signal(SIGCHLD, sig_chld);

        if ((pid = fork()) == 0)
        {
            close(fd);
            proccess_data(connfd);
            exit(EXIT_SUCCESS);
        }

        close(connfd);
    }

    return EXIT_SUCCESS;
}

void proccess_data(const int &connfd)
{
    for (;;)
    {
        char buff[gMaxMsgSz];
        if (!Read(connfd, buff, gMaxMsgSz))
            break;

        std::stringstream ssBuff;
        ssBuff << buff;

        std::string cmnd;
        getline(ssBuff, cmnd, ' ');
        if (cmnd == "read")
        {
            std::ifstream f;
            f.open("../data.txt");

            std::stringstream data;
            data << f.rdbuf();
            f.close();
            std::string bf{data.str()};
            Write(connfd, const_cast<char *>(bf.c_str()), gMaxMsgSz);
        }
        else if (cmnd == "write")
        {
            auto parseData = parse(ssBuff);

            if (std::holds_alternative<int>(parseData))
            {
                Write(connfd, msgs::failW, gMaxMsgSz);
            }
            else
            {
                std::ofstream f;
                f.open("../data.txt", std::ios_base::app);

                f << '\n' + std::get<std::string>(parseData);
                f.close();
                Write(connfd, msgs::succ, gMaxMsgSz);
            }
        }
        else
        {
            Write(connfd, msgs::unk, gMaxMsgSz);
        }
        // Write(connfd, buff, gMaxMsgSz);
    }
}

std::variant<std::string, int> parse(std::stringstream &ssBuff)
{
    /*
        The function parsing data for write to file
        return data if success
        return -1 for error parse
    */

    std::string data, parseData;
    bool flg{false};
    while (ssBuff >> data)
    {
        if (flg)
            parseData += data + ' ';
        if (data == "-a")
            flg = true;
    }
    if (flg)
        return std::variant<std::string, int>(parseData);
    else
        return std::variant<std::string, int>(-1);
}