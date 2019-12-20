/*
 * Source.cpp
 *
 *  Created on: Dec 2, 2019
 *      Author: artem
 */

#include <iostream>
#include <string>
#include <string.h>
#include <assert.h>
#include <math.h>

typedef int nod_val;

#define MAX_NAME_LEN 256
#define MAX_NUM_OF_NODES 512

using namespace std;

enum {
    NUM = 1,
    VAR = 2,
    OP = 3
};

class Node {
private:
    nod_val val;
    int type;
    Node* left  = NULL;
    Node* right = NULL;
public:
    void Construct(int type, nod_val val) {
        switch(type) {
        case OP:
        case NUM:
            this->type = type;
            this->val  = val;
            break;
        case VAR:
            fprintf(stderr, "Node::Construct for operation & numbers used for variable!\n");
                Construct(type);
            break;
        default:
            fprintf(stderr, "Node::Construct: unknown type!\n");
        }
    }

    void Construct(int type) {
        switch(type) {
        case VAR:
            this->val = 'x';
            this->type = VAR;
        break;
        case NUM:
            fprintf(stderr, "Node::Construct for variables used for number!\n");
            Construct(type, 0);
            break;
        case OP:
            fprintf(stderr, "Node::Construct for variables used for operation!\n");
            Construct(type, 0);
            break;
        default:
            fprintf(stderr, "Node::Construct: unknown type!\n");
        }
    }

    void Construct(int type, nod_val val, Node* left, Node* right) {
        switch(type) {
        case OP:
            this->type  = type;
            this->val   = val;
            this->left  = left;
            this->right = right;
            break;
        case VAR:
            fprintf(stderr, "Node::Construct for operation used for variable!\n");
            Construct(type);
            break;
        case NUM:
            fprintf(stderr, "Node::Construct for operation used for number!\n");
            Construct(type, val);
            break;
        default:
            fprintf(stderr, "Node::Construct: unknown type!\n");
        }
    }

    void Construct(int type, nod_val val, Node* left) {
        assert(left  != NULL);
        switch(type) {
        case NUM:
        case OP:
            this->type  = type;
            this->val   = val;
            this->left  = left;
            break;
        case VAR:
            fprintf(stderr, "Node::Construct for operation & numbers used for variable!\n");
            Construct(type);
            break;
        default:
            fprintf(stderr, "Node::Construct: unknown type!\n");
        }
    }

    int FPrintGraphNode(FILE* fd) { //Создание графа (вспомогатльная функция)
        long id = this - (Node*)0x0; //Уникальный идентификатор для каждого узла, чтобы строить корректный граф
        long leftid  = 0;
        long rightid = 0;
        if(this->left != NULL) {
            leftid = this->left - (Node*)0x0;
            fprintf(fd, "%ld [label = ", leftid);
            if(this->left->type == NUM) {
                fprintf(fd, "\"%d\"]\n", this->left->val);
            }
            else
                fprintf(fd, "\"%c\"]\n", this->left->val);
            fprintf(fd, "%ld -> %ld\n", id, leftid);
        }
        if(this->right != NULL) {
            rightid = this->right - (Node*)0x0;
            fprintf(fd, "%ld [label = ", rightid);
            if(this->right->type == NUM) {
                fprintf(fd, "\"%d\"]\n", this->right->val);
            }
            else
                fprintf(fd, "\"%c\"]\n", this->right->val);
            fprintf(fd, "%ld -> %ld\n", id, rightid);
        }
        if(this->left != NULL)
            this->left->FPrintGraphNode(fd);
        if(this->right != NULL)
            this->right->FPrintGraphNode(fd);
        return 0;
    }

    int MakeGraphFile(char* FileName) { //Создание графа
        assert(FileName != NULL);
        FILE* fd;
        if((fd = fopen(FileName, "w")) == NULL) {
            perror("fopen");
            return -1;
        }
        fprintf(fd, "digraph G{\n");
        fprintf(fd, "node [style=\"filled\", fillcolor=\"orange\", fontcolor=\"blue\"];\n");
        long id = this - (Node*)0x0;
        fprintf(fd, "%ld [label = ", id);
        if(this->type == NUM) {
            fprintf(fd, "\"%d\"]\n", this->val);
        }
        else
            fprintf(fd, "\"%c\"]\n", this->val);
        this->FPrintGraphNode(fd);
        fprintf(fd, "}\n");
        fclose(fd);
        return 0;
    }

    int FPrintLeftNode(FILE* fd) {
        assert(fd != NULL);
        printf("(%d %d\n", this->type, this->val);
        fprintf(fd, "(%d %d\n", this->type, this->val);
        if(this->left != NULL)
            this->left->FPrintLeftNode(fd);
        if(this->right != NULL)
            this->right->FPrintRightNode(fd);
        else if(this->left != NULL) {
            printf(")\n");
            fprintf(fd, ")\n");
        }
        return 0;
    }

