#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <termios.h>
#include <arpa/inet.h>

#define SETBIT(y,x)  (y |= (1<<x))
#define RESETBIT(y,x)  (y &= ~(1<<x))
#define READBIT(y,x)	( (y>>x)&0x01 )
#define PRINT_BUG  printf

bool B_CTRL_C = false;

int client_sockfd=1;
uint32_t key_mask=0;
void *socket_read_thread(void *data)
{
    int client_len;

    char buf[127];

    struct sockaddr_in clientaddr;

    client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    clientaddr.sin_family = AF_INET;
    clientaddr.sin_addr.s_addr = inet_addr("192.168.1.1");
    clientaddr.sin_port = htons(23);



    client_len = sizeof(clientaddr);

    if (connect(client_sockfd, (struct sockaddr *)&clientaddr, client_len) < 0)
    {
        perror("Connect error: ");
        exit(0);
    }
    while(1)
    {
        int len= read(client_sockfd,buf,sizeof(buf));
        buf[len]='\0';
        write(1, buf,strlen(buf));
    }

    close(client_sockfd);
    exit(0);




}




void *socket_write_thread(void *data)
{

}





/*
   static const char *const evval[3] = {
   "RELEASED",
   "PRESSED ",
   "REPEATED"
   };
   */


typedef enum KEY_MASK{
    K_UP = 0,
    K_DOWN = 1,
    K_LEFT ,
    K_RIGHT,
    K_ESC,
    K_FORWARD,
    K_BACK,
    K_EMERCY,
    K_MODE,
    K_YAWR,
    K_YAWL,
    K_CTRL_C,

}KEY_MASK;

bool mode_flag=false;
bool prev_mode_flag;

void *keyboard_thread(void *data)
{
    const char *dev = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
    struct input_event ev;
    ssize_t n;
    int fd;

    fd = open(dev, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Cannot open %s: %s.\n", dev, strerror(errno));
        exit(1);
        //return EXIT_FAILURE;
    }


    /*
       ev.time: time of the event (struct timeval type)
       ev.type: EV_KEY
       ev.code: KEY_*, key identifier; see complete list in /usr/include/linux/input.h
       ev.value: 0 if key release, 1 if key press, 2 if autorepeat keypress
       */



    while (1) {
        n = read(fd, &ev, sizeof ev);
        if (n == (ssize_t)-1) {
            if (errno == EINTR)
                continue;
            else
                break;
        } else{
            if (n != sizeof ev) {
                errno = EIO;
                break;
            }
        }




        if (ev.type == EV_KEY ){
            int num=-1;
            switch(ev.code){
                case KEY_F1:
                    if(ev.value==1){
                        if(mode_flag==false){
                            printf("joy mode\n");
                            
                            /* into controller mode */
                            char buf[20] ="controller\n";
                            write(client_sockfd,buf,strlen(buf));
                            key_mask=0;

                            SETBIT(key_mask,K_EMERCY);
                            mode_flag=true;
                        }else{

                            char buf[10] ="escape\n";
                            write(client_sockfd,buf,strlen(buf));
                            
                            printf("keyboard mode\n");
                            mode_flag=false;
                        }
                    }
                    break;
                case KEY_UP:
                    num = K_FORWARD;
                    break;
                case KEY_DOWN:
                    num = K_BACK;
                    break;
                case KEY_ESC:
                    num = K_ESC;
                    break;
                case KEY_LEFT:
                    num = K_LEFT;
                    break;
                case KEY_RIGHT:
                    num = K_RIGHT;
                    break;
                case KEY_W:
                    num = K_UP;
                    break;
                case KEY_S:
                    num = K_DOWN;
                    break;
                case KEY_A:
                    num = K_YAWL;
                    break;
                case KEY_F:
                    num = K_CTRL_C;
                    break;
                case KEY_D:
                    num = K_YAWR;
                    break;
                case KEY_Q:
                    SETBIT(key_mask,K_EMERCY);
                    break;
                case KEY_E:
                    RESETBIT(key_mask,K_EMERCY);
                    //num = K_EMERCY;
                    break;

            }
            if(num!=-1){
                if(ev.value==0){//release
                    RESETBIT(key_mask,num);
                }else if(ev.value==1){//press
                    SETBIT(key_mask,num);
                }
            }

        }




    }
    exit(1);
}





