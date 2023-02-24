#include "syntax_tree.h"

// ���ڱ���
int i;

Ast newAst(char *name, int num, ...)
{
    // ���ɸ��ڵ�
    tnode father = (tnode)malloc(sizeof(struct treeNode));
    // ����ӽڵ�
    tnode temp = (tnode)malloc(sizeof(struct treeNode));
    if (!father)
    {
        yyerror("create treenode error");
        exit(0);
    }
    father->name = name;

    // �����б���� stdarg.h �÷�
    va_list list;
    // ��ʼ�������б�
    va_start(list, num);

    // ��ʾ��ǰ�ڵ㲻���ս���ţ������ӽڵ�
    if (num > 0)
    {
        // ��һ�����ӽڵ�
        temp = va_arg(list, tnode);
        setChildTag(temp);
        father->fchild = temp;
        // ���ڵ��к�Ϊ��һ�����ӽڵ���к�
        father->line = temp->line;

        // ����ڵ㣬�������
        if (num >= 2)
        {
            for (i = 0; i < num - 1; ++i)
            {
                temp->next = va_arg(list, tnode);
                temp = temp->next;
                // �ýڵ�Ϊ�����ڵ���ӽڵ�
                setChildTag(temp);
            }
        }
    }
    else //��ʾ��ǰ�ڵ����ս����Ҷ�ڵ㣩���߿յ��﷨��Ԫ����ʱnum��ʾ�кţ��յ�ԪΪ-1��,����Ӧ��ֵ����union
    {
        father->line = va_arg(list, int);
        // strcmp()==0 ��ʾ��ͬ
        if ((!strcmp(name, "ID")) || (!strcmp(name, "TYPE")))
        {
            char *str;
            str = (char *)malloc(sizeof(char) * 40);
            strcpy(str, yytext);
            father->id_type = str;
            // father->id_type = (char*)malloc(sizeof(char)*40);
            // strcpy(father->id_type,yytext);
        }
        else if (!strcmp(name, "INT"))
        {
            father->intval = atoi(yytext);
        }
        else
        {
            father->fltval = atof(yytext);
        }
    }
    return father;
}

// ���ڵ�->���ӽڵ�->���ӽڵ�....
void Preorder(Ast ast, int level)
{
    // printf(" into ");
    if (ast != NULL)
    {
        // �㼶�ṹ����
        for (i = 0; i < level; ++i)
        {
            printf(" ");
        }
        // printf(" rline ");
        if (ast->line != -1)
        {
            // printf(" prnt ");
            // ��ӡ�ڵ�����
            printf("%s", ast->name);
            // ���ݲ�ͬ���ʹ�ӡ�ڵ�����
            if ((!strcmp(ast->name, "ID")) || (!strcmp(ast->name, "TYPE")))
            {
                printf(": %s", ast->id_type);
            }
            else if (!strcmp(ast->name, "INT"))
            {
                printf(": %d", ast->intval);
            }
            else if (!strcmp(ast->name, "FLOAT"))
            {
                printf(": %f", ast->fltval);
            }
            else
            {
                // ��Ҷ�ڵ��ӡ�к�
                printf("(%d)", ast->line);
            }
        }
        printf("\n");
        // printf(" fchild ");
        Preorder(ast->fchild, level + 1);
        // printf(" next ");
        Preorder(ast->next, level);
    }
    // printf(" return ");
}

// ������
void yyerror(char *msg)
{
    hasFault = 1;
    fprintf(stderr, "Error type B at Line %d, %s ,before %s\n", yylineno, msg, yytext);
}

// ���ýڵ��ӡ״̬ �ýڵ�Ϊ�ӽڵ�
void setChildTag(tnode node)
{
    int i;
    for (i = 0; i < nodeNum; i++)
    {
        if (nodeList[i] == node)
        {
            nodeIsChild[i] = 1;
        }
    }
}

// ������ ɨ���ļ����ҷ���
// Ϊbison���Լ�����yylex()��������main�����в���Ҫ�ٵ�������
// bisonʹ��yyparse()�����﷨������������Ҫ������main�����е���yyparse()��yyrestart()
int main(int argc, char **argv)
{
    int j;
    printf("start analysis\n");
    if (argc < 2)
    {
        return 1;
    }
    for (i = 1; i < argc; i++)
    {
        // ��ʼ���ڵ��¼�б�
        nodeNum = 0;
        memset(nodeList, 0, sizeof(tnode) * 5000);
        memset(nodeIsChild, 0, sizeof(int) * 5000);
        hasFault = 0;

        FILE *f = fopen(argv[i], "r");
        if (!f)
        {
            perror(argv[i]);
            return 1;
        }
        yyrestart(f);
        yyparse();
        fclose(f);

        // �������з��ӽڵ�Ľڵ�
        if (hasFault)
            continue;
        for (j = 0; j < nodeNum; j++)
        {
            if (nodeIsChild[j] != 1)
            {
                Preorder(nodeList[j], 0);
            }
        }
    }
}

