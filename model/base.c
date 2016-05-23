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
#if defined(LIBZEYE_DEBUG)
    fprintf(stderr, "Success Connection\n");
#endif
    return client_;
}

/*
 * Parse the access_token from json simply;
 * */
static inline char* find_accecc(char * receive) {
    /* Confirm if it is Okay */
    char head[7] = {0};
    int status_code = -1;
    char detail[32] = {0};
    char * status = strstr(receive, "\r\n");
    *status = '\0';
    sscanf(receive, "%s %d %s", head, &status_code, detail);
    *status = '\r';
    if (status_code >= 300 && status_code < 400)
        return (char *)(SET_ACC_TOKEN_FAIL | URL_REDIR);
    else if (status_code >= 400 && status_code < 500)
        return (char *)(SET_ACC_TOKEN_FAIL | CLIENT_ERROR);
    else if (status_code >= 500 && status_code < 600)
        return (char *)(SET_ACC_TOKEN_FAIL | SERVER_ERROR);
    /* Normal */
    char * p = strstr(receive, "\r\n\r\n");
    char * p1 = strchr(p, ' ');
    char * start = p1 + 2;
    char * p2 = strchr(p1+2, '\"');
    *p2 = '\0';
    return start;
}

static INFO_STATUS set_access_token(user_t users) {
#define CONTENT_LEN 512
#define BUF_LEN 1024
    /* Connect */
    int client = make_connect();
    users->comm_hook = (void *)client;
    char content[CONTENT_LEN] = {0};
    int content_size = snprintf(content, CONTENT_LEN, "{\"username\": \"%s\",\"password\": \"%s\"}",
                                              users->username, users->password);

    char send_buf[BUF_LEN] = {0};
    snprintf(send_buf, BUF_LEN, "%sContent-length: %d\r\nConnection: keep-alive\r\n\r\n%s", post_head, content_size, content);
    send(client, send_buf, strnlen(send_buf, BUF_LEN), 0);
    int err = recv(client, send_buf, BUF_LEN, 0);
    fprintf(stderr, "Receive(%d): \n%s\n", err, send_buf);
    /* Parse */
    char * start = find_accecc(send_buf);
    /* Check For Error */
    if (IS_SET_ERROR((int)(start)))
        return GET_ERROR((int)(start));

#define PREFIX_LEN 4
#define ACCESS_TOKEN_LEN 173
    char * access_token = calloc(PREFIX_LEN + ACCESS_TOKEN_LEN+2, 1);
    if (unlikely(access_token == NULL))
        exit(-3);
    int writen = snprintf(access_token, PREFIX_LEN + ACCESS_TOKEN_LEN+1, "JWT %s", start);
    //fprintf(stderr, "Parse Accesstoken is (%d): %s\n", writen, access_token);
    assert(writen == 176);
    /* Store */
    users->access_token = access_token;
    return SET_ACC_TOKEN_SUCCESS;
#undef PREFIX_LEN
#undef ACCESS_TOKEN_LEN
}

static inline INFO_STATUS set_username(user_t users, const char * name) {
    int len = strlen(name);
    char * local = calloc(len + 1, 1);
    if(unlikely(local == NULL))
        return HEAP_OVERFLOW;
    strncpy(local, name, len);
    local[len] = '\0';
    users->username = local;
    return SET_USR_NAME_SUCCESS;
}

static inline INFO_STATUS set_password(user_t users, const char * pword) {
    int len = strlen(pword);
    char * local = calloc(len + 1, 1);
    if(unlikely(local == NULL))
        return HEAP_OVERFLOW;
    strncpy(local, pword, len);
    local[len] = '\0';
    users->password = local;
    return SET_PASSWD_SUCCESS;
}

user_t make_users(const char * username, const char * password) {
    api_uri = "api.zoomeye.org";
    user_t local = calloc(sizeof (struct user), 1);
    if (unlikely(local == NULL))
        return NULL;
    INFO_STATUS result = -1;
    result = set_username(local, username);
    result = set_password(local, password);
    result = set_access_token(local);
    if (result != SET_ACC_TOKEN_SUCCESS)
        return NULL;
#if defined(LIBZEYE_DEBUG)
    fprintf(stderr, "ACCESS_TOKEN: \n%s\n", local->access_token);
#endif
    return local;
}

void destroy_users(user_t frees) {
    free(frees->access_token);
    free(frees->username);
    free(frees->password);
    close((int)frees->comm_hook);
    free(frees);
}

void re_connect(user_t re) {
    close((int)re->comm_hook);
    free(re->access_token);
    set_access_token(re);
    //fprintf(stderr, "ACCESS_TOKEN: \n%s\n", re->access_token);
}

#if defined(WSX_RELEASE)
boolean change_user(user_t * old_usr, const char * username, const char * password) {
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