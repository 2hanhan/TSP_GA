#include <GA.h>
using namespace std;

void City::compute_distance(std::vector<City> &Citys)
{
    int num;
    num = Citys.size();
    for (int i = 0; i < num; ++i)
    {
        double d = 0;
        for (int j = 0; j < Citys[i].dim; ++j)
        {
            d += sqrt((pose[j] - Citys[i].pose[j]) * (pose[j] - Citys[i].pose[j]));
        }
        distance.push_back(d);
    }
}

Individual::Individual(int num)
{
    //生成随机序列
    for (int i = 0; i < num; ++i)
    {
        genes.push_back(i);
    }
    std::random_shuffle(genes.begin(), genes.end());
}

void Individual::evaluate_fitness(std::vector<City> &Citys)
{
    int num;
    num = Citys.size();

    double d1 = Citys[genes[num - 1]].distance[genes[0]];
    fitness = d1;
    for (int i = 0; i < num - 1; ++i)
    {
        d1 = Citys[genes[i]].distance[genes[i + 1]];
        fitness = fitness + d1;
    }
    fitness_max = 50000 - fitness;
}

GA::GA(std::vector<City> &Citys, int Individual_num, int gen_num, double mutate_prob) : Individual_num(Individual_num), gen_num(gen_num), Citys(Citys), mutate_prob(mutate_prob)
{
    //生成初始化种群
    for (int i = 0; i < Individual_num; ++i)
    {
        generation.push_back(Individual(Citys.size()));
        generation[i].evaluate_fitness(Citys);
    }
    get_best();
}

Individual GA::get_best()
{
    double min_fitness;
    best = 0;
    min_fitness = generation[0].fitness;
    for (int i = 1; i < Individual_num; ++i)
    {
        if (min_fitness > generation[i].fitness)
        {
            min_fitness = generation[i].fitness;
            best = i;
        }
    }

    std::cout << "当前迭代次数：" << iter << std::endl;
    /*
    std::cout << "最优解为：";
    for (int i = 0; i < Citys.size(); ++i)
    {
        std::cout << generation[best].genes[i] << "->";
    }
    std::cout << generation[best].genes[0] << std::endl;
    std::cout << "最佳适值为：" << generation[best].fitness << std::endl;
*/
    return generation[best];
}

void GA::train(std::vector<Individual> &best_generation)
{
    double best_fitness;
    int max_same = 200; // 50次不变结束迭代
    int same_num = 0;
    for (int i = 0; i < gen_num; ++i)
    {
        iter = i;
        //交叉
        PMXcross();
        //变异
        Inversionmutate();
        //适值函数
        for (int j = 0; j < generation.size(); ++j)
        {
            generation[j].evaluate_fitness(Citys);
        }
        //选择
        PRWselect();
        //保存每次迭代的最优结果
        best_generation.push_back(get_best());

        //结果不变换结束循环
        if (best_fitness == best_generation.back().fitness)
        {
            ++same_num;
        }
        else
        {
            same_num = 0;
        }
        best_fitness = best_generation.back().fitness;
        if (same_num == max_same)
            break;
        std::cout << "......" << std::endl;
    }
}

void GA::PMXcross()
{
    int num;
    num = Citys.size();

    //打乱顺序
    std::random_shuffle(generation.begin(), generation.end());
    for (int i = 0; i < Individual_num; i = i + 2)
    {
        Individual daughter(num);
        Individual son(num);

        //随机截取一段
        int s, t;
        std::vector<int> random_min_max;
        for (int j = 0; j < num; ++j)
        {
            random_min_max.push_back(j);
        }
        std::random_shuffle(random_min_max.begin(), random_min_max.end());
        if (random_min_max[0] < random_min_max[1])
        {
            s = random_min_max[0];
            t = random_min_max[1];
        }
        else
        {
            s = random_min_max[1];
            t = random_min_max[0];
        }

        //交换子串
        for (int j = 0; j < num; ++j)
        {
            if (j >= s && j <= t)
            {
                //原始子代1
                son.genes[j] = generation[i + 1].genes[j];
                //原始子代2
                daughter.genes[j] = generation[i].genes[j];
            }
            else
            {
                son.genes[j] = generation[i].genes[j];
                daughter.genes[j] = generation[i + 1].genes[j];
            }
        }

        //映射关系合法化
        for (int j = 0; j < num; ++j)
        {
            bool re = false;
            if (j >= s && j <= t)
            {
                continue;
            }
            for (int i1 = s; i1 <= t; ++i1)
            {
                if (son.genes[j] == son.genes[i1])
                {
                    son.genes[j] = daughter.genes[i1];
                    re = true;
                    break;
                }
            }
            if (re)
                j--;
        }
        for (int j = 0; j < num; ++j)
        {
            bool re = false;
            if (j >= s && j <= t)
            {
                continue;
            }
            for (int i1 = s; i1 <= t; ++i1)
            {
                if (daughter.genes[j] == daughter.genes[i1])
                {
                    daughter.genes[j] = son.genes[i1];
                    re = true;
                    break;
                }
            }
            if (re)
                j--;
        }
        //展示父子关系
        /*
        std::cout << s << ", " << t << std::endl;

        std::cout << "父代" << std::endl;
        for (int j = 0; j < Citys.size(); ++j)
        {
            std::cout << generation[i + 1].genes[j] << "->";
        }
        std::cout << generation[i + 1].genes[0] << std::endl;

        for (int j = 0; j < Citys.size(); ++j)
        {
            std::cout << generation[i].genes[j] << "->";
        }
        std::cout << generation[i].genes[0] << std::endl;

        std::cout << "子代" << std::endl;
        for (int j = 0; j < Citys.size(); ++j)
        {
            std::cout << daughter.genes[j] << "->";
        }
        std::cout << daughter.genes[0] << std::endl;

        for (int j = 0; j < Citys.size(); ++j)
        {
            std::cout << son.genes[j] << "->";
        }
        std::cout << son.genes[0] << std::endl;
        */

        generation.push_back(daughter);
        generation.push_back(son);
    }
}

