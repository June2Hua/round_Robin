#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<string.h>
#define NAME 20//进程名长度
#define n 3//系统进程数目
#define ARRIVETIME 5//最长到达时间
#define RUNTIME 5//最长运行时间
#define ROUND_ROBIN_TIME 1//时间片的大小
/*
	记录错误：
	1.函数传递参数，读操作可按值传递；修改操作需要用别名传递
	2.插入删除队列元素，队列分一下情况
		①为空
		②队列仅有一个元素
		③队列中有两个元素
		④队列中有多个元素
*/
//进程控制块（PCB）
struct PCB {
	char name[NAME];//进程名
	PCB* next = NULL;//链接指针
	int arriveTime;//到达时间
	int runTime;//估计运行时间
	char status;//进程状态
};
//循坏队列
struct CycleQueue {
	PCB* preProcess = NULL;//当前进程的前一个进程
	PCB* present = NULL;//当前进程
};
//显示进程
void printProcess(PCB process) {
	printf("进程名为:%s\t\t", process.name);
	printf("进程到达的时间为:%d\t\t", process.arriveTime);
	printf("进程估计运行时间为:%d\t\t", process.runTime);
	printf("进程状态为:%c\n", process.status);
}
//初始化进程
void initProcess(PCB processes[]) {
	srand(time(0));//随机种子，用于产生随机到达时间，估计运行时间
	char name[NAME] = "PCB";
	char num[5];//整型转化为字符串型
	for (int i = 0; i < n; i++) {
		_itoa_s(i+1, num, 5);//转化函数
		strcpy_s(processes[i].name, name);//复制
		strcat_s(processes[i].name, num);//添加为PCB+数字
		processes[i].arriveTime = rand() % ARRIVETIME;
		processes[i].runTime = rand() % RUNTIME + 1;
		processes[i].status = 'N';
		printProcess(processes[i]);
	}
}
//快速排序算法
int partition(PCB list[], int low, int high) {
	int pivotkey = list[low].arriveTime;//选择需要排序的第一个数作为枢轴值
	int low_ = low;//记录左下标
	PCB tmp;//用于交换的中间变量
	while (low < high) {
		//寻找右边第一个小于枢轴值的下标
		while (low < high&&list[high].arriveTime >= pivotkey)
			high--;
		//寻找第一个大于枢轴值的下标
		while (low < high&&list[low].arriveTime <= pivotkey)
			low++;
		//交换两个数值
		tmp = list[low];
		list[low] = list[high];
		list[high] = tmp;
	}
	//将枢轴值放到枢轴中心
	tmp = list[low_];
	list[low_] = list[low];
	list[low] = tmp;
	return low;//返回枢轴中心下标
}
//进行快排
void quickSort(PCB list[], int low, int high) {
	if (low < high) {
		int position = partition(list, low, high);//得到枢轴中心
		quickSort(list, low, position - 1);//以枢轴中心为中点，左侧的数组进行快排
		quickSort(list, position + 1, high);//以枢轴中心为中点，右侧的数组进行快排
	}
}
//所有进程当前时间减一个时间片
void arriveTimeSub(PCB processes[]) {
	for (int i = 0; i < n; i++)
		processes[i].arriveTime -= ROUND_ROBIN_TIME;
}
//时间片轮转法,若有进程运行结束，则返回true。若没有进程结束，则返回false
bool round_Robin(CycleQueue& cycleQueue) {
	printf("准备执行:");
	printProcess(*cycleQueue.present);//显示当前进程信息
	(cycleQueue.present->runTime) -= ROUND_ROBIN_TIME;//估计运行时间减1
	if (cycleQueue.present->runTime <= 0) {//进程的剩余运行时间为0,退出循环队列
		printf("当前进程%s运行结束,退出循环队列\n", cycleQueue.present->name);
		cycleQueue.present->status = 'C';//将该进程的状态置为完成状态“C”
		return true;
	}
	return false;
}
//就绪队列插入一个进程，先插入后移动
//循坏队列为空，则没有元素，若present和preProcess相同，则仅有一个元素
void insertCycleQueue(CycleQueue& cycleQueue,PCB& process) {
	//循坏队列为空
	if (cycleQueue.present == NULL) {
		//当前进程
		cycleQueue.present = &process;
		cycleQueue.present->next = &process;
		//前一个进程
		cycleQueue.preProcess = &process;
		cycleQueue.preProcess->next = &process;
		return;
	}
	//循坏队列仅有一个,****************************************
	if (cycleQueue.present == cycleQueue.preProcess) {
		PCB& presentProcess = *cycleQueue.present;
		cycleQueue.preProcess = &process;
		cycleQueue.preProcess->next = &presentProcess;
		cycleQueue.present = &presentProcess;
		cycleQueue.present->next = cycleQueue.preProcess;
		return;
	}
	//循坏队列有多个
	cycleQueue.preProcess->next = &process;
	cycleQueue.preProcess = cycleQueue.preProcess->next;
	process.next = cycleQueue.present;
}
//将当前进程移出循坏队列
void removeFromCycleQueue(CycleQueue& cycleQueue) {
	if (cycleQueue.present == NULL) {
		printf("错误！队列为空\n");
		return;
	}
	//循坏队列仅有一个元素
	if (cycleQueue.preProcess == cycleQueue.present) {
		cycleQueue.preProcess = NULL;
		cycleQueue.present = NULL;
		return;
	}
	//循坏队列仅有两个元素
	if (cycleQueue.preProcess == cycleQueue.present->next) {
		cycleQueue.preProcess->next = cycleQueue.preProcess;
		cycleQueue.present = cycleQueue.preProcess;
		return;
	}
	//循坏队列有多个元素,***************		present发生变化
	cycleQueue.preProcess->next = cycleQueue.present->next;
	cycleQueue.present->next = NULL;
	cycleQueue.present = cycleQueue.preProcess->next;
}
int main() {
	PCB processes[n];//进程集合
	CycleQueue cycleQueue;//循坏队列
	int index = 0;//进程集合下标
	bool completed = false;//是否有进程完成
	printf("************************初始化进程如下：************************\n\n");
	initProcess(processes);//初始化进程
	quickSort(processes, 0, n - 1);//使用快速排序
	printf("\n************************排序之后进程如下：************************\n\n");
	for (int i = 0; i < n; i++)
		printProcess(processes[i]);
	printf("\n************************时间片轮转法：************************\n\n");
	while (index != n || cycleQueue.present != NULL) {
		completed = false;
		//判断是否有到达时间为0的，加入到循坏队列中
		while (processes[index].arriveTime <= 0) {
			printf("%s进入了循坏队列\n", processes[index].name);
			insertCycleQueue(cycleQueue, processes[index]);//插入到循坏队列中
			index++;
		}
		//循坏队列非空
		if (cycleQueue.present != NULL) {
			//时间片轮转法
			completed = round_Robin(cycleQueue);
			//如果有进程完成
			if (completed) {
				//当前进程移出循坏队列
				removeFromCycleQueue(cycleQueue);
			}
			else {//如果没有进程完成
				cycleQueue.preProcess = cycleQueue.preProcess->next;//指针移动到下一个
				cycleQueue.present = cycleQueue.present->next;//指针移动到下一个
			}
		}
		arriveTimeSub(processes);//达到时间减一个时间片
	}
	printf("所有进程运行完成\n");
	system("pause");
}