    int FPrintRightNode(FILE* fd) {
        assert(fd != NULL);
        printf(",%d %d\n", this->type, this->val);
        fprintf(fd, ",%d %d\n", this->type, this->val);
        if(this->left != NULL)
            this->left->FPrintLeftNode(fd);
        if(this->right != NULL)
            this->right->FPrintRightNode(fd);
        else if(this->left != NULL) {
            printf(")\n");
            fprintf(fd, ")\n");
        }
        printf(")\n");
        fprintf(fd, ")\n");
        return 0;
    }

    int SaveTree(char* filename) {
        assert(filename != NULL);
        FILE* fd;
        if((fd = fopen(filename, "w")) == NULL) {
            perror("fopen");
            return -1;
        }
        printf("%d %d\n", this->type, this->val);
        fprintf(fd, "%d %d\n", this->type, this->val);
        this->left->FPrintLeftNode(fd);
        this->right->FPrintRightNode(fd);
        fclose(fd);
        return 0;
    }

    void TreeConstruct(char* filename) {
        assert(filename != NULL);
        FILE* fd;
        if((fd = fopen(filename, "r")) == NULL) {
            perror("fopen");
            return;
        }
        //tqueue queue;
        //if(fgets(nod->name, MAX_NAME_LEN, fd) == NULL)
        //    return -1;
        this->left = NULL;
        this->right = NULL;
        if(fscanf(fd, "%d%d", &this->type, &this->val) == EOF) {
            fclose(fd);
            return;
        }
        char buff[MAX_NAME_LEN+1];
        fgets(buff, MAX_NAME_LEN, fd);
        this->FScanNode(buff, fd);
        //queue.cur = 0;
        fclose(fd);
    }

    int FScanNode(char* CurStr, FILE* fd) {
        assert(fd  != NULL);
        //char buff[MAX_NAME_LEN+1];
        if(fgets(CurStr, MAX_NAME_LEN + 1, fd) == NULL || CurStr[0] == '\n') {
            return -1;
        }
        else {
            int BuffLen = strlen(CurStr);
            if(BuffLen > 0 && CurStr[BuffLen - 1] == '\n')
                CurStr[BuffLen - 1] = '\0';
            if(CurStr[0] != '(')
                return 0;
            else {
                this->left = new Node;
                sscanf(CurStr + 1, "%d%d", &this->left->type, &this->left->val);
                if(this->left->FScanNode(CurStr, fd) == -1) return -1;
                if(CurStr[0] != ',') {
                    if(CurStr[0] != ')') {
                        fprintf(stderr, "Invalid format of input file!!!\n");
                        return -1;
                    }
                }
                else {
                    this->right = new Node;
                    sscanf(CurStr + 1, "%d%d", &this->right->type, &this->right->val);
                    if(this->right->FScanNode(CurStr, fd) == -1) return -1;
                    if(CurStr[0] != ')') {
                        fprintf(stderr, "Invalid format of input file!!!\n");
                        return -1;
                    }
                }
                fgets(CurStr, MAX_NAME_LEN + 1, fd);
            }
        }
        return 0;
    }

    nod_val TreeCount() { //Калькулятор дерева
        switch(this->type) {
        case OP:
            if(this->left == NULL) {
                fprintf(stderr, "There is broken tree after operation \"%c\"!\n", this->val);
                return 0;
            }
            switch(this->val) {
            case '+':
                if(this->right == NULL) {
                    fprintf(stderr, "There is broken tree after operation \"%c\"!\n", this->val);
                }
                return this->left->TreeCount() + this->right->TreeCount();
            case '-':
                if(this->right == NULL) {
                    fprintf(stderr, "There is broken tree after operation \"%c\"!\n", this->val);
                }
                return this->left->TreeCount() - this->right->TreeCount();
            case '*':
                if(this->right == NULL) {
                    fprintf(stderr, "There is broken tree after operation \"%c\"!\n", this->val);
                }
                return this->left->TreeCount() * this->right->TreeCount();
            case '/':
                if(this->right == NULL) {
                    fprintf(stderr, "There is broken tree after operation \"%c\"!\n", this->val);
                }
                return this->left->TreeCount() / this->right->TreeCount();
            case '^':
                if(this->right == NULL) {
                    fprintf(stderr, "There is broken tree after operation \"%c\"!\n", this->val);
                }
                return (nod_val)pow(this->left->TreeCount(), this->right->TreeCount());
            case 's':
                return (nod_val)sin(this->left->TreeCount());
            case 'c':
                return (nod_val)cos(this->left->TreeCount());
            default:
                fprintf(stderr, "There is broken tree, unknown operation \"%c\"!\n", this->val);
                return 0;
            }
            break;
        case NUM:
            return this->val;
        case VAR:
            return 0;
        default:
            fprintf(stderr, "There is broken tree, unknown type number \"%d\"!\n", this->type);
            return 0;
        }
    }

};

class Analyzer { // =, /
private:
    char* s;
    string varstr;

    Node* OpUnion(Node* nod1, Node* nod2, char op) {
        assert(nod1  != NULL);
        assert(nod2  != NULL);
        Node* total  = new Node;
        total->Construct(OP, op, nod1, nod2);
        return total;
    }

