#pragma once
#include <boost/asio.hpp>
#include <vector>

#include "Singleton.h"

class AsioIOServicePool : public Singleton<AsioIOServicePool> {
  friend Singleton<AsioIOServicePool>;

 public:
  using IOService = boost::asio::io_context;
  using WorkGuard = boost::asio::executor_work_guard<IOService::executor_type>;
  using WorkGuardPtr = std::unique_ptr<WorkGuard>;

  ~AsioIOServicePool();
  AsioIOServicePool(const AsioIOServicePool&) = delete;
  AsioIOServicePool& operator=(const AsioIOServicePool&) = delete;

  // ʹ�� round-robin �ķ�ʽ����һ�� io_service
  boost::asio::io_context& GetIOService();
  void Stop();

 private:
  AsioIOServicePool(std::size_t size = std::thread::hardware_concurrency());
  std::vector<IOService> _ioServices;
  std::vector<WorkGuardPtr> _workGuards;
  std::vector<std::thread> _threads;
  std::size_t _nextIOService;
};