//
// Created by root on 4/25/16.
//

#include <sys/socket.h>
#include "zeye_info.h"
extern const char * api_uri;
static const char * resource_info = "/resources-info";
/*
 * @param model     it means that which type of the ZoomEye Request, will be defined in the head of each file
 *        attribute Authorization of access token
 *        content   if the method is GET, It will be '\r\n'
 * */
const char * make_request(const user_t restrict user,
                          const char * restrict model, const char * restrict attribute,
                          const char * restrict content) {
#define res_len (strlen(resource_info)+strlen(api_uri)+(strlen(attribute))+strlen(content)+40)
    /* For Space Calculation */
    char * request = malloc(res_len);
    if (unlikely(request == NULL))
        return NULL;
    int size = snprintf(request, res_len, "GET %s HTTP/1.1\r\nHost:%s\r\nAuthorization: %s\r\n%s",
                                model, api_uri, attribute, content);
    /* Check For Exception */
    if (unlikely(size < 0)) {
        free(request);
        return NULL;
    }
    fprintf(stderr, "Sent: \n %s \n", request);
    char * user_info = NULL;
    if (unlikely(requests((int)user->comm_hook, request, size, &user_info)==FALSE)) {
        free(request);
        return NULL;
    }
    fprintf(stderr, "Receive:\n %s\n", user_info);
    free(request);
    return user_info;
#undef USER_INFO_LEN
#undef res_len
}

void get_account_info(user_t user) {
    /* make request and recv the response */
    const char * request = make_request(user, resource_info, user->access_token, "\r\n");
    if (unlikely(request == NULL))
        return;
    fprintf(stderr, "%s\n", request);
    /* TODO
     * Deal the Data which receive from ZoomEye
     * */
    /* End */
    free(request);
}