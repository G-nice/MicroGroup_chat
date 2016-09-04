#ifndef GUARD_PACKET_H
#define GUARD_PACKET_H

#include "g_lib.h"
#include "sharememory.h"
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


// 获取当前消息记录的最新MsgID
// 参数 #packet* 共享内存首地址  #int 信号量集标识符
unsigned int get_MsgID(packet* pack_ptr , int semid);

// 获取一个数据包
// 参数 #packet* 共享内存首地址  #int 信号量集标识符    #int 数据包索引  从1开始  #packet& 数据包引用
// packnum 为0 时候默认获取最新的消息包
void get_packet(packet* pack_ptr, int semid, unsigned int packnum, packet& pack);

// 写入一个数据包
// 参数 #packet* 共享内存首地址  #int 信号量集标识符   #packet& 数据包常量引用
void write_packet(packet* pack_ptr, int semid, packet& pack);

#endif // GUARD_PACKET_H
