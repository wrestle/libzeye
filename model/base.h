//
// Created by root on 4/24/16.
//

#ifndef LIBZEYE_BASE_H
#define LIBZEYE_BASE_H

#define TRUE 1
#define FALSE 0
#define likely(x) __builtin_expect(!!(x),0)
#define unlikely(x) __builtin_expect(!!(x),1)
const char * api_uri;
#include <malloc.h>
#include <string.h>
struct user {
    char * username;
    char * password;
    char * access_token;
    void * comm_hook;
};
typedef struct user * user_t;
typedef char boolean;

typedef enum {
    /* Base Status */
    SET_ACC_TOKEN_FAIL    = 1,
    SET_USR_NAME_FAIL     = 1,
    SET_PASSWD_FAIL       = 1, /* 32/64 bit Machine will remain the last bit in address */
    SET_ACC_TOKEN_SUCCESS = 2 << 2,
    SET_USR_NAME_SUCCESS  = 2 << 3,
    SET_PASSWD_SUCCESS    = 2 << 4,
    /* Network Status Error Detail */
    SERVER_ERROR = 2 << 5,
    CLIENT_ERROR = 2 << 6,
    URL_REDIR    = 2 << 7,
    /* Local Error Detail */
    HEAP_OVERFLOW = 2 << 8,
}INFO_STATUS;

#if !defined(IS_SET_ERROR)
#define IS_SET_ERROR(err_code) (((err_code) & SET_ACC_TOKEN_FAIL) || ((err_code) & SET_USR_NAME_FAIL) || ((err_code) & SET_PASSWD_FAIL))
#define GET_ERROR(err_code) ((err_code) & SERVER_ERROR) | ((err_code) & CLIENT_ERROR) | ((err_code) & URL_REDIR)
#endif

/* Create a new User, make connect to the ZoomEye */
user_t make_users(const char * username, const char * password);
void destroy_users(user_t frees);
void re_connect(user_t re);

#if defined(WSX_RELEASE)
boolean change_user(user_t * old_usr, const char * username, const char * password);
#endif
#endif //LIBZEYE_BASE_H
