#pragma once
#include <grpcpp/grpcpp.h>

#include <mutex>

#include "data.h"
#include "message.grpc.pb.h"
#include "message.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
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

class ChatServiceImpl : public ChatService::Service {
 public:
  ChatServiceImpl();
  Status NotifyAddFriend(ServerContext* context, const AddFriendReq* request,
                         AddFriendRsp* reply) override;

  Status NotifyAuthFriend(ServerContext* context, const AuthFriendReq* request,
                          AuthFriendRsp* reply) override;

  Status NotifyTextChatMsg(ServerContext* context,
                           const TextChatMsgReq* request,
                           TextChatMsgRsp* reply) override;

 private:
};
