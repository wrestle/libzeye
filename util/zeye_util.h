//
// Created by root on 4/27/16.
//
// http://yqall02.baidupcs.com/file/384834062dc35867dbc1569ab06150ec?bkt=p3-00009781f1df78bc5c719356ba3c5f1add23&fid=873144231-250528-201776335713664&time=1476778854&sign=FDTAXGERLBH-DCb740ccc5511e5e8fedcff06b081203-Eite%2FZsR%2BGjvTi9qEdjq3DeK3MM%3D&to=yqhb&fm=Yan,B,U,nc&sta_dx=15825842&sta_cs=&sta_ft=gz&sta_ct=0&sta_mt=0&fm2=Yangquan,B,U,nc&newver=1&newfm=1&secfm=1&flow_ver=3&pkey=00009781f1df78bc5c719356ba3c5f1add23&sl=75956300&expires=8h&rt=sh&r=808906943&mlogid=6764910327606760817&vuk=873144231&vbdid=3749863503&fin=dpdk-16.07.tar.gz&fn=dpdk-16.07.tar.gz&slt=pm&uta=0&rtype=1&iv=0&isw=0&dp-logid=6764910327606760817&dp-callid=0.1.1&csl=600&csign=1xO%2BdO2iv2ZNIa6G28bYTdsJ8Hs%3D
#ifndef LIBZEYE_ZEYE_UTIL_H
#define LIBZEYE_ZEYE_UTIL_H

#include "../model/zeye_info.h"
#include "../model/zeye_host_srch.h"
/* It is about your Message */
struct zeye_user{
    int     last_err;   /* last Request error code */
    char *  err_detail; /* Request Error Detail */
    void *  comm_usr;   /* Something indead */
    boolean consistency; /* Store it(Account Info: access token) to the Disk ? */
};
typedef struct zeye_user * zeye_user_t;

/* It is about your Receive Data */
typedef char * zeye_data;
struct zeye_obj{
#if defined(JSON_DEPENDENCY)
    json_object * parse_data;
#endif
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
 * @return if it return NULL, It means that your account is in error
 *          check for Correct
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
#if defined(JSON_DEPENDENCY)
/*
 * Parse data which deal with the json-c
 * WARNING:
 * But You should take the responsibility to the json_object's memory manage.
 * */
const json_object * get_parsed_data(const zeye_obj_t user);
#endif

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
