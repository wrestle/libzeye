## libzeye

### ZoomEye SDK - C 
- C语言版的**SDK**

### 可选择依赖

- `json-c`
	- 默认是不开启的

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
- 如果需要使用`json-c`库来解析，请修改`CMakeLists.txt`文件，在`CMAKE_C_FLAGS`的值中添加`-DJSON_DEPENDENCY` 即可
	- 安装好`json-c`库后，在`CMakelists.txt`中的`LINK_DIRECTORIES()`为自己的路径，或者默认使用这个也可以
- 接着
- `mkdir cmake-build && cd cmake_build`
- `cmake ..`
- `make`
- `./zoomeye` 测试

> 默认是生成动态链接库，如果想要使用 静态链接库，可以在CMakeLists.txt中修改，我已经写好参数，只需要取消注释并且将动态库的生成注释掉即可

> 当然前提是，需要在`main.c`中写好自己的**用户名和密码**
		