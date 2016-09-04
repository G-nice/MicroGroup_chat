#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>
#include <time.h>
#include <sys/wait.h>


#include "packet.h"
#include "signal_handle.h"
#include "semaphore.h"
#include "sharememory.h"
#include "g_lib.h"

//extern int sem_id;
//extern int shm_id;

#define CHAT_RECORD_NUM 128

#ifndef BUFFSIZE
    #define BUFFSIZE 4096
#endif // BUFFSIZE


// 全局变量
int socket_fd, client_fd;  // 套接字放全局只是为了让信号处理程序使用
unsigned int latest_ID;  // 最新消息标号
packet* pack_ptr = NULL;  //C++11  nullptr  // 放全局是为了让信号处理程序使用






int main(int argc, char* argv[])
{

//    char buff[BUFFSIZE];  //s输出缓冲区   //4096
    char client_addr_str[SIZEOFADDRSTR];
    int n = 0;  //计数
    time_t ticks;
    pid_t clild_pid = 0;
//    sighandler_t sh;
    packet packet_buff;
    memset(&packet_buff, 0, sizeof(packet));

    extern int sem_id;
    extern int shm_id;    // 信号量集  共享内存  标识符





// 检查参数  使用方法提示
    if (argc != 3)
    {
        fprintf(stderr, "Usage: chat_server <IP> <PORT>\n");
        exit(1);
    }

    // 创建 服务器监听 套接字
    socket_fd = listen_server(argv[1], argv[2]);

    //  设置所有信号处理函数
    register_signal_handler();

    // 进行子进程结束信号处理
    if (Signal(SIGCHLD, signal_handler) == SIG_ERR)
        perror("set signal SIGCHLD handler");

    if (Signal(SIGUSR1, SIG_IGN) == SIG_ERR)    //忽略SIGUSR1
        perror("set signal SIGUSR1 handler");
printf("set ignore SIGUSR1\n");

    // 读者----写者问题
    // 创建信号量集
    sem_id = Semget(IPC_PRIVATE, 3, 0666 | IPC_CREAT);
    // 设置信号量集初值
    set_semvalue(sem_id, MUTEX, 1);
    set_semvalue(sem_id, PREWMUTEX, 0);
    set_semvalue(sem_id, READERNUM, 128);    // 最多允许128个读者同时读取数据


    // 创建共享内存
    shm_id = Shmget(IPC_PRIVATE, CHAT_RECORD_NUM*sizeof(packet), 0666 | IPC_CREAT);
    pack_ptr = (packet*)shmat(shm_id, 0, 0);
    // 设置聊天记录标志为 0
    pack_ptr[0].MsgID = 0;

    // 等待客户连接
    printf("start wait client connect(accept)\n");

    while(true)  // true
    {

        // 接收客户端连接请求  输出连接相关信息
        if ( (client_fd = accept_client(socket_fd, client_addr_str)) == -1)
            continue;


        if ( (clild_pid =  fork()) == -1)
        {
            close(client_fd);
            shmdt(pack_ptr);  // 解除共享内存 绑定
            del_sem_set(sem_id);
            del_shemem(shm_id);
            err_exit("fork error");
        }
        else if (clild_pid == 0)  //clild poccess
        {
            close(socket_fd);
            register_signal_handler();


            // TODO: 进行与客户端聊天记录的同步
            // 进行与客户端聊天记录的同步
            // 先使用recv_int32() 接收一个客户端发来的聊天记录同步数据
            // 然后使用 send_int32() 发送相差的记录条数    // 没必要
            // 再发送给客户端相差的聊天记录信息
            unsigned int record_ID = recv_int32(client_fd);
//printf("~recv record_ID %u\n", record_ID);
            latest_ID = get_MsgID(pack_ptr, sem_id);  // 获取当前最新的消息标号
//printf("~get_MsgID %u\n", get_MsgID(pack_ptr, sem_id));
            for (unsigned int i = record_ID + 1; i <= latest_ID; i++)
            {
                get_packet(pack_ptr, sem_id, i, packet_buff);
                sendn(client_fd, &packet_buff, sizeof(packet), 0);
//                milliseconds_sleep(100);  // 挂起100毫秒
                usleep(10 *1000);    // 挂起10毫秒
            }
//printf("~synchronization finished.\n");  // 同步完成

            while(true)  // true
            {

                if ((n = recvn(client_fd, &packet_buff, sizeof(packet), 0)) == -1)    //-1时候出错  0时对方结束
                {
                    close(client_fd);
                    shmdt(pack_ptr);  // 解除共享内存 绑定
                    err_exit("recv massage error");
                }
                else if (n == 0)    // 对方关闭连接
                {
                    ticks = time(NULL);
                    printf("at %.24s\t exit connection with %s\n", ctime(&ticks), client_addr_str );
                    close(client_fd);
                    break;
                }
// printf("recv: %d bytes\n", n);

                // 填充消息包  并写入消息记录共享内存
                get_time(packet_buff.time_str, sizeof(packet_buff.time_str));
                printf("%s %s USER: %s\n##  send: %s\n", client_addr_str, packet_buff.time_str, packet_buff.identity, packet_buff.massage);  // 服务器日志
                write_packet(pack_ptr, sem_id, packet_buff);  // 写入消息记录共享内存  并且更新packet_buff中的MsgID

                kill(0, SIGUSR1);  // 通知推送消息
//printf("~get_MsgID %u\n", get_MsgID(pack_ptr, sem_id));

                // 同步客户端最新消息标号
                latest_ID = packet_buff.MsgID;  // 更新自己最新消息记录标号
//                send_int32(socket_fd, latest_ID);
            }
            shmdt(pack_ptr);  // 解除共享内存 绑定
            exit(EXIT_SUCCESS);
        }
        else  //parent poccess
        {
            close(client_fd);
        }
    }

    del_sem_set(sem_id);
    del_shemem(shm_id);

    exit(EXIT_SUCCESS);
}
