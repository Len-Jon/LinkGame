/*连连看*/
#include<stdio.h>
#include<time.h>
#include<stdlib.h>

void rank(FILE *fl)//读取排行榜数据 
{
	rewind(fl);
	long score[4][5];
	int i , j;
	
	fread(score,sizeof(long),20,fl);
	
	printf("排行榜\n");
	
	for(i=0;i<4;i++)
	{
		if(i==0)
		{
			printf("4*4 限时\n");
		}
		else if(i==1)
		{
			printf("6*6 限时\n");
		}
		else if(i==2)
		{
			printf("4*4 不限时\n");
		}
		else
		{
			printf("6*6 不限时\n");
		}
		for(j=0;j<5;j++)
		{
			printf("No.%d-%dms\t",j+1,score[i][j]);
		}
		printf("\n\n");
	}
	rewind(fl);
	
}

void initialize(char **array , int lenth)//初始化数组 
{	
    srand(time(NULL));
	int l = lenth - 2;
	int i , j;
	
	for(i = 0 ; i < lenth ; i++)//全体初始化 
	{
		for(j = 0 ; j < lenth ; j++)
		{
			array[i][j] = ' ';
		}
	}
	for(i = 1 ; i <= l/2 ; i++)//随机生成元素 
	{
		for(j = 1 ; j <= l ; j++)
		{
			array[i][j] = rand()%26 + 65 ;
			
			array[i+l/2][j] = array[i][j];
		}
	}
}

void restart(char **array , int lenth)//restart重置位置 
{
	srand(time(NULL)); 
	int l = lenth - 2;
	int i , x1 , y1 , x2 , y2;
	char temp;
	
	for(i = 1 ; i <= l*l/2 ; i++)//随机交换 次数为元素总个数的一半 
	{
		x1 = rand()%l + 1;
		y1 = rand()%l + 1;
		x2 = rand()%l + 1;
		y2 = rand()%l + 1;
		
		temp = array[x1][y1];
		array[x1][y1] = array[x2][y2];
		array[x2][y2] = temp;
	}	
}

void map(char **array , int lenth)//游戏输出界面 
{
	system("cls");
	int i , j;
	
	for(i = 1 ; i < lenth ; i++)
	{
		if(i == 1)
		{
			for(j = 0 ; j < lenth - 1 ;j++)
			{
				if(j == 0)
				{
					printf("  ");
				}
				else
				{
					printf("%2d" , j);
				}
			}
			printf("\n");
		}
		else
		{
			printf("%2d" , i-1);
			
			for(j = 1 ; j < lenth - 1 ; j++)
			{
				printf("%2c" , array[i - 1][j]);
			}
			printf("\n"); 
		}
	}
} 
 
void clear(char **array , int r1 , int l1 , int r2 , int l2)//消除元素
{
	array[r1][l1] = ' ';
	array[r2][l2] = ' ';
} 

int search0(char **array , int r1 , int l1 , int r2 , int l2)//直线检测 
{//两点之间全为空格 
	int flag = 0;
	int i ;
	int min,max;
	
	if(r1 == r2 || l1 == l2)
	{
		if(r1 == r2)
		{
			min = l1<l2 ? l1 : l2;
			max = l1>l2 ? l1 : l2;
			
			if(1 == max-min)
			{
				flag = 1;
			}
			else
			{
				for(i=min+1 ; i<max ; i++)
				{
					if(array[r1][i] != ' ')
					    break;
				}
				if(i == max)
				{
					flag = 1;
				}
				else
				{
					flag = 0;
				}
			}
		}
		else
		{
			min = r1<r2 ? r1 : r2;
			max = r1>r2 ? r1 : r2;
			
			if(1 == max-min)
			{
				flag = 1;
			}
			else
			{
				for(i=min+1 ; i<max ; i++)
				{
					if(array[i][l1] != ' ')
				    	break;
				}
				if(i == max)
				{
					flag = 1;
				}
				else
				{
					flag = 0;
				}
			}
		}
	}
	else
	{
		flag = 0;
	}
	return flag; 
} 

int search1(char **array , int r1 , int l1 , int r2 , int l2)//单拐点检测 
{//画两个十字，如有单拐点，拐点必定是其中一个交点 
	int flag = 0;
	int x,y;
	if(search0(array,r1,l1,r1,l2) && array[r1][l2] == ' ')//注意拐点本身也为空格 
	{
		if(search0(array,r1,l2,r2,l2))
		{
			flag = 1;
		}
	}
	if(search0(array,r1,l1,r2,l1) && array[r2][l1] == ' ')//同上 
	{
		if(search0(array,r2,l1,r2,l2))
		{
			flag = 1;
		}
	}
	return flag;
} 

