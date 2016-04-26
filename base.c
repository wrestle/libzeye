//
// Created by root on 4/24/16.
//
#include <assert.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include "base.h"
static const char * post_head = "POST /user/login HTTP/1.1\r\nHost: api.zoomeye.org\r\n";
/* Connect to the ZoomEye Host */
static int make_connect() {
    int client_ = socket(AF_INET, SOCK_STREAM, 0);
    struct addrinfo easy_to_use;
    struct addrinfo *result;
    struct addrinfo *p;
    memset(&easy_to_use, 0, sizeof(easy_to_use));
    easy_to_use.ai_family = AF_UNSPEC;
    easy_to_use.ai_flags  = AI_PASSIVE;
    easy_to_use.ai_socktype = SOCK_STREAM;
    getaddrinfo("api.zoomeye.org", "http", &easy_to_use, &result);

    for (p = result; p != NULL ; p = p->ai_next) {
        if (connect(client_, p->ai_addr, p->ai_addrlen) == -1)
            continue;
        break;
    }
    if (p == NULL)
        exit(-1);
    freeaddrinfo(result);
    fprintf(stderr, "Success Connection\n");
    return client_;
}

/*
 * Parse the access_token from json simply;
 * */
static inline char* find_accecc(char * receive) {
    char * p = strstr(receive, "\r\n\r\n");
    char * p1 = strchr(p, ' ');
    char * start = p1 + 2;
    char * p2 = strchr(p1+2, '\"');
    *p2 = '\0';
    return start;
}

static void set_access_token(user_t users) {
#define CONTENT_LEN 512
#define BUF_LEN 1024
    /* Connect */
    int client = make_connect();
    users->comm_hook = (void *)client;
    char content[CONTENT_LEN] = {0};
    int content_size = snprintf(content, CONTENT_LEN, "{\"username\": \"%s\",\"password\": \"%s\"}",
                                              users->username, users->password);

    char send_buf[BUF_LEN] = {0};
    snprintf(send_buf, BUF_LEN, "%sContent-length: %d\r\n\r\n%s", post_head, content_size, content);
    send(client, send_buf, strnlen(send_buf, BUF_LEN), 0);
    recv(client, send_buf, BUF_LEN, 0);
    //fprintf(stderr, "Receive: \n%s\n", send_buf);
    /* Parse */
    char * start = find_accecc(send_buf);

#define PREFIX_LEN 4
#define ACCESS_TOKEN_LEN 173
    char * access_token = malloc(PREFIX_LEN + ACCESS_TOKEN_LEN+1);
    if (access_token == NULL)
        ;
    int writen = snprintf(access_token, PREFIX_LEN + ACCESS_TOKEN_LEN+1, "JWT %s", start);
    //fprintf(stderr, "Parse Accesstoken is (%d): %s\n", writen, access_token);
    assert(writen == 176);
    /* Store */
    users->access_token = access_token;
    return;
#undef PREFIX_LEN
#undef ACCESS_TOKEN_LEN
}

static inline int set_username(user_t restrict users, const char * restrict name) {
    int len = strlen(name);
    char * local = malloc(len + 1);
    if(unlikely(local == NULL))
        return -1;
    strncpy(local, name, len);
    local[len] = '\0';
    users->username = local;
}

static inline int set_password(user_t restrict users, const char * restrict pword) {
    int len = strlen(pword);
    char * local = malloc(len + 1);
    if(unlikely(local == NULL))
        return -1;
    strncpy(local, pword, len);
    local[len] = '\0';
    users->password = local;
}

user_t make_users(const char * restrict username, const char * restrict password) {
    api_uri = "api.zoomeye.org";
    user_t local = malloc(sizeof (struct user));
    set_username(local, username);
    set_password(local, password);
    set_access_token(local);
    return local;
}

void destroy_users(user_t frees) {
    free(frees->access_token);
    free(frees->username);
    free(frees->password);
    close((int)frees->comm_hook);
}

#if defined(WSX_RELEASE)
boolean change_user(user_t * restrict old_usr, const char * restrict username, const char * restrict password) {
    user_t local = make_users(username, password);
    if (unlikely(local == NULL)) {
        return FALSE;
    }
    else {
        destroy_users(*old_usr);
        *old_usr = local;
    }
    return TRUE;
}
#endif