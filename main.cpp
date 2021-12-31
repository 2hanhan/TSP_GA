#include <iostream>
#include <vector>
#include <GA.h>
//保存结果
#include <chrono>
#include <fstream>
#include <sstream>

#define CITY_NUM 31        //城市数量
#define POSE_DIM 2         //坐标维度
#define GEN_NUM 5000       //迭代次数
#define INDIVIDUAL_NUM 500 //种群数量
#define MUTATE_PROB 0.1    //变异比例
using namespace std;

//模板函数：将string类型变量转换为常用的数值类型（此方法具有普遍适用性）
template <class Type>
Type stringToNum(const string &str)
{
    istringstream iss(str);
    Type num;
    iss >> num;
    return num;
}

//随机初始化
void init_citys(std::vector<City> &Citys, int dim)
{
    int num;
    num = Citys.size();
    // 产生随机种子  把0换成NULL也行
    srand(time(0));
    //初始化城市位置
    for (int i = 0; i < num; ++i)
    {
        //生成0-100之间的随机数
        Citys[i].dim = dim;
        for (int j = 0; j < dim; ++j)
        {
            Citys[i].pose.push_back(drand48() * 500);
        }
        Citys[i].n = i;
    }
    //计算城市间的距离
    for (int i = 0; i < num; ++i)
    {
        std::cout << "城市：" << i << "位置:"
                  << "(";

        for (int j = 0; j < Citys[i].dim; ++j)
        {
            std::cout << Citys[i].pose[j];
        }

        std::cout << ")" << std::endl;

        Citys[i].compute_distance(Citys);

        for (int j = 0; j < num; ++j)
        {
            std::cout << "距离城市" << j << "距离" << Citys[i].distance[j] << std::endl;
        }
    }
}

//读取city0.csv文件中的城市数据
void read_city(std::vector<City> &Citys)
{
    ifstream inFile("city0.csv", ios::in);
    std::string lineStr;
    std::vector<std::vector<std::string>> strArray;
    while (getline(inFile, lineStr))
    {
        // 存成二维表结构
        std::stringstream ss(lineStr);
        std::string str;
        std::vector<std::string> lineArray;
        // 按照逗号分隔
        while (getline(ss, str, ','))
            lineArray.push_back(str);
        strArray.push_back(lineArray);
    }
    int num = Citys.size();
    //读取坐标
    for (int i = 0; i < num; ++i)
    {

        Citys[i].pose.push_back(stringToNum<double>(strArray[i][1]));
        Citys[i].pose.push_back(stringToNum<double>(strArray[i][2]));
        Citys[i].n = i;
        Citys[i].dim = 2;
    }
    //计算城市间的距离
    for (int i = 0; i < num; ++i)
    {
        std::cout << "城市：" << i << "位置:"
                  << "(";

        for (int j = 0; j < Citys[i].dim; ++j)
        {
            std::cout << Citys[i].pose[j];
        }

        std::cout << ")" << std::endl;

        Citys[i].compute_distance(Citys);

        for (int j = 0; j < num; ++j)
        {
            std::cout << "距离城市" << j << "距离" << Citys[i].distance[j] << std::endl;
        }
    }
}

//保存城市数据
void save_city(std::vector<City> &Citys)
{
    ofstream outFile;
    outFile.open("city.csv", ios::out);
    // 保存城市信息
    outFile << endl;
    for (int i = 0; i < Citys.size(); ++i)
    {
        outFile << "城市编号" << ',' << i << endl;
        outFile << "城市坐标";
        for (int j = 0; j < Citys[i].pose.size(); j++)
        {
            outFile << ',' << Citys[i].pose[j];
        }
        outFile << endl;
        outFile << "与城市";
        for (int j = 0; j < Citys.size(); ++j)
        {
            outFile << "与城市" << j << "距离" << ',' << Citys[i].distance[j] << std::endl;
        }
    }
}

//保存实验结果
void save_data(std::vector<Individual> &best_generation,
               int gen_num,
               int indvidual_num,
               double mutate_prob,
               ofstream &results)
{
    //保存结果信息
    results << "迭代次数" << ',' << gen_num << ',' << "种群数量" << ',' << indvidual_num << ',' << "变异比例" << ',' << mutate_prob << endl;
    results << "迭代次数" << ',' << "适值" << ',' << "路径" << endl;
    for (int i = 0; i < best_generation.size(); ++i)
    {
        results << i << ',' << best_generation[i].fitness << ',' << best_generation[i].fitness_max << ',';
        for (int j = 0; j < best_generation[i].genes.size(); ++j)
        {
            results << best_generation[i].genes[j] << ',';
        }
        results << best_generation[i].genes[0] << endl;
    }
}

int main(int argc, char **argv)
{
    ofstream results;
    results.open("result.csv", ios::out);

    int num = CITY_NUM;
    int dim = POSE_DIM;
    int indvidual_num = INDIVIDUAL_NUM;
    int gen_num = GEN_NUM;
    double mutate_prob = MUTATE_PROB;

    //创建城市模型
    std::vector<City> Citys(num);
    //随机初始化city
    // init_citys(Citys, dim);
    //读取city
    read_city(Citys);
    save_city(Citys);

    //部分城市
    /*
    std::vector<City> Citys_lest;
    std::vector<City> Citys_less;
    for (int i = 0; i < num / 3; i++)
    {
        Citys_lest.push_back(Citys[i]);
    }

    for (int i = 0; i < 2 * num / 3; i++)
    {
        Citys_less.push_back(Citys[i]);
    }
    */
    std::vector<Individual> best_generation;

    for (int i = 1; i < 21; ++i)
    {
        //
        indvidual_num = INDIVIDUAL_NUM * i;

        //种群初始化
        GA ga(Citys,
              indvidual_num, //种群数量
              gen_num,       //迭代数量
              mutate_prob);  //变异比例

        //迭代计算
        chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
        ga.train(best_generation);
        chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
        chrono::duration<double> time_used = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
        results << time_used.count() << ',' << "秒" << endl;

        //保存结果数据
        results << endl;
        save_data(best_generation,
                  gen_num,
                  indvidual_num,
                  mutate_prob,
                  results);

        //清空最优保存列表
        best_generation.clear();
    }
}