int search2(char **array , int r1 , int l1 , int r2 , int l2 , int lenth)//双拐点检测 
{//遍历一个十字上的除了它本身的所有点，满足search0且拐点满足search1 
	int flag = 0;
	int i;
	int row,line;
	
	for(i=0; i<lenth ; i++)//按列遍历 
	{
		row = r1;
		line = i;
		
		if(array[row][line] != ' ')//临时点不为空格则跳过 
		{
			 continue;
		}

		if(search0(array,row,line,r1,l1) && search1(array,row,line,r2,l2))
		{
			flag = 1;
		}
		
	} 
	
	for(i=0; i<lenth ; i++)//按行遍历 
	{
		row = i;
		line = l1;
		
		if(array[row][line] != ' ')
		{
			 continue;
		}
		
		if(search0(array,row,line,r1,l1) && search1(array,row,line,r2,l2))
		{
			flag = 1;
		}
		
	} 
	return flag;
}
 
int check(char **array , int r1 , int l1 , int r2 , int l2 , int lenth)//检查输入 正确返回1 错误返回0 
{
	int flag;
	if(array[r1][l1] != array[r2][l2])
	{
		flag = 0;
	}
    else if(search0(array,r1,l1,r2,l2))
	{
		flag = 1;
	}
	else if(search1(array,r1,l1,r2,l2))
	{
		flag = 1;
	}
	else if(search2(array,r1,l1,r2,l2,lenth))
	{
		flag = 1;
	}
	else
	{
	    flag = 0;
	}
	return flag;
} 

int victory(char **array , int  lenth)//游戏结束判断 :完成返回0 ， 未完成返回1 
{
	int flag = 0;
	int i , j ;
	for(i = 1 ; i < lenth -1 ; i++)
	{
		for(j = 0 ; j < lenth -1 ; j++)
		{
			if(array[i][j] != ' ')
			{
				flag = 1;
			}
		}
	}
	return flag;
}

void insert(FILE *fl , long time , int lenth , int mode)//成绩排序并插入 
{
	rewind(fl);
	long score[4][5];
	long *p;
	int i , j , index;
	
	fread(score , sizeof(long) , 20 , fl);
	rewind(fl);
	
	switch(mode)//根据模式取指针 
	{
		case 1:
		    switch(lenth)
		    {
		    	case 6: p = *(score+0);break;
		    	
		    	case 8: p = *(score+1);break;
			}break;
		
		case 2:
			switch(lenth)
			{
				case 6: p = *(score+2);break;
				
				case 8: p = *(score+3);break;
			}break;
	}
	
	for(i = 0 ; i < 5 ; i++)//排序填入 
	{
		if(p[i]==0)//排除初始化的数据 
		{
			p[i] = time; 
			
			break;
		}
		if(p[i]>time)
		{
			for(j=4;j>i;j--)
			{
				p[j]=p[j-1];
			}
			p[i] = time;
			break;
		}
	}	
	fwrite(score,sizeof(long),20,fl);
	
	rewind(fl);
}




void GAME_1(FILE *fl , char **array , int lenth)//限时模式 
{
	rewind(fl);
	int row_1 , row_2 , line_1 , line_2;//坐标行列 
	long begin_t,end_t,t;
	long t0,t1;
	
	begin_t = clock();
	do
	{	
	    t0 = clock();
	    
again:	map(array,lenth);
		
		printf("\n15s内输入坐标(如 1,2 2,3 表示第一行第二列与第二行第三列)\n输入 0,0 0,0 重新排序\n");

		scanf("%d,%d %d,%d", &row_1 , &line_1 , &row_2 , &line_2);

		if(row_1==0 && row_2==0 && line_1==0 && line_2==0)
		{
			restart(array,lenth);
			
			goto again;
		}
		
		if( (row_1 == row_2 && line_1 == line_2) || row_1<1 || row_1>lenth-2 || line_1<1 || line_1>lenth-2 || row_2<1 || row_2>lenth-2 || line_2<1 || line_2>lenth-2)
		{
			printf("请输入合法坐标！\n");
			
			system("pause");
			
			goto again;
		}		//检查输入数据 

		if(check(array,row_1,line_1,row_2,line_2,lenth))
		{
			t1 = clock();
			if(t1 - t0 > 15000)
			{
				break;
			}
			clear(array,row_1,line_1,row_2,line_2);
		}
		else
		{
			printf("不符合条件！\n");
			
			system("pause");
		}
	}while(victory(array , lenth));
	
	if(!victory(array,lenth))
	{
		system("cls");
		
		map(array,lenth);
		
		end_t = clock();
		
		t = end_t - begin_t;
		
		printf("You Win!\n",t);
		
		printf("用时%dms\n",t);
		
		insert(fl,t,lenth,1);
		
		system("pause");
	}
	else
	{
		system("cls");
		printf("TIME OUT!\n\nGAME OVER!\n");
	} 
} 

