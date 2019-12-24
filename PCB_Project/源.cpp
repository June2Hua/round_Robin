#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<string.h>
#define NAME 20//进程名长度
#define n 10//系统进程数目
#define ARRIVETIME 20//最长到达时间
#define RUNTIME 20//最长运行时间
//进程控制块（PCB）
struct PCB {
	char name[NAME];//进程名
	PCB* next = NULL;//链接指针
	int arriveTime;//到达时间
	int runTime;//估计运行时间
	char status;//进程状态
};
struct circusQueue {
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
//初始化循环队列
void initQueue(PCB* header,PCB* &present, PCB* &preProcess,PCB processes[]) {
	//初始化第一个队首指针
	header = &processes[0];
	present = header;
	//初始化每一个进程的链接指针
	for (int i = 0; i < n-1; i++)
		processes[i].next = &processes[i + 1];
	processes[n - 1].next = header;//形成循环队列
	preProcess = &processes[n - 1];//当前运行进程指针的前一个指针
}
//所有进程当前时间减1
void arriveTimeSub(PCB processes[]) {
	for (int i = 0; i < n; i++)
		processes[i].arriveTime--;
}
//时间片轮转法,若有进程运行结束，则返回true。若没有进程结束，则返回false
bool round_Robin(PCB* present, PCB* preProcess) {
	printProcess(*present);//显示当前进程信息
	(present->runTime)--;//估计运行时间减1
	if (present->runTime == 0) {//进程的剩余运行时间为0,退出循环队列
		printf("当前进程%s运行结束,退出循环队列\n\n",present->name);
		present->status = 'C';//将该进程的状态置为完成状态“C”
		return true;
	}
	return false;
}

int main() {
	PCB processes[n];//进程集合
	PCB* header = NULL;//队首指针
	PCB* present = NULL;//当前运行进程指针
	PCB* preProcess = NULL;//当前运行进程指针的前一个指针
	printf("************初始化进程如下：************\n");
	initProcess(processes);//初始化进程
	quickSort(processes, 0, n - 1);//使用快速排序
	initQueue(header, present, preProcess, processes);//初始化循环队列
	bool isProcessComplete;//是否有进程结束
	printf("************排序之后进程如下：************\n");
	for (int i = 0; i < n; i++)
		printProcess(processes[i]);
	printf("************时间片轮转法：************\n");
	while (preProcess!=present) {
		isProcessComplete = round_Robin(present, preProcess);
		if (isProcessComplete) {
			preProcess->next = present->next;//退出循环队列
			present->next = NULL;//下一个指针为NULL
			present = preProcess->next;//指针移动到下一个
		}
		else {
			preProcess = preProcess->next;//指针移动到下一个
			present = present->next;//指针移动到下一个
		}
	}
	//处理最后一个进程
	while(true) {
		printProcess(*present);//显示当前进程信息
		(present->runTime)--;//估计运行时间减1
		if (present->runTime == 0) {//进程的剩余运行时间为0,退出循环队列
			printf("当前进程运行结束,退出循环队列\n");
			present->status = 'C';//将该进程的状态置为完成状态“C”
			preProcess->next = present->next;//退出循环队列
			present->next = NULL;//下一个指针为NULL
			break;
		}
	}
	printf("所有进程运行完成\n");
	system("pause");
}