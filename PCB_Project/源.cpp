#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<string.h>
#define NAME 20//����������
#define n 10//ϵͳ������Ŀ
#define ARRIVETIME 20//�����ʱ��
#define RUNTIME 20//�����ʱ��
//���̿��ƿ飨PCB��
struct PCB {
	char name[NAME];//������
	PCB* next = NULL;//����ָ��
	int arriveTime;//����ʱ��
	int runTime;//��������ʱ��
	char status;//����״̬
};
struct circusQueue {
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
//��ʼ��ѭ������
void initQueue(PCB* header,PCB* &present, PCB* &preProcess,PCB processes[]) {
	//��ʼ����һ������ָ��
	header = &processes[0];
	present = header;
	//��ʼ��ÿһ�����̵�����ָ��
	for (int i = 0; i < n-1; i++)
		processes[i].next = &processes[i + 1];
	processes[n - 1].next = header;//�γ�ѭ������
	preProcess = &processes[n - 1];//��ǰ���н���ָ���ǰһ��ָ��
}
//���н��̵�ǰʱ���1
void arriveTimeSub(PCB processes[]) {
	for (int i = 0; i < n; i++)
		processes[i].arriveTime--;
}
//ʱ��Ƭ��ת��,���н������н������򷵻�true����û�н��̽������򷵻�false
bool round_Robin(PCB* present, PCB* preProcess) {
	printProcess(*present);//��ʾ��ǰ������Ϣ
	(present->runTime)--;//��������ʱ���1
	if (present->runTime == 0) {//���̵�ʣ������ʱ��Ϊ0,�˳�ѭ������
		printf("��ǰ����%s���н���,�˳�ѭ������\n\n",present->name);
		present->status = 'C';//���ý��̵�״̬��Ϊ���״̬��C��
		return true;
	}
	return false;
}

int main() {
	PCB processes[n];//���̼���
	PCB* header = NULL;//����ָ��
	PCB* present = NULL;//��ǰ���н���ָ��
	PCB* preProcess = NULL;//��ǰ���н���ָ���ǰһ��ָ��
	printf("************��ʼ���������£�************\n");
	initProcess(processes);//��ʼ������
	quickSort(processes, 0, n - 1);//ʹ�ÿ�������
	initQueue(header, present, preProcess, processes);//��ʼ��ѭ������
	bool isProcessComplete;//�Ƿ��н��̽���
	printf("************����֮��������£�************\n");
	for (int i = 0; i < n; i++)
		printProcess(processes[i]);
	printf("************ʱ��Ƭ��ת����************\n");
	while (preProcess!=present) {
		isProcessComplete = round_Robin(present, preProcess);
		if (isProcessComplete) {
			preProcess->next = present->next;//�˳�ѭ������
			present->next = NULL;//��һ��ָ��ΪNULL
			present = preProcess->next;//ָ���ƶ�����һ��
		}
		else {
			preProcess = preProcess->next;//ָ���ƶ�����һ��
			present = present->next;//ָ���ƶ�����һ��
		}
	}
	//�������һ������
	while(true) {
		printProcess(*present);//��ʾ��ǰ������Ϣ
		(present->runTime)--;//��������ʱ���1
		if (present->runTime == 0) {//���̵�ʣ������ʱ��Ϊ0,�˳�ѭ������
			printf("��ǰ�������н���,�˳�ѭ������\n");
			present->status = 'C';//���ý��̵�״̬��Ϊ���״̬��C��
			preProcess->next = present->next;//�˳�ѭ������
			present->next = NULL;//��һ��ָ��ΪNULL
			break;
		}
	}
	printf("���н����������\n");
	system("pause");
}