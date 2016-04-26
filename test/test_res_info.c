//
// Created by root on 4/25/16.
//
#include "test.h"

void test_res_info(user_t user) {
    get_account_info(user);
}

void test_host_search(user_t user) {
    host_search(user, "port:21", "1", "app,os");
}

void test_web_search(user_t user) {
    web_search(user, "port:21", "1", "webapp,os");
}