#include "Stack.h"

//初始化
void StackInit(Stack* ps)
{
	ps->_a = (DataType*)malloc(sizeof(DataType)*START_SIZE);
	ps->_capacity = START_SIZE;
	ps->_top = 0;
}

//销毁函数
void StackDestory(Stack* ps)
{
	assert(ps);
	if (ps->_a == NULL)
		return;
	free(ps->_a);
	ps->_a = NULL;
}

//Push函数
void StackPush(Stack* ps, DataType x)
{
	assert(ps);
	//需要扩容
	if (ps->_capacity == ps->_top) 
	{
		ps->_a = realloc(ps->_a, sizeof(DataType)*(ps->_capacity + ADD_SIZE));
		ps->_capacity += ADD_SIZE;
		if (ps->_a == NULL)
		{
			perror("增容出错\n");
			return;
		}
	}
	ps->_a[ps->_top++] = x;
}

//Pop函数
void StackPop(Stack* ps)
{
	assert(ps);
	if (ps->_top == 0)
		return;
	ps->_top--;
}

//返回INT_MAX就表示没有元素了
DataType StackTop(const Stack* ps)
{
	Pos s;
	s._col = -1;
	s._row = -1;
	assert(ps);
	assert(ps);
	if (ps->_top == 0)
		return s;
	return ps->_a[ps->_top-1];
}

//判断空栈，0表示空、1表示非空
int StackEmpty(const Stack* ps)
{
	assert(ps);
	return ps->_top == 0 ? 0 : 1;
}

//获取栈中元素个数
int StackSize(const Stack* ps)
{
	assert(ps);
	return ps->_top;
}