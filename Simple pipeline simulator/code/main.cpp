#include <iostream>
#include <vector>
#include <string>
#include "simulator.h"
using namespace std;

// generate code
inst_item t1 = inst_item("add", 0, 1, 2);   // add $t0 $t1 $t2
inst_item t2 = inst_item("load", 4, 5, 1);  // load $t4 $t5 1
inst_item t3 = inst_item("add", 7, 8, 9);   // add $t7 $t8 $t9
inst_item t4 = inst_item("add", 4, 0, 3);   // add $t4 $t0 $t3
inst_item t5 = inst_item("add", 1, 2, 4);   // add $t1 $t2 $t4
inst_item t6 = inst_item("beqz", 0, -1, 1); // beqz $t0 1

/*
code1:
add $t0 $t1 $t2
load $t4 $t5 1
add $t7 $t8 $t9
*/
vector<inst_item> code1 = {t1, t2, t3};
/*
code2:
add $t0 $t1 $t2
add $t4 $t0 $t3
load $t4 $t5 1
*/
vector<inst_item> code2 = {t1, t4, t2};

/*
code3:
load $t4 $t5 1
add $t1 $t2 $t4
add $t0 $t1 $t2
add $t7 $t8 $t9
*/
vector<inst_item> code3 = {t2, t5, t1, t3};

/*
code4:
add $t0 $t1 $t2
beqz $t0 1
load $t4 $t5 1
add $t7 $t8 $t9
*/
vector<inst_item> code4 = {t1, t6, t2, t3};

int main()
{
    // 没有流水线冲突的场景
    simulator sim1;
    sim1.load_code(code1, false);
    sim1.run();
    sim1.show_pipeline("sim1.txt");

    // 有RAW冲突的场景，不使用前向传递
    simulator sim2_1;
    sim2_1.load_code(code2, false);
    sim2_1.run();
    sim2_1.show_pipeline("sim2_1.txt");
    // 有RAW冲突的场景，使用前向传递
    simulator sim2_2;
    sim2_2.load_code(code2, true);
    sim2_2.run();
    sim2_2.show_pipeline("sim2_2.txt");

    // 有RAW冲突的场景，不使用前向传递
    simulator sim3_1;
    sim3_1.load_code(code3, false);
    sim3_1.run();
    sim3_1.show_pipeline("sim3_1.txt");
    // 有RAW冲突的场景，使用前向传递
    simulator sim3_2;
    sim3_2.load_code(code3, true);
    sim3_2.run();
    sim3_2.show_pipeline("sim3_2.txt");
    sim3_2.statistic("statistic_sim3_2.txt");

    // 运行到断点的演示
    simulator sim3_3;
    sim3_3.load_code(code3, true);
    sim3_3.run_to_bp(3, "EX");
    sim3_3.show_pipeline("sim3_3.txt");
    

    // 有分支的演示
    simulator sim4;
    sim4.load_code(code4, true);
    sim4.run();
    sim4.show_pipeline("sim4.txt");
    sim4.statistic("statistic_sim4.txt");

    // 命令交互功能
    simulator sim_inter;
    sim_inter.load_code(code1, false);
    char c = 'a';
    while (c != 'q')
    {
        cout << "single cycle(n):" << endl;
        cout << "run to the end(r):" << endl;
        cout << "run to break point(b)" << endl;
        cin >> c;
        switch (c)
        {
        case 'n':
            sim_inter.run_next();
            sim_inter.show_pipeline("sim_inter.txt");
            break;
        case 'r':
            sim_inter.run();
            sim_inter.show_pipeline("sim_inter.txt");
            break;
        case 'b':
            int to_inst;
            string to_stage;
            cout << "to which instruction:" << endl;
            cin >> to_inst;
            cout << "to which stage:" << endl;
            cin >> to_stage;
            sim_inter.run_to_bp(to_inst, to_stage);
            sim_inter.show_pipeline("sim_inter.txt");
            break;
        }
    }

    return 0;
}
