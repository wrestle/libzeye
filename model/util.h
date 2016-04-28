//
// Created by root on 4/26/16.
//

#ifndef LIBZEYE_UTIL_H
#define LIBZEYE_UTIL_H

#include "base.h"
#include <json-c/json.h>
#include <json-c/bits.h>

typedef struct {
    int    error_code;
    char * err_massage;
}message;

typedef enum {
   OVERFLOW = 0,
    NORMAL = 1,
}uecode;

/*
 * Send the HTTP Request
 * And
 * Recv the HTTP Response
 * User who call 'requests' should keep responsibility to the MEMORY Manage
 * which is make into 'get'
 * @param send   Request Page
 *        length Request Page's length
 *        get    Response Page
 * @return FALSE if it meet a Error, or else TRUE
 * */
boolean requests(int fd, const char * restrict send, int length, char ** restrict get);

struct rep_status{
    int status_code;
    int content_length;
};
typedef struct rep_status* rep_status_t;
/*
 * Cut/Parse the Response Page,
 * remain the Content and status_code
 * */
boolean deal_response(char * response , rep_status_t rep);

void match_err_detail(int err_code, char ** result);

json_object* parse_response(const char * json);
void rel_rep_data(json_object* free);
#endif //LIBZEYE_UTIL_H
