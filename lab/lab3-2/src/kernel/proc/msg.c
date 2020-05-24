#include "msg.h"
void init_msgtab()
{
    int i =  0;
    for(i = 0 ; i < NR_MSG ; ++i )  {msg_buff[i].state =  MSG_FREE; }
    list_init(&msg_list);
}

void send(int pid , struct Message *m)
{
      struct task_struct *  p ;
      p = find_pcb(pid);
      if (p->message->value < 0)
      {
        p->msg->src  = m->src;
        p->msg->dest = m->dest;
        p->msg->type = m->type;
        memcpy(p->msg->payload,m->payload,20);
        V(p->message);
      }
     else
      {
        int i = 0 ;
        for(i=0;i<NR_MSG;i++) {if (msg_buff[i].state==MSG_FREE)  {break;} } 
        memcpy(m->payload,"i am sender",20);  
        msg_buff[i].msg.src  = m->src;
        msg_buff[i].msg.dest = m->dest;
        msg_buff[i].msg.type = m->type;
        memcpy(msg_buff[i].msg.payload,m->payload,20);
        msg_buff[i].state = MSG_USED;
        list_add_before(&msg_list,&msg_buff[i].linklist);
        V(p->message);
     }
}

void receive(int pid , struct Message *m)
{
   struct task_struct * receiver = current;
   if (receiver->message->value > 0)
   {
      P(receiver->message);
      ListHead * q = &msg_list;

      struct MsgBuff tmp;
      tmp.state = - 1 ;
      struct MsgBuff * buff = &tmp;

        // printk("%d...",current->state);
      while (q->next != &msg_list)           
      {  
           buff = list_entry(q->next,struct MsgBuff,linklist);
           if (buff->msg.dest == receiver->pid)   {break;}
           else {q = q->next;}
      } 
      m->src = buff->msg.src;
      m->dest = buff->msg.dest;
      m->type = buff->msg.type;

      memcpy(m->payload,buff->msg.payload,20);
      buff->state = MSG_FREE;
  
      list_del(&buff->linklist);
   }    
   else
    {
       P(receiver->message); 
       m->src = receiver->msg->src;
       m->dest = receiver->msg->dest;
       m->type = receiver->msg->type;
       memcpy(m->payload,receiver->msg->payload,20);  
   }
}
