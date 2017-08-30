//只要是指针，就要养成定义时指向NULL，使用前为其开辟空间，使用后释放空间的习惯
//OnedayarabbitwasinthegardeneatingcarrotsWhenhewasabouttogohomehesawthefarmerSoheneededtohide
//One day a rabbit was in the garden eating carrots When he was about to go home he saw the farmer
//One day a rabbit ate 99 carrots in the garden, when he was about to go home he saw 3 farmers...
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct{
    unsigned int weight;
    unsigned int parent,lchild,rchild;
}HTNode, HuffmanTree;//动态分配数组存储哈夫曼树

typedef struct{
    char ch;
    int wei;
}Letter;

typedef char **HuffmanCode;//动态分配数组存储哈夫曼编码表

//在HT[1..i-1]选择parent为0且weight最小的两个结点，其序号分别为s1和s2
void Select(HuffmanTree *HT, int i, int *s1, int *s2){
    int j, Wmin, m = 0;

    //寻找最大权重最大的值，方便之后寻找最大值时设置初始比较值。
    for(j = 1; j <= i; j++)
        if(HT[j].parent == 0 && HT[j].weight > m)
            m = HT[j].weight;
/*错误记录：原本是在if里就进行m+1，但是因为这样就会改变下一次m对比的值，举例子，权值是1112，当前m=1，则按照错误版本，m=1+1
那么下次比较是m就是2，并且不会有比2更大的数了最终循环完后m=2，实际上我想要的结果是对比完后，m=3。*/

    //寻找最小的值
    Wmin = m + 1;//加1的原因：设置一个比数列中所有数都大的数。
    for(j = 1; j <= i; j++)
        if(HT[j].parent == 0 && HT[j].weight < Wmin){
            Wmin = HT[j].weight;
            *s1 = j;
        }//if

    //寻找第二小的值
    Wmin = m + 1;
    for(j = 1; j <= i; j++)
        if(HT[j].parent == 0 && j != *s1 && HT[j].weight < Wmin){
            //此处不加<=号是因为如果碰到两个权值相等的第二小，我们选择的是序号较小的那个
            Wmin = HT[j].weight;
            *s2 = j;
        }//if

    if(*s1 > *s2){//把序号小的靠前
        j = *s1;
        *s1 = *s2;
        *s2 = j;
    }//if
}//Select

//设置一个哈夫曼树结点
void SetHTNode(HuffmanTree *T, int w, int p, int l, int r){
    T->weight = w;
    T->parent = p;
    T->lchild = l;
    T->rchild = r;
}//SetHTNode

//w存放n个字符的权值（均>0），构造哈夫曼树HT，并求出n个字符的哈夫曼编码HC。
void HuffmanCoding(HuffmanTree *HT, HuffmanCode HC, Letter *w, int n){
    int i, j;//用于循环
    int *s1,*s2;//用于寻找权值最小的两个结点的序号
    s1 = (int*)malloc(sizeof(int));
    s2 = (int*)malloc(sizeof(int));
    int m;

    HuffmanTree *p = NULL;

    if(n <= 1)
        exit(0);
    m = 2 * n - 1;

    //初始化哈夫曼树
    for(p = HT + 1, i = 1; i <= n; ++i, ++p, ++w)
        SetHTNode(p, w->wei, 0, 0, 0);
    for(; i <= m; ++i, ++p)
        SetHTNode(p, 0, 0, 0, 0);

    //建哈夫曼树
    for(i = n + 1; i <= m; ++i){
        //在HT[1..i-1]选择parent为0且weight最小的两个结点，其序号分别为s1和s2
        Select(HT, i - 1, s1, s2);
        HT[*s1].parent = i;
        HT[*s2].parent = i;
        HT[i].lchild = *s1;
        HT[i].rchild = *s2;
        HT[i].weight = HT[*s1].weight + HT[*s2].weight;
    }//for

/*打印哈夫曼树表
    printf("i  weight  parent  lchild  rchild\n");
    for(i = 1; i <= m; i++)
        printf("%-2d%5d%6d%7d%8d\n",i,HT[i].weight,HT[i].parent,HT[i].lchild,HT[i].rchild);
*/

    //从叶子到根逆向求每个字符的哈夫曼编码
    int c, f, start;//中间变量
    char *cd = NULL;
    cd = (char*)malloc(n * sizeof(char));//分配求编码的工作空间
    cd[n - 1] = '\0';//编码结束符
    for(i = 1; i <= n; ++i){//逐个字符求哈夫曼编码
        start = n - 1;//编码结束符位置
        for(c = i, f = HT[i].parent; f != 0; c = f, f = HT[f].parent)//从叶子到根逆向求编码
            if(HT[f].lchild == c)
                cd[--start] = '0';
            else
                cd[--start] = '1';
            HC[i] = (char*)malloc((n - start) * sizeof(char));//为第i个字符编码分配空间
            strcpy(HC[i], &cd[start]);//从cd复制编码（串）到HC
    }//for

    //释放工作空间
    free(cd);
    free(s1);
    free(s2);
}//HuffanCoding

//输入字符串,返回字符串的长度
int InputWord(char *str, int len){
    char ch;
    int count = 0;//实际字符数
    while((ch = getchar()) != '\n'){
        count++;
        if(count >= len)
            str = (char*)realloc(str, sizeof(char) * (len++));
        str[count - 1] = ch;
    }//while
    str[count] = '\0';

    return count;
}//InputWord

