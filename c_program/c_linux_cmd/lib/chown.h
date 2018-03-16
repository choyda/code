#ifndef _CHOWN_H_
#define _CHOWN_H_

#include <sys/types.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <grp.h>
#include <pwd.h>
#include <string.h>

/*
    �ı��ļ����û�ID����ID

    ����ֵ
        ==  0   �ɹ�
        >   0   ʧ��(errno)
        <   0   �û�����
*/

int
_chown(const char *pfile, uid_t owner, gid_t group)
{
    int             save_errno = errno;
    int             ret = 0;

    if(pfile == NULL || pfile[0] == 0)
        return -1;

    if(chown(pfile, owner, group) != 0)
        ret = errno;

    errno = save_errno;

    return ret;
}

/*
    �õ����û���ID

    ����ֵ
        ==  0   �ɹ�
        >   0   ʧ��(errno)
        <   0   �û�����
*/
int
_getgrnam(const char *group_name, gid_t *gid)
{
    struct group    *gro;
    int             save_errno;
    int             ret = 0;

    if(group_name == NULL || group_name[0] == 0)
        return -1;

    if(gid == NULL)
        return -2;

    save_errno = errno;
    if((gro = getgrnam(group_name)) == NULL)
    {
        if(errno == 0)
        {
            fprintf(stderr,
                    "not existed [%s] group name\n",
                    group_name);
            ret = -3;
        }
        else
            ret = errno;
    }
    else
    {
        *gid = gro->gr_gid;
    }

    errno = save_errno;

    return ret;
}

/*
    �õ����û�ID

    ����ֵ
        ==  0   �ɹ�
        >   0   ʧ��(errno)
        <   0   �û�����
*/
int
_getpwnam(const char *user_name, uid_t *uid)
{
    struct passwd    *pa;
    int             save_errno;
    int             ret = 0;

    if(user_name == NULL || user_name[0] == 0)
        return -1;

    if(uid == NULL)
        return -2;

    save_errno = errno;
    if((pa = getpwnam(user_name)) == NULL)
    {
        if(errno == 0)
        {
            fprintf(stderr,
                    "not existed [%s] user name\n",
                    user_name);
            ret = -3;
        }
        else
            ret = errno;
    }
    else
    {
        *uid = pa->pw_uid;
    }

    errno = save_errno;

    return ret;
}

/*
    �ı��û���
*/
int
_chgrp(const char *file, const char *group_name)
{
    int                 err;
    gid_t               gid;

    if((err = (_getgrnam(group_name, &gid))) != 0)
        return err;

    if((err = _chown(file, -1, gid)) != 0)
        return err;

    return 0;
}

/*
    �ı��û�
*/
int
_chusr(const char *file, const char *user_name)
{
    int                 err;
    uid_t               uid;

    if((err = (_getpwnam(user_name, &uid))) != 0)
        return err;

    if((err = _chown(file, uid, -1)) != 0)
        return err;

    return 0;
}

/*
    �ı��û����û���
*/
int
_chusr2grp(const char *file,
           const char *user_name           /*  �������û�����:�û������� */
          )
{
    const char      sep = ':';
    char            usrname[256] = {0};
    char            grpname[256] = {0};
    int             i, len = strlen(user_name);
    int             err = 0;

    if(len + 1 > sizeof(usrname))
        return -1;

    for(i = 0; i < len; ++i)
    {
        if(user_name[i] == sep)
        {
            strncpy(usrname, user_name, i);
            strncpy(grpname, user_name+i+1, sizeof(grpname) - 1);
            break;
        }
    }

    /* ֻ���û��� */
    if(i == len)
        strncpy(usrname, user_name, sizeof(usrname) - 1);

    if(usrname[0] != 0)
        err |= _chusr(file, usrname);
    if(grpname[0] != 0)
        err |= _chgrp(file, grpname);

    return err;
}

#endif

