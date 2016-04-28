## libzeye

### ZoomEye SDK - C 
- C语言版的**SDK**

### 依赖
- `json-c`

### 实现

- `账户信息获取`
- `Web 搜索`
- `Host 搜索`

### 使用

- 可以参见`main.c`
- 以及`util/zeye_util.h`的注释
- 使用时，只需使用`util/zeye_util.h`即可
- 更详细的可以查看`doc/user.md`

### 如何构建

- 安装好`json-c`库后，修改`CMakelists.txt`中的`LINK_DIRECTORIES()`为自己的路径，或者默认使用这个也可以
- 接着
- `mkdir cmake-build && cd cmake_build`
- `cmake ..`
- `make`
- `./libzeye` 测试

> 当然前提是，需要在`main.c`中写好自己的**用户名和密码**
		