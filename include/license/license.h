// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LICENSEDLL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LICENSE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#include <qglobal.h>

#ifdef Q_OS_WIN
#ifdef LICENSEDLL_EXPORTS
#define LICENSE_API __declspec(dllexport)
#else
#define LICENSE_API __declspec(dllimport)
#endif
#endif
#ifdef Q_OS_LINUX
   #define LICENSE_API
#endif

/*
*  重要说明
   这里所有返回字符串的函数，都有可能在下次调用这里的函数后发生改变，
   请获得字符串后，若还需在再次调用这里的函数之后使用，需要先把获得的字符串做备份
   否则，字符串会改变。
*/

extern "C"
{
/*
* 设置在寻找许可证文件时是否弹出默认的对话框, 默认不弹出
* prompt: 设置0不弹出，1弹出
*/
LICENSE_API void setPromptForFile(int prompt);

/*
* 设置是否检查用户改动日期的操作, 默认不检查
* check: 设置0不检查, 1检查
*/
LICENSE_API void setCheckBadDate(int check);

/*
* 初始化FLEXnet并创建一个新的job
* 必须在使用Flex前第一个调用， 如果没有调用
* 那么在调用checkout和setDefaultPath函数时会自动调用这个函数
* 
* 返回0表示成功， 否则出错， 返回值为错误码， 调用getErrorString获取错误信息
*/
LICENSE_API int newJob();

/*
* 释放与job相关的内存
* 不再需要Flexnet时调用
*/
LICENSE_API void freeJob();

/*
* 对一个feature请求一个license
* featureName:         feature name    最长30字节, 英文字母,数字等符号都可以, 忽略大小写
* licenseVersion:      相应feature的license的版本号, 浮点数格式的字符串
                       如"12345.123", "123.456789", 该值<=license文件中相应
					   的版本号, 默认值为“1.000”
  numberOfLicense:     checkout的license数量, 一般是1，默认值也为1

* 返回0表示成功， 否则出错， 返回值为错误码， 调用getErrorString获取错误信息
*/
LICENSE_API int checkOut(const char* featureName, const char* licenseVersion="1.000", int numberOfLicense=1);

/*
* 释放指定feature的license
* featureName:         feature name    最长30字节, 英文字母,数字等符号都可以

* 返回0表示成功， 否则出错， 返回值为错误码， 调用getErrorString获取错误信息
*/
LICENSE_API int checkIn(const char* featureName);

/*
* 指定license文件的缺省路径或license文件，如 "g:\\temp", "g:\\temp\\u2.lic"
* 如果指定目录，那么会对该目录下所有*.lic文件搜索license
* 也可以指定license服务器的名称， 如3456@cubetan-pc，这里3456是port号， 
* cubetan-pc是在网络上的主机名

* 返回0表示成功， 否则出错， 返回值为错误码， 调用getErrorString获取错误信息
*
* 注意：一般在应用程序中都应该调用该函数设置license文件所在的默认位置
* 通过这个函数设置后，flexnet也会通过环境变量中的LM_LICENSE_FILE与VENDOR_LICENSE_FILE
* 指定的路径进行查找，而且是比这里设置的路径还要优先查找
* 但在环境变量中指定的路径在win7系统中如果不重启则有问题，有时候不认
* 路径中不能含有中文，也就是path只能含有ASCII码
*/
LICENSE_API int setDefaultPath(const char* path);

/*
* 按错误编号获取描述错误的字符串
* status:  错误编号
*
* 返回: 错误信息的字符串, 不需要释放错误字符串所指内存
*/
LICENSE_API const char* getErrorString(int status);

/* 
*  测试经过成功的checkout后的feature是否是demo类型的，
*  必须在checkout后执行，否则出错
*  featureName:  feature name
*
*  返回值:
*  return 1, then feature is a demo hostid
*  return 0, then not a demo hostid
*  return <0, FLEXible API errno, then have errors, get error info by call getErrorString
*/
LICENSE_API int isDemo(const char* featureName);

/* 
*  测试经过成功的checkout后的feature是否是any类型的，
*  必须在checkout后执行，否则出错
*  featureName:  feature name
*
*  返回值:
*  return 1, then feature is a any hostid
*  return 0, then not a any hostid
*  return <0, then have errors, get error info by call getErrorString
*/
LICENSE_API int isAny(const char* featureName);


/* 
*  获取经过成功的checkout后的feature还有多少有效日期，
*  必须在checkout后执行，否则出错
*  featureName:  feature name
*
*  返回值:
*  return >0, number of days until expiration
*  return ==0, the license will expire tonight at midnight
*  return <0, FLEXible API errno
*  return 3650000, Unexpiring license
*/
LICENSE_API int getExpireDays(const char* featureName);

/*
* 在有license服务器的情况下, 获取使用feature的用户列表， 使用前必须调用了newJob
* featureName: feature name
*
*返回值： 在没有license服务器情况下， 返回空字符串
*        在license服务器情况下， 返回使用该feature的用户列表，用户的表达方式是username@host
*        如果有多个用户使用，在各个用户之间使用$字符分割
*/
LICENSE_API const char* getUserList(const char* featureName);

/*
* 获取所有license文件中的feature列表
*
* 返回值: 各个feature之间使用字符$分割的字符串
*/
LICENSE_API const char* getFeatureList();

/*
* 在使用license服务器的情况下, 往日志中写入信息
* msg: 待写入的信息, 需要英文, 最大字符串长度为145
*/
LICENSE_API void logToServer(const char* msg);


/*
* 获取license中feature的信息， 可以在checkout之前使用， 调用该函数之前如果没有调用newJob，那么在函数中会自动调用
* feature： feature name
* type： 信息类型
*		0    VENDOR_STRING="..."
*       1	 NOTICE="..."
*       2    START=dd-mmm-yyyy
*       3    ISSUED=dd-mmm-yyyy
*       4    Expiration Date {dd-mmm-yyyy | permanent} 若是permanent，则会返回1-jan-0,或者1-jan-00, 1-jan-0000
*       5    Conccurency number 获取带License服务器情况下的并发Feature数目，在不带License服务器情况下这个值是0
*       6    VERSIONS="1.0" or "1.0$2.0$3.0"  获取Feature的版本号，如果有多个版本则用$分隔
*       7    TotalInUse 该feature当前被使用的总数
* status： 输出参数
*   ==0  成功获取了信息
*   !=0  获取信息失败，返回值为错误码， 调用getErrorString获取错误信息

* 返回值：
*   获取的信息字符串，不可修改，也不要释放该字符串，字符串是按utf-8编码的，因此转换后可以显示中文字符
*/
LICENSE_API const char* getFeatureInfo(const char* feature, int type, int* status);

/*
* 获取license中feature的信息，可以在checkout之前使用， 调用该函数之前如果没有调用newJob，那么在函数中会自动调用
* feature： feature name
* status： 输出参数
*   ==0  成功获取了信息
*   !=0  获取信息失败，返回值为错误码， 调用getErrorString获取错误信息
* 返回值：
* 一个json格式的字符串， 不可修改，也不要释放该字符串，字符串是按utf-8编码的，因此转换后可以显示中文字符
* 下面是一个返回结果：
{"Name":"test1", "ExpireDays":22, "ExpirationDate":"10-mar-2011", "ISSUED":"", "NOTICE":"A G A IN", "START":"", "VENDOR_STRING":"", "VERSIONS":["1.0", "2.0"], 
 "ConccurencyNumber":2, "UserList":["cubetan@cubetan-PC", "cubetan@cubetan-PC"], "TotalInUse":2 }
 相关项的意义与getFeatureInfo（）函数的一致
*/
LICENSE_API const char* getFeatureJSON(const char* feature, int* status);

/*
* 根据输入的feature name与版本号验证有效性
* feature： feature name
* licenseVersion:      相应feature的license的版本号, 浮点数格式的字符串, 长度不能超过10
* 返回值：
* 若有效返回0，否则是非0， 获取信息失败，返回值为错误码， 调用getErrorString获取错误信息
*/
LICENSE_API int available(const char* feature, const char* licenseVersion);

/*
* 获取所有可以搜索feature的license文件， 包含license服务器名称
* 返回值： 以字符$分割的各个license文件或license服务器名称
*/
LICENSE_API const char* getLicenseFiles();

/* 定义通过heartbeat方式与license server重连的三个回调函数指针类型
*  feature:			feature name
*  pass：			当前尝试的重连次数
*  total_attempts:	最多尝试的重连次数
*  interval:		重连间隔,按秒计数
*  
*  返回值: 返回的整数不会使用
*/
typedef int (*fpReConnect)(const char* feature, int pass, int total_attempts, int interval);
typedef int (*fpReConnectDone)(const char* feature, int tries, int total_attempts, int interval);
typedef void (*fpExitCall)(const char* feature);

/*
* 设置当与license server发生每次重连时的回调函数
* fpReConnect: 用于处理重连时的回调函数指针
* 注意： 这个回调函数会在每次重连时都会调用一次
*/
LICENSE_API void setReConnectCallbackFun(fpReConnect fp);

/*
* 设置当与license server发生重连成功时的回调函数
* fpReConnectDone: 用于处理重连成功时的回调函数指针
*/
LICENSE_API void setReConnectDoneCallbackFun(fpReConnectDone fp);

/*
* 设置当与license server发生重连时的回调函数
* fpExitCall: 用于处理重连达到指定次数，但没成功连接时的回调函数指针
* 注意：当这个回调函数调用后，对任何有关Flexnet API的调用结果都是未定义的，
* 因此，这个回调函数被调用后应该终止应用程序
*/
LICENSE_API void setExitCallCallbackFun(fpExitCall fp);

}
