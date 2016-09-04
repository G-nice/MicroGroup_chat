#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <time.h>


#define BUFFSIZE 4196


#include "g_lib.h"
#include "packet.h"
#include "epoll.h"


//typedef void (*sighandler_t) (int);

int main(int argc, char* argv[])
{
    int server_fd;    //, rec_len = 0;
    int latest_ID = 0;
    char buff[BUFFSIZE];
    //char identity[32];
    packet pack_send, pack_recv;
    memset(&pack_send, 0, sizeof(packet));
    memset(&pack_recv, 0, sizeof(packet));

    sighandler_t sh;

    int epfd;  // epoll句柄
    int ready_num;

//    unsigned int MsgID = 0;
    static int n = 0;  //recv 记数


    // 错误参数  使用方法提示
    if (argc != 3)
    {
        fprintf(stderr, "Usage: chat_client <IP> <PORT>\n");
        exit(EXIT_SUCCESS);
    }

    // 连接服务器
    server_fd = connect_server(argv[1], argv[2]);

    // 创建epoll句柄
    if ((epfd = epoll_create(EPOLL_SIZE)) == -1 )
        err_exit("epoll create error");

    epoll_event events[EPOLL_SIZE];
    epoll_add_event(epfd, STDIN_FILENO);
    epoll_add_event(epfd, server_fd);


    // 询问用户昵称
    printf("Please input your nickname: ");
    fgets(pack_send.identity, sizeof(pack_send.identity), stdin);
    pack_send.identity[strlen(pack_send.identity) - 1] = '\0';  //去除结尾回车 \n
    printf("Welcome %s\nChat now. Have fun.\n\n", pack_send.identity);


    // 进行消息记录同步
    // 判断同路径下有没有保存消息号码的文件  若有 读取其中的号码
    int MsgID_record = 0;  // 上次浏览的最后消息记录标号
    FILE* fp;
    if (access("./MsgID_record.dat", F_OK) == 0)
    {
        fp = fopen("./MsgID_record.dat", "rb+");
        fread(&MsgID_record, sizeof(int), 1, fp);
        rewind(fp);  // 读写指针重置到文件起始位置
    }
    else
    {
        MsgID_record = 0;
        fp = fopen("./MsgID_record.dat", "wb");
    }


    send_int32(server_fd, MsgID_record);

    static int i;    // epoll 循环变量
    static int nbytes = 0;    // read 读取输入缓冲字节数

    while (true)
    {
        ready_num = epoll_wait(epfd, events, EPOLL_SIZE, EPOLL_TIMEOUT);  // -1
//printf("~epoll ready_num: %d\n", ready_num);
        for (i = 0; i < ready_num; ++i)
        {
            // 输入就绪
            if (events[i].data.fd == STDIN_FILENO)
            {
//printf("~inputing\n");
                nbytes = read(STDIN_FILENO, buff, BUFFSIZE);
                if (nbytes == -1 || nbytes == 0)  // read 出错 或 读取到文件末尾
                    continue;

                buff[nbytes] = '\0';
                if (strcmp("//exit\n", buff) == 0)
                {
                    close(server_fd);
                    // 保存最后的消息号码
                    fwrite(&latest_ID, sizeof(int), 1, fp);
                    fclose(fp);
                    exit(EXIT_SUCCESS);
                }
                        // 填写时间戳
                get_time(pack_send.time_str, sizeof(pack_send.time_str));
//printf("%s\n", pack_send.time_str);

                memset(pack_send.massage, 0, sizeof(pack_send.massage));
//printf("%u\n",  sizeof(pack_send.massage));
//                pack_send.MsgID = -1;
                strcpy(pack_send.massage, buff);    //strcpy  TO <-- from
                pack_send.massage[strlen(pack_send.massage) - 1] = '\0';  //去除结尾回车 \n

                sh = Signal(SIGPIPE, SIG_IGN);    //忽略SIGPIPE
                if (sendn(server_fd, &pack_send, sizeof(packet), 0) == -1 )
                {
                    Signal(SIGPIPE, sh);  //恢复SIGPIPE
                    err_exit("send message error");
                }
                Signal(SIGPIPE, sh);  //恢复SIGPIPE
                latest_ID++;  //最新消息号自增
//printf("~send finfshed\n");
            }
            // 有消息推送
            else if (events[i].data.fd == server_fd)
            {
//printf("~receiving\n");
                if ( (n = recvn(server_fd, &pack_recv, sizeof(packet), 0)) == -1)
                {
                    perror("recv message error");
                    exit(EXIT_FAILURE);
                }
                else if (n == 0)
                {
                    fprintf(stderr, "server has closed.\n");
                    close(server_fd);
                    exit(EXIT_SUCCESS);
                }
                // 输出对话内容
                fprintf(stdout, "#%s\t%s\n", pack_recv.time_str, pack_recv.identity);
                fprintf(stdout, "  %s\n", pack_recv.massage);
                // 更新消息号码
                latest_ID = pack_recv.MsgID;
            }
        }
    }
    // 保存最后的消息号码
    fwrite(&latest_ID, sizeof(int), 1, fp);
    fclose(fp);

    close(server_fd);
    exit(0);
}
