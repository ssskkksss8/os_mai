#include "socket.hpp"

Msg::Msg(){
    action = noAction;
    arg1 = 0;
    arg2 = 0;
}

Msg::Msg(ActionType action, int arg1, int arg2){
    this->action = action;
    this->arg1 = arg1;
    this->arg2 = arg2;
}

void Msg::setMsgArgs(int new_arg1, int new_arg2){
    arg1 = new_arg1;
    arg2 = new_arg2;
}

Socket::Socket(){
    context = zmq_ctx_new();//создается контекст
    socket = zmq_socket(context, ZMQ_PAIR);
    zmq_setsockopt(socket, ZMQ_SNDTIMEO, &WAIT_TIME, sizeof(int));//таймаут отправки
}

Socket::~Socket(){
    zmq_close(socket);
    zmq_ctx_term(context);
}

void * Socket::getSocket(){
    return socket;
}

int Socket::sendMsg(Msg * m){
    zmq_msg_t input; //создается объект 
    zmq_msg_init(&input);
    zmq_msg_init_size(&input, sizeof(Msg));//выделение памяти
    zmq_msg_init_data(&input, m, sizeof(Msg), nullptr, nullptr);//копируются данные объекта
    int res = zmq_msg_send(&input, getSocket(), 0);//отправка сообщений
    if (res == -1){
        zmq_msg_close(&input);//сообщение закрывается
        return 0;
    } 
    return 1;
}

int Socket::sendMsgNoWait(Msg * m){ //неблокирующая отправка
    zmq_msg_t input;
    zmq_msg_init(&input);
    zmq_msg_init_size(&input, sizeof(Msg));
    zmq_msg_init_data(&input, m, sizeof(Msg), nullptr, nullptr);
    int res = zmq_msg_send(&input, getSocket(), ZMQ_DONTWAIT);
    if (res == -1){
        zmq_msg_close(&input);
        return 0;
    } 
    return 1;
}

int Socket::recvMsg(Msg &m){//получение сообщения
    zmq_msg_t output;
    zmq_msg_init(&output);
    int res = zmq_msg_recv(&output, getSocket(), 0);
    if (res == -1){
        zmq_msg_close(&output);
        return 0;
    }
    m = *static_cast<Msg *>(zmq_msg_data(&output));//успешно -> копируем вобъект
    zmq_msg_close(&output);
    return 1;
}

int Socket::sendRecvMsg(Msg * input, Msg & output){
    if (!sendMsg(input)){
        return 0;
    }
    if (!recvMsg(output)){
        return 0;
    }
    return 1;
}


int Socket::sendNoWaitRecvMsg(Msg * input, Msg & output){
    if (!sendMsgNoWait(input)){
        return 0;
    }
    if (!recvMsg(output)){
        return 0;
    }
    return 1;
}

BindingSocket::BindingSocket(int id){//привязка сокета к указанному порту
    zmq_bind(getSocket(), ("tcp://*:" + std::to_string(PORT_BASE + id)).c_str());
}

ConnectingSocket::ConnectingSocket(int id){//подключение сокета
    zmq_connect(getSocket(), ("tcp://localhost:" + std::to_string(PORT_BASE + id)).c_str());
}
