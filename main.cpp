#include <iostream>
#include<list>

using namespace std;

typedef struct MemoryBlock {
    int start;
    int length;
    string status;

    void set(int start, int length, string status) {
        this->start = start;
        this->length = length;
        this->status = status;
    }

    bool operator<(MemoryBlock b) {
        return this->start < b.start;
    }
} MemoryBlock;

bool compare(MemoryBlock a, MemoryBlock b) {
    return a.start < b.start;
}

bool compare2(MemoryBlock a, MemoryBlock b) {
    return a.length < b.length;
}

bool compare3(MemoryBlock a, MemoryBlock b) {
    return a.length > b.length;
}

void randInit(int MAX, list<MemoryBlock> &f, list<MemoryBlock> &w) {
    //初始化空闲内存内存总大小;
    MemoryBlock f1, f2;

    f1.set(20, 30, "空闲");
    f.push_back(f1);
    f2.set(80, 10, "空闲");
    f.push_back(f2);
    MemoryBlock m1, m2, m3;
    m1.set(0, 20, "job1");
    w.push_back(m1);
    m2.set(50, 30, "job2");
    w.push_back(m2);
    m3.set(90, MAX - 90, "job3");
    w.push_back(m3);
}

void showFree(list<MemoryBlock> p) {
    p.sort(compare);
    cout << "空闲分区表:" << endl;
    cout << "起始地址\t" << "长度\t" << "状态" << endl;
    for (auto it = p.begin(); it != p.end(); it++) {
        cout << it->start << "\t\t" << it->length << "\t" << it->status << endl;
    }
    cout << endl;
}

void showMemory(list<MemoryBlock> f, list<MemoryBlock> w) {
    cout << "内存分配情况:" << endl;
    cout << "起始地址\t" << "长度\t" << "状态" << endl;
    w.sort(compare);
    f.merge(w, compare);
    for (auto it = f.begin(); it != f.end(); it++) {
        cout << it->start << "\t\t" << it->length << "\t" << it->status << endl;
    }
    cout << endl;

}

void generalFit(list<MemoryBlock> &f, list<MemoryBlock> &w, bool (*cmp)(MemoryBlock, MemoryBlock)) {
    string name;
    int length;
    char choose = 'y';
    while (choose == 'y') {
        if (f.size() == 0) {
            cout << "内存已满，分配失败";
            return;
        }
        bool flag = false;
        f.sort(cmp);
        showFree(f);
        cout << "请输入作业名称以及大小:(例如 job1 100)" << endl;
        cin >> name >> length;
        for (auto it = f.begin(); it != f.end(); ++it) {
            if (length <= it->length) {
                MemoryBlock w1;
                w1.set(it->start, it->length, name);
                w.push_back(w1);
                if (it->length - length > 0) {
                    MemoryBlock f1;
                    f1.set(it->start + length, it->length - length, "空闲");
                    f.insert(it, f1);
                }
                f.erase(it);
                flag = true;
                break;
            }
        }
        if (!flag) {
            cout << "分配失败，未找到满足大小的空闲区" << endl;
            return;
        }
        cout << "分配成功" << endl;
        showFree(f);
        cout << "是否要继续分配?选择(y/n):";
        cin >> choose;
    }
}

void loopFirstFit(list<MemoryBlock> &f, list<MemoryBlock> &w) {
    int lastFit = 0;
    string name;
    int length;
    char choose = 'y';
    while (choose == 'y') {
        if (f.size() == 0) {
            cout << "内存已满，分配失败";
            return;
        }
        cout << "请输入作业名称以及大小:(例如 job1 100)" << endl;
        cin >> name >> length;
        bool flag = false;
        int cnt = f.size();
        auto it = f.begin();
        for (int i = 0; i < lastFit % f.size(); ++i) {
            ++it;
        }
        while (cnt--) {
            if (length <= it->length) {
                MemoryBlock w1;
                w1.set(it->start + 0, length, name);
                w.push_back(w1);
                if (it->length - length > 0) {
                    MemoryBlock f1;
                    f1.set(it->start + length, it->length - length, "空闲");
                    f.insert(it, f1);
                }
                f.erase(it);
                flag = true;
                lastFit++;
                break;
            }
            if (++it == f.end()) { //判断的时候会自增
                it = f.begin();
            }
            lastFit++;
        }
        if (!flag) {
            cout << "分配失败，未找到满足大小的空闲区" << endl;
            lastFit = 0;
            return;
        }
        cout << "分配成功" << endl;
        showFree(f);
        cout << "是否要继续分配?选择(y/n):";
        cin >> choose;
    }
}

