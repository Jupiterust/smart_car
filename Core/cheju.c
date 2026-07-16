//=============================================================
// 雷达距离接收模块 (TC264) - 双路版本
//=============================================================
#include "cheju.h"   // 引入自定义头文件，里面应该声明了 RadarInstance_t 结构体、
                     // RADAR_CONF_MIN / RADAR_TARGET_MM / RADAR_STABLE_CNT / RADAR_RELEASE_MM 等宏

// 定义两个雷达实例的全局变量，并做初始化：
// distance 初始值设为 9999，是一个"哨兵值"（sentinel value），
// 表示"还没收到过有效数据 / 距离远到可以认为没有障碍物"，避免上电瞬间用0这种危险的默认值
RadarInstance_t radar1 = {.distance = 9999};   // 帧头0xBB，对应第一路雷达
RadarInstance_t radar2 = {.distance = 9999};   // 帧头0xCC，对应第二路雷达

// 静态指针，记录"当前正在往哪个实例里灌数据"
// static 意味着这个变量只在本文件内可见，外部文件无法直接访问，防止误用
// 初始值为0(NULL)，表示"目前还没锁定是哪一路，处于寻找帧头的阶段"
static RadarInstance_t *radar_active = 0;

//-------------------------------------------------------------
// 帧解析,逐字节喂入。根据帧头(0xBB/0xCC)自动分流到对应实例
// 帧格式:[0xBB或0xCC] | dL | dH | conf | xor | 0x0D
// 也就是共6字节一帧：1字节帧头 + 2字节距离(低位在前) + 1字节置信度 + 1字节异或校验 + 1字节帧尾
//-------------------------------------------------------------
void Radar_Feed_Byte(uint8_t b)
{
    // 这个函数被设计成"字节流驱动的状态机"：串口中断每收到一个字节就调用一次这个函数，
    // 不在中断里做阻塞等待，符合中断服务函数"越快越好"的原则

    // 阶段0:还没确定是哪一路,先找帧头
    if (radar_active == 0)
    {
        // radar_active 为空指针，说明上一帧已经处理完/还没开始，现在处于"寻找帧头"状态
        if (b == 0xBB)      radar_active = &radar1;   // 收到0xBB，说明这是第一路雷达的帧，把active指针指向radar1
        else if (b == 0xCC) radar_active = &radar2;   // 收到0xCC，说明这是第二路雷达的帧，指向radar2
        else return;   // 既不是0xBB也不是0xCC,丢弃这个字节,继续等下一个
                        // 这一行是关键的容错机制：如果串口出现干扰字节、或者上一帧没对齐，
                        // 这里会不断丢弃垃圾字节，直到重新捕获到合法帧头，防止解析器永久卡死

        // 走到这里说明成功识别了帧头，做一些初始化准备接收后续字节
        radar_active->cnt      = 0;   // cnt 是当前实例内部的字节计数器，清零，准备开始存 dL/dH/conf/xor 这4个字节
        radar_active->rx_state = 1;   // 把这一路的状态机推进到"状态1"：接收数据阶段
        radar_active->byte_cnt++;     // byte_cnt 是一个统计量，记录这一路总共处理过多少字节（调试/诊断用，不影响主逻辑）
        return;   // 本次调用到此结束，等下一个字节进来
    }

    // 走到这里说明 radar_active 不为空，正在某一路的数据接收过程中
    RadarInstance_t *r = radar_active;   // 取个局部指针别名，方便后面代码书写简洁（等价于 radar_active，只是少打几个字）
    r->byte_cnt++;   // 每收到一个属于当前帧的字节，计数+1（同样是统计用途）

    switch (r->rx_state)
    {
        case 1:                              // 收 dL dH conf xor 共4字节
            r->buf[r->cnt++] = b;            // 把当前字节存进缓冲区buf，同时cnt自增（先用后加）
            if (r->cnt >= 4) r->rx_state = 2; // 攒够4个字节（dL, dH, conf, xor）后，切到"验证帧尾"状态
            break;                            // 不够4个字节就先break，等下一个字节继续走case 1

        case 2:                              // 验帧尾 0x0D + 异或校验
            if (b == 0x0D)                   // 只有收到的字节正好是约定的帧尾0x0D，才认为这一帧结构完整
            {
                // 计算异或校验：把dL、dH、conf三个字节做异或，结果应该等于协议里传来的第4个字节(xor校验位)
                uint8_t chk = r->buf[0] ^ r->buf[1] ^ r->buf[2];
                if (chk == r->buf[3])        // 校验通过，说明这一帧数据是可信的，没有被干扰损坏
                {
                    // 小端拼接：buf[0]是低8位，buf[1]是高8位，组合成16位距离值（单位大概率是mm，需要看协议文档确认）
                    r->distance   = (uint16_t)(r->buf[0] | (r->buf[1] << 8));
                    r->confidence = r->buf[2];   // buf[2]是置信度字节，代表这次测距结果的可信程度
                    r->updated    = 1;           // 置位"有新数据"标志，通知主循环："这一路雷达有新的一帧数据了，可以去处理"
                    r->frame_cnt++;              // 成功解析的总帧数计数（统计/调试用，可以用来判断雷达数据是否稳定持续在更新）
                }
                // 注意：如果校验失败(chk != buf[3])，这里什么都不做，直接跳过——
                // 也就是说校验失败的帧会被静默丢弃，不会更新distance，也不会报错，这是一种"宁缺毋滥"的容错策略
            }
            // 无论帧尾对不对、校验对不对，只要走到这个case，这一帧的"生命周期"都结束了，
            // 所以下面两行是无条件执行的收尾动作：
            r->rx_state  = 0;      // 把这一路自己的状态复位，为下一次"轮到它"接收做准备
            radar_active = 0;      // 关键：把全局的active指针清空，回到"阶段0:找帧头"
                                    // 这一行使得下一个字节无论是0xBB还是0xCC，都能重新被正确识别并分流到对应实例
            break;

        default:                   // 理论上不会走到这里（rx_state只会是0/1/2），是防御性编程
            r->rx_state  = 0;      // 出现意外状态时，强制复位，保证系统不会卡死在一个非法状态里
            radar_active = 0;
            break;
    }
}

