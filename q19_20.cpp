#include<iostream>
#include<fstream>
#include<vector>
#include<math.h>
#include<algorithm>

using namespace std;

/*
程序要解决的问题是，样本数据集和测试数据集都是9维的数据，要求分别拿出每一维
数据使用不同的hyposesis计算数据的e_in，然后在每一维中找出一个最好的hypothesis，
最后在得到的9个最好的的hypothesis中，再选取一个最好的，作为全局最优的hypothesis，
记录此时的hypothesis的参数（所在维度以及e_in）。然后使用该hypothesis对测试集中的
数据进行测试，计算其e_out。
*/

#define DEMENSION 9//原数据测试集是9维数据

//测试集和训练集的数据结构体：原始数据+对应标签值
struct record
{
	double x[DEMENSION];
	int y;
};

//单维数据结构体，分别取出9维数据中的每一维和对应的标签值组成新的数据结构
struct single_demension
{
	double x;
	int y;
};

//hypothesis结构体：符号值+参考点
struct hypothesis
{
	int s;
	double theta;
};

//读取原始数据集，并将数据转化为向量类型
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

//单独的每一维数据向量中的数据值的比较函数
bool my_compare(single_demension &v1,single_demension &v2)
{
	return v1.x < v2.x;
}

void sort_data(vector<single_demension> &data)
{
	sort(data.begin(),data.end(),my_compare);
}

//计算9维数据中的单独某一维数据在对应hypothesis下的错误情况，第三个参数表示9维向量的向量个数
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
	//分离出9维中的每一维
	for(i = 0;i < DEMENSION;i++)
	{
		vector<single_demension> single_demension;
		//分离出的每一维按照向量数重新组合成新的纵向向量作为数据集，以便计算平均错误率
		int j;
		for(j = 0;j < training_data_size;j++)
		{
			temp_d.x = training_data[j].x[i];
			temp_d.y = training_data[j].y;
			single_demension.push_back(temp_d);
		}
		sort(single_demension.begin(),single_demension.end(),my_compare);//对新组合的数据集进行排序

		//定义hypothesis，并计算在新组合数据集上的e_in
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
				best_demension = i + 1;//i从0计数，训练数据的维度从1计数
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

//使用找到的最优hypothesis对测试集进行验证，计算e_out
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
	fstream data_file1("C://Users/Administrator/Desktop/台大林轩田机器学习基石资源汇总/work/work_2/data/train_data.txt");
	fstream data_file2("C://Users/Administrator/Desktop/台大林轩田机器学习基石资源汇总/work/work_2/data/test_data.txt");
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
	//寻找最优hypothesis，并计算对应维度数据的e_in
	double error_in = e_in(training_data,best_h,best_demension,training_data_size);
	//使用上述最优维度的hypothesis对测试集中的每一维数据都进行相同的测试，并计算平均错误率e_out
	double error_out = e_out(test_data,best_demension,best_h,test_data_size);
	cout << "最佳维度：" << best_demension << ",最小e_in：" << error_in << "其对应的e_out为：" << error_out << endl;

	while(1);
}
	