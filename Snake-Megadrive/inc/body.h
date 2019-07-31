#ifndef _BODY_H_
#define _BODY_H_

#include <genesis.h>

#define up		0
#define down	1
#define left	2
#define right	3


typedef unsigned int        size_t;

typedef struct              s_body_node
{
    u16                     x;
    u16                     y;
    struct s_body_node      *prev;
    struct s_body_node      *next;
}                           t_body_node;

typedef struct              s_body_list 
{
    size_t                  size;
    struct s_body_node      *first;
    struct s_body_node      *last;
}                           t_body_list;


t_body_list                 *init_body();
u8                          push_front_body(t_body_list *list);
void                        free_body(t_body_list *list);
void                        body_follow_head(t_body_list *list, Vect2D_s16 *snakeHead);

#endif // _BODY_H_
