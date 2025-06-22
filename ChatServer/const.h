#pragma once
#include <boost/beast/http.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/system/system_error.hpp>
#include <memory>
#include <iostream>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include <chrono>
#include <functional>
#include <map>
#include <unordered_map>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include <boost/filesystem.hpp>
#include <memory>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <string>
#include <queue>
#include <mutex>
#include <cassert>

#include "Singleton.h"
#include "hiredis.h"


namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;       // from <boost/beast/http.hpp>
namespace net = boost::asio;       // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>

enum ErrorCodes {
	Success = 0,
	Error_Json = 1001,				//Json��������
	RPCFailed = 1002,				//RPC����ʧ��
	VarifyExpired = 1003,			//��֤�����
	VarifyCodeErr = 1004,			//��֤�����
	UserExist = 1005,				//�û��Ѵ���
	PasswdErr = 1006,				//�������
	EmailNotMatch = 1007,			//���䲻ƥ��
	PasswdUpFailed = 1008,			//�������ʧ��
	PasswdInvalid = 1009,			//���벻�Ϸ�
	UidInvalid = 1010,				//�û�ID��Ч
	TokenInvalid = 1011,			//Token��Ч
};

enum MSG_IDS {
	MSG_CHAT_LOGIN = 1005,		// �û���¼
	MSG_CHAT_LOGIN_RSP = 1006,	// �û���¼���ذ�
	ID_SEARCH_USER_RSP = 1008,           // �����û��ذ�
	ID_SEARCH_USER_REQ = 1007,           // �����û�����
	ID_ADD_FRIEND_REQ = 1009,            // ���Ӻ�������
	ID_ADD_FRIEND_RSP = 1010,            // ���Ӻ��ѻظ�
	ID_NOTIFY_ADD_FRIEND_REQ = 1011,     // ֪ͨ���Ӻ�������
	ID_AUTH_FRIEND_REQ = 1013,           // ��֤��������
	ID_AUTH_FRIEND_RSP = 1014,           // ��֤���ѻظ�
	ID_NOTIFY_AUTH_FRIEND_REQ = 1015,    // ֪ͨ��֤��������
	ID_TEXT_CHAT_MSG_REQ = 1017,         // �ı�������Ϣ����
	ID_TEXT_CHAT_MSG_RSP = 1018,         // �ı�������Ϣ�ظ�
	ID_NOTIFY_TEXT_CHAT_MSG_REQ = 1019,  // ֪ͨ�ı�������Ϣ����
	ID_NOTIFY_TEXT_CHAT_MSG_RSP = 1020,  // ֪ͨ�ı�������Ϣ�ظ�
};

#define CODEPREFIX "code_"
#define USERTOKENPREFIX  "utoken_"
#define USERIPREFIX "uip_"
#define IPCOUNTPREFIX "ipcount_"
#define LOGIN_COUNT "logincount"
#define USER_BASE_INFO "ubaseinfo_"
#define NAME_INFO "nameinfo_"

#define MAX_LENGTH 1024*2
#define HEAD_TOTAL_LEN 4		// ͷ���ܳ���
#define HEAD_ID_LEN 2			// ͷ��id����
#define HEAD_DATA_LEN 2			// ͷ�����ݳ���
#define MAX_RECVQUE 10000		// �����ն��г���
#define MAX_SENDQUE 10000		// ����Ͷ��г���

/**
* Defer�࣬���������������ǰ����ָ���ĺ���
*/
class Defer {
public:
	Defer(std::function<void()> func) : func_(func) {}
	~Defer() { func_(); };
private:
	std::function<void()> func_;
};
