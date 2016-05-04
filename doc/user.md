## libzeye 使用指南

1. 使用前需要登录

		zeye_obj_t login(const char * username, const char * password, boolean consistency);

	返回值是一个`zeye_obj_t`，每一个操作都需要传入这个类型的参数，`login`之后需要检查其是否为`NULL`，如果是则要查看自己的账号密码是否正确。

2. 使用完毕后需要退出

		void logout(zeye_obj_t);

3. 主要功能只有两个接口

		boolean fetch_account_info(zeye_obj_t user);
		boolean explore(zeye_obj_t user, zeye_search_t type,
                const char * query, const char * page, const char * facet);

4. 主要数据结构为
	1. `zeye_obj_t`: 控制所有的操作，信息存储。包含一个**用户信息**(`you`)，一个本次获取的原始数据`raw_data`，本次获取的原始数据经过处理后(`json-c`)的二次数据(`parse_data`)
	2. `zeye_user_t`: 存放着本次的`HTTP-Status code` 和 `原因`，以及一个内部操作数据结构`comm_usr`,`consistency`未开放，为持久化的功能。

5. 完整的时候，在`main.c`中
