//
// Created by root on 4/25/16.
//

#ifndef LIBZEYE_ZEYE_HOST_SRCH_H
#define LIBZEYE_ZEYE_HOST_SRCH_H

#include "base.h"
#include "util.h"
/* For Search Type */
typedef enum {
    HOST_SEARCH = 0,
    WEB_SEARCH  = 1,
}search_t;

/* Search Param */
struct search {
    user_t user;
    const char * query_attr;
    const char * page_attr;
    const char * facet_attr;
    search_t     type;
};
typedef struct search * psearch;

/* Host Search
 * @param  query
 *         page
 *         facet  the same as the ZoomEye API of the Host Search
 * @return return struct has the status code and response message
 * The Only thing that should be NOTICE is that, each param MUST be C-Style String WITHOUT extra ""
 * anywhere!
 * For example:
 * Correct : search(tmp, "port:21", "1", "app,os");
 * False   : search(tmp, "\"port:21\"", "1", "app,os");
 * */
message host_search(user_t user,
        const char * query, const char * page, const char * facet);

/* Web Search
 * @param  query
 *         page
 *         facet  the same as the ZoomEye API of the Host Search
 * @return return struct has the status code and response message
 * The Only thing that should be NOTICE is that, each param MUST be C-Style String WITHOUT extra ""
 * anywhere!
 * For example:
 * Correct : search(tmp, "port:21", "1", "webapp,os");
 * False   : search(tmp, "\"port:21\"", "1", "webapp,os");
 * */
message web_search(user_t user,
               const char * query, const char * page, const char * facet);
#endif //LIBZEYE_ZEYE_HOST_SRCH_H
