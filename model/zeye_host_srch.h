//
// Created by root on 4/25/16.
//

#ifndef LIBZEYE_ZEYE_HOST_SRCH_H
#define LIBZEYE_ZEYE_HOST_SRCH_H

#include "../base.h"
#include "util.h"
/* Host Search
 * @param  query
 *         page
 *         facet  the same as the ZoomEye API of the Host Search
 * The Only thing that should be NOTICE is that, each param MUST be C-Style String WITHOUT extra ""
 * anywhere!
 * For example:
 * Correct : search(tmp, "port:21", "1", "app,os");
 * False   : search(tmp, "\"port:21\"", "1", "app,os");
 * */
void host_search(user_t restrict user,
        const char * restrict query, const char * restrict page, const char * restrict facet);

/* Web Search
 * @param  query
 *         page
 *         facet  the same as the ZoomEye API of the Host Search
 * The Only thing that should be NOTICE is that, each param MUST be C-Style String WITHOUT extra ""
 * anywhere!
 * For example:
 * Correct : search(tmp, "port:21", "1", "app,os");
 * False   : search(tmp, "\"port:21\"", "1", "app,os");
 * */
void web_search(user_t restrict user,
               const char * restrict query, const char * restrict page, const char * restrict facet);
#endif //LIBZEYE_ZEYE_HOST_SRCH_H
