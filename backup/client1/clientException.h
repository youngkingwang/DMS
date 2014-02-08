#ifndef CLIENT_EXCEPTION_H
#define CLIENT_EXCEPTION_H

#include<exception>
#include<string>

using namespace std;


class ClientException: public exception
{
public:
	ClientException() throw()
	{
		m_msg = "采集客户端异常";
	}
	ClientException(const string& arg) throw()
		:m_msg(arg)
	{
	}
	virtual ~ClientException()throw(){}

	virtual const char* what() const throw()
	{
		return m_msg.c_str();
	}
private:
	string m_msg;
};

class ReadException: public ClientException
{
public:
	ReadException()
		: ClientException("客户端读取异常")
	{
	}
	ReadException(const string& arg)
		:ClientException(arg)
	{
	}
};

class SendException: public ClientException
{
public:
	SendException()
		: ClientException("客户端发送异常")
	{
	}
	SendException(const string& arg)
		:ClientException(arg)
	{
	}
};

class MatchLogRecException: public ReadException
{
public:
	MatchLogRecException()
		: ReadException("客户端读取匹配记录异常")
	{
	}
	MatchLogRecException(const string& arg)
		:ReadException(arg)
	{
	}
};

class ReadFailLoginException: public ReadException
{
public:
	ReadFailLoginException()
		: ReadException("客户端读取匹配失败登录记录异常")
	{
	}
	ReadFailLoginException(const string& arg)
		:ReadException(arg)
	{
	}
};

class BackupException: public ClientException
{
public:
	BackupException()
		: ClientException("客户端备份日志异常")
	{
	}
	BackupException(const string& arg)
		:ClientException(arg)
	{
	}
};

class SendDataException: public SendException
{
public:
	SendDataException()
		: SendException("客户端发送数据异常")
	{
	}
	SendDataException(const string& arg)
		:SendException(arg)
	{
	}
};

class SocketException: public ClientException
{
public:
	SocketException()
		: ClientException("客户端网络异常")
	{
	}
	SocketException(const string& arg)
		:ClientException(arg)
	{
	}
};



#endif

