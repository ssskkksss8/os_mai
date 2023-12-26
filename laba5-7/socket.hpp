#pragma once

#include <unistd.h>
#include "zmq.h"
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>

constexpr int PORT_BASE = 4000;
constexpr int WAIT_TIME = 1000;

enum ActionType{
    noAction,
    create,
    connect,
    success,
    fail,
    add, 
    check,
    ping,
    busy,
    die
};

struct Msg{
    ActionType action;
    int arg1, arg2;
    Msg();
    Msg(ActionType action, int arg1, int arg2);
    void setMsgArgs(int new_arg1, int new_arg2);
};

class Socket{
    void * context;
    void * socket;
public:
    Socket();
    ~Socket();
    void * getSocket();
    int sendMsg(Msg * m);
    int recvMsg(Msg &m);
    int sendRecvMsg(Msg * input, Msg & output);
    int sendMsgNoWait(Msg * m);
    int sendNoWaitRecvMsg(Msg * input, Msg & output);
};

class BindingSocket : public Socket{
public:
    BindingSocket(int id);
};

class ConnectingSocket : public Socket{
public:
    ConnectingSocket(int id);
};