#ifndef WEBSERVER_SRC_BASE_THREAD_H
#define WEBSERVER_SRC_BASE_THREAD_H

#include "noncopyable.h"
#include <atomic>
#include <functional>
#include <pthread.h>
#include <string>

namespace web {
using Functor = std::function<void()>;

class Thread : public noncopyable {
 public:
  Thread(Functor threadFunc, const std::string &name = std::string());
  ~Thread();

  std::string getName() const { return name_; }
  pthread_t getThreadId() const { return threadId_; }
  Functor getThreadFunc() const { return threadFunc_; }

  void start();
  void join();

  static pid_t gettid();

 private:
  // 线程状态
  bool joined_;
  bool started_;

  // 线程私有成员
  pthread_t threadId_;
  Functor threadFunc_; // 线程调用函数
  std::string name_; // 线程名称
  pid_t tid_; // 线程ID

  // 线程计数器
  static std::atomic<int> numGenerated_;
};

struct ThreadData {
  Functor func_;
  const std::string name_;
  pthread_t tid_;

  ThreadData(Functor func, std::string &name, pthread_t id)
      : func_(std::move(func)), name_(name), tid_(id) {}

  void runThread();
};

} // namespace web

#endif // WEBSERVER_SRC_BASE__THREAD_H