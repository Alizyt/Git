#include "mb.h"
#include "init.h"
#include "fattester.h"
#include "controlfunction.h"

#define True  1
#define False 0

u8 bottonPressed=0;//bottonValue=0,bollardControlType;
eBollardBottonValue bottonValue;
eBollardControlType bollardControlType;
//u8 remoteControl;
u8 limitReach=0;//limitValue=0,limitValuePre=0,
eLimitValue limitValue,limitValuePre;
u8 statusChange=0;//bollardStatus=0,bollardStatusPre=0,
eBollardStatus bollardStatus,bollardStatusPre;
u8 cascadeChange=0;//cascadeConnection=0,cascadeConnectionPre=0,
eCascadeConnection cascadeConnection,cascadeConnectionPre,cascadeConnectionAft;

u8 groundCoilOnOff=1,synchroOnOff=1,remoteOnOff=1;

int main(void)
{
	systemInit();
	
	TIM_Cmd(TIM3, ENABLE);

	//启动FreeModbus
	eMBEnable();
	while(1)
	{
		//FreeModbus不断查询
		eMBPoll();
		//Botton_Control();
		
		//本地按键控制查询
		bottonValue=Botton_Scan(1);
		if(bottonValue)
		{
			bottonPressed=True;
			switch(bottonValue)
			{
				case BottonUp:
					bollardControlType=Control_Bollard_Up;
					break;
				case BottonDown:
					bollardControlType=Control_Bollard_Down;
					break;
				case BottonStop:
					bollardControlType=Control_Bollard_Stop;
					break;
			}	
		}
		
		//限位到达查询
		limitValuePre=limitValue;
		limitValue=Limit_Scan();
		if((limitValue)&&(limitValue!=limitValuePre))
		{
			limitReach=True;
		}
		
		//升降柱状态查询
		bollardStatusPre=bollardStatus;
		bollardStatus=Bollard_Status_Scan();
		if(bollardStatus!=bollardStatusPre)
		{
			statusChange=True;
		}

#if synchroOnOff==1		
		//级联输入查询
		cascadeConnectionPre=cascadeConnection;
		cascadeConnection=Cascade_Connection_Scan();
		if(cascadeConnection&&(cascadeConnection!=cascadeConnectionPre))
		{
			cascadeConnectionAft=cascadeConnection;
			cascadeChange=True;
		}
#endif
		
	}
}

