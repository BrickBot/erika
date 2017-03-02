#ifndef _OSEK_TASK_HPP_
#define _OSEK_TASK_HPP_

class osek_core
{
  void (*_startuphook)(void);
public:
  osek_core(void (*startup)(void)) { _startuphook = startup; };

  void DisableAllInterrupts(void) { ::DisableAllInterrupts(); };
  void EnableAllInterrupts(void) { ::EnableAllInterrupts(); };
  void StartOS(int m) { ::StartOS(m); };
  void StartupHook(void) { _startuphook(); };
};


class osek_task 
{
  TaskType _id;

public:
  osek_task(TaskType i) : _id(i) { };

  virtual void run(void) {};

  StatusType ActivateTask(void) { return ::ActivateTask(_id); };
  void TerminateTask(void) { ::TerminateTask(); };
};


#define CPPDeclareTask(task)                                    \
class _##task##_t : public osek_task {                  \
public:                                                 \
  virtual void run(void);                               \
  _##task##_t(TaskType i) : osek_task(i) { };           \
};                                                      \
extern class _##task##_t CPP_##task;

#define CPPTASK(tt, id)                          \
class _##tt##_t CPP_##tt(id);                 \
  extern "C" { void Func##tt(void) { CPP_##tt.run(); } } \
  void _##tt##_t::run(void)

#endif
