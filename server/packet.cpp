#include "packet.h"

// 发送数据包
// 参数  #int:目的地套接字   数据包常量引用
// 返回值为  发送状态  -1 发送失败
int send_packet(int sockfd, const packet& pac)
{
    int ret = 0;


    return ret;
}

// 接收数据包
// 参数 #int 接收源的套接字
// 返回接收状态
int recv_packet(int sockfd, packet& pac)
{
    int ret = 0;


    return ret;
}

// 获取当前消息记录的最新MsgID
// 参数 #packet* 共享内存首地址  #int 信号量集标识符
unsigned int get_MsgID(packet* pack_ptr , int semid)
{
    unsigned int MsgID;
    sigset_t oset;
    block_specified_signals(&oset);

    semaphore_z(semid, PREWMUTEX);
    semaphore_p(semid, READERNUM);
    if (get_semvalue(semid, READERNUM) == 1)
        semaphore_p(semid, MUTEX);

    /* ...读... */
    MsgID = pack_ptr[0].MsgID;

    if (get_semvalue(semid, READERNUM) == 1)
        semaphore_v(semid, MUTEX);
    semaphore_v(semid, READERNUM);

    unblock_all_signals(&oset);

    return MsgID;
}



// 获取一个数据包
// 参数 #packet* 共享内存首地址  #int 信号量集标识符    #int 数据包索引  从1开始  #packet& 数据包引用
// packnum 为0 时候默认获取最新的消息包
void get_packet(packet* pack_ptr, int semid, unsigned int packnum, packet& pack)
{
    sigset_t oset;
    block_specified_signals(&oset);

    semaphore_z(semid, PREWMUTEX);
    semaphore_p(semid, READERNUM);
    if (get_semvalue(semid, READERNUM) == 1)
        semaphore_p(semid, MUTEX);

    /* ...读... */
    if (packnum == 0)
        packnum = pack_ptr[0].MsgID;

    pack = pack_ptr[packnum];

    if (get_semvalue(semid, READERNUM) == 1)
        semaphore_v(semid, MUTEX);
    semaphore_v(semid, READERNUM);

    unblock_all_signals(&oset);

    return;
}


// 写入一个数据包
// 参数 #packet* 共享内存首地址  #int 信号量集标识符   #packet& 数据包常量引用
void write_packet(packet* pack_ptr, int semid, packet& pack)
{
    extern int latest_ID;
    sigset_t oset;

    block_specified_signals(&oset);

    semaphore_v(semid, PREWMUTEX);
    semaphore_p(semid, MUTEX);

    /* ...写... */
    latest_ID = ++pack_ptr[0].MsgID;
    pack.MsgID = latest_ID;
    pack_ptr[latest_ID] = pack;


    semaphore_v(semid, MUTEX);
    semaphore_p(semid, PREWMUTEX);

    unblock_all_signals(&oset);

    return;
}
