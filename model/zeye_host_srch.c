//
// Created by root on 4/25/16.
//

#include "zeye_info.h"
#include "zeye_host_srch.h"

extern const char * api_uri;
static const char * search_in_type[] =
        {
        "/host/search",
        "/web/search",
        };
/*
 * Make a complete query string for make_request
 * */
static inline char *
make_query(const char * query, const char * page, const char * facet)
{
#define query_len (strlen(query)+strlen(page)+strlen(facet)+(22)+1)
    int sizes = query_len;
    char *local = calloc(sizes, 1);
    if (unlikely(local == NULL))
        return NULL;
    int writen = snprintf(local, sizes, "query=\"%s\"&page=%s&facet=%s", query, page, facet);
    //fprintf(stderr, "Query String (%d Bytes): \n%s\n", writen, local);
    return local;
#undef query_len
}

/*
 * Send Request and recv Response
 * */
static char *
make_request(const user_t user,
                          const char * model, const char * query, const char * attribute, /* host_search, query, Authorization */
                          const char * content) {
#define res_len (strlen(model)+strlen(query)+strlen(api_uri)+strlen(attribute)+strlen(content)+42)
    int sizes = res_len;
    char *local = calloc(sizes, 1);
    if (unlikely(local == NULL))
        return NULL;
    /* Create Request Page */
    int writen = snprintf(local, sizes, "GET %s?%s HTTP/1.1\r\nHost: %s\r\nAuthorization: %s\r\n%s",
                                            model, query, api_uri, user->access_token, content);
    if (unlikely(writen < 0)){
        free(local);
        return NULL;
    }
    //fprintf(stderr, "Request Page (%d Bytes): \n%s\n", writen, local);
    char * resource = NULL;
    /* Send and Recv */
    if (unlikely(requests((int)user->comm_hook, local, sizes, &resource) == FALSE)) {
        free(local);
        return NULL;
    }
    //fprintf(stderr, "Receive: \n%s\n", resource);
    return resource;
#undef res_len
}

static inline message search_in(const psearch s) {
    const user_t user  = s->user;
    const char * query = s->query_attr;
    const char * page  = s->page_attr;
    const char * facet = s->facet_attr;
    /* Make Complete query String include all Parameters(query, page, facet) */
    const char * quert_str = make_query(query, page, facet);
    if (unlikely(quert_str == NULL)) {
        char * tmp = calloc(1, 1);
        *tmp = '\0';
        return (message) {.error_code = 0, .err_massage = tmp};
    }
    /* Send Request and recv Response */
    char * response = NULL;
Retry:
    response = make_request(user, search_in_type[s->type], quert_str, user->access_token, "\r\n");
    /* Deal Response */
    if (NULL == response) {
        re_connect(user);
        goto Retry;
    }
    struct rep_status status;
    deal_response(response, &status);
    if (strlen(response) != status.content_length) {
        free(response);
        /* Bad idea but sufficient(at now), would be replace by epoll/select mechanism later */
        goto Retry;
    }
    free(quert_str);
    //fprintf(stderr, "Receive(%d)(%d Byte/%d Byte): \n%s\n",status.status_code,strlen(response),status.content_length,response);

    /* End */
    return (message){.error_code = status.status_code, .err_massage = response};
}

message host_search(user_t user,
                 const char * query, const char * page, const char * facet)
{
    return search_in(&(struct search){.user = user,
            .query_attr = query, .page_attr = page, .facet_attr = facet,
            .type = HOST_SEARCH});

}

message web_search(user_t user,
               const char * query, const char * page, const char * facet)
{
    return search_in(&(struct search){.user = user,
                               .query_attr = query, .page_attr = page, .facet_attr = facet,
                               .type = WEB_SEARCH});
}

