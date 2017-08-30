/*Test1:
1,2 3,5,0 0,-4 0,0
1,2 2,1 -9.8,-8 0,0
*/

/*Test2:
0,3 0,0
1,2 2,3 0,0
*/

/*Test3:
3,0 0,0
9.9,0 0,0
*/
#include<stdio.h>
#include<stdlib.h>
#define LEN sizeof(ElemType)

typedef struct Poly{
    float coef; //系数
    int expn;   //指数
    struct Poly *next;
}ElemType;

ElemType *LinkList();//建立空链表
void DestroyPolyn(ElemType *p);//销毁一元多项式P
void PrintPolyn(ElemType *p);//打印输出一元多项式P
ElemType *CreatPolyn(ElemType *head);//输入系数和指数，建立表示一元多项式的有序链表P
ElemType *ListInsert_L(ElemType *head, ElemType *e);//处理并插入元素e
ElemType *MultiplyPolyn(ElemType *Pa, ElemType *Pb);//相乘

ElemType *LinkList(){
    ElemType *head;
    head = NULL;
    return head;
};

void DestroyPolyn(ElemType *p){//销毁一元多项式P
    ElemType *q;

    if(!p)//空表就不用销毁了
        exit(0);

    while(p->next){
        q = p->next;
        p->next = q->next;
        free(q);
    }//while

    if(! p->next)//删除头结点
        free(p);
}//DestroyPolyn

void PrintPolyn(ElemType *p){//打印输出一元多项式P
    ElemType *q;
    q = p;

    if(!p)//空表即表达式为0
        printf("0");

    while(q){
        if(q->expn == 0)
            printf("%g ",q->coef);//指数为0，直接输出系数
        else if(q->coef == 1 && q->expn != 1)
            printf("x^%d ",q->expn);//系数为1，可省略
        else if(q->coef == 1 && q->expn == 1)
            printf("x ");//指数为1，可省略
        else if(q->coef != 1 && q->expn == 1)
            printf("%gx ",q->coef);
        else
            printf("%gx^%d ",q->coef,q->expn);

        if(q->next != NULL && q->coef != 0 && q->next->coef > 0)
        //打印符号，若当前项为最后一项或下一项系数非正，则不打印
        //实际上判断系数非零在此是多余的，因为之前的输入和计算处理已经排除了系数为零的项，但是为了思路清晰，还是写上
            printf("+ ");
        q = q->next;
    }//while
    printf("\n");
}//PrintPolyn

ElemType *CreatPolyn(ElemType *head){
    //输入系数和指数，建立表示一元多项式的有序链表P
    ElemType *p1;
    p1 = (ElemType*)malloc(LEN);
    scanf("%f,%d",&p1->coef,&p1->expn);//输入时，同一项的系数和指数用逗号隔开，项与项之间用空格隔开
    while( (p1->coef != 0) || (p1->expn != 0) ){
        if(p1->coef != 0){//系数为零，无效处理
            head = ListInsert_L(head, p1);
            p1 = (ElemType*)malloc(LEN);
        }//if
        scanf("%f,%d",&p1->coef,&p1->expn);
    }//while
    return head;
}//CreatPolyn

ElemType *ListInsert_L(ElemType *head, ElemType *e){
    //处理并插入元素e
    ElemType *p0, *p1, *p2;
	p1 = head;
	p0 = e;

    if(head == NULL){
		head = p0;
		p0->next = NULL;
	}//if
	else{
        while((p0->expn < p1->expn) && (p1->next!=NULL)){//寻找要插入的地方。降序
            p2 = p1;//p2指向的是比插入结点小的结点。
            p1 = p1->next;//p1指向的是比插入结点大的结点。综上：p2>p0>p1。
        }//while

        if(p0->expn > p1->expn){
            if(head == p1)//要插入的地方是开头。
                head = p0;
            else
                p2->next = p0;
            p0->next=p1;
        }//if
        else if(p0->expn == p1->expn){
            p1->coef += p0->coef;
            if(p1->coef == 0){//系数为0，删除结点
                p2->next = p1->next;
                free(p1);
            }//if
        }//else if
        else{//要插入的地方是结尾。
            p1->next = p0;
            p0->next = NULL;
        }
	}//else

    return head;
}//ListInsert_L

ElemType *MultiplyPolyn(ElemType *Pa, ElemType *Pb){
    //多项式乘法：Pc = Pa * Pb
    ElemType *ha,*hb,*qa,*qb,*Pc,*c;//qa指向Pa中的当前结点，qb同理
    float ccoef;
    int cexpn;
    ha = Pa;
    hb = Pb;

    if(!Pa || !Pb)
        return NULL;
    Pc = LinkList();
    for(qa = ha; qa; qa = qa->next){//当qa非空，逐个检索qa
        for(qb = hb; qb; qb = qb->next){
            ccoef = qa->coef * qb->coef;
            cexpn = qa->expn + qb->expn;
            if(ccoef != 0){
                c = (ElemType*)malloc(LEN);
                c->coef = ccoef;
                c->expn = cexpn;
                Pc = ListInsert_L(Pc, c);
            }
        }//for(qb)
    }//for(qa)

    //计算完毕，释放Pa和Pb
    DestroyPolyn(ha);
    DestroyPolyn(hb);

    return Pc;
}//MultiplyPolyn

int main(){
    ElemType *Pa,*Pb,*Pc;
    printf("====================一元多项式相乘====================\n");

    printf("Please input Pa:(End for 0,0)");
    Pa = LinkList();
    Pa = CreatPolyn(Pa);
    printf("Pa = ");
    PrintPolyn(Pa);

    printf("Please input Pb:(End for 0,0)");
    Pb = LinkList();
    Pb = CreatPolyn(Pb);
    printf("Pb = ");
    PrintPolyn(Pb);

    Pc = MultiplyPolyn(Pa, Pb);
    printf("\nPc = Pa * Pb\n");
    printf("Pc = ");
    PrintPolyn(Pc);
    DestroyPolyn(Pc);

    return 0;
}
