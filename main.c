#include "test/test.h"

int main(int argc, char* argv[]) {
    user_t tmp = make_users("wsxinchina@gmail.com", "WSX88619973wsx");
    //test_res_info(tmp); /* Pass */
    test_host_search(tmp); /* Pass */
    //test_web_search(tmp); /* Pass */
    destroy_users(tmp);
}