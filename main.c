#include "test/test.h"

int main(int argc, char* argv[]) {
    //user_t tmp = make_users("", "");
    //test_res_info(tmp); /* Pass */
    //test_host_search(tmp); /* Pass */
    //test_web_search(tmp); /* Pass */
    //destroy_users(tmp);
    boolean status;
    zeye_obj_t me = login("username@email.com", "password", 0);
    fputs("C1<<<<<<<<<<<<<<<<<<<<<\n", stderr);
    status = fetch_account_info(me);
    if (FALSE == status) {
        fprintf(stderr, "(status (%d): %s\n)", me->you->last_err, me->you->err_detail);
    }
    else {
        fprintf(stderr, ">>Response %s\n", get_raw_data(me));
    }
    fputs("C2<<<<<<<<<<<<<<<<<<<<<\n", stderr);
    status = explore(me, ZEYE_HOST_SEARCH, "port:21", "1", "app,os");
    if (FALSE == status) {
        fprintf(stderr, "(status (%d): %s\n)", me->you->last_err, me->you->err_detail);
    }
    else {
        fprintf(stderr, ">>Response %s\n", get_raw_data(me));
    }

    fputs("C3<<<<<<<<<<<<<<<<<<<<<\n", stderr);
    status = explore(me, ZEYE_WEB_SEARCH, "port:21", "1", "webapp,os");
    if (FALSE == status) {
        fprintf(stderr, "(status (%d): %s\n)", me->you->last_err, me->you->err_detail);
    }
    else {
        fprintf(stderr, ">>Response %s\n", get_raw_data(me));
    }
    logout(me);
}