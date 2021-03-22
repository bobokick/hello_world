# 智能计算第三周作业题解

## 问题描述：
##### 给出一个图，求出给定起点，终点及其必须所经历的点和边，求出其最佳路径
![map3](https://user-images.githubusercontent.com/62290022/111962958-f3806800-8b2d-11eb-9e7b-fdecd229cc8a.jpg)
## 问题分析：
##### 从给出的图来看，我们可以发现：
- 图的形式是有向图和无向图的混合
- 图的某些顶点有多条边
- 图的边都有颜色信息
##### 从给出的图以及限定条件来看，我们可以发现以下要求
- 路径必须要从给定的起点到给定的终点
- 路径要经过给定的节点(起点和终点也算在给定点的其中)
- 路径要经过给定的边，不能多于也不能少于

## 思路过程
##### 由于本题是关于图的路径选择问题，我们就能够想到使用一些基础的图算法，如广度优先搜索算法（BFS），接下来我们就使用BFS算法来求出给定条件的最佳路径

### 1. 算法构造
##### BFS算法的核心思维也就是从起点开始，不断的寻找当前顶点的邻居，每次寻找一个邻居后，将其邻居加入路径中，并判断当前的路径是否满足给定的条件，满足则输出其路径，结束寻找，否则将邻居以及对应的路径加入到队列中，再从队列中找出下一个顶点继续寻找。

##### 1.1 我们首先需要构造图模型，既然需要构造图模型，就需要用一种图表示法来表示图的各种信息，由于图有多重边，用邻接矩阵的表示方法比较难以实现，所以我们可以用邻接表来表示，在每个顶点链接其每个邻居的信息。为了方便起见，我构造了邻居类和顶点类，来表示其顶点及邻居的信息

###### 以下是邻接表的构造
```
// 记录顶点的某邻居名及关联的边
    class Neighbor_info
    {
    friend class Graphs;
        string neighbor_name;
        vector<string> pass_edges;
        static Neighbor_info neighbor(string neighbor_name, vector<string> pass_edges)
        {
            Neighbor_info neighbor(neighbor_name, pass_edges);
            return neighbor;
        }
    public:
        Neighbor_info(string neighbor_name = {}, vector<string> pass_edges = {}): neighbor_name(neighbor_name), pass_edges(pass_edges) {}
    };
    // 记录顶点名及其邻居
    class Vertex_info
    {
    friend class Graphs;
        string vertex_name;
        vector<Neighbor_info> neighbors;
        static Vertex_info vertex(string vertex, vector<string> neighbors_vers, vector<vector<string>> neighbors_sides)
        {
            vector<Neighbor_info> neighs;
            for (vector<string>::size_type neigh = 0; neigh < neighbors_vers.size(); ++neigh)
            {
                Neighbor_info ne_info(neighbors_vers[neigh], neighbors_sides[neigh]);
                neighs.push_back(ne_info);
            }
            Vertex_info ver_info(vertex, neighs);
            return ver_info;
        }
        static Vertex_info vertex(string vertex_name, vector<Neighbor_info> neighbors)
        {
            Vertex_info vertex(vertex_name, neighbors);
            return vertex;
        }
    public:
        Vertex_info(string vertex_name = {}, vector<Neighbor_info> neighbors = {}): vertex_name(vertex_name), neighbors(neighbors) {}
    };
```

##### 1.2 构造BFS算法，我们需要记录每次遍历某顶点的路径，路径信息包括顶点信息和边信息，所以我们可以构造一个路径类来记录路径

###### 以下是路径的构造
```
// 记录当前路径的信息
    class Path_info
    {
    friend class Graphs;
        string end_point;
        vector<string> vertex_path;
        vector<string> edge_path;
    public:
        Path_info(string end_point = {}, vector<string> vertex_path = {}, vector<string> edge_path = {}): end_point(end_point), vertex_path(vertex_path), edge_path(edge_path) {}
    };
```

##### 1.3 构造BFS算法之前还需要从一个顶点来寻找其邻居并构建相应的路径

###### 以下是从顶点来构造路径
```
static vector<Path_info> build_path(Vertex_info &vertex, vector<string> ver_path, vector<string> &edg_path)
    {
        vector<Path_info> path_list{};
        vector<string> edgs;
        ver_path.push_back(vertex.vertex_name);
        for (auto neighbor: vertex.neighbors)
            for (auto edge: neighbor.pass_edges)
            {
                edgs = edg_path;
                edgs.push_back(edge);
                Path_info path(neighbor.neighbor_name, ver_path, edgs);
                path_list.push_back(path);
            }
        return path_list;
    }
```

##### 1.4 构造BFS算法之前还需要判断路径是否满足的算法

###### 以下是判断路径算法
```
// 判断两列表之间的包含关系
    static bool contained_judge(vector<string> &list, vector<string> sublist)
    {
        map<string, int> dict;
        map<string, int> subdict;
        vector<string>::size_type list_size = list.size();
        vector<string>::size_type sublist_size = sublist.size();
        if (sublist.size() == 0)
            return true;
        else if (sublist.size() > list.size())
            return false;
        for (vector<string>::size_type count = 0; count < list_size; ++count)
        {
            if (count < sublist_size)
                subdict[sublist[count]] += 1;
            dict[list[count]] += 1;
        }
        for (auto val: sublist)
            if (dict.count(val) > 0)
            {
                if (dict[val] < subdict[val])
                    return false;
            }
            else
                return false;
        return true;
    }
```

```
// 求出两个列表的交集
    static vector<string> intersection(vector<string> list, vector<string> list2)
    {
        map<string, int> dict;
        map<string, int> dict2;
        vector<string> temp_list{};
        vector<string>::size_type list_size = list.size();
        vector<string>::size_type list2_size = list2.size();
        if (list_size == 0 && list2_size == 0)
            return temp_list;
        else if (list_size == 0)
            return list2;
        else if (list2_size == 0)
            return list;
        for (vector<string>::size_type i = 0; i < list_size || i < list2_size; ++i)
        {
            if (i < list_size)
                dict[list[i]] += 1;
            if (i < list2_size)
                dict2[list2[i]] += 1;
        }
        for (auto val: dict)
        {
            unsigned size = val.second;
            for (unsigned i = 0; i < size; ++i)
                temp_list.push_back(val.first);
        }
        for (auto val: dict2)
        {
            unsigned size = 0;
            if (dict.count(val.first) == 0)
                size = val.second;
            else if (val.second > dict[val.first])
                size = val.second - dict[val.first];
            for (unsigned i = 0; i < size; ++i)
                temp_list.push_back(val.first);
        }
        return temp_list;
    }
```

```
// 判断当前路径是否符合约束条件，有严格包含判断（equal）和不严格包含判断（more and equal）
    static bool path_judge_me(string &ends, vector<string> &required_vers, vector<string> &required_edges, Path_info &temp_path)
    {
        if (temp_path.end_point == ends && contained_judge(temp_path.vertex_path, required_vers) && contained_judge(temp_path.edge_path, required_edges))
            return true;
        else
            return false;
    }
    static bool path_judge_e(string &ends, vector<string> &required_vers, vector<string> &required_edges, Path_info &temp_path, unsigned max_amout)
    {
        if (temp_path.end_point != ends || temp_path.vertex_path.size() != max_amout - 1 || temp_path.edge_path.size() != max_amout - 1)
            return false;
        else if (contained_judge(temp_path.vertex_path, required_vers) && contained_judge(temp_path.edge_path, required_edges))
            return true;
        else
            return false;
    }
    // 判断给定的约束条件是否有效
    static bool exists_judge(string starts, string ends, vector<string> &required_vetex, vector<string> &required_edge, vector<Vertex_info> &maps)
    {
        vector<string> temp;
        vector<string> vertexs_list{};
        map<string, int> edge_dict;
        for (auto ver: maps)
            for (auto neib: ver.neighbors)
                for (auto ed: neib.pass_edges)
                    edge_dict[ed] = 1;
        for (auto val: maps)
            vertexs_list.push_back(val.vertex_name);
        if (!(contained_judge(vertexs_list, {starts}) && contained_judge(vertexs_list, {ends})))
            return false;
        if (required_vetex.size() != 0)
        {
            for (auto val: required_vetex)
                if (!contained_judge(vertexs_list, {val}))
                    return false;
        }
        if (required_edge.size() != 0)
        {
            for (auto val: required_edge)
                if (edge_dict.count(val) == 0)
                    return false;
        }
        return true;
    }
```

##### 1.5 加上队列用来保存之前遍历的路径以便进行下一个寻找后，我们就可以构造BFS算法了

```
// 求某图约束条件下的最短路径，可选择判断模式，默认不严格判断（equal or more and equal）
    static Optimal_path find_optimal_path(string &starts, string &ends, vector<string> &required_vetex, vector<string> &required_edge, vector<Vertex_info> maps, bool strict_judge = false)
    {
        deque<Path_info> queue;
        Optimal_path optimal_path{};
        string find_ver;
        vector<string> ver_path{};
        vector<string> edg_path{};
        Path_info temp_path;
        vector<Path_info> temp_list;
        unsigned mark = 0;
        bool path_judge;
        unsigned max_amout;
        if (required_vetex.size() + 2 > required_edge.size() + 1)
            max_amout = required_vetex.size() + 2;
        else
            max_amout = required_edge.size() + 1;
        if (!exists_judge(starts, ends, required_vetex, required_edge, maps))
            return optimal_path;
        while (!queue.empty() || mark == 0)
        {
            if (mark == 0)
            {
                mark = 1;
                find_ver = starts;
            }
            else
            {
                temp_path = queue[0];
                if (!strict_judge)
                    path_judge = path_judge_me(ends, required_vetex, required_edge, temp_path);
                else
                {
                    if (temp_path.vertex_path.size() > max_amout - 1 || temp_path.edge_path.size() > max_amout - 1)
                        return optimal_path;
                    else
                        path_judge = path_judge_e(ends, required_vetex, required_edge, temp_path, max_amout);
                }
                if (path_judge)
                {
                    optimal_path = temp_path;
                    return optimal_path;
                }
                else
                {
                    find_ver = temp_path.end_point;
                    ver_path = temp_path.vertex_path;
                    edg_path = temp_path.edge_path;
                    queue.pop_front();
                }
            }
            for (auto val: maps)
            {
                if (find_ver == val.vertex_name)
                {
                    temp_list = build_path(val, ver_path, edg_path);
                    for (auto val: temp_list)
                        queue.push_back(val);
                    break;
                }
            }
        }
        return optimal_path;
    }
```

### 算法分析
- 最坏时间复杂度: O(max(V, E, LV, LE)) (V, E, LV, LE分别是图的顶点数，边数, 约束顶点数，约束边数)
- 最坏空间复杂度: O(max(v)!) (v是图中顶点的最大度数)
- 使用广度优先搜索方法实现对给定约束条件下的最短路径（遍历点数最少）搜索，适用于混合图（包括环和多重边），不适用于加权图
