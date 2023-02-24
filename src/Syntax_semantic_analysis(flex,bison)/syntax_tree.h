#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>  // �䳤�������� ͷ�ļ�

// ����
extern int yylineno;
// �ı�
extern char* yytext;
// ������
void yyerror(char *msg);

// �����﷨��
typedef struct treeNode{
    // ����
    int line;
    // Token����
    char* name;
    // fchild�ǵ�һ�����ӽڵ㣬next���ֵܽڵ㣬ʹ�ú����ֵܱ�ʾ��
    struct treeNode *fchild,*next;
    // �����壬ͬһʱ��ֻ�ܱ���һ����Ա��ֵ������ռ�������������͵��ڴ��С
    union{
        // id���ݻ���type���ͣ�int/float��
        char* id_type;
        // �������ֵ
        int intval;
        float fltval;
    };
}* Ast,* tnode;

// ��������﷨��(�ڵ�)
Ast newAst(char* name,int num,...);

// ��������﷨��
void Preorder(Ast ast,int level);

// ���нڵ�����
int nodeNum;
// ������нڵ�
tnode nodeList[5000];
int nodeIsChild[5000];
// ���ýڵ��ӡ״̬
void setChildTag(tnode node);

// bison�Ƿ��дʷ��﷨����
int hasFault;