//-------------------------------------------------------------
// 距离判断,主循环周期调用,需要分别给radar1和radar2各调一次
// 防抖(连续N帧) + 回滞(到达/离开两套阈值) + 锁存(只触发一次)
//-------------------------------------------------------------
void Radar_Distance_Judge(RadarInstance_t *r)
{
    // 这个函数不在中断里跑，而是主循环里周期性调用，每次传入radar1或radar2的指针，
    // 各自独立判断"是否已经到达目标距离"

    if (!r->updated) return;   // 如果这一路自从上次调用后没有新数据(updated==0)，就没必要重新判断，直接返回
                                // 这样可以避免在"没有新数据"的情况下反复用旧数据做判断，节省CPU
    r->updated = 0;             // 消费掉这个"有新数据"标志，避免下次调用时重复处理同一帧数据

    uint16_t d = r->distance;   // 取出最新距离值，存入局部变量，方便下面多次使用（避免每次都解引用指针）
    uint8_t  c = r->confidence; // 取出最新置信度
    if (c < RADAR_CONF_MIN) return;   // 如果这一帧的置信度低于设定的最低门限，认为这次测距不可信，直接丢弃不处理
                                       // 这是过滤噪声/无效测距的关键一步，防止低质量数据触发误判

    if (!r->arrived)   // 分支A：当前处于"尚未到达"状态（还没触发过任务）
    {
        if (d < RADAR_TARGET_MM)   // 距离小于目标阈值，说明可能已经接近/到达目标了
        {
            if (++r->below_cnt >= RADAR_STABLE_CNT)
            // 先自增below_cnt(连续满足条件的帧数计数)，再判断是否达到"稳定帧数"门限
            // 这就是"防抖"逻辑：不是一帧满足条件就立刻触发，而是要求连续N帧都满足才算数，
            // 防止某一帧噪声数据（比如偶然测距偏小）导致误触发
            {
                r->arrived   = true;    // 正式确认"到达"，锁存状态改变
                r->below_cnt = 0;       // 计数器清零，为下一次可能的判断做准备（虽然锁存后一般不会再用到）
                // ========= 距离到达,执行任务(只触发一次) =========
                // 这里是一个"占位注释"，实际触发任务的代码应该写在这里
                // 因为r->arrived这次从false变成true，只会经过这一行一次，
                // 之后即使d持续小于RADAR_TARGET_MM，也不会再进入这个if块（因为外层已经是if(!r->arrived)），
                // 这就实现了"锁存 + 只触发一次"的效果，避免任务被重复触发多次
            }
        }
        else r->below_cnt = 0;
        // 只要有一帧距离不满足"小于目标值"，立刻把连续计数清零
        // 这是防抖逻辑必须配套的部分：必须是"连续"满足条件才算数，中间断一次就重新计数
    }
    else   // 分支B：当前处于"已到达"状态（之前已经触发过一次任务）
    {
        if (d > RADAR_RELEASE_MM) r->arrived = false;
        // 只有当距离拉开到超过"释放阈值"（通常比目标阈值更大，形成回滞区间）时，
        // 才把arrived重置为false，允许下一次重新触发
        // 这就是"回滞(hysteresis)"设计：到达阈值(TARGET_MM)和离开阈值(RELEASE_MM)不是同一个值，
        // 中间留有一段缓冲区，防止小车/障碍物恰好停在临界距离附近来回抖动时反复触发/取消任务
    }
}



