#ifndef PATH_SEARCH_H
#define PATH_SEARCH_H
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <deque>
using namespace std;

// 最坏时间复杂度: O(max(V, E, LV, LE)) (V, E, LV, LE分别是图的顶点数，边数, 约束顶点数，约束边数)
// 最坏空间复杂度: O(max(v)!) (v是图中顶点的最大度数)
// 使用广度优先搜索方法实现对给定约束条件下的最短路径（遍历点数最少）搜索，适用于混合图（包括环和多重边），不适用于加权图
// 建立一个图类，可以对图对象进行设置顶点以及求约束条件下的最短路径
class Graphs
{
friend class Test_case;
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
    // 记录约束条件下的最短路径及显示格式
    class Optimal_path
    {
    friend class Graphs;
        vector<string> opt_vertexs;
        vector<string> opt_edges;
    public:
        Optimal_path(Path_info opt_path = {}): opt_vertexs({}), opt_edges(opt_path.edge_path)
        {
            opt_path.vertex_path.push_back(opt_path.end_point);
            opt_vertexs = opt_path.vertex_path;
        }
        void display_path() const
        {
            vector<string>::size_type v_size = opt_vertexs.size();
            vector<string>::size_type e_size = opt_edges.size();
            if (v_size == 0 || e_size == 0)
                cout << "there is no available route!";
            else
            {
                cout << "the optimal route:\n";
                for (vector<string>::size_type count = 0; count < v_size; ++count)
                {
                    if (count + 1 != v_size)
                        cout << "vertex: " << opt_vertexs[count] << " " << "-> ";
                    else
                        cout << "vertex: " << opt_vertexs[count];
                    if (count < e_size)
                        cout << "edge: " <<  opt_edges[count] << " " << "-> \n";
                }
            }
        }
    };
    vector<Vertex_info> vers_list;  // 该图的顶点列表
    // 判断两列表是否相等
    static bool compare_equal(vector<string> &list, vector<string> &list2)
    {
        map<string, int> dict;
        map<string, int> dict2;
        vector<string>::size_type list_size = list.size();
        vector<string>::size_type list2_size = list2.size();
        if (list_size != list2_size)
            return false;
        for (vector<string>::size_type i = 0; i < list_size || i < list2_size; ++i)
        {
            if (i < list_size)
                dict[list[i]] += 1;
            if (i < list2_size)
                dict2[list2[i]] += 1;
        }
        for (auto val: list)
        {
            if (dict2.count(val) != 0)
            {
                if (dict[val] != dict2[val])
                    return false;
            }
            else
                return false;
        }
        return true;
    }
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
    // 求出两个列表的并集
    static vector<string> unions(vector<string> list, vector<string> list2)
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
    // 判断当前路径是否符合约束条件，有严格包含判断（equal）和不严格包含判断（more and equal）
    static bool path_judge_me(string &ends, vector<string> &required_vers, vector<string> &required_edges, Path_info &temp_path)
    {
        if (temp_path.end_point == ends && contained_judge(temp_path.vertex_path, required_vers) && contained_judge(temp_path.edge_path, required_edges))
            return true;
        else
            return false;
    }
    static bool path_judge_e(string &ends, vector<string> &required_vers, vector<string> &required_edges, Path_info &temp_path, unsigned &max_amout)
    {
        vector<string> temp1{temp_path.vertex_path};
        temp1.push_back(ends);
        vector<string> temp2{unions(required_vers, {temp1[0], ends})};
        if (temp_path.end_point != ends || temp_path.vertex_path.size() != max_amout - 1 || temp_path.edge_path.size() != max_amout - 1)
            return false;
        else if (contained_judge(temp1, temp2) && contained_judge(temp_path.edge_path, required_edges))
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
    // 建立从某顶点通往其所有邻居的可行路径
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
    // 求某图约束条件下的最短路径，可选择判断模式，默认严格判断（equal）
    static Optimal_path find_optimal_path(string &starts, string &ends, vector<string> &required_vetex, vector<string> &required_edge, vector<Vertex_info> maps, bool strict_judge = true)
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
public:
    // 初始化图数据
    Graphs(vector<string> vertexs = {}, vector<vector<string>> neighbor_vers = {}, vector<vector<vector<string>>> neighbor_sides = {})
    {
        set_vers_list(vertexs, neighbor_vers, neighbor_sides);
    }
    // 设置图的顶点数据
    void set_vers_list(vector<string> vertexs, vector<vector<string>> neighbor_vers, vector<vector<vector<string>>> neighbor_sides)
    {
        for (vector<string>::size_type ver = 0; ver < vertexs.size(); ++ver)
        {
            vector<Neighbor_info> neighs;
            for (vector<string>::size_type neigh = 0; neigh < neighbor_vers[ver].size(); ++neigh)
            {
                Neighbor_info ne_info(neighbor_vers[ver][neigh], neighbor_sides[ver][neigh]);
                neighs.push_back(ne_info);
            }
            Vertex_info ver_info(vertexs[ver], neighs);
            vers_list.push_back(ver_info);
        }
    }
    void set_vers_list(vector<Vertex_info> vertex_list)
    {
        vers_list = vertex_list;
    }
    // 添加顶点到该图
    void add_vertex(string vertex_name, vector<string> neighbors_vers, vector<vector<string>> neighbors_sides)
    {
        vers_list.push_back(build_vertex(vertex_name, neighbors_vers, neighbors_sides));
    }
    void add_vertex(Vertex_info vertex)
    {
        vers_list.push_back(vertex);
    }
    // 建立顶点对象
    static Vertex_info build_vertex(string vertex_name, vector<string> neighbors_vers, vector<vector<string>> neighbors_sides)
    {
        return Vertex_info::vertex(vertex_name, neighbors_vers, neighbors_sides);
    }
    static Vertex_info build_vertex(string vertex_name, vector<Neighbor_info> neighbors)
    {
        return Vertex_info::vertex(vertex_name, neighbors);
    }
    // 建立邻居对象
    static Neighbor_info build_neighbor(string neighbor_name, vector<string> pass_edges)
    {
        return Neighbor_info::neighbor(neighbor_name, pass_edges);
    }
    // 求该图在给定的约束条件下的最短路径
    void search_optimal_path(string starts, string ends, vector<string> required_vetex = {}, vector<string> required_edge = {}) const
    {
        Optimal_path opt_path = find_optimal_path(starts, ends, required_vetex, required_edge, vers_list);
        opt_path.display_path();
    }
};

// 建立测试类，方便添加测试用例
class Test_case
{
    class Case
    {
    friend class Test_case;
        vector<Graphs::Vertex_info> graph;
        vector<Graphs::Optimal_path> test_list;
    public:
        Case(vector<Graphs::Vertex_info> ver_list = {}): graph(ver_list) {}
        void add_test(string starts, string ends, vector<string> required_vetex, vector<string> required_edge, bool strict_judge = true)
        {
            Graphs::Optimal_path test = Graphs::find_optimal_path(starts, ends, required_vetex, required_edge, graph, strict_judge);
            test_list.push_back(test);
        }
        void pop_test()
        {
            test_list.pop_back();
        }
        void start_test(unsigned test_index) const
        {
            test_list[test_index].display_path();
        }
    };
    vector<Case> case_list;
public:
    Test_case(vector<Case> case_list = {}): case_list(case_list) {}
    Test_case(Case cases) {add_case(cases);}
    void start_case_test(unsigned case_index, unsigned int test_index) const
    {
        cout << "Map" << case_index + 1 << " -> " << "Case" << test_index + 1 << ":\n";
        case_list[case_index].start_test(test_index);
        cout << "\n\n";
    }
    void add_case(Case cases)
    {
        case_list.push_back(cases);
    }
    void add_case_test(unsigned case_index, string starts, string ends, vector<string> required_vetex = {}, vector<string> required_edge = {}, bool strict_judge = true)
    {
        case_list[case_index].add_test(starts, ends, required_vetex, required_edge, strict_judge);
    }
};
#endif