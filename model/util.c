//
// Created by root on 4/26/16.
//

#include <unistd.h>
#include <assert.h>
#include "util.h"
#include <errno.h>
/* Not Thread safe */
static int err_code = NORMAL;
static const char * err_message[] = {
        "400-Request Error! Please Check the API Document",   // 400
        "401-Request Unauthorized! Invalid Access token or Wrong token",   // 401
        "402-Resource has limited! Credits insufficient",      // 402
        "403-Request Forbidden! You have no permittion to it", // 403
        "404-Request Fail! Resource has nothing it Server(404 Not Found)", // 404
        "405-Request Fail! Request method not allowed", // 405
        "422-Reqiest Fail! Request param is invalid",   // 422
        /* Server Error */
        "500-Server Internal Error", // 500
        "503-Service Unavailable",   // 503
        /* Success */
        "200-Request Success", // 200
        "201-Create Success",  // 201
};

int expand_(void ** old, int * old_len) {
    int len = *old_len;
    len <<= 1;
    char * local = calloc(len,1);
    if (unlikely(local == NULL)) {
        err_code = OVERFLOW;
        return 0;
    }
    memcpy(local, *old, *old_len);
    free(*old);
    int add = len - *old_len;
    *old = local;
    *old_len = len;
    return add;
}

boolean requests(int fd, const char * send, int length, char ** get) {
    int size = 0;
    while(length > 0) {
        size = write(fd, send, length);
#if defined(LIBZEYE_DEBUG)
        fprintf(stderr, "write %d size\n", size);
#endif
        length -= size;
    }
    /* Storage */
#define DEFAULT_LEN 2048
    char * reading = NULL;
    reading = calloc(DEFAULT_LEN, 1);
    if (unlikely(reading == NULL))
        exit(-1);
    int r_len = DEFAULT_LEN;
    int r_less = r_len;
    /* Reading Buffer */
    int reads = 0;
    char buffer[DEFAULT_LEN/2+1] = {0};
    /* T T
     * Using sleep : That is a Trick
     * For damn TCP Slice
     * */
    sleep(2);
    while((reads = read(fd, buffer, DEFAULT_LEN/2)) > 0) {
        //fprintf(stderr, "Read %d size\n", reads);
        buffer[reads] = '\0';
        if (r_less < reads && err_code == NORMAL) {
            r_less += expand_(&reading, &r_len);
            if (unlikely(r_less < reads))
                exit(-2);
        }else if (err_code == OVERFLOW){
            err_code = NORMAL;
            free(reading);
            return FALSE;
        }
        strcat(reading, buffer/*, reads*/);
        r_less -= reads;
        /* Read until there is nothing */
        if (reads != DEFAULT_LEN/2)
            break;
    }
    if (reads < 0) {
        fprintf(stderr, "Error occur! Read %d \n", reads);
        if (EPIPE == errno)
            fputs("EPIPE!\n",stderr);
        if (EAGAIN == errno)
            fputs("EAGAIN\n", stderr);
        free(reading);
        return FALSE;
    }
    else if (reads == 0) {
        //fprintf(stderr, "Read Nothing\n");
        free(reading);
        return FALSE;
    }
    *get = reading;
    return TRUE;
}

int read_line(const char * stream, char * buf, int buf_len) {
    char * line_end = strstr(stream, "\r\n");
    /* line_end should never be NULL */
    int len = line_end-stream+2;
    assert(buf_len > len);
    memcpy(buf, stream, len);
    return len;
}

boolean deal_response(char * response , rep_status_t rep) {
    char local_buf[512] = {0};
    const char * start = response;
    if (response == NULL) exit(-2);
    int readn = 0;
    int content_len = -1;
    int status_code = -1;
    /* Parse Head/Attribute */
    char *head = strstr(start, "HTTP/1.1");
    sscanf(head+9, "%d", &status_code);
    while((readn = read_line(start, local_buf, 512)) &&
            strncasecmp(local_buf, "\r\n", 2) != 0) {
        if (strncasecmp(local_buf, "Content-Length", 14) == 0) {
            char * end = strstr(local_buf, "\r\n");
            *end = '\0';
            sscanf(local_buf+16, "%d", &content_len);
            *end = '\r';
        }
        start += readn;
    }
    assert(content_len != -1);
    rep->content_length = content_len;
    rep->status_code    = status_code;
    char * content = strstr(response, "\r\n\r\n");
    if (content == NULL)
        return FALSE;
    content += 4;
    /* Cut the Head/Attribute */
    memmove(response, content, content_len);
    response[content_len] = '\0';
    return TRUE;
}

void match_err_detail(int err_code, char ** result) {
    int len = 0;
    const char * local = NULL;
    switch (err_code) {
        case 400 :
            len = strlen(err_message[0]);
            local = err_message[0];
            break;
        case 401 :
            len = strlen(err_message[1]);
            local = err_message[1];
            break;
        case 402 :
            len = strlen(err_message[2]);
            local = err_message[2];
            break;
        case 403 :
            len = strlen(err_message[3]);
            local = err_message[3];
            break;
        case 404 :
            len = strlen(err_message[4]);
            local = err_message[4];
            break;
        case 405 :
            len = strlen(err_message[5]);
            local = err_message[5];
            break;
        case 422 :
            len = strlen(err_message[6]);
            local = err_message[6];
            break;
        case 500 :
            len = strlen(err_message[7]);
            local = err_message[7];
            break;
        case 503 :
            len = strlen(err_message[8]);
            local = err_message[8];
            break;
        case 200 :
            len = strlen(err_message[9]);
            local = err_message[9];
            break;
        case 201 :
            len = strlen(err_message[10]);
            local = err_message[10];
            break;
        default:
            local = "Unknown Error Occur!";
            len = strlen(local);
            break;
    }
    *result = calloc(len, 1);
    if (unlikely(*result == NULL))
        return;
    strncpy(*result, local, len);
    return;
}

json_object* parse_response(const char * json) {
    json_object * parse = json_tokener_parse(json);
    if (unlikely(is_error(parse)))
        return NULL;
    return parse;
}

void rel_rep_data(json_object* free) {
    json_object_put(free);
}