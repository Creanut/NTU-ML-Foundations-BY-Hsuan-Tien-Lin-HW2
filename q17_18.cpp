#include<iostream>
#include<stdlib.h>
#include<vector>
#include<algorithm>
#include<math.h>
#include<stdio.h>

using namespace std;

#define DATASIZE 20 //������-1��1�����ڵ���ֵ��ĸ���

//ѵ�������ṹ�嶨�壬��������һά�����ϣ�����double���͵�ѵ��������һά��
struct record
{
	double x;//��������
	int y;//������ǩ
};

//���෽ʽ�ṹ�嶨��
/*
�ṹ���е�sΪ���ͷ���ķ�ʽ��������ѡȡ����Ҳ�Ϊ+1��Ҳ������ѡȡ��������+1��
���Է��෽ʽ�У�s����ȡ��ֵΪ+1��-1��
theta��ʾ����ѡȡ���λ�ã������ڶ��������ڻ��ֳ���21��С���䣬����theta������
��21��С������ȡֵ��
*/
struct hypothesis
{
	int s;
	double theta;
};

int sign(double x)
{
	if(x <= 0)
		return -1;
	else
		return 1;
}

//����ѵ�����ݼ������������������21���㣬������ÿ�����Ӧ�ı�ǩֵ
/*
���ɺ��������Ҫӳ��Ϊ��Ӧά�ȵ�����
*/
void get_rand_data(vector<record> &training_data)
{
	int i;
	for(i = 0;i < DATASIZE;i++)
	{
		record temp;
		temp.x = 2.0 * rand() / double(RAND_MAX) - 1.0;
		temp.y = sign(temp.x);
		training_data.push_back(temp);
	}
}

//������������20%�����ݱ�ǩֵ���ԭ�����෴��
//20%�����������ȡ
void get_noise(vector<record> &training_data)
{
	int i;
	for(i = 0;i < DATASIZE;i++)
	{
		double randnum = rand() / double(RAND_MAX);
		if(randnum < 0.2)
			training_data[i].y = -1 * training_data[i].y;
	}
}

bool my_compare(record &v1,record &v2)
{
	return v1.x < v2.x;
}

//��ѵ�����ݼ��е����ݰ������ݵĴ�С�����������
void sort_training_data(vector<record> &training_data)
{
	sort(training_data.begin(),training_data.end(),my_compare);
}

//����ÿһ��hyposesis�Ĵ�����
/*
ÿ��hyposesis�Ĵ����ʼ��㷽����
����ѵ�����е�ÿһ�������Ӧ�㷨����ѡȡ�Ĳο�������λ�ã�
��ǰ����ο�������ֵ��Ȼ��ȡ����ֵ��Ȼ������㷨�ж�Ӧ��sֵ��ˡ�
���ý������ѵ�����е����ݱ�ǩֵ���Ƚϣ�����ȽϽ����һ�£�������ۼ���������
*/
double calculate_error(vector<record> &training_data,hypothesis &h)
{
	int i;
	int error = 0;
	for(i = 0;i < DATASIZE;i++)
	{
		int temp = h.s * sign(training_data[i].x - h.theta);
		if(temp != training_data[i].y)
			error++;
	}
	return error / double(DATASIZE);
}

//����1Ϊ���������ѵ����������2Ϊ�ҵ�������㷨��������������㷨��e_in
double e_in(vector<record> &training_data,hypothesis &best_h)
{
	hypothesis temp;
	double min_error_rate = 1.0;
	int i;
	for(i = 0;i < DATASIZE + 1;i++)
	{
		temp.s = 1;
		//�����ѵ�����е���Сֵ�����㷨�ο����λ��Ӧ������Сֵ����࣬��������ʹ�òο���С����Сֵ
		if(i == 0)
			temp.theta = (training_data[0].x - 1.0) / 2.0;
		else if(i == DATASIZE)
		//�������ֵ�����㷨�ο����λ��Ӧ�������ֵ���Ҳ࣬��������ʹ�òο���������ֵ
			temp.theta = (training_data[DATASIZE-1].x + 1.0) / 2.0;
		//����ο��������ݵ�֮��
		else
			temp.theta = (training_data[i-1].x + training_data[i].x) / 2.0;

		double error_rate = calculate_error(training_data,temp);
		//�����ǰ�㷨�Ĵ����ʱ���С�����ʻ�ҪС�����滻��С�����ʣ�����¼��ǰ�㷨Ϊ����㷨
		if(error_rate < min_error_rate)
		{
			best_h = temp;
			min_error_rate = error_rate;
		}
	}
	
	for(i = 0;i < DATASIZE + 1;i++)
	{
		temp.s = -1;
		if(i == 0)
			temp.theta = (training_data[0].x - 1.0) / 2.0;
		else if(i == DATASIZE)
			temp.theta = (training_data[DATASIZE-1].x + 1.0) / 2.0;
		else 
			temp.theta = (training_data[i-1].x + training_data[i].x) / 2.0;
		
		double error_rate = calculate_error(training_data,temp);
		if(error_rate < min_error_rate)
		{
			best_h = temp;
			min_error_rate = error_rate;
		}
	}
	return min_error_rate;
}

//��������㷨��e_out
double e_out(hypothesis &best_h)
{
	return 0.5 + 0.3 * double(best_h.s) * (double)(fabs(best_h.theta) - 1.0);
}

void main()
{
	int i;
	double total_e_in_rate = 0.0;
	double total_e_out_rate = 0.0;

	int seed[5000];
	int j;
	//�������5000������
	for(j = 0;j < 5000;j++)
	{
		seed[j] = rand();
	}
	//�����������5000�����Ӳ���datasize��С��һ���������Ϊѵ����
	//5000�ε��������
	for(i = 0;i < 5000;i++)
	{
		srand(seed[i]);//�����ѵ����
		vector<record> training_data;
		get_rand_data(training_data);//��ȡѵ��������
		get_noise(training_data);//��������
		sort_training_data(training_data);//������ѵ������������
		hypothesis best_h = {0,0};
		double min_error_rate = e_in(training_data,best_h);//����ÿ�ε����в�����42���㷨�����ҳ������㷨�Ĵ�����
		cout << "��" << i << "�ε�����" << endl;
		cout << "min e_in:" << min_error_rate <<endl;
		total_e_in_rate += min_error_rate;//�ۼ�ÿ�ε����в����������㷨�Ĵ�����
		total_e_out_rate += e_out(best_h);//�ۼ�ÿ�ε����в����������㷨�����������
		cout << "e_out:" << e_out(best_h) << endl;

	}

	cout << "the average e_in:" << total_e_in_rate / 5000 << endl;
	cout << "the average e_out:" << total_e_out_rate / 5000 << endl;

	while(1);
}