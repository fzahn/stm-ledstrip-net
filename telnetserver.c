#include "lwipopts.h"
#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "telnetserver.h"
#include <string.h>
#include <stdint.h>


static struct tcp_pcb *telnetserver_pcb;


enum telnetserver_states
{
  ES_NONE = 0,
  ES_ACCEPTED,
  ES_RECEIVED,
  ES_CLOSING
};

/* structure for maintaing connection infos to be passed as argument
   to LwIP callbacks*/
struct telnetserver_struct
{
  uint8_t state;             /* current connection state */
  struct tcp_pcb *pcb;    /* pointer on the current tcp_pcb */
  struct pbuf *p;         /* pointer on the received/to be transmitted pbuf */
};



static err_t telnetserver_accept (void *arg, struct tcp_pcb *newpcb, err_t err);
static err_t telnetserver_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static void telnetserver_error(void *arg, err_t err);
static err_t telnetserver_poll(void *arg, struct tcp_pcb *tpcb);
static err_t telnetserver_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
static void telnetserver_send(struct tcp_pcb *tpcb, struct telnetserver_struct *es);
static void telnetserver_connection_close(struct tcp_pcb *tpcb, struct telnetserver_struct *es);





void telnetserver_init(void) {

	telnetserver_pcb = tcp_new();

	if (telnetserver_pcb != NULL) {
		err_t err;
		//bind telnetserver to port 23 (telnet)
		err = tcp_bind(telnetserver_pcb, IP_ADDR_ANY, 23);

		if (err ==ERR_OK)
		{

			//start listening for telnet
			telnetserver_pcb = tcp_listen(telnetserver_pcb);

			//accept incomming connections and use callback_funciton telnetserver_accept
			tcp_accept(telnetserver_pcb, telnetserver_accept);
		} else {
			//handle bind pcb not possible
		}
	} else {
		//handle can not create new pcb
	}


}


static err_t telnetserver_accept (void *arg, struct tcp_pcb *newpcb, err_t err)
{
	err_t ret_err;

	//allocate structure es to maintian tcp connection infos
	struct telnetserver_struct *es;
	es = (struct telnetserver_struct *)mem_malloc(sizeof(struct telnetserver_struct));

	if (es!=NULL)
	{
		es->state = ES_ACCEPTED;
		es->pcb = newpcb;
		es->p = NULL;

		tcp_arg(newpcb, es);

		tcp_recv(newpcb, telnetserver_recv);

		tcp_err(newpcb, telnetserver_error);

		tcp_poll(newpcb, telnetserver_poll, 1);

		ret_err= ERR_OK;
	}
	return ret_err;

}

static err_t telnetserver_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
	struct telnetserver_struct *es;
	es = (struct telnetserver_struct *)arg;
	struct pbuf *incoming_pbuf;
	incoming_pbuf=es->p;
	char stringincoming[30];
	strcpy(p->payload,stringincoming);




	struct pbuf *outgoing_pbuf;

	char stringoutgoing[30];
	strcpy (stringincoming,stringoutgoing);
	outgoing_pbuf=pbuf_alloc(PBUF_TRANSPORT, 0, PBUF_REF);
	outgoing_pbuf->payload=stringoutgoing;
	outgoing_pbuf->len = outgoing_pbuf->tot_len = strlen(stringoutgoing);
	es->p=outgoing_pbuf;
	telnetserver_send(tpcb,es);
	err = ERR_OK;
	return err;

}

static void telnetserver_error(void *arg, err_t err)
{

}

static err_t telnetserver_poll(void *arg, struct tcp_pcb *tpcb)
{
	 err_t ret_err;
	  struct telnetserver_struct *es;

	  es = (struct telnetserver_struct *)arg;
	  if (es != NULL)
	  {
	    if (es->p != NULL)
	    {
	      /* there is a remaining pbuf (chain) , try to send data */
	      telnetserver_send(tpcb, es);
	    }
	    else
	    {
	      /* no remaining pbuf (chain)  */
	      if(es->state == ES_CLOSING)
	      {
	        /*  close tcp connection */
	        telnetserver_connection_close(tpcb, es);
	      }
	    }
	    ret_err = ERR_OK;
	  }
	  else
	  {
	    /* nothing to be done */
	    tcp_abort(tpcb);
	    ret_err = ERR_ABRT;
	  }
	  return ret_err;

}

static err_t telnetserver_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
	err_t ret_err = ERR_OK;
	return ret_err;
}

static void telnetserver_send(struct tcp_pcb *tpcb, struct telnetserver_struct *es)
{
  struct pbuf *ptr;
  err_t wr_err = ERR_OK;

  while ((wr_err == ERR_OK) &&
         (es->p != NULL) &&
         (es->p->len <= tcp_sndbuf(tpcb)))
  {

    /* get pointer on pbuf from es structure */
    ptr = es->p;

    /* enqueue data for transmission */
    wr_err = tcp_write(tpcb, ptr->payload, ptr->len, 1);

    if (wr_err == ERR_OK)
    {
      u16_t plen;

      plen = ptr->len;

      /* continue with next pbuf in chain (if any) */
      es->p = ptr->next;

      if(es->p != NULL)
      {
        /* increment reference count for es->p */
        pbuf_ref(es->p);
      }

      /* free pbuf: will free pbufs up to es->p (because es->p has a reference count > 0) */
      pbuf_free(ptr);

      /* Update tcp window size to be advertized : should be called when received
      data (with the amount plen) has been processed by the application layer */
      tcp_recved(tpcb, plen);
   }
   else if(wr_err == ERR_MEM)
   {
      /* we are low on memory, try later / harder, defer to poll */
     es->p = ptr;
   }
   else
   {
     /* other problem ?? */
   }
  }
}


static void telnetserver_connection_close(struct tcp_pcb *tpcb, struct telnetserver_struct *es)
{
	 /* remove all callbacks */
	  tcp_arg(tpcb, NULL);
	  tcp_sent(tpcb, NULL);
	  tcp_recv(tpcb, NULL);
	  tcp_err(tpcb, NULL);
	  tcp_poll(tpcb, NULL, 0);

	  /* delete es structure */
	  if (es != NULL)
	  {
	    mem_free(es);
	  }

	  /* close tcp connection */
	  tcp_close(tpcb);
}