volatile bool tast2_start_test_distance = false;

volatile bool task2_prepare_correct = false;
volatile bool task2_start_correct = false;
volatile bool does_task2_send_a_signal = false;
void task2_tast_dis(void){
    if(tast2_start_test_distance == true){
        if(170 <= radar1.distance  &&  radar1.distance <= 270 && wheel_asix.yaw > 80)
        {
            tast2_start_test_distance = false;
            // 速度
            task2_start = true;
            following_speed[0] = 0;
            following_speed[1] = 0;
            following_speed[2] = 0;
            following_speed[3] = 0;
            // 开始
            task_start_signal_from_me = 'B';
            UART_PutChar(UART0,task_start_signal_from_me);
            // 校准位置
            task2_prepare_correct = true;
        }
    }
}


volatile bool tast4_start_test_distance = false;
volatile bool task4_speed_adjust_start = false;

volatile bool task4_prepare_correct = false;
volatile bool task4_start_correct = false;
// 起点
void task4_test_distance(void){
    if(tast4_start_test_distance == true){
        if(radar2.distance > 190 && radar2.distance < 300 && wheel_asix.yaw > 40){
            following_speed[0] = 0;
            following_speed[1] = 0;
            following_speed[2] = 0;
            following_speed[3] = 0;
            task4_speed_adjust_start = true;
            task_start_signal_from_me = 'C';
            UART_PutChar(UART0,task_start_signal_from_me);
            task4_prepare_correct = true;
        }
    }
}


volatile bool tast3_start_test_distance_flag = false;
volatile bool tast3_prepare_to_shoot = false;
volatile bool tast3_prepare_to_correct = false;
volatile bool tast3_start_to_correct = false;

 void task3_start_test_distance(void){
    if(tast3_start_test_distance_flag == true){

        if(radar2.distance > 950 &&  radar2.distance < 1300 && wheel_asix.yaw < 20 && wheel_asix.yaw > -10){
            tast3_prepare_to_shoot = true;
            tast3_start_test_distance_flag = false;
            UART_PutChar(UART1,task_start_signal_from_me);
            following_speed[0] = 0;
            following_speed[1] = 0;
            following_speed[2] = 0;
            following_speed[3] = 0;

            tast3_prepare_to_shoot = true;
            tast3_prepare_to_correct = true;

        }
    }
}