int setecho(int fd, int onoff)
{
    struct termios t;

    if (tcgetattr(fd, &t) == -1)
        return -1;

    if (onoff == 0)
        t.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);
    else
        t.c_lflag |= (ECHO | ECHOE | ECHOK | ECHONL);

    if (tcsetattr(fd, TCSANOW, &t) == -1)
        return -1;
}



int main(int argc, char* argv[])
{
int i;

if(argc<1){
    printf("input argument not enought\n");
    return 0;
}

for(i=1;i<argc;i++){
    if(strcmp(argv[i],"--help")==0){
        printf("*************  HELP  *************\n"
                "--red \n"
                "--white \n"
                "--input BADDR\n"
                );
    }else if(strcmp(argv[i],"--input")==0){
        i++;
        char buf[40];
        sprintf(buf,"sudo pand --connect %s -dGN -n",argv[i]);

        system("sudo ifconfig bnep0 192.168.1.2 up");
    }else if(strcmp(argv[i],"--red")==0){
        system("sudo pand --connect A0:14:3D:50:E8:B2 -dGN -n");
        system("sudo ifconfig bnep0 192.168.1.2 up");
    }else if(strcmp(argv[i],"--white")==0){
        system("sudo pand --connect A0:14:3D:6A:75:11 -dGN -n");
        system("sudo ifconfig bnep0 192.168.1.2 up");
    }
}





    char buf[127];

    pthread_t p_thread[3];
#if 1
    if (pthread_create(&p_thread[0], NULL, socket_read_thread, NULL)<0)
    {
        perror("thread create error : ");
        exit(0);
    }
#endif
#if 0
    if (pthread_create(&p_thread[1], NULL, socket_write_thread, NULL)<0)
    {
        perror("thread create error : ");
        exit(0);
    }
#endif
#if 1
    if (pthread_create(&p_thread[1], NULL, keyboard_thread, NULL)<0)
    {
        perror("thread create error : ");
        exit(0);
    }
#endif
    fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);

    while(1){
            usleep(90000);
        //PRINT_BUG("%d %d %d %d \n",READBIT(key_mask,K_UP),READBIT(key_mask,K_DOWN),READBIT(key_mask,K_LEFT),READBIT(key_mask,K_RIGHT));
        if(mode_flag==false){

            if(prev_mode_flag==true){
                PRINT_BUG("setecho 1 \n");
                setecho(STDIN_FILENO, 1);
            }
            int len = read(STDIN_FILENO,buf,sizeof(buf));
            if(len>0){
                write(client_sockfd,buf,len);
            }
            prev_mode_flag=mode_flag;


        }else if(mode_flag==true){
            if(READBIT(key_mask,K_CTRL_C)==1){
                char buf[10];
                buf[0]=3;
                buf[1]='\n';
                buf[2]='a';
                buf[3]='\n';
                write(client_sockfd,buf,4);
                printf("Ctrl C\n");
                setecho(0,1);
                usleep(300000); 
                continue;
            }
            if(prev_mode_flag==false){
                PRINT_BUG("setecho 0 \n");
                setecho(STDIN_FILENO, 0);
            }
            
            /*  O_JAE 수정을 해야될 라인 */
            sprintf(buf,"%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d\n",READBIT(key_mask,K_EMERCY),READBIT(key_mask,K_UP),READBIT(key_mask,K_DOWN),READBIT(key_mask,K_FORWARD),READBIT(key_mask,K_BACK),READBIT(key_mask,K_LEFT),READBIT(key_mask,K_RIGHT),READBIT(key_mask,K_YAWL),READBIT(key_mask,K_YAWR),0,0,0,0,0,0,0);
                write(client_sockfd,buf,strlen(buf));
            

            prev_mode_flag=mode_flag;
        }
        if(READBIT(key_mask,K_ESC)){
            goto end;
        }
    }




    int status;
    pthread_join(p_thread[0], (void **)&status);
    pthread_join(p_thread[1], (void **)&status);
    pthread_join(p_thread[2], (void **)&status);



    //***************************************************************************
end:
    strcpy(buf,"escape\n");
    write(client_sockfd,buf,strlen(buf));
    strcpy(buf,"0\n");
    write(client_sockfd,buf,strlen(buf));
    strcpy(buf,"exit\n");
    write(client_sockfd,buf,strlen(buf));
    setecho(STDIN_FILENO, 1);


}
