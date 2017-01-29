/*
Will return an exception with any message to help debug problems

Author : Romesh Selvanathan
*/

#ifndef __MYEXCEPTION_H__
#define __MYEXCEPTION_H__

class MyException
{
public:
	MyException(const char* pTxt) : pReason(pTxt) {};
	const char* GetReason(){return pReason;}
private:
	const char* pReason;
};

#endif