//统计部分，计算各字母权值,返回字母（即权值）的个数
int CountWeight(Letter *let, char *str, int count, int n){
    int i, j, num = 0;//n为默认不同字母的个数的初始值，num为实际不同字母的个数
    for(i = 0; i < n; i++)
        let[i].wei = 0;
    for(i = 0; i < count; i++){
        if(('a' <= str[i] && str[i] <= 'z') || (str[i] == ' ') || ('A' <= str[i] && str[i] <= 'Z')){
        //考虑到小写字母出现的频率普遍比大写高，为了提高效率，所以先判断是否小写再判断是否大写
            for(j = 0; j < num; j++)
                if(let[j].ch == str[i]){
                    let[j].wei++;
                    break;
                }//if
            if(j == num){
                let[j].ch = str[i];
                let[j].wei++;
                num++;
            }//if
        }//if(a~z || A~Z)
    }//for
    if(num < n)//节省内存空间
        let = (Letter*)realloc(let, sizeof(Letter) * (num));
    return num;
}//CountWeight

//输入一段100—200字的英文短文，存入一文件a中。
void WriteText(char *str, int len){
    FILE *fp1 = NULL;
    if((fp1 = fopen("a.txt", "wb")) == NULL){
        printf("cannot open file\n");
        return;
    }
    if(fwrite(str,sizeof(char) * len,1,fp1)!=1)
        printf("file write error\n");
    fclose(fp1);
}//WriteText

//用每个字母编码对原短文进行编码，码文存入文件b中。
int WriteCode(char *str, Letter *let, HuffmanCode HC, int num, int len){
    FILE *fp2 = NULL;
    int i,j;
    int n = 0;//文件中的编码的长度
    if((fp2 = fopen("b.txt", "wb")) == NULL){
        printf("cannot open file\n");
        return;
    }//if
    for(i = 0; i < len; i++)
        if(('a' <= str[i] && str[i] <= 'z') || (str[i] == ' ') || ('A' <= str[i] && str[i] <= 'Z'))
            for(j = 0; j < num; j++)
                if(str[i] == let[j].ch){
                    if(fputs(HC[j + 1],fp2) != 0)
                        printf("file write error\n");
                    else
                        n += strlen(HC[j + 1]);
                }//if

    fclose(fp2);
    return n + 1;//算上'\0'
}//WriteCode

//从文件b中读出编码
void ReadCode(char *s, int n){
    FILE *fp2;
    if((fp2 = fopen("b.txt","rb")) == NULL){
        printf("cannot open file\n");
        return;
    }//if
    fgets(s,n,fp2);//函数默认读入n-1个字符，最后自动添加'\0'，n已经预留了
    fclose(fp2);
}//ReadCode

//用Haffman树对b中码文进行译码，结果存入文件c中
void TranslateCode(char *s, Letter *let, HuffmanTree *HT, int num){
    FILE *fp3 = NULL;
    if((fp3 = fopen("c.txt", "wb")) == NULL){
        printf("cannot open file\n");
        return;
    }//if

    int i, j = 2 * num - 1;
    for(i = 0; s[i] != '\0'; i++){
        //思路：从哈夫曼树的根节点开始，根据读取的字符寻找下一个结点，当当前结点没有左右孩子，即找到叶子结点，存入文件
        if(s[i]=='0')
            j = HT[j].lchild;
        else
            j = HT[j].rchild;

        if(HT[j].lchild == 0 && HT[j].rchild == 0)
            if(fwrite(let + j - 1,sizeof(char),1,fp3)!=1)
                printf("file write error\n");
            else
                j = 2 * num - 1;
    }//for
    fclose(fp3);
}//TranslateCode

int main(){
    int len = 100;//英文短文长度的初始值
    int num = 53;//权值个数（53为初始值）
    int i;
    char *str = (char*)malloc(len * sizeof(char));//存放输入的英文短文
    Letter *let = (Letter*)malloc(num * sizeof(Letter));//存放各字母的权值

    HuffmanTree *HT;
    HuffmanCode HC;

    printf("Please input a sentence in English:\n");
    len = InputWord(str, len);//输入英文短文并返回其的长度
    num = CountWeight(let, str, len, num);//计算英文短文中各字母的出现的次数（即权值）并返回权值的个数
    while(len == 1 || num == 1){
        printf("Input Error! Please input >= 1 char!\n");
        len = InputWord(str, len);
        num = CountWeight(let, str, len, num);
    }

    WriteText(str, len);//存入文件a

    HC = (HuffmanCode)malloc((num + 1) * sizeof(char*));//分配n个字符编码的头指针向量
    HT = (HuffmanTree*)malloc((2 * num) * sizeof(HTNode));//0号单元未用

    HuffmanCoding(HT, HC, let, num);//对各字母进行哈夫曼编码

    //输出各字母权值和编码
    printf("\nchar num  code\n");
    for(i = 0; i < num; i++){
        printf("%2c  :%2d   ",let[i].ch,let[i].wei);
        puts(HC[i + 1]);
    }//for

    int n = WriteCode(str, let, HC, num, len);//将原文编码写入文件b，并返回编码的长度（已经算上'\0'）
    char s[n];
    ReadCode(s, n);//从文件b中读出编码
    TranslateCode(s, let, HT, num);//用Haffman树对b中码文进行译码，结果存入文件c中

    //释放空间
    free(str);
    free(let);
    free(HC);
    free(HT);
    return 0;
}
