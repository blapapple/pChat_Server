#pragma once
#include <grpcpp/grpcpp.h>
#include <json/json.h>
#include <json/reader.h>
#include <json/value.h>

#include <queue>

#include "ConfigMgr.h"
#include "Singleton.h"
#include "const.h"
#include "data.h"
#include "message.grpc.pb.h"
#include "message.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using message::AddFriendReq;
using message::AddFriendRsp;

using message::ChatService;
using message::GetChatServerRsp;
using message::LoginReq;
using message::LoginRsp;

using message::AuthFriendReq;
using message::AuthFriendRsp;

using message::TextChatData;
using message::TextChatMsgReq;
using message::TextChatMsgRsp;

class ChatConPool {
 public:
  ChatConPool(size_t poolSize, std::string host, std::string port);
  ~ChatConPool();
  void Close();
  std::unique_ptr<ChatService::Stub> getConnection();
  void returnConnection(std::unique_ptr<ChatService::Stub> context);

 private:
  size_t poolSize_;
  size_t capacity;
  std::atomic<bool> b_stop;
  std::string host_;
  std::string port_;
  // ��֤����Ψһ�ԣ�ʹ��unique����
  std::queue<std::unique_ptr<ChatService::Stub>> connections_;
  std::mutex mutex_;
  std::condition_variable cond_;
};

class ChatGrpcClient : public Singleton<ChatGrpcClient> {
  friend class Singleton<ChatGrpcClient>;

 public:
  ~ChatGrpcClient() {};

  AddFriendRsp NotifyAddFriend(std::string server_ip, const AddFriendReq& req);
  AuthFriendRsp NotifyAuthFriend(std::string server_ip,
                                 const AuthFriendReq& req);
  TextChatMsgRsp NotifyTextChatMsg(std::string server_ip,
                                   const TextChatMsgReq& req,
                                   const Json::Value& rtvalue);

 private:
  ChatGrpcClient();
  std::unordered_map<std::string, std::unique_ptr<ChatConPool>>
      _pools;  // һ��ip��Ӧһ�����ӳأ������ӳ��л�ȡ���������������
};
