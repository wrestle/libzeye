//
// Created by root on 4/25/16.
//

#include "zeye_info.h"
extern const char * api_uri;
static const char * host_srch = "/host/search";
static const char * web_srch = "/web/search";

static inline char *
make_query(const char * restrict query, const char * restrict page, const char * restrict facet)
{
#define query_len (strlen(query)+strlen(page)+strlen(facet)+(22)+1)
    int sizes = query_len;
    char *local = malloc(sizes);
    if (unlikely(local == NULL))
        return NULL;
    int writen = snprintf(local, sizes, "query=\"%s\"&page=%s&facet=%s", query, page, facet);
    fprintf(stderr, "Query String (%d Bytes): \n%s\n", writen, local);
    return local;
#undef query_len
}
static const char * make_request(const user_t restrict user,
                          const char * restrict model, const char * restrict query, const char * restrict attribute, /* host_search, query, Authorization */
                          const char * restrict content) {
#define res_len (strlen(model)+strlen(query)+strlen(api_uri)+strlen(attribute)+strlen(content)+42)
    int sizes = res_len;
    char *local = malloc(sizes);
    if (unlikely(local == NULL))
        return NULL;
    int writen = snprintf(local, sizes, "GET %s?%s HTTP/1.1\r\nHost: %s\r\nAuthorization: %s\r\n%s",
                                            model, query, api_uri, user->access_token, content);
    if (unlikely(writen < 0)){
        free(local);
        return NULL;
    }
    fprintf(stderr, "Request Page (%d Bytes): \n%s\n", writen, local);
    char * resource = NULL;
    if (unlikely(requests((int)user->comm_hook, local, sizes, &resource) == FALSE)) {
        free(local);
        return NULL;
    }
    return resource;
}

void host_search(user_t restrict user,
                 const char * restrict query, const char * restrict page, const char * restrict facet){
    /* Make Complete query String include all Parameters(query, page, facet) */
    const char * quert_str = make_query(query, page, facet);
    if (unlikely(quert_str == NULL))
        return;
    /* Send Request and recv Response */
    char * response = NULL;
Retry:
    response = make_request(user, host_srch, quert_str, user->access_token, "\r\n");
    /* Deal Response */
    struct rep_status status;
    deal_response(response, &status);
    if (strlen(response) != status.content_length)
        goto Retry;
    fprintf(stderr, "Receive(%d Byte/%d Byte): \n%s\n",strlen(response),status.content_length,response);
    /* TODO
     * Deal the json data
     * */
    /* End */
    free(response);
    return;
}

void web_search(user_t restrict user,
               const char * restrict query, const char * restrict page, const char * restrict facet) {
    /* Make Complete query String include all Parameters(query, page, facet) */
    const char * quert_str = make_query(query, page, facet);
    if (unlikely(quert_str == NULL))
        return;
    /* Send Request and recv Response */
    char * response;
Retry:
    response = make_request(user, web_srch, quert_str, user->access_token, "\r\n");
    struct rep_status status;
    deal_response(response, &status);
    if (strlen(response) != status.content_length)
        goto Retry;
    fprintf(stderr, "Receive(%d Byte/%d Byte): \n%s\n",strlen(response),status.content_length,response);
    /* TODO
     * Deal the json data
     * */
    /* End */
    free(response);
    return;
}