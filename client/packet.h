#ifndef GUARD_PACKET_H
#define GUARD_PACKET_H

#include "g_lib.h"
#include "signal_handle.h"

// 通讯协议

typedef struct
{
    unsigned int MsgID;
    char identity[32];
    char time_str[32];
    char massage[188];
} packet;


// 发送数据包
// 参数  #int:目的地套接字   数据包常量引用
// 返回值为  发送状态  -1 发送失败
int send_packet(int sockfd, const packet& pac);

// 接收数据包
// 参数 #int 接收源的套接字
// 返回接收状态
int recv_packet(int sockfd, packet& pac);



#endif // GUARD_PACKET_H
