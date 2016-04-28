//
// Created by root on 4/27/16.
//

#include "zeye_util.h"

const zeye_data get_raw_data(const zeye_obj_t user) {
    return user->raw_data;
}

const json_object * get_parsed_data(const zeye_obj_t user) {
    return json_object_get(user->parse_data);
}

int get_status_code(const zeye_obj_t user) {
    return user->you->last_err;
}
const char * get_status_detail(const zeye_obj_t user) {
    return user->you->err_detail;
}

static void clear(zeye_obj_t user) {
    zeye_user_t zeye_usr = user->you;
    if (zeye_usr->err_detail != NULL) {
        free(zeye_usr->err_detail);
        zeye_usr->err_detail = NULL;
    }
    zeye_usr->last_err = 0;
    if (user->raw_data != NULL) {
        free(user->raw_data);
        user->raw_data = NULL;
    }
    if (user->parse_data != NULL) {
        json_object_put(user->parse_data);
        user->parse_data = NULL;
    }
}

zeye_obj_t login(const char * username, const char * password, boolean consistency) {
    user_t local = make_users(username, password);
    if (unlikely(local == NULL))
        return NULL;
    zeye_user_t usr = malloc(sizeof(struct zeye_user));
    if (unlikely(usr == NULL)) {
        destroy_users(local);
        return NULL;
    }
    usr->comm_usr = local;
    usr->err_detail = NULL;
    usr->last_err = 0;
    zeye_obj_t zeye = malloc(sizeof(struct zeye_obj));
    if (unlikely(zeye == NULL)) {
        destroy_users(local);
        free(usr);
        return NULL;
    }
    zeye->you = usr;
    zeye->parse_data = NULL;
    zeye->raw_data   = NULL;
    return zeye;
}
void logout(zeye_obj_t rel) {
    user_t local_user = rel->you->comm_usr;
    destroy_users(local_user);
    if (rel->you->err_detail != NULL)
        free(rel->you->err_detail);
    free(rel->you);
    if (rel->parse_data != NULL)
        json_object_put(rel->parse_data);
    if (rel->raw_data != NULL)
        free(rel->raw_data);
    free(rel);
}

/*
 * Update the account info from the ZoomEye
 * */
boolean fetch_account_info(zeye_obj_t user) {
    /* empty last information */
    clear(user);
    message tmp = get_account_info(user->you->comm_usr);
    /* HTTP Status Code is larger than 100 */
    if (tmp.error_code < 100) {
        return FALSE;
    }
    user->you->last_err = tmp.error_code;
    /* Fill the detail with C-Style String */
    match_err_detail(tmp.error_code, &(user->you->err_detail));
    /* Raw data */
    user->raw_data   = tmp.err_massage;
    /* json-c data */
    user->parse_data = json_tokener_parse(tmp.err_massage);
    if (200 == tmp.error_code || 201 == tmp.error_code)
        return TRUE;
    else
        return FALSE;
}

boolean explore(zeye_obj_t user, zeye_search_t type,
                const char * query, const char * page, const char * facet)
{
    clear(user);
    message tmp;
    switch (type) {
        case ZEYE_WEB_SEARCH:
            tmp = web_search(user->you->comm_usr, query, page, facet);
            break;
        case ZEYE_HOST_SEARCH:
            tmp = host_search(user->you->comm_usr, query, page, facet);
            break;
        default :
            return FALSE;
    }
    if (tmp.error_code < 100) {
        return FALSE;
    }
    user->you->last_err = tmp.error_code;
    /* Fill the detail with C-Style String */
    match_err_detail(tmp.error_code, &(user->you->err_detail));
    /* Raw data */
    user->raw_data   = tmp.err_massage;
    /* json-c data */
    user->parse_data = json_tokener_parse(tmp.err_massage);
    if (200 == tmp.error_code || 201 == tmp.error_code)
        return TRUE;
    else
        return FALSE;

}