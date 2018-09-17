#include "Stack.h"

//��ʼ��
void StackInit(Stack* ps)
{
	ps->_a = (IDataType*)malloc(sizeof(IDataType)*START_SIZE);
	ps->_capacity = START_SIZE;
	ps->_top = 0;
}

//���ٺ���
void StackDestory(Stack* ps)
{
	assert(ps);
	if (ps->_a == NULL)
		return;
	free(ps->_a);
	ps->_a = NULL;
}

//Push����
void StackPush(Stack* ps, IDataType x)
{
	assert(ps);
	//��Ҫ����
	if (ps->_capacity == ps->_top) 
	{
		ps->_a = realloc(ps->_a, sizeof(IDataType)*(ps->_capacity + ADD_SIZE));
		ps->_capacity += ADD_SIZE;
		assert(ps->_a);
	}
	ps->_a[ps->_top++] = x;
}

//Pop����
void StackPop(Stack* ps)
{
	assert(ps);
	if (ps->_top == 0)
		return;
	ps->_top--;
}

//����INT_MAX�ͱ�ʾû��Ԫ����
IDataType StackTop(const Stack* ps)
{
	assert(ps);
	assert(ps->_top != 0);
	if (ps->_top == 0)
		return INT_MAX;
	return ps->_a[ps->_top-1];
}

//�жϿ�ջ��0��ʾ�ա�1��ʾ�ǿ�
int StackEmpty(const Stack* ps)
{
	assert(ps);
	return ps->_top == 0 ? 0 : 1;
}

//��ȡջ��Ԫ�ظ���
int StackSize(const Stack* ps)
{
	assert(ps);
	return ps->_top;
}