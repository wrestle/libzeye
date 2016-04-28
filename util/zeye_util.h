//
// Created by root on 4/27/16.
//

#ifndef LIBZEYE_ZEYE_UTIL_H
#define LIBZEYE_ZEYE_UTIL_H

#include "../model/zeye_info.h"
#include "../model/zeye_host_srch.h"
/* It is about your Message */
struct zeye_user{
    int     last_err;   /* last Request error code */
    char *  err_detail; /* Request Error Detail */
    void *  comm_usr;   /* Something indead */
    boolean consistency; /* Store it to the Disk ? */
};
typedef struct zeye_user * zeye_user_t;

/* It is about your Receive Data */
typedef char * zeye_data;
struct zeye_obj{
    json_object * parse_data;
    zeye_data     raw_data;
    zeye_user_t   you;
};
typedef struct zeye_obj * zeye_obj_t;

typedef enum {
    ZEYE_HOST_SEARCH = 1,
    ZEYE_WEB_SEARCH  = (1 << 1),
}zeye_search_t;

/*
 * You should login before Anything
 * */
zeye_obj_t login(const char * username, const char * password, boolean consistency);
/*
 * You should logout at the end of the Using.
 * */
void logout(zeye_obj_t);

/*
 * Pure data which return from the ZoomEye Server without change.
 *
 * */
const zeye_data get_raw_data(const zeye_obj_t user);
/*
 * Parse data which deal with the json-c
 * WARNING:
 * But You should take the responsibility to the json_object's memory manage.
 * */
const json_object * get_parsed_data(const zeye_obj_t user);


/*
 * Get the HTTP Response Status Code
 * */
int get_status_code(const zeye_obj_t user);
/*
 * Get the HTTP Response Status Code's Detail
 * */
const char * get_status_detail(const zeye_obj_t user);

/*
 * Update(Fetch) the account info from the ZoomEye
 * if success(200,201) it will return TRUE or else
 * if fail(other status) it will return False
 * take the error detail will be helpful
 * */
boolean fetch_account_info(zeye_obj_t user);

/* explore the Internet
 * @param  query
 *         page
 *         facet  the same as the ZoomEye API of the Host Search
 * @return return struct has the status code and response message
 * The Only thing that should be NOTICE is that, each param MUST be C-Style String WITHOUT extra ""
 * anywhere!
 * For web serach example:
 * Correct : search(tmp, "port:21", "1", "webapp,os");
 * False   : search(tmp, "\"port:21\"", "1", "webapp,os");
 * */
boolean explore(zeye_obj_t user, zeye_search_t type,
                const char * query, const char * page, const char * facet);
#endif //LIBZEYE_ZEYE_UTIL_H
