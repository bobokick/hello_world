#include "path_search.h"


int main()
{
    // 添加3个图的数据
    Test_case cases;
    cases.add_case
    ({{{"0", {{"3", {"blue"}},{"1", {"red","green"}}}}, 
    {"1", {{"0", {"red","green"}},{"2", {"blue"}}}}, 
    {"2", {{"1", {"blue"}},{"5", {"green"}}}}, 
    {"3", {{"0", {"blue"}},{"2", {"red"}},{"4", {"green"}}}}, 
    {"4", {{"3", {"green"}},{"5", {"red","blue"}}}}, 
    {"5", {{"4", {"red","blue"}},{"2", {"green"}}}}}});
    cases.add_case
    ({{{"0", {{"1", {"blue"}},{"2", {"red"}},{"4", {"green"}}}}, 
    {"1", {{"0", {"blue"}},{"2", {"green"}},{"5", {"red"}}}}, 
    {"2", {{"0", {"red"}},{"1", {"green"}},{"3", {"blue"}}}}, 
    {"3", {{"2", {"blue"}},{"5", {"green"}},{"4", {"red"}}}}, 
    {"4", {{"0", {"green"}},{"3", {"red"}},{"5", {"blue"}}}}, 
    {"5", {{"1", {"red"}},{"3", {"green"}},{"4", {"blue"}}}}}});
    cases.add_case
    ({{{"0", {{"1", {"blue"}},{"6", {"green"}},{"9", {"red"}}}}, 
    {"1", {{"2", {"red"}},{"3", {"green"}},{"3", {"blue"}}}}, 
    {"2", {{"0", {"green"}}, {"4", {"blue"}},{"5", {"red"}}}}, 
    {"3", {{"1", {"green"}},{"4", {"red"}},{"6", {"blue"}}}}, 
    {"4", {{"1", {"green"}},{"5", {"red"}},{"7", {"blue"}}}}, 
    {"5", {{"2", {"green"}},{"2", {"red"}},{"8", {"blue"}}}}, 
    {"6", {{"0", {"green"}},{"7", {"red"}},{"9", {"blue"}}}}, 
    {"7", {{"3", {"green"}},{"8", {"red"}},{"8", {"blue"}}}}, 
    {"8", {{"4", {"green"}},{"7", {"blue"}},{"9", {"red"}}}}, 
    {"9", {{"0", {"red"}},{"5", {"green"}},{"6", {"blue"}}}}}});

    // 添加3个图对应的测试数据
    cases.add_case_test(0, "4", "0", {"1","5"}, {"red","blue","blue","green"}, 0);
    cases.add_case_test(0, "5", "0", {"4","4"}, {"red","red","red","green","blue"}, 0);

    cases.add_case_test(1, "3", "5", {"0"}, {"red","red","blue","blue"}, 0);
    cases.add_case_test(1, "5", "0", {}, {"red","red","green","green","blue"}, 0);
    cases.add_case_test(1, "5", "0", {"3","4"}, {"red","green","green","green","green","blue"}, 0);
    cases.add_case_test(1, "4", "5", {"0","1","5","5"}, {"red","red","red","green","green","blue"}, 0);

    cases.add_case_test(2, "9", "4", {"0","7"}, {"red","red","green","green","blue"}, 0);
    cases.add_case_test(2, "2", "8", {"0","4"}, {"red","green","green","blue","blue","blue"}, 0);
    cases.add_case_test(2, "5", "8", {"6","9"}, {"red","green","green","green","blue","blue"}, 0);
    cases.add_case_test(2, "3", "0", {"2","4","9"}, {"red","red","red","green","green","blue","blue"}, 0);
    cases.add_case_test(2, "1", "8", {"2","3","6"}, {"red","red","green","green","green","green","blue"}, 0);

    // 按顺序开始执行3个图对应的测试
    cases.start_case_test(0, 0);
    cases.start_case_test(0, 1);

    cases.start_case_test(1, 0);
    cases.start_case_test(1, 1);
    cases.start_case_test(1, 2);
    cases.start_case_test(1, 3);

    cases.start_case_test(2, 0);
    cases.start_case_test(2, 1);
    cases.start_case_test(2, 2);
    cases.start_case_test(2, 3);
    cases.start_case_test(2, 4);
    system("pause");
    return 0;
}