void memoryRecovery(list<MemoryBlock> &f, list<MemoryBlock> &w) {
    string name;
    int start = 0;
    int length = 0;
    bool isPre = false;
    bool isNext = false;
    cout << "请输入要回收的作业名称:";
    cin >> name;
    for (auto it = w.begin(); it != w.end(); ++it) {
        if (it->status == name) {
            start = it->start;
            length = it->length;
            break;
        }
    }
    if (length == 0) {
        cout << "未找到作业号" << endl;
        return;
    }
    MemoryBlock f1;
    f1.set(start, length, "空闲");
    if (f.size() == 0) {
        f.insert(f.cbegin(), f1);
    } else {
        for (auto it = f.begin(); it != f.end(); ++it) {
            if (start + length < it->start) {
                if (!isNext)                 //前后不相邻
                    f.insert(it, f1);
                break;                      // 仅与第一个空闲区后相邻
            } else if (start + length == it->start) {
                isPre = true;
                if (isPre && isNext) break;               //前后都相邻
                it->start = start;                        //前相邻
                it->length += length;
                break;
            } else if (start == it->start + it->length) {
                isNext = true;                           //后相邻，多加一层判断
                it->length += length;              //记录后相邻，判断是否与下一个分区前相邻，如果不是 break，如果是，update
                auto it4 = next(it);
                if (it4 != f.end() && it4->start == it->length + it->start) {
                    it->length += it4->length;
                    f.erase(it4);
                }
                break;
            } else if (start > it->start + it->length) {     //要回收的块在it后面
                auto it4 = next(it);
                if (it4 != f.end()) continue;                  //如果下一个空闲区不为空，for进入下一轮
                f.insert(it, f1);                 //如果为空，在此处新建空闲区
                break;
            }
        }
    }
    //删去工作块
    for (auto it = w.begin(); it != w.end(); ++it) {
        if (start == it->start) {
            w.erase(it);
            break;
        }
    }
    cout << "回收完成" << endl;
    showMemory(f, w);
}

int main() {
    list<MemoryBlock> f;
    list<MemoryBlock> w;
    int MAX = 128;
    randInit(MAX, f, w);
    int choose;
    cout << "==========模拟内存管理==========";
    cout << "内存空间大小:" << MAX << endl;
    while (1) {
        cout << "请选择功能" << endl << "1.分配内存\t" << "2.回收内存\t" << "3.查看内存分配情况\t" << "4.退出" << endl;
        cout << "请输入您选择的功能序号:";
        cin >> choose;
        switch (choose) {
            case 1:
                int selectAlgorithm;
                cout << "请选择的分配算法:" << endl;
                cout << "1.首次适应算法\t" << "2.循环首次适应算法\t" << "3.最佳适应算法\t" << "4.最坏适应算法" << endl;
                cout << "请输入算法序号:";
                cin >> selectAlgorithm;
                switch (selectAlgorithm) {
                    case 1:
                        generalFit(f, w, compare);
                        break;
                    case 2:
                        loopFirstFit(f, w);
                        break;
                    case 3:
                        generalFit(f, w, compare2);
                        break;
                    case 4:
                        generalFit(f, w, compare3);
                        break;
                    default:
                        cout << "输入有误?，请重新输入" << endl;
                        break;
                }
                break;
            case 2:
                memoryRecovery(f, w);
                break;
            case 3:
                showMemory(f, w);
                break;
            case 4:
                exit(0);
            default:
                cout << "输入有误，请重新输入" << endl;
                break;
        }
    }
}

