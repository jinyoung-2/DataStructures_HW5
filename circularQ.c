/*
 * circularQ.c
 *
 *  Data Structures, Homework #5
 *  Department of Computer Science at Chungbuk National University
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4	//큐의 크기를 4로 지정

typedef char element;  //큐의 원소의 자료형을 char 자료형으로 설정
typedef struct {
	element queue[MAX_QUEUE_SIZE];  //크기가 MAX_QUEUE_SIZE인 1차원배열 큐 선언
	int front, rear;
}QueueType;  //구조체 선언

QueueType *createQueue();
int freeQueue(QueueType *cQ);
int isEmpty(QueueType *cQ);
int isFull(QueueType *cQ);
void enQueue(QueueType *cQ, element item);
void deQueue(QueueType *cQ, element* item);
void printQ(QueueType *cQ);
void debugQ(QueueType *cQ);
element getElement();

int main(void)
{
	QueueType *cQ = createQueue();  //원형 큐 생성 
	element data;
	char command;  
    printf("[----- [jinyounglee] [2020039063] -----]\n");

	do{
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'i': case 'I':
			data = getElement();
			enQueue(cQ, data);
			break;
		case 'd': case 'D':
			deQueue(cQ, &data);
			break;
		case 'p': case 'P':
			printQ(cQ);
			break;
		case 'b': case 'B':
			debugQ(cQ);
			break;
		case 'q': case 'Q':
   	        freeQueue(cQ);
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');  //반복 조건 


	return 1; //정상종료 
}

/* 원형 큐 형성 함수 */
QueueType *createQueue()
{
	QueueType *cQ; //구조체 QueueType를 가리키는 포인터 변수 cQ 선언
	cQ = (QueueType *)malloc(sizeof(QueueType));  // 동적할당 
	//후처리검사 
	if(cQ==NULL) //동적할당 실패
		return NULL;
	/*원형 큐가 형성될 땐, 값이 아무것도 존재하지 않으므로
	   front=rear=0 */
	cQ->front = 0; 	//front를 0으로 초기화
	cQ->rear = 0;	//rear를 0으로 초기화
	return cQ;
}

/* 원형 큐 해제하는 함수*/
int freeQueue(QueueType *cQ)
{
    if(cQ == NULL) return 1;  //cQ가 동적할당에 실패
    free(cQ);	//cQ 동적할당 해제 
	//후처리 검사
	cQ=NULL;
	if(cQ!=NULL)
	{
		printf("Error\n");
		freeQueue(cQ);  
	}
    return 1;
}


/* 원형 큐에 원소 input하는 함수 */
element getElement()
{
	element item;
	printf("Input element = ");
	scanf(" %c", &item);  //item 입력
	return item;
}

/* 원형 큐가 비어있는지 확인하는 함수 */
int isEmpty(QueueType *cQ)
{
	if(cQ->rear==cQ->front)
	{
		printf("Circular Queue is Empty!\n");
    	return 0;   //비어있음
	}
    else
		return 1;	//비어있지 않음
}

/* 원형 큐가 가득찼는지 확인하는 함수 */
int isFull(QueueType *cQ)
{
	/* 큐에 공간 하나를 비워둠(for 큐가 empty일때와 full일때 구분)
		front가 위치한 곳은 항상 데이터가 들어올 수 없음
	 	원형큐는 구조상, front는 큐의 첫 원소의 위치로부터 시계 반대 방향으로 하나 앞 위치를 가리키므로*/	
   if((cQ->rear+1)%MAX_QUEUE_SIZE==cQ->front)  
   {										      											 
		printf("Circular Queue is Full!\n");
   	    return 0;  //가득참
   }
   else
		return 1; //가득차지 않음
}


/* 원형 큐에 원소를 삽입하는 함수 */
void enQueue(QueueType *cQ, element item)
{
	//전처리검사 - 가득찼으면 원소를 삽입하지 못함
	if(isFull(cQ)==0)
		printf("Circular Queue is Full!\n");
	else
	{
		cQ->rear=(cQ->rear+1)%MAX_QUEUE_SIZE;  //rear 1 증가
		cQ->queue[cQ->rear]=item;	//큐의 rear위치에 원소 대입(삽입)
	}

	return 0;  
}

/* 원형 큐의 원소를 제거하는 함수 */
void deQueue(QueueType *cQ, element *item)  //data의 주소를 가리키는 포인터 변수 item
{
	//전처리검사 - 비어있으면 원소를 제거하지 못함
	if(isEmpty(cQ)==0)
		printf("Circular Queue is Empty!\n");
	else
	{	
		cQ->front = (cQ->front + 1) % MAX_QUEUE_SIZE; 	//front 1 증가
		*item = cQ->queue[cQ->front];		//다음 dequeue될 원소의 값 대입 
		/*item=&(cQ->queue[cq->front])*/ 	//다음 dequeue될 원소의 주소로 변경
	}

	return 0;  
}

/* 원형 큐 출력함수 */
void printQ(QueueType *cQ)
{
	int i, first, last;

	//front와 rear를 0으로 초기화하여, 값을 출력하려면 +1을 해줘야 함.(module 연산 이용)
	first = (cQ->front + 1)%MAX_QUEUE_SIZE;  
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE;

	printf("Circular Queue : [");

	i = first;
	while(i != last){ //값을 출력하려면, i가 rear를 넘어서는 안 됨. 
		printf("%3c", cQ->queue[i]);
		i = (i+1)%MAX_QUEUE_SIZE;

	}
	printf(" ]\n");
}

/*디버그 하는 함수*/
void debugQ(QueueType *cQ)
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++)
	{
		if(i == cQ->front) {  
			printf("  [%d] = front\n", i);
			continue;
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]);  // 큐의 원소값 출력

	}
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear);  //front, rear 위치 출력
}