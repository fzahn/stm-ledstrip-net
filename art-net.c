#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/udp.h"
#include "art-net.h"
#include "art-net_packets.h"
#include <string.h>


#ifndef ARTNET_DEBUG
#define ARTNET_DEBUG         LWIP_DBG_OFF
#endif

void artnet_send_poll_reply(struct udp_pcb * upcb)
{

     
    //p = pbuf_alloc(PBUF_TRANSPORT,sizeof(msg),PBUF_RAM);
}

void artnet_recv(void * arg, struct udp_pcb * upcb,
                                         struct pbuf * p,
                                         struct ip_addr * addr,
                                         u16_t port)
{
    artnet_packet_t * artnet_packet = (artnet_packet_t*) p->payload;
    
    printf("Datagramm received!\r\n");
    if (p->len >= 3)
    {
        if (strncmp("Art-Net", (char*) artnet_packet->id, 8) == 0);
        {
	    printf("%s\n", artnet_packet->id);
            printf("OpCode: %x \r\n", artnet_packet->op_code);

            switch (artnet_packet->op_code)
	    {
                case ARTNET_OP_POLL:
                    printf("Art-Net Poll\r\n");
		    uint16_t proto_version = ((artnet_packet->data.poll.version_hi << 8) | artnet_packet->data.poll.version_lo);
		    if (proto_version > 14)
                    {
			//artnet_send_poll_reply();
                    }
                    break;
                case ARTNET_OP_POLL_REPLY:
		    printf("Art-Net PollReply\r\n");
                    break;
            }
        }
    }
    pbuf_free(p);
}

void artnet_init(void)
{
    struct udp_pcb *pcb;

    LWIP_DEBUGF(ARTNET_DEBUG, ("art-net_init\r\n"));

    pcb = udp_new();
    udp_bind(pcb, IP_ADDR_ANY, 0x1936);

    udp_recv(pcb, artnet_recv, NULL);
}


