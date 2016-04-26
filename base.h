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

/* Create a new User, make connect to the ZoomEye */
user_t make_users(const char * restrict username, const char * restrict password);
void destroy_users(user_t frees);
#if defined(WSX_RELEASE)
boolean change_user(user_t * restrict old_usr, const char * restrict username, const char * restrict password);
#endif
#endif //LIBZEYE_BASE_H
