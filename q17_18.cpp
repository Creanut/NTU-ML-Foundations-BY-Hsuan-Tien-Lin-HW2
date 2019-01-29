#include<iostream>
#include<stdlib.h>
#include<vector>
#include<algorithm>
#include<math.h>
#include<stdio.h>

using namespace std;

#define DATASIZE 20 //定义在-1到1区间内的数值点的个数

//训练样本结构体定义，由于是在一维数轴上，所以double类型的训练样本是一维的
struct record
{
	double x;//样本数据
	int y;//样本标签
};

//分类方式结构体定义
/*
结构体中的s为线型分类的方式，可以是选取点的右侧为+1，也可以是选取点的左侧是+1，
所以分类方式中，s可以取的值为+1和-1；
theta表示的是选取点的位置，由于在定义区间内划分成了21个小区间，所以theta可以在
这21个小区间内取值。
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

//生成训练数据集，在区间内随机生成21个点，并计算每个点对应的标签值
/*
生成后的数据需要映射为对应维度的向量
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

//引入噪声，将20%的数据标签值变成原来的相反数
//20%的数据随机抽取
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

//对训练数据集中的数据按照数据的大小进行排序操作
void sort_training_data(vector<record> &training_data)
{
	sort(training_data.begin(),training_data.end(),my_compare);
}

//计算每一种hyposesis的错误率
/*
每种hyposesis的错误率计算方法：
计算训练集中的每一个点与对应算法中所选取的参考点的相对位置：
当前点与参考点做差值，然后取符号值，然后与该算法中对应的s值相乘。
所得结果再与训练集中的数据标签值做比较，如果比较结果不一致，则错误累加器自增。
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

//参数1为输入的数据训练集，参数2为找到的最佳算法，函数返回最佳算法的e_in
double e_in(vector<record> &training_data,hypothesis &best_h)
{
	hypothesis temp;
	double min_error_rate = 1.0;
	int i;
	for(i = 0;i < DATASIZE + 1;i++)
	{
		temp.s = 1;
		//如果是训练集中的最小值，则算法参考点的位置应该在最小值的左侧，下述运算使得参考点小于最小值
		if(i == 0)
			temp.theta = (training_data[0].x - 1.0) / 2.0;
		else if(i == DATASIZE)
		//遇到最大值，则算法参考点的位置应该在最大值的右侧，下述运算使得参考点大于最大值
			temp.theta = (training_data[DATASIZE-1].x + 1.0) / 2.0;
		//其余参考点在数据点之间
		else
			temp.theta = (training_data[i-1].x + training_data[i].x) / 2.0;

		double error_rate = calculate_error(training_data,temp);
		//如果当前算法的错误率比最小错误率还要小，则替换最小错误率，并记录当前算法为最佳算法
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

//计算最佳算法的e_out
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
	//随机产生5000个种子
	for(j = 0;j < 5000;j++)
	{
		seed[j] = rand();
	}
	//用随机产生的5000个种子产生datasize大小的一组随机数作为训练集
	//5000次的随机迭代
	for(i = 0;i < 5000;i++)
	{
		srand(seed[i]);//随机数训练集
		vector<record> training_data;
		get_rand_data(training_data);//获取训练集向量
		get_noise(training_data);//加入噪声
		sort_training_data(training_data);//对数据训练集进行排序
		hypothesis best_h = {0,0};
		double min_error_rate = e_in(training_data,best_h);//计算每次迭代中产生的42中算法，并找出最优算法的错误率
		cout << "第" << i << "次迭代：" << endl;
		cout << "min e_in:" << min_error_rate <<endl;
		total_e_in_rate += min_error_rate;//累加每次迭代中产生的最优算法的错误率
		total_e_out_rate += e_out(best_h);//累加每次迭代中产生的最优算法的输出错误率
		cout << "e_out:" << e_out(best_h) << endl;

	}

	cout << "the average e_in:" << total_e_in_rate / 5000 << endl;
	cout << "the average e_out:" << total_e_out_rate / 5000 << endl;

	while(1);
}