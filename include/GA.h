#ifndef GA_H_ICLUDE
#define GA_H_ICLUDE

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <functional>

//城市类
class City
{
public:
    //计算城市之间的距离
    void compute_distance(std::vector<City> &Citys);
    //城市编号
    int n = -1;
    // 坐标维度
    int dim;
    //城市位置
    std::vector<double> pose;
    //与其他城市的距离
    std::vector<double> distance;
};

//个体类
class Individual
{
public:
    //初始化基因
    Individual(int num);
    //计算适值
    void evaluate_fitness(std::vector<City> &Citys);
    //基因染色体编码
    std::vector<int> genes;
    //适值
    double fitness_max;
    double fitness;
};

class GA
{
public:
    //设置遗传算法参数
    GA(std::vector<City> &Citys, int Individual_num, int gen_num, double mutate_prob);
    //获取当前最优个体
    Individual get_best();
    //迭代训练
    void train(std::vector<Individual> &best_generation);
    //交叉算子
    void PMXcross();
    //变异算子
    void Inversionmutate();
    //选择下一代
    void PRWselect();

    //城市数据
    std::vector<City> Citys;
    //变异比例
    double mutate_prob;
    //种群个体数目
    int Individual_num;
    //最大迭代次数
    int gen_num;
    //当前种群
    std::vector<Individual> generation;
    //当前迭代次数
    int iter = 0;
    //最优的个体在当前种群的编号
    int best;
};

#endif // GA_H_ICLUDE