#ifndef _CHAR_MAP_H_
#define _CHAR_MAP_H_

#include "./err_msg.h"
#include <string.h>

#define MAX_SET_NUM  255

struct char_set
{
    size_t          size;
    char            set_data[MAX_SET_NUM];
};

struct map_relation
{
    char        map_src;
    char        map_dst;
};


struct char_map
{
    size_t                  size;
    struct map_relation     map_ral[MAX_SET_NUM];
};

static void
init_char_set(struct char_set *s)
{
    memset(s, 0, sizeof(struct char_set));
}

static int
add_set(char c, struct char_set *s)
{
    if(s == NULL || s->size == MAX_SET_NUM-1)
        return 1;

    s->set_data[s->size] = c;
    s->size += 1;

    return 0;
}

static int
add_mut_set(char begin, char end, struct char_set *s)
{
    char            tmp;
    int             rc;

    if(begin > end)
    {
        pr_msg("[%c]is bigger or equal than[%c]\n", begin, end);
        return 1;
    }

    for(tmp = begin; tmp <= end; ++tmp)
        if((rc = add_set(tmp, s)) != 0)
            return rc;

    return 0;
}

int
fill_set(char *str, struct char_set *s)
{
    size_t          str_size, i;
    const char      SEP_CHAR = '-';
    int             rc;

    if(s == NULL || str == NULL)
        return 1;

    init_char_set(s);
    str_size = strlen(str);
    if(str_size == 0)
        return 1;

    if(str[0] == SEP_CHAR || str[str_size-1] == SEP_CHAR)
    {
        pr_msg("error:first or last char is '-'\n");
        return 1;
    }


    for(i = 0; i < str_size; ++i)
    {
        if(str[i] != SEP_CHAR)
        {
            if((rc = add_set(str[i], s)) != 0)
                return rc;
        }
        else
        {
            if((rc = add_mut_set(str[i-1]+1, str[i+1], s)) != 0)/*   i  > 0  && i < str_size - 1  */
                return rc;
            ++i;
        }
    }

    return 0;
}

void
join_set(struct char_set *set1, struct char_set *set2, struct char_map *c_map)
{
    size_t              i, map_count = set1->size;
    char                map_dst;

    memset(c_map, 0, sizeof(struct char_map));
    c_map->size = map_count;
    for(i = 0; i < map_count; ++i)
    {
        c_map->map_ral[i].map_src = set1->set_data[i];
        if(i >= set2->size)

            map_dst = set2->set_data[set2->size-1];
        else
            map_dst = set2->set_data[i];
        c_map->map_ral[i].map_dst = map_dst;
    }

}

int
fill_map(char *s1, char *s2, struct char_map *c_map)
{
    int                 rc;
    struct char_set     s1_set;
    struct char_set     s2_set;

    if(s1 == NULL || s1[0] == 0 ||
       s2 == NULL || s2[0] == 0 ||
       c_map == NULL)
        return 1;

    if((rc = fill_set(s1, &s1_set)) != 0)
        return rc;

    if((rc = fill_set(s2, &s2_set)) != 0)
        return rc;

    join_set(&s1_set, &s2_set, c_map);

    return 0;
}

#endif

