互斥量mutexundefined
进程间也可以使用互斥锁，来达到同步的目的。但应在pthread_mutex_init初始化之前，
修改其属性为进程间共享。mutex的属性修改函数主要有以下几个。
主要应用函数：
pthread_mutexattr_t mattr 类型：用于定义mutex锁的【属性】
pthread_mutexattr_init函数：初始化一个mutex属性对象
int pthread_mutexattr_init(pthread_mutexattr_t *attr);
pthread_mutexattr_destroy函数：销毁mutex属性对象 (而非销毁锁)
int pthread_mutexattr_destroy(pthread_mutexattr_t *attr);
pthread_mutexattr_setpshared函数：    修改mutex属性。
int pthread_mutexattr_setpshared(pthread_mutexattr_t *attr, int pshared);
参2：pshared取值：
线程锁：PTHREAD_PROCESS_PRIVATE (mutex的默认属性即为线程锁，进程间私有)
进程锁：PTHREAD_PROCESS_SHARED
进程间mutex示例:
进程间使用mutex来实现同步：
                                                                                                                                                                                                        【process_mutex.c】