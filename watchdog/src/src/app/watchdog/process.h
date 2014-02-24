#ifndef __PROCESS_H__
#define __PROCESS_H__
/**
 * void monitor_start();
 * int monitor_state();
 * void monitor_stop();
 */
void * process(void *p);
/**
 * 注册的 管理 结构体 struct Manger
 *
 * process_regist(int cmd, void(*)());
 */

struct Manager
{

};

struct callback_para
{
	void * value;
	void	(* onfinish)();
};


void onfinish();

void process(msg_t *a);

void manager_init();
void regist();

#endif
