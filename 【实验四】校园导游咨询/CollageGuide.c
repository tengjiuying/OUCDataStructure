#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#define TRUE 1
#define FALSE 0
#define MAX_VERTEX_NUM 13//最大顶点个数，在此及景点个数
#define INFINITY 30000//默认最大值
//备注：无穷大要自己备注，因为在DIJ算法中会出现无穷大加一个数，如果此时把无穷大设置为系统的无穷大，则会越界。

typedef struct{//顶点
    int num;//景点序号
    char *name;//景点名字
    char *info;//景点简介
}VertexType;

typedef struct{//图
    VertexType vexs[MAX_VERTEX_NUM];
    int arcs[MAX_VERTEX_NUM][MAX_VERTEX_NUM];//数组为权值
    int vexnum, arcnum;//图的当前顶点数和弧数
}MGrpah;

//给顶点赋值
VertexType CreatPlace(int num, char *name, char *info){
    VertexType v;
    v.num = num;
    v.name = name;
    v.info = info;
    return v;
}//VertexType

//创建地图
void CreatMGrpah(MGrpah *G){
    //设置顶点（景点）
    G->vexnum = MAX_VERTEX_NUM;
    G->arcnum = 23;
    G->vexs[0] = CreatPlace(0,"北门外","撸串好地方");
    G->vexs[1] = CreatPlace(1,"旧操场","崂山区最早的操场");
    G->vexs[2] = CreatPlace(2,"北区餐厅","好吃的集中地");
    G->vexs[3] = CreatPlace(3,"新操场","厕所很臭，偶尔还有远方飘来的大气污染");
    G->vexs[4] = CreatPlace(4,"体育馆","要钱的，还有点阴森");
    G->vexs[5] = CreatPlace(5,"西门","海大正门");
    G->vexs[6] = CreatPlace(6,"凯旋门","高大的门，从该门进入大学，从该门走出大学");
    G->vexs[7] = CreatPlace(7,"九蛋广场","九颗石球围成的广场");
    G->vexs[8] = CreatPlace(8,"教学区","跑男好去处");
    G->vexs[9] = CreatPlace(9,"行远楼","海大行远楼，一跳解千愁");
    G->vexs[10] = CreatPlace(10,"信院","海大规模最大的学院");
    G->vexs[11] = CreatPlace(11,"图书馆","好好学习天天向上");
    G->vexs[12] = CreatPlace(12,"五子顶","海大最高峰");

    //给邻接矩阵赋值
    int i, j;
    for(i = 0; i < MAX_VERTEX_NUM; i++)
        for(j = 0; j < MAX_VERTEX_NUM; j++)
            if(i == j)
                G->arcs[i][j] = 0;
            else
                G->arcs[i][j] = INFINITY;

    G->arcs[0][2] = G->arcs[2][0] = 52;
    G->arcs[0][3] = G->arcs[3][0] = 100;
    G->arcs[0][5] = G->arcs[5][0] = 105;
    G->arcs[1][2] = G->arcs[2][1] = 20;
    G->arcs[1][3] = G->arcs[3][1] = 53;
    G->arcs[1][4] = G->arcs[4][1] = 40;
    G->arcs[1][5] = G->arcs[5][1] = 38;
    G->arcs[2][3] = G->arcs[3][2] = 41;
    G->arcs[2][4] = G->arcs[4][2] = 47;
    G->arcs[2][7] = G->arcs[7][2] = 39;
    G->arcs[2][8] = G->arcs[8][2] = 43;
    G->arcs[2][12] = G->arcs[12][2] = 41;
    G->arcs[3][11] = G->arcs[11][3] = 110;
    G->arcs[4][5] = G->arcs[5][4] = 15;
    G->arcs[4][6] = G->arcs[6][4] = 11;
    G->arcs[5][6] = G->arcs[6][5] = 18;
    G->arcs[5][8] = G->arcs[8][5] = 55;
    G->arcs[5][9] = G->arcs[9][5] = 100;
    G->arcs[5][10] = G->arcs[10][5] = 76;
    G->arcs[6][7] = G->arcs[7][6] = 13;
    G->arcs[6][8] = G->arcs[8][6] = 30;
    G->arcs[6][9] = G->arcs[9][6] = 74;
    G->arcs[7][8] = G->arcs[8][7] = 15;
    G->arcs[8][9] = G->arcs[9][8] = 67;
    G->arcs[8][11] = G->arcs[11][8] = 32;
    G->arcs[8][12] = G->arcs[12][8] = 24;
    G->arcs[9][10] = G->arcs[10][9] = 50;
    G->arcs[9][11] = G->arcs[11][9] = 43;
    G->arcs[10][11] = G->arcs[11][10] = 44;
    G->arcs[11][12] = G->arcs[12][11] = 47;
}//CreatMGrpah

