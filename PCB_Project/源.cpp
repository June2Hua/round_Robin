#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<string.h>
#define NAME 20//����������
#define n 3//ϵͳ������Ŀ
#define ARRIVETIME 5//�����ʱ��
#define RUNTIME 5//�����ʱ��
/*
	��¼����
	1.�������ݲ������������ɰ�ֵ���ݣ��޸Ĳ�����Ҫ�ñ�������
	2.����ɾ������Ԫ�أ����з�һ�����
		��Ϊ��
		�ڶ��н���һ��Ԫ��
		�۶�����������Ԫ��
		�ܶ������ж��Ԫ��
*/
//���̿��ƿ飨PCB��
struct PCB {
	char name[NAME];//������
	PCB* next = NULL;//����ָ��
	int arriveTime;//����ʱ��
	int runTime;//��������ʱ��
	char status;//����״̬
};
//ѭ������
struct CircusQueue {
	PCB* preProcess = NULL;//��ǰ���̵�ǰһ������
	PCB* present = NULL;//��ǰ����
};
//��ʾ����
void printProcess(PCB process) {
	printf("������Ϊ:%s\t\t", process.name);
	printf("���̵����ʱ��Ϊ:%d\t\t", process.arriveTime);
	printf("���̹�������ʱ��Ϊ:%d\t\t", process.runTime);
	printf("����״̬Ϊ:%c\n", process.status);
}
//��ʼ������
void initProcess(PCB processes[]) {
	srand(time(0));//������ӣ����ڲ����������ʱ�䣬��������ʱ��
	char name[NAME] = "PCB";
	char num[5];//����ת��Ϊ�ַ�����
	for (int i = 0; i < n; i++) {
		_itoa_s(i+1, num, 5);//ת������
		strcpy_s(processes[i].name, name);//����
		strcat_s(processes[i].name, num);//���ΪPCB+����
		processes[i].arriveTime = rand() % ARRIVETIME;
		processes[i].runTime = rand() % RUNTIME + 1;
		processes[i].status = 'N';
		printProcess(processes[i]);
	}
}
//���������㷨
int partition(PCB list[], int low, int high) {
	int pivotkey = list[low].arriveTime;//ѡ����Ҫ����ĵ�һ������Ϊ����ֵ
	int low_ = low;//��¼���±�
	PCB tmp;//���ڽ������м����
	while (low < high) {
		//Ѱ���ұߵ�һ��С������ֵ���±�
		while (low < high&&list[high].arriveTime >= pivotkey)
			high--;
		//Ѱ�ҵ�һ����������ֵ���±�
		while (low < high&&list[low].arriveTime <= pivotkey)
			low++;
		//����������ֵ
		tmp = list[low];
		list[low] = list[high];
		list[high] = tmp;
	}
	//������ֵ�ŵ���������
	tmp = list[low_];
	list[low_] = list[low];
	list[low] = tmp;
	return low;//�������������±�
}
//���п���
void quickSort(PCB list[], int low, int high) {
	if (low < high) {
		int position = partition(list, low, high);//�õ���������
		quickSort(list, low, position - 1);//����������Ϊ�е㣬����������п���
		quickSort(list, position + 1, high);//����������Ϊ�е㣬�Ҳ��������п���
	}
}
//���н��̵�ǰʱ���1
void arriveTimeSub(PCB processes[]) {
	for (int i = 0; i < n; i++)
		processes[i].arriveTime--;
}
//ʱ��Ƭ��ת��,���н������н������򷵻�true����û�н��̽������򷵻�false
bool round_Robin(CircusQueue& circusQueue) {
	printf("׼��ִ��:");
	printProcess(*circusQueue.present);//��ʾ��ǰ������Ϣ
	(circusQueue.present->runTime)--;//��������ʱ���1
	if (circusQueue.present->runTime == 0) {//���̵�ʣ������ʱ��Ϊ0,�˳�ѭ������
		printf("��ǰ����%s���н���,�˳�ѭ������\n", circusQueue.present->name);
		circusQueue.present->status = 'C';//���ý��̵�״̬��Ϊ���״̬��C��
		return true;
	}
	return false;
}
//�������в���һ�����̣��Ȳ�����ƶ�
//ѭ������Ϊ�գ���û��Ԫ�أ���present��preProcess��ͬ�������һ��Ԫ��
void insertCircusQueue(CircusQueue& circusQueue,PCB& process) {
	//ѭ������Ϊ��
	if (circusQueue.present == NULL) {
		//��ǰ����
		circusQueue.present = &process;
		circusQueue.present->next = &process;
		//ǰһ������
		circusQueue.preProcess = &process;
		circusQueue.preProcess->next = &process;
		return;
	}
	//ѭ�����н���һ��,****************************************
	if (circusQueue.present == circusQueue.preProcess) {
		PCB& presentProcess = *circusQueue.present;
		circusQueue.preProcess = &process;
		circusQueue.preProcess->next = &presentProcess;
		circusQueue.present = &presentProcess;
		circusQueue.present->next = circusQueue.preProcess;
		return;
	}
	//ѭ�������ж��
	circusQueue.preProcess->next = &process;
	circusQueue.preProcess = circusQueue.preProcess->next;
	process.next = circusQueue.present;
}
//����ǰ�����Ƴ�ѭ������
void removeFromCircusQueue(CircusQueue& circusQueue) {
	if (circusQueue.present == NULL) {
		printf("���󣡶���Ϊ��\n");
		return;
	}
	//ѭ�����н���һ��Ԫ��
	if (circusQueue.preProcess == circusQueue.present) {
		circusQueue.preProcess = NULL;
		circusQueue.present = NULL;
		return;
	}
	//ѭ�����н�������Ԫ��
	if (circusQueue.preProcess == circusQueue.present->next) {
		circusQueue.preProcess->next = circusQueue.preProcess;
		circusQueue.present = circusQueue.preProcess;
		return;
	}
	//ѭ�������ж��Ԫ��,***************		present�����仯
	circusQueue.preProcess->next = circusQueue.present->next;
	circusQueue.present->next = NULL;
	circusQueue.present = circusQueue.preProcess->next;
}
int main() {
	PCB processes[n];//���̼���
	CircusQueue circusQueue;//ѭ������
	int index = 0;//���̼����±�
	bool completed = false;//�Ƿ��н������
	printf("************************��ʼ���������£�************************\n\n");
	initProcess(processes);//��ʼ������
	quickSort(processes, 0, n - 1);//ʹ�ÿ�������
	printf("\n************************����֮��������£�************************\n\n");
	for (int i = 0; i < n; i++)
		printProcess(processes[i]);
	printf("\n************************ʱ��Ƭ��ת����************************\n\n");
	while (index != n || circusQueue.present != NULL) {
		completed = false;
		//�ж��Ƿ��е���ʱ��Ϊ0�ģ����뵽ѭ��������
		while (processes[index].arriveTime <= 0) {
			printf("%s������ѭ������\n", processes[index].name);
			insertCircusQueue(circusQueue, processes[index]);//���뵽ѭ��������
			index++;
		}
		//ѭ�����зǿ�
		if (circusQueue.present != NULL) {
			//ʱ��Ƭ��ת��
			completed = round_Robin(circusQueue);
			//����н������
			if (completed) {
				//��ǰ�����Ƴ�ѭ������
				removeFromCircusQueue(circusQueue);
			}
			else {//���û�н������
				circusQueue.preProcess = circusQueue.preProcess->next;//ָ���ƶ�����һ��
				circusQueue.present = circusQueue.present->next;//ָ���ƶ�����һ��
			}
		}
		arriveTimeSub(processes);//�ﵽʱ���1
	}
	printf("���н����������\n");
	system("pause");
}