    Node* OpUnion(Node* nod1, char op) {
        assert(nod1  != NULL);
        Node* total  = new Node;
        total->Construct(OP, op, nod1);
        return total;
    }

    void MyAssert(bool b) {
        if(!b)
            fprintf(stderr, "Ошибка, ожидалась скобка! (символ \"(\" или \")\", зависит от контекста)\n");
    }
public:
    Node* tree;
    Analyzer(const char* s) {
        this->s = strdup(s);
        this->tree = this->GetG();
    }

    Node* GetG() {
        Node *val = GetE();
        if(*s != '#')
            fprintf(stderr, "Ожидался завершающий символ \"#\"!\n");
        return val;
    }

    Node* GetN() {
        int val = 0;
        char *start = s;
        while(*s >= '0' && *s <= '9') {
            val = val * 10 + (*(s++) - '0');
        }
        if(start == s) return NULL;
        Node* n = new Node;
        n->Construct(NUM, val);
        return n;
    }

    Node *GetE() {
        Node* val = GetT();
        Node* total;
        while(*s == '+' || *s == '-') {
            char oper = *s;
            s++;
            Node* val2 = GetT();
            total = OpUnion(val, val2, oper);
            //delete val;
            val = total;
        }
        return val;
    }

    Node *GetT() {
        Node* val = GetPow();
        Node* total;
        while(*s == '*' || *s == '/') {
            char oper = *s;
            s++;
            Node* val2 = GetPow();
            total = OpUnion(val, val2, oper);
            //delete val;
            //delete val2;
            val = total;
        }
        return val;
    }

    Node* GetP() {
        if(*s == '(') {
            s++;
            Node* val = GetE();
            MyAssert(*s == ')');
            s++;
            return val;
        }
        else
            return GetVN();
    }

    Node* GetV() {
        char *start = s;
        if(this->varstr == "") {
            while((*s >= 'A' && *s <= 'Z') || (*s >= 'a' && *s <= 'z') || (*s == '_')) {
                this->varstr += *s;
                s++;
            }
        }
        else {
            int i = 0;
            while((*s >= 'A' && *s <= 'Z') || (*s >= 'a' && *s <= 'z') || (*s == '_')) {
                if(this->varstr[i] != *s) {
                    fprintf(stderr, "Our analyzer works only with 1 variable!\n");
                    return NULL;
                }
                i++;
                s++;
            }
        }
        if(start == s) return NULL;
        else {
            Node* n = new Node;
            n->Construct(VAR);
            return n;
        }
    }

    Node* GetVN() {
        Node *val1 = NULL,
              *val2 = NULL;
        if((val1 = GetSC()) != NULL)
            return val1;
        if((val1 = GetN()) != NULL) {
            if((val2 = GetV()) != NULL) {
                Node* total;
                total = OpUnion(val1, val2, '*');
                return total;
            }
            else {
                return val1;
            }
        }
        else
            return GetV();
    }

    Node* GetSin() {
        Node* val;
        if(strncmp(s, "sin", 3) == 0)
            s += 3;
        else
            return NULL;
        MyAssert(*s == '(');
        s++;
        val = GetE();
        MyAssert(*s == ')');
        s++;
        Node* val2 = new Node;
        val2->Construct(OP, 's', val);
        return val2;
    }

    Node* GetCos() {
        Node* val;
        if(strncmp(s, "cos", 3) == 0)
            s += 3;
        else
            return NULL;
        MyAssert(*s == '(');
        s++;
        val = GetE();
        MyAssert(*s == ')');
        s++;
        Node* val2 = new Node;
        val2->Construct(OP, 'c', val);
        return val2;
    }

    Node* GetSC() {
        Node* val;
        if((val = GetSin()) != NULL)
            return val;
        else if((val = GetCos()) != NULL)
            return val;
        else return NULL;
    }

    Node *GetPow() {
        Node* val = GetP();
        Node* total;
        while(*s == '^') {
            s++;
            Node* val2 = GetP();
            total = OpUnion(val, val2, '^');
            val = total;
        }
        return val;
    }


};

int main(int argc, char* argv[]) {
    puts("Для создание графа-дерева введите первым параметром командной строки имя файла (расширение .dot).");
    puts("Для сохранения графа-дерева картинкой введите вторым параметром командной строки имя файла (расширение .png).");
    puts("Для сохранения графа-дерева в качестве исходного кода (для возможности последующего восстановления вычисляемого выражения) ведите третьим параметром командной строки имя файла (расширение .dot).");
    puts("Введите вычисляемое выражение, без пробелов, с завершающим символом #");
    char input[1024];
    fgets(input, 1024, stdin);
    Analyzer a = Analyzer(input);
    if(argc > 1) {
        a.tree->MakeGraphFile(argv[1]);
        if(argc > 2) {
            sprintf(input, "dot %s -Tpng -o %s", argv[1], argv[2]);
            system(input);
            if(argc > 3) {
                a.tree->SaveTree(argv[3]);
            }
        }
    }
    printf("Ответ: %d\n", a.tree->TreeCount());
    return 0;
}
