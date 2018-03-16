#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit */
#include <getopt.h>

int
my_test(int argc, char *argv[])
{
    int             c;
    /*��ѡ��--(��һ��:���֣��ڶ���:�Ƿ�������� ������:��0��������:��ѡ����)*/
    /*���ص��ĸ���(�����������ΪNULL,��Ȼ�ͷ���0)*/
    static struct option long_options[] =
    {
        {"add", 1, 0, 0},
        {"delete", 0, 0, 0},
        {"create", 1, 0, 'c'},
        {0, 0, 0, 0}
    };

    /*�̲�������-*/
    static const char short_options[] = "s:m:t";


    while(1)
    {

        int option_index = 0;
        c = getopt_long(argc, argv,
                        short_options,long_options,
                        &option_index);
        if(c == -1)
            break;

        switch(c)
        {
            case 0:/*���long_optionsԪ���еĵ�����ΪNULL���򷵻����ĵ��ĸ�Ԫ��*/
                printf ("option %s ", long_options[option_index].name);
                if (optarg)
                    printf (" with arg '%s'", optarg);
                printf ("\n");
                break;

            case 'c':
                printf ("option2 %s ", long_options[option_index].name);
                if (optarg)
                    printf (" with arg %s", optarg);
                printf ("\n");
                break;

            case 's':
                printf ("option s with value '%s'\n", optarg);
                break;
            case 'm':
                printf ("option m with value '%s'\n", optarg);
                break;
            case 't':
                printf ("option t \n");
                break;

            case '?':
                break;
            default:
                printf ("?? getopt returned character code %d \n", c);
        }
    }
    /*У������Ƿ���û�����Ĳ���*/
    if (optind < argc)
    {
        printf ("non-option ARGV-elements: ");
        while (optind < argc)
            printf ("%s ", argv[optind++]);
        printf ("\n");
    }
    return 0;
}

int
main(int argc, char **argv)
{
    return my_test(argc, argv);
}
