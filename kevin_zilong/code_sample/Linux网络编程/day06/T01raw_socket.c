#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <netinet/in.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/if_arp.h>
#include <arpa/inet.h>

int main()
{
    int fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if(fd < 0)
    {
        perror("socket");
        return 0;
    }

    struct ifreq ifstruct;
    strcpy(ifstruct.ifr_name, "eth0");

    // get interface index  //eth0
    ioctl(fd, SIOCGIFINDEX, &ifstruct);

    // bind, ll--> low level
    struct sockaddr_ll addr;
    addr.sll_family = AF_PACKET;
    addr.sll_ifindex = ifstruct.ifr_ifindex;        // TODO: ifindex interface(wangka)
    addr.sll_protocol = htons(ETH_P_ALL);
    addr.sll_hatype = ARPHRD_ETHER;
    addr.sll_pkttype = PACKET_OTHERHOST;
    addr.sll_halen = 6;
    addr.sll_addr[6] = 0;
    addr.sll_addr[7] = 0;

    // get mac addr
    ioctl(fd, SIOCGIFHWADDR, &ifstruct);
    
    ioctl(fd, SIOCGIFFLAGS, &ifstruct);
    ifstruct.ifr_flags |= IFF_PROMISC; // 
    ioctl(fd, SIOCSIFFLAGS, &ifstruct);

    int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0)
    {
        printf("bind error\n");
        return 0;
    }


    while(1)
    {
        unsigned char buf[2048]; // MAX is 1518
        unsigned char* p = buf;
        ret = read(fd, buf, sizeof(buf));
  //      printf("ret is %d\n", ret);

        // first 6 bytes = MAC_ADDR
//        printf("%02x:%02x:%02x:%02x:%02x:%02x\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);

        p += 6; //
        p += 6;

        unsigned short type = *(unsigned short*)p;
        type = ntohs(type);
   //     printf("type is : %04x\n", type);

        if(type != 0x0800)  // not IP
        {
            continue;
        }

        p += 2;

        uint32_t srcaddr = *(uint32_t*)(p+12);
        uint32_t dstaddr = *(uint32_t*)(p+16);

        struct in_addr in;
        in.s_addr = srcaddr;
      //  printf("%s  ", inet_ntoa(in));

        in.s_addr = dstaddr;
    //    printf("%s\n", inet_ntoa(in));


        uint8_t f = *p; //
        f &= 0x0f;
        f *= 4;

        uint8_t ptype = *(p+9); // 6 --> tcp 17 --> udp
  //      printf("ptype = %d\n", ptype);

        // cross ip head
        p += f;
        //printf("f is %d\n", f);

        if(ptype == 17)
            continue;

        f = *(p+12);  // headerlen, 1111
        f &= 0xf0;
        f >>= 4;
        f *= 4;
//        printf("tcp header len=%d\n", f);


        p += f;

        printf("%s\n\n\n", p);

    }

}




