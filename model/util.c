//
// Created by root on 4/26/16.
//

#include <unistd.h>
#include <assert.h>
#include "util.h"
/* Not Thread safe */
static int err_code = NORMAL;

int expand_(void ** old, int * old_len) {
    int len = *old_len;
    len <<= 1;
    char * local = malloc(len);
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

boolean requests(int fd, const char * restrict send, int length, char ** restrict get) {
    int size = 0;
    while(length > 0) {
        size = write(fd, send, length);
        length -= size;
    }
    /* Storage */
#define DEFAULT_LEN 2048
    char * reading = NULL;
    reading = malloc(DEFAULT_LEN);
    if (unlikely(reading == NULL))
        return FALSE;
    int r_len = DEFAULT_LEN;
    int r_less = r_len;
    /* Reading Buffer */
    int reads = 0;
    char buffer[DEFAULT_LEN/2] = {0};
    /* T T
     * Using sleep : That is a Trick, Will Be fixed with epoll listen
     * For damn TCP Slice
     * */
    sleep(2);
    while((reads = read(fd, buffer, DEFAULT_LEN/2)) > 0) {
        buffer[reads] = '\0';
        if (r_less < reads && err_code == NORMAL) {
            r_less += expand_(&reading, &r_len);
        }else if (err_code == OVERFLOW){
            err_code = NORMAL;
            free(reading);
            return FALSE;
        }
        strncat(reading, buffer, reads);
        r_less -= reads;
        /* Read until there is nothing */
        if (reads != DEFAULT_LEN/2)
            break;
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

boolean deal_response(char * restrict response , rep_status_t restrict rep) {
    char local_buf[512] = {0};
    const char * start = response;
    int readn = 0;
    int content_len = -1;
    int status_code = -1;
    /* Parse Head/Attribute */
    while((readn = read_line(start, local_buf, 512)) &&
            strncasecmp(local_buf, "\r\n", 2) != 0) {
        if (strncasecmp(local_buf, "HTTP", 4) == 0) {
            sscanf(local_buf+9, "%d", &status_code);
        }
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
