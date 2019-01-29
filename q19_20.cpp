#include<iostream>
#include<fstream>
#include<vector>
#include<math.h>
#include<algorithm>

using namespace std;

/*
����Ҫ����������ǣ��������ݼ��Ͳ������ݼ�����9ά�����ݣ�Ҫ��ֱ��ó�ÿһά
����ʹ�ò�ͬ��hyposesis�������ݵ�e_in��Ȼ����ÿһά���ҳ�һ����õ�hypothesis��
����ڵõ���9����õĵ�hypothesis�У���ѡȡһ����õģ���Ϊȫ�����ŵ�hypothesis��
��¼��ʱ��hypothesis�Ĳ���������ά���Լ�e_in����Ȼ��ʹ�ø�hypothesis�Բ��Լ��е�
���ݽ��в��ԣ�������e_out��
*/

#define DEMENSION 9//ԭ���ݲ��Լ���9ά����

//���Լ���ѵ���������ݽṹ�壺ԭʼ����+��Ӧ��ǩֵ
struct record
{
	double x[DEMENSION];
	int y;
};

//��ά���ݽṹ�壬�ֱ�ȡ��9ά�����е�ÿһά�Ͷ�Ӧ�ı�ǩֵ����µ����ݽṹ
struct single_demension
{
	double x;
	int y;
};

//hypothesis�ṹ�壺����ֵ+�ο���
struct hypothesis
{
	int s;
	double theta;
};

//��ȡԭʼ���ݼ�����������ת��Ϊ��������
void get_data(fstream &data_file,vector<record> &data)
{
	while(!data_file.eof())
	{
		record temp;
		int i;
		for(i = 0;i < DEMENSION;i++)
			data_file >> temp.x[i];
		data_file  >> temp.y;
		data.push_back(temp);
	}
	data_file.close();
}

int sign(double x)
{
	if(x <= 0)
		return -1;
	else
		return 1;
}

//������ÿһά���������е�����ֵ�ıȽϺ���
bool my_compare(single_demension &v1,single_demension &v2)
{
	return v1.x < v2.x;
}

void sort_data(vector<single_demension> &data)
{
	sort(data.begin(),data.end(),my_compare);
}

//����9ά�����еĵ���ĳһά�����ڶ�Ӧhypothesis�µĴ��������������������ʾ9ά��������������
double calculate_error(vector<single_demension> &data,hypothesis &h,int n)
{
	int i;
	int error = 0;
	for(i = 0;i < n;i++)
	{
		int temp = h.s * sign(data[i].x - h.theta);
		if(temp != data[i].y)
			error++;
	}
	return error / double(n);
}

double e_in(vector<record> &training_data,hypothesis &best_h,int &best_demension,int training_data_size)
{
	int i;
	double min_error_rate = 1.0;
	hypothesis temp_h = {0,0};
	single_demension temp_d = {0,0};
	//�����9ά�е�ÿһά
	for(i = 0;i < DEMENSION;i++)
	{
		vector<single_demension> single_demension;
		//�������ÿһά����������������ϳ��µ�����������Ϊ���ݼ����Ա����ƽ��������
		int j;
		for(j = 0;j < training_data_size;j++)
		{
			temp_d.x = training_data[j].x[i];
			temp_d.y = training_data[j].y;
			single_demension.push_back(temp_d);
		}
		sort(single_demension.begin(),single_demension.end(),my_compare);//������ϵ����ݼ���������

		//����hypothesis������������������ݼ��ϵ�e_in
		for(j = 0;j < training_data_size+1;j++)
		{
			temp_h.s = 1;
			if(j == 0)
				temp_h.theta = (single_demension[0].x - 1.0) / 2.0;
			else if(j == training_data_size)
				temp_h.theta = (single_demension[training_data_size - 1].x + 1.0) / 2.0;
			else
				temp_h.theta = (single_demension[j-1].x +single_demension[j].x) / 2.0;

			double error_rate = calculate_error(single_demension,temp_h,training_data_size);
			if(error_rate < min_error_rate)
			{
				best_h = temp_h;
				min_error_rate = error_rate;
				best_demension = i + 1;//i��0������ѵ�����ݵ�ά�ȴ�1����
			}
		}

		for(j = 0;j < training_data_size+1;j++)
		{
			temp_h.s = -1;
			if(j == 0)
				temp_h.theta = (single_demension[0].x - 1.0) / 2.0;
			else if(j == training_data_size)
				temp_h.theta = (single_demension[training_data_size - 1].x + 1.0) / 2.0;
			else
				temp_h.theta = (single_demension[j-1].x + single_demension[j].x) / 2.0;

			double error_rate = calculate_error(single_demension,temp_h,training_data_size);
			if(error_rate < min_error_rate)
			{
				best_h = temp_h;
				min_error_rate = error_rate;
				best_demension = i + 1;
			}
		}
	}
	return min_error_rate;
}

//ʹ���ҵ�������hypothesis�Բ��Լ�������֤������e_out
double e_out(vector<record> test_data,int best_demension,hypothesis best_h,int test_data_size)
{
	int j;
	vector<single_demension> best_demension_data;
	for(j = 0;j < test_data_size;j++)
	{
		single_demension temp_td = {0,0};
		temp_td.x = test_data[j].x[best_demension-1];
		temp_td.y = test_data[j].y;
		best_demension_data.push_back(temp_td);
	}
	return calculate_error(best_demension_data,best_h,test_data_size);
}

void main()
{
	vector<record> training_data;
	vector<record> test_data;
	fstream data_file1("C://Users/Administrator/Desktop/̨�����������ѧϰ��ʯ��Դ����/work/work_2/data/train_data.txt");
	fstream data_file2("C://Users/Administrator/Desktop/̨�����������ѧϰ��ʯ��Դ����/work/work_2/data/test_data.txt");
	if(data_file1.is_open() && data_file2.is_open())
	{
		get_data(data_file1,training_data);
		get_data(data_file2,test_data);
	}
	else
	{
		cout << "the data file is error!" << endl;
		exit(1);
	}
	int training_data_size = training_data.size();
	int test_data_size = test_data.size();
	hypothesis best_h = {0,0};
	int best_demension = 0;
	//Ѱ������hypothesis���������Ӧά�����ݵ�e_in
	double error_in = e_in(training_data,best_h,best_demension,training_data_size);
	//ʹ����������ά�ȵ�hypothesis�Բ��Լ��е�ÿһά���ݶ�������ͬ�Ĳ��ԣ�������ƽ��������e_out
	double error_out = e_out(test_data,best_demension,best_h,test_data_size);
	cout << "���ά�ȣ�" << best_demension << ",��Сe_in��" << error_in << "���Ӧ��e_outΪ��" << error_out << endl;

	while(1);
}
	