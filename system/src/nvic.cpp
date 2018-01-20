#include "nvic.hpp"

namespace nvic{
	static u8 NVIC_Group;

	void SetVectorTable(u32 VectTab,u32 Offset){
	//设置向量表偏移地址
	//VectTab:基址
	//Offset:偏移量
		SCB->VTOR = VectTab|Offset;//设置NVIC的向量表偏移寄存器
	//用于标识向量表是在CODE区还是在RAM区
	}
	void GroupConfig(u8 Group){
	//设置NVIC分组
	//NVIC_Group:NVIC分组0~4总共5组
	//组划分:
	//组0:0位抢占优先级,4位响应优先级
	//组1:1位抢占优先级,3位响应优先级
	//组2:2位抢占优先级,2位响应优先级
	//组3:3位抢占优先级,1位响应优先级
	//组4:4位抢占优先级,0位响应优先级
		NVIC_Group=Group;
		u32 temp,temp1;
		temp1=(~NVIC_Group)&0x07;//取后三位
		temp1<<=8;
		temp=SCB->AIRCR;//读取先前的设置
		temp&=0X0000F8FF;//清空先前分组
		temp|=0X05FA0000;//写入钥匙
		temp|=temp1;
		SCB->AIRCR=temp;//设置分组
	}
	void Init(u8 Channel, u8 PreemptionPriority, u8 SubPriority){
	//设置NVIC
	//Channel:中断编号
	//PreemptionPriority:抢占优先级
	//SubPriority:响应优先级
	//SubPriority和PreemptionPriority的原则是,数值越小,越优先
		u32 temp;
		temp=PreemptionPriority<<(4-NVIC_Group);
		temp|=SubPriority&(0x0f>>NVIC_Group);
		temp&=0xf;//取低四位
		NVIC->ISER[Channel/32]|=(1<<Channel%32);//使能中断位(要清除的话,相反操作就OK)
		NVIC->IP[Channel]|=temp<<4;//设置响应优先级和抢断优先级
	}


	void DISABLE(void){
	//关闭所有中断(但是不包括fault和NMI中断)
		__ASM volatile("cpsid i");
	}
	void ENABLE(void){
	//开启所有中断
		__ASM volatile("cpsie i");
	}
}