//
// Created by root on 4/25/16.
//

#ifndef LIBZEYE_TEST_H
#define LIBZEYE_TEST_H

#include "../model/base.h"
#include "../model/zeye_info.h"
#include "../model/zeye_host_srch.h"
#include "../util/zeye_util.h"
#include <stdlib.h>
void test_res_info(user_t user);
void test_host_search(user_t user);
void test_web_search(user_t user);
void test_fetch(zeye_obj_t);

void test_login(const char * username, const char * passwd);
#endif //LIBZEYE_TEST_H
