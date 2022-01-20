#include "task_queue_type.h"

#include <algorithm>
#include <utility>

namespace {
struct {
  bool operator()(const std::pair<unsigned long, TaskQueueType::Task>& a,
                  std::pair<unsigned long, TaskQueueType::Task>& b) const {
    return a.first > b.first;
  }
} comparator;
}  // namespace

void TaskQueueType::postOneShotTask(const TaskQueueType::Task& task,
                                    unsigned long delayMs) {
  unsigned long currentMillis = millis_();
  timedTasks_.push_back(std::make_pair(currentMillis + delayMs, task));
  std::push_heap(timedTasks_.begin(), timedTasks_.end(), comparator);
}

void TaskQueueType::postRecurringTask(const Task& task) {
  recurringTasks_.push_back(task);
}

void TaskQueueType::process() {
  auto time = millis_();
  while (!timedTasks_.empty() && time > timedTasks_.front().first) {
    timedTasks_.front().second();
    std::pop_heap(timedTasks_.begin(), timedTasks_.end(), comparator);
    timedTasks_.pop_back();
  }
  for (auto& c : recurringTasks_) {
    c();
  }
}