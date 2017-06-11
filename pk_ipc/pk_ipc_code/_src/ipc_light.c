/*=====================================================================================*/
/**
 * ipc_light.c
 * author : puch
 * date : Oct 22 2015
 *
 * description : Any comments
 *
 */
/*=====================================================================================*/
#define CLASS_IMPLEMENTATION
/*=====================================================================================*
 * Project Includes
 *=====================================================================================*/
#include "ipc_light.h"
/*=====================================================================================* 
 * Standard Includes
 *=====================================================================================*/

/*=====================================================================================* 
 * Local X-Macros
 *=====================================================================================*/
#undef CLASS_NAME
#undef CLASS_INHERITS
#undef CLASS_MEMBERS
#undef CLASS_METHODS

#define CLASS_NAME IPC_Light
#define CLASS_INHERITS IPC
#define CLASS_MEMBERS(_member) \

#define CLASS_METHODS(_method, _void_method) \
void _method(ctor, IPC_Process_Id_T const, uint32_t const) \
/*=====================================================================================* 
 * Local Define Macros
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Type Definitions
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Function Prototypes
 *=====================================================================================*/
 static void IPC_Light_Ctor(IPC_Light_T * const this, IPC_Process_Id_T const pid, uint32_t const max_tasks);
 static IPC_Process_Id_T IPC_Light_get_tid(IPC_T * const super);
 static IPC_Process_Id_T IPC_Light_get_pid(IPC_T * const super);
 static void IPC_Light_set_mailbox(IPC_T * const super, uint32_t const, uint32_t const);
 static void IPC_Light_notify_ready(IPC_T * const super, IPC_Task_Id_T const);
 static size_t IPC_Light_get_date_length(IPC_T * const super);
 static char const * IPC_Light_get_date(IPC_T * const super);
 /*=====================================================================================*
  * Local Object Definitions
  *=====================================================================================*/
 CLASS_DEFINITION
 /*=====================================================================================*
  * Exported Object Definitions
  *=====================================================================================*/

 /*=====================================================================================*
  * Local Inline-Function Like Macros
  *=====================================================================================*/

 /*=====================================================================================*
  * Local Function Definitions
  *=====================================================================================*/
 void IPC_Light_init(void)
 {
    printf("%s \n", __FUNCTION__);
    IPC_Light_Obj.IPC = IPC();

    memcpy(&IPC_Light_Vtbl.IPC, IPC_Light_Obj.IPC.vtbl, sizeof(IPC_Light_Vtbl.IPC));
    IPC_Light_Vtbl.IPC.Object.rtti = &IPC_Light_Rtti;
    IPC_Light_Vtbl.IPC.Object.destroy = IPC_Light_Dtor;
    IPC_Light_Vtbl.IPC.get_pid = IPC_Light_get_tid;
    IPC_Light_Vtbl.IPC.get_pid = IPC_Light_get_pid;
    IPC_Light_Vtbl.IPC.set_mailbox = IPC_Light_set_mailbox;
    IPC_Light_Vtbl.IPC.notify_ready = IPC_Light_notify_ready;
    IPC_Light_Vtbl.IPC.get_date_length = IPC_Light_get_date_length;
    IPC_Light_Vtbl.IPC.get_date = IPC_Light_get_date;
    IPC_Light_Vtbl.ctor = IPC_Light_Ctor;

 }
 void IPC_Light_shut(void) {}

 void IPC_Light_Dtor(Object_T * const obj)
 {
 }
 /*=====================================================================================*
  * Exported Function Definitions
  *=====================================================================================*/
 void IPC_Light_Ctor(IPC_Light_T * const this, IPC_Process_Id_T const pid, uint32_t const max_tasks)
 {
    this->IPC.vtbl->ctor(&this->IPC, pid, max_tasks);
 }

 IPC_Task_Id_T IPC_Light_get_tid(IPC_T * const super)
 {

 }

 IPC_Process_Id_T IPC_Light_get_pid(IPC_T * const super)
 {

 }

 void IPC_Light_set_mailbox(IPC_T * const super, IPC_Task_Id_T const tid, uint32_t const mail_elements)
 {

 }

 void IPC_Light_notify_ready(IPC_T * const super, IPC_Task_Id_T const task_id)
 {
    Task_T * const task = super->vtbl->search_task(super, task_id);

 }

 size_t IPC_Light_get_date_length(IPC_T * const super)
 {

 }

 char const * IPC_Light_get_date(IPC_T * const super)
 {

 }
/*=====================================================================================* 
 * ipc_light.c
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
