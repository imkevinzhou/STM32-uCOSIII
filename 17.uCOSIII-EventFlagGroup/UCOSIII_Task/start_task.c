#include "start_task.h"
#include "task.h"


OS_FLAG_GRP event_flag;

//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ	
CPU_STK START_TASK_STK[START_STK_SIZE];


//���ȼ�0���жϷ������������� OS_IntQTask()
//���ȼ�1��ʱ�ӽ������� OS_TickTask()
//���ȼ�2����ʱ���� OS_TmrTask()
//���ȼ�OS_CFG_PRIO_MAX-2��ͳ������ OS_StatTask()
//���ȼ�OS_CFG_PRIO_MAX-1���������� OS_IdleTask()


//��ʼ������
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	
	OS_CRITICAL_ENTER();	//�����ٽ���

	OSFlagCreate((OS_FLAG_GRP *)&event_flag,
								(CPU_CHAR *)"event flag",
								(OS_FLAGS)0x00,
								(OS_ERR *)&err
								);
	OSTaskCreate((OS_TCB 	* )&Led0TaskTCB,		
					(CPU_CHAR	* )"led0 task", 		
									(OS_TASK_PTR )led0_task, 			
									(void		* )0,					
									(OS_PRIO	  )LED0_TASK_PRIO,     
									(CPU_STK   * )&LED0_TASK_STK[0],	
									(CPU_STK_SIZE)LED0_STK_SIZE/10,	
									(CPU_STK_SIZE)LED0_STK_SIZE,		
									(OS_MSG_QTY  )0,					
									(OS_TICK	  )0,					
									(void   	* )0,					
									(OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
									(OS_ERR 	* )&err);	
	//���������������
	OSTaskCreate((OS_TCB 	* )&FloatTaskTCB,		
				 (CPU_CHAR	* )"float test task", 		
                 (OS_TASK_PTR )float_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )FLOAT_TASK_PRIO,     	
                 (CPU_STK   * )&FLOAT_TASK_STK[0],	
                 (CPU_STK_SIZE)FLOAT_STK_SIZE/10,	
                 (CPU_STK_SIZE)FLOAT_STK_SIZE,		
                 (OS_MSG_QTY  )5,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);							 
  //����Э�鴦������
	OSTaskCreate((OS_TCB 	* )&ProtocolTaskTCB,		
				 (CPU_CHAR	* )"protocol task", 		
                 (OS_TASK_PTR )protocol_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )Protocol_TASK_PRIO,     	
                 (CPU_STK   * )&Protocol_TASK_STK[0],	
                 (CPU_STK_SIZE)Protocol_STK_SIZE/10,	
                 (CPU_STK_SIZE)Protocol_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);				
	OS_CRITICAL_EXIT();	//�����ٽ���							 
						 
								 
	OSTaskDel(NULL,&err);
}