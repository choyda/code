## 这个版本要实现的是多进程客户端
## 加上了错误处理，和常用函数的封装，
注意点：accept，read，write 慢速系统调用的错误返回值  ECONNABORTED ，EINTR。 ctrl+c，会产生这个错误，要重启函数。
还是有就是readn，readnline函数的使用。