void GA::Inversionmutate()
{
    int num;
    num = Citys.size();

    //打乱顺序
    std::random_shuffle(generation.begin(), generation.end());
    for (int i = 0; i < generation.size(); ++i)
    {
        //随机截取一段
        int s, t;
        std::vector<int> random_min_max;
        for (int j = 0; j < num; ++j)
        {
            random_min_max.push_back(j);
        }
        std::random_shuffle(random_min_max.begin(), random_min_max.end());
        if (random_min_max[0] < random_min_max[1])
        {
            s = random_min_max[0];
            t = random_min_max[1];
        }
        else
        {
            s = random_min_max[1];
            t = random_min_max[0];
        }

        //反转变异
        for (int j = 0; j < mutate_prob * generation.size(); j++)
        {
            /*
            std::cout << "变异前" << std::endl;
            for (int j1 = 0; j1 < Citys.size(); ++j1)
            {
                std::cout << generation[j].genes[j1] << "->";
            }
            std::cout << generation[j].genes[0] << std::endl;
            */

            std::vector<int> mutate_genes;
            for (int i1 = s; i1 <= t; ++i1)
            {
                mutate_genes.push_back(generation[j].genes[i1]);
            }
            for (int i1 = 0; i1 < mutate_genes.size(); ++i1)
            {
                generation[j].genes[t - i1] = mutate_genes[i1];
            }
            /*
            std::cout << "变异后" << std::endl;
            for (int j1 = 0; j1 < Citys.size(); ++j1)
            {
                std::cout << generation[j].genes[j1] << "->";
            }
            std::cout << generation[j].genes[0] << std::endl;
            */
        }
    }
}

bool lessmark(const Individual &Individual1, const Individual &Individual2)
{
    return Individual1.fitness < Individual2.fitness;
}

void GA::PRWselect()
{
    int group_num = 10; // 小组数
    int group_size;     //小组人数
    int group_winner;   //小组获胜人数
    group_winner = Individual_num / group_num;
    group_size = generation.size() / group_num;
    //复制一份
    std::vector<Individual> generation_copy;

    //打乱顺序
    std::random_shuffle(generation.begin(), generation.end());
    /*
    std::cout << "更新前：" << std::endl;
    for (int i = 0; i < generation.size(); ++i)
    {
        for (int j1 = 0; j1 < Citys.size(); ++j1)
        {
            std::cout << generation[i].genes[j1] << "->";
        }
        std::cout << generation[i].genes[0] << std::endl;
    }
    */
    for (int i = 0; i < group_num; ++i)
    {
        //排序
        std::vector<Individual>::iterator Individual_s;
        std::vector<Individual>::iterator Individual_t;
        Individual_s = generation.begin() + i * group_size;
        Individual_t = generation.begin() + (i + 1) * group_size;
        sort(Individual_s, Individual_t, lessmark);
        //保存优胜者
        for (int j = 0; j < group_winner; ++j)
        {
            generation_copy.push_back(generation[j + i * group_size]);
        }
    }
    generation.clear();

    //更新
    for (int j = 0; j < generation_copy.size(); ++j)
    {
        generation.push_back(generation_copy[j]);
    }
    /*
    std::cout << "更新后：" << std::endl;
    for (int i = 0; i < generation.size(); ++i)
    {
        for (int j1 = 0; j1 < Citys.size(); ++j1)
        {
            std::cout << generation[i].genes[j1] << "->";
        }
        std::cout << generation[i].genes[0] << std::endl;
    }
    */
}
