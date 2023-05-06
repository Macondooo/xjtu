#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

// struct of items
struct log_item
{
    int number;   // 记录这是第几条指令
    bool install; // 是否被阻塞
    string inst;
    string stage;
    log_item(int n, bool i, string inst, string stage) : number(n), install(i), inst(inst), stage(stage) {}
};
struct inst_item
{
    string inst; // 指令的内容是什么（load,store,add,beqz）
    /*
    以下分别是三个字段的值，add三个字段都是寄存器的编号
    load/store 最后一个字段是立即数
    beqz 第一个字段是寄存器编号，最后一个字段是立即数
    */
    int d1;
    int d2;
    int d3;
    inst_item(string inst, int d1, int d2, int d3) : inst(inst), d1(d1), d2(d2), d3(d3) {}
};

// simulator class !!!
/*
所有的冲突都是在ID段检测的
beqz分支指令的条件测试和分支目标地址的计算都提前到ID段完成
*/
class simulator
{
public:
    // 和cpu有关的变量
    int reg[10] = {0};      // 分别代表从 $t0 到 $t9的值,随机设置的
    int data_mem[64] = {0}; // 设置了64w的datamem大小

    // 模拟器控制全局变量
    bool redirect = false;  // 是否使用重定向技术
    int cur_inst_num = -1;          // 当前正在执行到第几条指令
    vector<vector<log_item>> logs;  // 记录流水线日志
    vector<inst_item> instructions; // 记录全部的指令
    int count_install = 0;          // 记录stall的个数
    int count_branch = 0;           // 记录branch的个数
    int count_branch_taken = 0;     // 记录branch taken的个数

    // function
    void load_code(vector<inst_item> code, bool flag); // 导入代码，完成初始设置
    void run_next();                                   // 单步执行一个周期
    void run_to_bp(int to_inst, string to_stage);      // 执行到断点
    void run();                                        // 执行到程序结束
    void show_register();                              // 查看流水线寄存器状态
    void show_pipeline(string path);                   // 查看流水线各个段
    void statistic(string path);                       // 程序执行后的统计性能分析
};

void simulator::load_code(vector<inst_item> code, bool flag)
{
    instructions = code;
    redirect = flag;
    cur_inst_num = -1;
    count_install = 0;
    count_branch = 0;
    count_branch_taken = 0;
    return;
}

