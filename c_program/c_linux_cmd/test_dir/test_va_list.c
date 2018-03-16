#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#define INT_TYPE  100000
#define STR_TYPE  100001
const int CHAR_TYPE = 100002;
const int LONG_TYPE = 100003;
const int FLOAT_TYPE = 100004;
const int DOUBLE_TYPE = 100005;

//测试va_start,va_arg的使用方法,函数参数在堆栈中的地址分布情况
void arg_test(int i, ...);
//第一个参数定义可选参数个数,用于循环取初参数内容
void arg_cnt(int cnt, ...);
//第一个参数定义可选参数个数,用于循环取初参数内容
//可变参数采用arg_type,arg_value...的形式传递,以处理不同的可变参数类型
void arg_type(int cnt, ...);

int main(int argc,char *argv[])
{
	int int_size = sizeof(int);
	printf("int_size=%d\n", int_size);								//int_size=4
	arg_test(0, 4);
	arg_cnt(4,1,2,3,4);
	arg_type(2, INT_TYPE, 222, STR_TYPE, "ok,hello world!");

	return 0;
}

//arg_test(0, 4);
void arg_test(int i, ...)
{
	int j=0;
	va_list arg_ptr;
	
	va_start(arg_ptr, i);
	//打印参数i在堆栈中的地址
	printf("&i = %p\n", &i);										//&i=0012FE88
	//打印va_start之后arg_ptr地址,
	//应该比参数i的地址高sizeof(int)个字节
	//这时arg_ptr指向下一个参数的地址
	printf("arg_ptr = %p\n", arg_ptr);								//arg_ptr=0012FE8C
	
	//打印va_arg后arg_ptr的地址
	//应该比调用va_arg前高sizeof(int)个字节
	//这时arg_ptr指向下一个参数的地址
	j=*((int *)arg_ptr);
	printf("%d %d\n", i, j);										//0 4
	j=va_arg(arg_ptr, int);
	printf("arg_ptr = %p\n", arg_ptr);								//arg_ptr=0012FE90
	
	va_end(arg_ptr);
	printf("%d %d\n", i, j);										//0 4
}

//arg_cnt(4,1,2,3,4);
void arg_cnt(int cnt, ...)
{
	int value=0;
	int i=0;
	int arg_cnt=cnt;
	va_list arg_ptr;
	va_start(arg_ptr, cnt);
	for(i = 0; i < cnt; i++)
	{
		value = va_arg(arg_ptr,int);
		printf("value%d=%d\n", i+1, value);							//value1=1
		//value2=2
		//value3=3
		//value4=4
	}
}

//arg_type(2, INT_TYPE, 222, STR_TYPE, "ok,hello world!");
void arg_type(int cnt, ...)
{
	int arg_type=0;
	int int_value=0;
	int i = 0;
	int arg_cnt=cnt;
	char *str_value = NULL;
	va_list arg_ptr;
	va_start(arg_ptr, cnt);
	for(i = 0; i < cnt; i++)
	{
		arg_type = va_arg(arg_ptr,int);
		switch(arg_type)
		{
		case INT_TYPE:
			int_value = va_arg(arg_ptr,int);
			printf("value%d=%d\n", i+1, int_value);					//value1=222
			break;
		case STR_TYPE:
			str_value = va_arg(arg_ptr,char*);
			printf("value%d=%s\n", i+1, str_value);					//value2=4282180
			break;
		default:
			break;
		}
	}
}