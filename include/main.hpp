#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <string>

const short gMaxMsgSz{256};

int Socket(const int &_domain, const int &_type, const int &_protocol)
{
    int fd = socket(_domain, _type, _protocol);
    if (fd < 0)
    {
        std::cerr << "bad socket: " << errno << '\n';
        exit(EXIT_FAILURE);
    }
    return fd;
}

void Bind(const int &_fd, const struct sockaddr_in &_addr, const socklen_t &_len)
{
    int bn = bind(_fd, (struct sockaddr *)&_addr, _len);
    if (bn < 0)
    {
        std::cerr << "bad bind: " << errno << '\n';
        exit(EXIT_FAILURE);
    }
}

void Listen(const int &_fd, const int &_n)
{
    int lsn = listen(_fd, _n);
    if (lsn < 0)
    {
        std::cerr << "bad lsn: " << errno << '\n';
        exit(EXIT_FAILURE);
    }
}

int Accept(const int &_fd, const struct sockaddr_in &_cliAddr, socklen_t &_len)
{
    int acp = accept(_fd, (struct sockaddr *)&_cliAddr, &_len);
    if (acp < 0)
    {
        std::cerr << "bad acp: " << errno << '\n';
        exit(EXIT_FAILURE);
    }
    return acp;
}

void Recv(const int &_connfd, char *buff, const size_t &_n, const int &_flg)
{
    ssize_t rv = recv(_connfd, buff, _n, _flg);
    if (rv < 0)
    {
        std::cerr << "bad recv: " << errno << '\n';
        exit(EXIT_FAILURE);
    }
}

void Send(const int &_connfd, const void *buff, const size_t &_n, const int &_flg)
{
    ssize_t sd = send(_connfd, buff, _n, _flg);
    if (sd < 0)
    {
        std::cerr << "bad send: " << errno << '\n';
        exit(EXIT_FAILURE);
    }
}

void Inet_pton(const int &_domain, const std::string &_ip, in_addr &_bf)
{
    int pn = inet_pton(_domain, _ip.c_str(), &_bf);
    if (pn < 1)
    {
        std::cerr << "bad pton: " << errno << '\n';
        exit(EXIT_FAILURE);
    }
}

void Connect(const int &_fd, const struct sockaddr_in &_addr, const socklen_t &_len)
{
    int cn = connect(_fd, (struct sockaddr *)&_addr, _len);
    if (cn < 0)
    {
        std::cerr << "bad connect: " << errno << '\n';
        exit(EXIT_FAILURE);
    }
}

ssize_t Read(const int &_fd, char *_buff, const size_t &_size)
{
    ssize_t rd = read(_fd, _buff, _size);
    if (rd < 0)
    {
        std::cerr << "bad read: " << errno << '\n';
        exit(EXIT_FAILURE);
    }
    return rd;
}

void Write(const int &_fd, char *_buff, const size_t &_size)
{
    int wt = write(_fd, _buff, _size);
    if (wt < 0)
    {
        std::cerr << "bad write: " << errno << '\n';
        exit(EXIT_FAILURE);
    }
}

void Signal(const int &_sig, const sighandler_t &_handler)
{
    sighandler_t sg;

    if ((sg = signal(_sig, _handler)) == SIG_ERR)
    {
        std::cerr << "bad signal: " << errno << '\n';
        exit(EXIT_FAILURE);
    }
}

void sig_chld(int signo)
{
    pid_t pid;
    int stat;

    pid = wait(&stat);
    return;
}