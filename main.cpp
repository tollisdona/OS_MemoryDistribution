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
    //��ʼ�������ڴ��ڴ��ܴ�С;
    MemoryBlock f1, f2;

    f1.set(20, 30, "����");
    f.push_back(f1);
    f2.set(80, 10, "����");
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
    cout << "���з�����:" << endl;
    cout << "��ʼ��ַ\t" << "����\t" << "״̬" << endl;
    for (auto it = p.begin(); it != p.end(); it++) {
        cout << it->start << "\t\t" << it->length << "\t" << it->status << endl;
    }
    cout << endl;
}

void showMemory(list<MemoryBlock> f, list<MemoryBlock> w) {
    cout << "�ڴ�������:" << endl;
    cout << "��ʼ��ַ\t" << "����\t" << "״̬" << endl;
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
            cout << "�ڴ�����������ʧ��";
            return;
        }
        bool flag = false;
        f.sort(cmp);
        showFree(f);
        cout << "��������ҵ�����Լ���С:(���� job1 100)" << endl;
        cin >> name >> length;
        for (auto it = f.begin(); it != f.end(); ++it) {
            if (length <= it->length) {
                MemoryBlock w1;
                w1.set(it->start, it->length, name);
                w.push_back(w1);
                if (it->length - length > 0) {
                    MemoryBlock f1;
                    f1.set(it->start + length, it->length - length, "����");
                    f.insert(it, f1);
                }
                f.erase(it);
                flag = true;
                break;
            }
        }
        if (!flag) {
            cout << "����ʧ�ܣ�δ�ҵ������С�Ŀ�����" << endl;
            return;
        }
        cout << "����ɹ�" << endl;
        showFree(f);
        cout << "�Ƿ�Ҫ��������?ѡ��(y/n):";
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
            cout << "�ڴ�����������ʧ��";
            return;
        }
        cout << "��������ҵ�����Լ���С:(���� job1 100)" << endl;
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
                    f1.set(it->start + length, it->length - length, "����");
                    f.insert(it, f1);
                }
                f.erase(it);
                flag = true;
                lastFit++;
                break;
            }
            if (++it == f.end()) { //�жϵ�ʱ�������
                it = f.begin();
            }
            lastFit++;
        }
        if (!flag) {
            cout << "����ʧ�ܣ�δ�ҵ������С�Ŀ�����" << endl;
            lastFit = 0;
            return;
        }
        cout << "����ɹ�" << endl;
        showFree(f);
        cout << "�Ƿ�Ҫ��������?ѡ��(y/n):";
        cin >> choose;
    }
}

void memoryRecovery(list<MemoryBlock> &f, list<MemoryBlock> &w) {
    string name;
    int start = 0;
    int length = 0;
    bool isPre = false;
    bool isNext = false;
    cout << "������Ҫ���յ���ҵ����:";
    cin >> name;
    for (auto it = w.begin(); it != w.end(); ++it) {
        if (it->status == name) {
            start = it->start;
            length = it->length;
            break;
        }
    }
    if (length == 0) {
        cout << "δ�ҵ���ҵ��" << endl;
        return;
    }
    MemoryBlock f1;
    f1.set(start, length, "����");
    if (f.size() == 0) {
        f.insert(f.cbegin(), f1);
    } else {
        for (auto it = f.begin(); it != f.end(); ++it) {
            if (start + length < it->start) {
                if (!isNext)                 //ǰ������
                    f.insert(it, f1);
                break;                      // �����һ��������������
            } else if (start + length == it->start) {
                isPre = true;
                if (isPre && isNext) break;               //ǰ������
                it->start = start;                        //ǰ����
                it->length += length;
                break;
            } else if (start == it->start + it->length) {
                isNext = true;                           //�����ڣ����һ���ж�
                it->length += length;              //��¼�����ڣ��ж��Ƿ�����һ������ǰ���ڣ�������� break������ǣ�update
                auto it4 = next(it);
                if (it4 != f.end() && it4->start == it->length + it->start) {
                    it->length += it4->length;
                    f.erase(it4);
                }
                break;
            } else if (start > it->start + it->length) {     //Ҫ���յĿ���it����
                auto it4 = next(it);
                if (it4 != f.end()) continue;                  //�����һ����������Ϊ�գ�for������һ��
                f.insert(it, f1);                 //���Ϊ�գ��ڴ˴��½�������
                break;
            }
        }
    }
    //ɾȥ������
    for (auto it = w.begin(); it != w.end(); ++it) {
        if (start == it->start) {
            w.erase(it);
            break;
        }
    }
    cout << "�������" << endl;
    showMemory(f, w);
}

int main() {
    list<MemoryBlock> f;
    list<MemoryBlock> w;
    int MAX = 128;
    randInit(MAX, f, w);
    int choose;
    cout << "==========ģ���ڴ����==========";
    cout << "�ڴ�ռ��С:" << MAX << endl;
    while (1) {
        cout << "��ѡ����" << endl << "1.�����ڴ�\t" << "2.�����ڴ�\t" << "3.�鿴�ڴ�������\t" << "4.�˳�" << endl;
        cout << "��������ѡ��Ĺ������:";
        cin >> choose;
        switch (choose) {
            case 1:
                int selectAlgorithm;
                cout << "��ѡ��ķ����㷨:" << endl;
                cout << "1.�״���Ӧ�㷨\t" << "2.ѭ���״���Ӧ�㷨\t" << "3.�����Ӧ�㷨\t" << "4.���Ӧ�㷨" << endl;
                cout << "�������㷨���:";
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
                        cout << "��������?������������" << endl;
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
                cout << "������������������" << endl;
                break;
        }
    }
}

