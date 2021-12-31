# TSP_GA
##描述
TSP_GA C++遗传算法处理TSP问题
##使用的算法
1.  交叉算子 PMXcross
2.  变异算子 Inversionmutate
3.  适值函数 距离
4.  选择策略 PRWselect轮盘赌

## 使用
### clone
```bash
https://github.com/2hanhan/TSP_GA.git
cd TSP_GA
sudo chmod +x build.sh
```
### 修改参数
可以更改main.cpp中以下部分修改参数
```c++
#define CITY_NUM 31        //城市数量
#define POSE_DIM 2         //坐标维度
#define GEN_NUM 5000       //迭代次数
#define INDIVIDUAL_NUM 500 //种群数量
#define MUTATE_PROB 0.1    //变异比例
```
### 运行
```bash
./build.sh
./main
```

## 结果
保存在result的表格中
