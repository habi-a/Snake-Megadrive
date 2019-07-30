#include "body.h"

t_body_list *init_body()
{
    t_body_list *list;

    list = (t_body_list*)MEM_alloc(sizeof(t_body_list));
    list->first = NULL;
    list->last = NULL;
    list->size = 0;
    return (list);
}

void push_front_body(t_body_list *list)
{
    t_body_node* new_node;

    new_node = (t_body_node*)MEM_alloc(sizeof(t_body_node));
    new_node->next = list->first;
    new_node->prev = NULL;
    if (list->first) 
        list->first->prev = new_node;
    else 
        list->last = new_node;
    list->first = new_node;
    list->size++;
}

void body_follow_head(t_body_list *list, Vect2D_s16 *snakeHeadLastPosition)
{
    t_body_node *current;
    
    current = list->first;
    while (current != NULL)
    {
        if (current->next != NULL) {
            current->x = current->next->x;
            current->y = current->next->y;
        } else {
            current->x = snakeHeadLastPosition->x;
            current->y = snakeHeadLastPosition->y;
        }
        current = current->next;
    }
}

void free_body(t_body_list *list)
{
    t_body_node *next;
    t_body_node *current = list->first;

    while (current != NULL)
    {
        next = current->next;
        MEM_free(current);
        current = next;
    }
    MEM_free(list);
}