void simulator::run_next()
{
    vector<log_item> new_log;
    bool install_flag = false; // 如果前面的指令install了，后面的指令也要install,并且不能加入新的指令
    if (!logs.empty())
    {
        vector<log_item> pre_log = logs.back();
        for (int i = 0; i < pre_log.size(); i++) // 是从最早开始执行的指令遍历的
        {
            if (install_flag && pre_log[i].inst != "INSTALL") // 如果前面的指令install了，后面的指令也要install，保证流水线的顺序
                new_log.push_back(log_item(pre_log[i].number, true, pre_log[i].inst, pre_log[i].stage));
            else
            {
                if (pre_log[i].stage == "IF")
                    new_log.push_back(log_item(pre_log[i].number, false, pre_log[i].inst, "ID"));

                if (pre_log[i].stage == "ID") // 上一个周期在ID段，这个周期要判断能不能流出
                {
                    // 需要读的寄存器
                    inst_item cur_inst = instructions[pre_log[i].number];

                    // 对于beqz在ID段，要判断会不会跳转
                    if (pre_log[i].inst == "beqz" && cur_inst.d1 == 0)
                    {
                        count_branch_taken++;
                        cur_inst_num = cur_inst_num + cur_inst.d3 - 1;
                        new_log.push_back(log_item(pre_log[i].number, false, pre_log[i].inst, "EX"));
                        new_log.push_back(log_item(-1, false, "ABORTED", "ABORTED"));
                        i = i + 1;
                        continue;
                    }

                    // 判断有没有RAW冲突
                    int source_reg1 = -1, source_reg2 = -1;
                    if (cur_inst.inst == "add")
                    {
                        source_reg1 = cur_inst.d2;
                        source_reg2 = cur_inst.d3;
                    }
                    else
                        source_reg1 = cur_inst.d2;

                    bool flag = false; // 判断有没有遇到写后读冲突

                    for (int j = i - 1; j >= 0 && pre_log[j].number != -1; j--) // 查看在其前面执行的指令
                    {
                        inst_item tmp_inst = instructions[pre_log[j].number];
                        // add指令的RAW冲突
                        if (tmp_inst.inst == "add" && (tmp_inst.d1 == source_reg1 || tmp_inst.d1 == source_reg2))
                        {
                            if (pre_log[j].stage == "IF" || pre_log[j].stage == "ID") // 没产生结果
                            {
                                new_log.push_back(log_item(pre_log[i].number, true, pre_log[i].inst, pre_log[i].stage));
                                count_install++;
                                install_flag = true;
                            }
                            else if (pre_log[j].stage == "EX" || pre_log[j].stage == "EX*" || pre_log[j].stage == "MEM") // 产生结果了但是还没写回
                            {
                                if (redirect) // 可以重定向，就流出了
                                    new_log.push_back(log_item(pre_log[i].number, false, pre_log[i].inst, "EX*"));
                                else
                                {
                                    new_log.push_back(log_item(pre_log[i].number, true, pre_log[i].inst, pre_log[i].stage));
                                    count_install++;
                                    install_flag = true;
                                }
                            }
                            else // 产生结果并且已经写回了
                                new_log.push_back(log_item(pre_log[i].number, false, pre_log[i].inst, "EX"));

                            flag = true;
                            break;
                        }
                        // load指令的RAW冲突
                        else if (tmp_inst.inst == "load" && (tmp_inst.d1 == source_reg1 || tmp_inst.d1 == source_reg2))
                        {
                            if (pre_log[j].stage == "MEM") // 产生结果还没写回
                            {
                                if (redirect) // 可以重定向，就流出了
                                    new_log.push_back(log_item(pre_log[i].number, false, pre_log[i].inst, "EX*"));
                                else
                                {
                                    new_log.push_back(log_item(pre_log[i].number, true, pre_log[i].inst, pre_log[i].stage));
                                    count_install++;
                                    install_flag = true;
                                }
                            }
                            else if (pre_log[j].stage == "WB") // 产生结果并且已经写回
                                new_log.push_back(log_item(pre_log[i].number, false, pre_log[i].inst, "EX"));
                            else // 没产生结果
                            {
                                new_log.push_back(log_item(pre_log[i].number, true, pre_log[i].inst, pre_log[i].stage));
                                count_install++;
                                install_flag = true;
                            }

                            flag = true;
                            break;
                        }
                    }

                    if (!flag)
                        new_log.push_back(log_item(pre_log[i].number, false, pre_log[i].inst, "EX"));
                }

                if (pre_log[i].stage == "EX" || pre_log[i].stage == "EX*")
                    new_log.push_back(log_item(pre_log[i].number, false, pre_log[i].inst, "MEM"));
                if (pre_log[i].stage == "MEM")
                    new_log.push_back(log_item(pre_log[i].number, false, pre_log[i].inst, "WB"));
                if (pre_log[i].stage == "WB" || pre_log[i].stage == "" || pre_log[i].stage == "ABORTED") // 这个指令的流水阶段已经执行完了，下一个周期不执行
                    new_log.push_back(log_item(-1, false, "", ""));
            }
        }
    }

    // 查看是否有需要新加入的指令
    if ((cur_inst_num < int(instructions.size()) - 1)) // 可以加入新的指令
    {
        if (!install_flag)
        {
            cur_inst_num = cur_inst_num + 1; // 如果跳转，cur_inst_num之前已经被修改过了
            new_log.push_back(log_item(cur_inst_num, false, instructions[cur_inst_num].inst, "IF"));
            if (instructions[cur_inst_num].inst == "beqz")
                count_branch++;
        }
        else
            new_log.push_back(log_item(-1, true, "INSTALL", "INSTALL")); // 由于前面的指令INSTALL，导致不能流入新指令

    }

    logs.push_back(new_log);

    return;
}

void simulator::run_to_bp(int to_inst, string to_stage)
{
    while (logs.empty() || logs.back().back().stage != to_stage || logs.back().back().number != to_inst)
        run_next();
    return;
}
void simulator::run()
{
    // 如果日志为空、没有执行到最后一条指令的最后一个阶段，就运行
    while (logs.empty() || logs.back().back().stage != "WB")
        run_next();
    return;
}

void simulator::show_register()
{
    cout << "show register: " << endl;
    for (int i = 0; i < 10; i++)
        cout << "$t" << i << ": " << reg[i] << endl;
    return;
}

void simulator::show_pipeline(string path) // 输出到txt
{
    ofstream outfile(path);
    for (int i = 0; i < logs.size(); i++)
    {
        vector<log_item> l = logs[i];
        for (int j = 0; j < l.size(); j++)
            if (l[j].stage == "")
                outfile << setw(10) << setiosflags(ios::left) << " ";
            else if (l[j].install)
                outfile << setw(10) << setiosflags(ios::left) << "INSTALL";
            else if (l[j].inst == "ABORTED")
                outfile << setw(10) << setiosflags(ios::left) << "ABORTED";
            else
                outfile << setw(10) << setiosflags(ios::left) << l[j].inst + "." + l[j].stage;
        outfile << endl;
    }
    outfile.close();

    return;
}

void simulator::statistic(string path)
{
    ofstream outfile(path);
    outfile << logs.size() << " Cycle(s) executed" << endl;

    outfile << count_install << " stalls, ("
            << double(count_install) / logs.size() * 100 << " \% of all Cycle(s))" << endl;

    outfile << count_branch << " conditional branchs, ("
            << double(count_branch) / instructions.size() * 100 << " \% of all Instruction(s))" << endl;

    if (count_branch)
        outfile << "  " << count_branch_taken << " taken, ("
                << double(count_branch_taken) / count_branch * 100 << " \% of all cond.Branches)" << endl;

    outfile.close();
}