void ShortestPath_DIJ(MGrpah *G, int v0 ,int P[][G->vexnum], int *D){
    //用Dijkstra算法求无向图G的v0顶点到其余顶点v的最短路径P[v]及其带权长度D[v]。
    //若P[v][w]为TRUE，则w是从v0到v当前求得最短路径上的顶点。
    //final[v]为TRUE当且仅当v∈S，即已经求得从v0到v的最短路径。
    int v, i, j, w, min;
    int final[G->vexnum];

    for(v = 0; v < G->vexnum; ++v){
        final[v] = FALSE;
        D[v] = G->arcs[v0][v];
        for(w = 0; w < G->vexnum; ++w)
            P[v][w] = FALSE;//设空路径
        if(D[v] < INFINITY){
            P[v][v0] = TRUE;
            P[v][v] = TRUE;
        }//if
    }//for
    D[v0] = 0;
    final[v0] = TRUE;//初始化，v0顶点属于S集
    //开始主循环，每次求得v0到某个v顶点的最短路径，并加v到S集
    for(i = 1; i < G->vexnum; ++i){//其余G.vexnum-1个顶点
        min = INFINITY + 1;//当前所知离v0顶点的最近距离
        for(w = 0; w < G->vexnum; ++w)
            if(!final[w])//w顶点在V-S中
                if(D[w] < min){//w顶点离v0顶点更近
                    v = w;
                    min = D[w];
                }//if
        final[v] = TRUE;//离v0顶点最近的v加入S集
        for(w = 0; w < G->vexnum; ++w)//更新当前最短路径及距离
            if(!final[w] && (min + G->arcs[v][w] < D[w])){//修改D[w]和P[w]，w∈V-S
                D[w] = min + G->arcs[v][w];
                for(j = 0; j < G->vexnum; j++)
                    P[w][j] = P[v][j];
                P[w][w] = TRUE;//P[w]=P[v]+[w]
            }//if
    }//for
}//ShortestPath_DIJ

//查询景点信息
void SeeInfo(MGrpah *G){
    int i, n;
    printf("There are %d scenic spots.\n",G->vexnum);
    for(i = 0; i < G->vexnum; i++)
        printf("    %-2d: %s\n",G->vexs[i].num,G->vexs[i].name);

    printf("\n输入你想查看的景点信息的景点序号(%d结束该功能)：",G->vexnum);
    scanf("%d",&n);
    while(n != G->vexnum){
        while(n < 0 || n > G->vexnum){
            printf("输入错误，请重新输入(%d结束该功能)：",G->vexnum);
            scanf("%d",&n);
        }
        if(n != G->vexnum){
            printf("%-2d: %s  %s\n",G->vexs[n].num,G->vexs[n].name,G->vexs[n].info);
            printf("\n继续查看景点信息请输入序号，(%d结束该功能)：",G->vexnum);
            scanf("%d",&n);
        }
    }
}//SeeInfo

//查询最短路径
void Search_Path(MGrpah *G){
    int i, start, end;
    printf("There are %d scenic spots.\n",G->vexnum);
    for(i = 0; i < G->vexnum; i++)
        printf("    %-2d: %s\n",G->vexs[i].num,G->vexs[i].name);

    printf("\n请输入起点和目的地的序号：(不在序号范围则退出该功能)");
    scanf("%d %d",&start, &end);

    while(0 <= start && start < G->vexnum && 0 <= end && end < G->vexnum){
        int P[G->vexnum][G->vexnum], D[G->vexnum];
        ShortestPath_DIJ(G, start, P, D);

        if(D[end] >= INFINITY)
            printf("无法达到！\n");
        else if(D[end] == 0)
            printf("原地！\n");
        else{
            printf("最短距离是：%d\n",D[end]);
            printf("最短路径是：");

            int i, j;
            int count = 0;//记录路径的顶点个数
            for(i = 0; i < G->vexnum; i++)
                if(P[end][i])
                    count++;

            int a[count];//记录路径的顶点序号
            a[0] = start;//保证头尾是用户输入的始终点
            a[count - 1] = end;
            for(i = 0, count = 1; i < G->vexnum; i++)
                if(P[end][i] && i != start && i != end)
                    a[count++] = i;
            count = count + 1;

            //让数组a按路径顺序排序
            int min1, tmp, i1;//均为中间变量，min1记录最小的权值，tmp用于交换序号,i1记录权值最小的序号
            for(i = 0; i < count - 1; i++){
                min1 = INFINITY;
                for(j = i + 1; j < count; j++)
                    //可能有多条通路，那么就选选择权值最小的
                    if(G->arcs[a[i]][a[j]] < min1){
                        min1 = G->arcs[a[i]][a[j]];
                        i1 = j;
                    }//if
                tmp = a[i1];
                a[i1] = a[i + 1];
                a[i + 1] = tmp;
            }//for

            for(i = 0; i < count - 1; i++)
                printf("%d%s -> ",a[i],G->vexs[a[i]].name);
            printf("%d%s\n",a[i],G->vexs[a[i]].name);
        }//else

        printf("\n继续查询请直接输入起点和目的地序号，不在序号范围则退出该功能：");
        scanf("%d %d",&start, &end);
    }//while
}//Search_Path

int main(){
    MGrpah *G = (MGrpah*)malloc(sizeof(MGrpah));
    CreatMGrpah(G);

    int fic;//fic是用户选择的功能序号
    printf("------Welcome to OUC!-------\n");
    printf("\n----------------------------------\n");
    printf("本系统提供两个功能：\n   1、查询某个景点的简介。\n   2、查询两景点间最短距离和路径。");
    printf("\n----------------------------------\n");
    printf("\n请选择功能(1 or 2)(退出该系统请输入其他数字)：");
    scanf("%d",&fic);

    while(fic == 1 || fic ==2){
        system("cls");
        switch(fic){
            case 1:SeeInfo(G);break;
            case 2:Search_Path(G);break;
        }//switch
        system("cls");
        printf("----------------------------------\n");
        printf("本系统提供两个功能：\n   1、查询某个景点的简介。\n   2、查询两景点间最短距离和路径。\n");
        printf("----------------------------------\n");
        printf("继续使用请输入功能序号(1 or 2)，退出则输入其他数字：");
        scanf("%d",&fic);
    }

    printf("\nThanks! Good Bye!\n");
    free(G);
    return 0;
}
