#include "test/test.h"

int main(int argc, char* argv[]) {
    //user_t tmp = make_users("", "");
    //test_res_info(tmp);    /* Pass */
    //test_host_search(tmp); /* Pass */
    //test_web_search(tmp);  /* Pass */
    //destroy_users(tmp);
    boolean status;
    zeye_obj_t me = login("", "", 0);
    if (me == NULL) exit(1);
    status = fetch_account_info(me);
    fprintf(stderr, ">>Response %s\n", get_raw_data(me));
    for (int i = 0; i < 3; ++i) {
        fputs("C3<<<<<<<<<<<<<<<<<<<<<\n", stderr);
        status = explore(me, ZEYE_WEB_SEARCH, "port:21", "1", "webapp,os");
        if (FALSE == status) {
            fprintf(stderr, "(status (%d): %s\n)", get_status_code(me), get_status_detail(me));
        }
        else {
            fprintf(stderr, ">>Response %s\n", get_raw_data(me));
        }
    }
    status = explore(me, ZEYE_HOST_SEARCH, "port:21", "1", "app,os");
    if (FALSE == status) {
        fprintf(stderr, "(status (%d): %s\n)", get_status_code(me), get_status_detail(me));
    }
    else {
        fprintf(stderr, ">>Response %s\n", get_raw_data(me));
    }
    status = fetch_account_info(me);
    fprintf(stderr, ">>Response %s\n", get_raw_data(me));
    logout(me);
}