void GAME_2(FILE *fl , char **array , int lenth)//不限时模式 
{
	rewind(fl);
	int row_1 , row_2 , line_1 , line_2;//坐标行列 
	long begin_t,end_t,t;
	begin_t = clock();
	do
	{
		map(array,lenth);
		
		printf("\n输入坐标(如 1,2 2,3 表示第一行第二列与第二行第三列)\n输入 0,0 0,0 重新排序\n");

		scanf("%d,%d %d,%d", &row_1 , &line_1 , &row_2 , &line_2);

		if(row_1==0 && row_2==0 && line_1==0 && line_2==0)
		{
			restart(array,lenth);
			
			continue;
		}
		
		if( (row_1 == row_2 && line_1 == line_2) || row_1<1 || row_1>lenth-2 || line_1<1 || line_1>lenth-2 || row_2<1 || row_2>lenth-2 || line_2<1 || line_2>lenth-2)
		{
			printf("请输入合法坐标！\n");
			
			system("pause");
			
			continue;
		}		//检查输入数据 

		if(check(array,row_1,line_1,row_2,line_2,lenth))
		{
			clear(array,row_1,line_1,row_2,line_2);
		}
		else
		{
			printf("不符合条件！\n");
			
			system("pause");
		}
	}while(victory(array , lenth));
	
	if(!victory(array,lenth))
	{
		system("cls");
		
		map(array,lenth);
		
		end_t = clock();
		
		t = end_t - begin_t;
		
		printf("You Win!\n",t);
		
		printf("用时%dms\n",t);
		
		insert(fl,t,lenth,2);
		
		system("pause");
	}
}

int main()
{
	/*定义部分*/
	int select;
	int lenth;
	char **array;
	int i , j ;

	int mode;
    long score[4][5];

	FILE *fl = fopen("rank3.0","rb+");    //排行榜数据文件 
	if(fl == NULL)
	{
		fl = fopen("rank3.0","wb+");
		for(i = 0 ; i < 4 ; i++)
		{
			for(j = 0 ; j < 5 ; j++)
			{
				score[i][j] = 0;
			}
		}
		for(i = 1 ; i <= 20 ; i++ )
		{
			fwrite(score,sizeof(long),20,fl);
		}
		rewind(fl);
	}
	
	/*定义部分*/ 
	
//------------------------------------------------//

    /*初始界面*/	
op:	printf("连连看3.0\n\t----Wei Qingbin\n");
	
    printf("1.进入游戏\n2.排行榜\n3.退出游戏\n");

	scanf("%d" , &select);
	
	switch(select)
	{
		case 1: system("cls"); break;
		
		case 2: system("cls"); rank(fl) ; goto op ; break; /*读取排行榜数据*/
		
		case 3: goto exto;break;
		
		default:printf("请输入有效数字\n");system("pause");system("cls");goto op;break;
	}
	/*初始界面*/ 
	
//------------------------------------------------//	
	
sel1:	printf("选择难度\n1.4*4\n2.6*6\n");//难度选择 
	
	scanf("%d" , &select);
	
	switch(select)
	{
		case 1:lenth = 6;break;
		
		case 2:lenth = 8;break;
		
		default:printf("别调皮呀！\n");system("pause");system("cls");goto sel1;break;
	}
	
//--------------------------------------------------//
	/*申请二维数组*/
	array = (char **)calloc(lenth , sizeof(char*)); 
	
	for(i = 0 ; i <lenth ; i++)
	{
		array[i] = (char *)calloc(lenth , sizeof(char));
	}
	
//--------------------------------------------------//
	
sel2:	printf("1.限时模式\n2.不限时模式\n");
	
	scanf("%d",&mode);
	


	
    initialize(array , lenth);
    restart(array , lenth);

    switch(mode)
	{
		case 1:GAME_1(fl , array , lenth);break;
		
		case 2:GAME_2(fl , array , lenth);break;
		
		default:printf("别闹了T。T\n"); system("pause");system("cls");goto sel2;break;
	}	

	free(array);
	
	goto op;
	
exto:fclose(fl);
	
	return 0;
}
