/*
 * Communication.h
 *
 *  Created on: Apr 13, 2026
 *      Author: TX
 */

#ifndef CORE_COMMUNICATION_H_
#define CORE_COMMUNICATION_H_

//
#include "motified_app.h"
#include "Task.h"


typedef enum{
    TASK_IDLE = 0,
    TASK_ONE_GRAB_OBJECT = 1,
}TASK_COMMAND;

typedef struct{
        TASK_COMMAND TASK_FLAG;
        //u16
}Message;



#endif /* CORE_COMMUNICATION_H_ */
