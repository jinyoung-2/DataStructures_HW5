/*
 * postfix.c
 *
 *  Data Structures, Homework #5
 *  Department of Computer Science at Chungbuk National University
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20

 /* stack 내에서 우선순위, lparen = 0 가장 낮음 */
 // 구조체 선언    --(,피연산자,뺄셈,덧셈,나눗셈,곱셈,) 순!!
typedef enum {
	lparen = 0,  /* ( 왼쪽 괄호 */
	rparen = 9,  /* ) 오른쪽 괄호 */
	times = 7,   /* * 곱셈 */
	divide = 6,  /* / 나눗셈 */
	plus = 5,    /* + 덧셈 */
	minus = 4,   /* - 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];   	/* infix expression을 저장하는 배열 */
char postfixExp[MAX_EXPRESSION_SIZE];	/* postfix로 변경된 문자열을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE];		/* postfix로 변환을 위해 필요한 스택 */
int evalStack[MAX_STACK_SIZE];			/* 계산을 위해 필요한 스택 */

int postfixStackTop = -1;  /* postfixStack용 top */
int evalStackTop = -1;	   /* evalStack용 top */

int evalResult = 0;	   	   /* 계산 결과 저장 */

void postfixPush(char x);
char postfixPop();
void evalPush(int x);
int evalPop();
void getInfix();
precedence getToken(char symbol);
precedence getPriority(char x);
void charCat(char* c);
void toPostfix();
void debug();
void reset();
void evaluation();

int main()
{
	char command;
	printf("[----- [jinyounglee] [2020039063] -----]\n");

	do {
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch (command) {
		case 'i': case 'I':
			getInfix();
			break;
		case 'p': case 'P':
			toPostfix();
			break;
		case 'e': case 'E':
			evaluation();
			break;
		case 'd': case 'D':
			debug();
			break;
		case 'r': case 'R':
			reset();
			break;
		case 'q': case 'Q':
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	} while (command != 'q' && command != 'Q');  //반복 조건

	return 1; //정상종료
}

/*postfixStack에 원소 삽입하는 함수*/
void postfixPush(char x)  
{
	if (postfixStackTop >= MAX_STACK_SIZE - 1)  //Stack에 더 이상 원소를 추가할 수 없음
		printf("postfixStack is Full\n");
	else
		postfixStack[++postfixStackTop] = x; //postfixStackTop을 증가시킨 후, 해당 Top위치에 원소 x 대입(삽입)
}

/*postfixStack에서 원소 제거하는 함수*/
char postfixPop()  
{
	char x;
	if (postfixStackTop == -1) { //stack이 비어있으면 제거할 원소가 없으므로, \0 리턴
		printf("postfixStack is Empty\n");
		return '\0';  //<정리> NULL=\0 
	}
	else 
		x = postfixStack[postfixStackTop--]; //Top 위치에 해당하는 원소를 x에 대입한 후, Top위치 1 감소시키기
	return x;  //삭제된 원소 x 반환
}

/* evalStack에 원소 삽입하는 함수*/
void evalPush(int x)	
{
	if (evalStackTop >= MAX_STACK_SIZE - 1)  //Stack에 더 이상 원소를 추가할 수 없음
		printf("evalStack is Full\n");
	else
		evalStack[++evalStackTop] = x;	// evalStackTop을 증가시킨 후, 해당 Top위치에 원소 x 대입(삽입)
}

/*evalStack에서 원소 제거하는 함수*/
int evalPop()	
{
	if (evalStackTop == -1)	//stack이 비어있으면 제거할 원소가 없음.
	{
		printf("evalStack is Empty\n");
		return -1; 
	}
	else
		return evalStack[evalStackTop--];	//Top 위치에 해당하는 원소를 x를 반환한 후, Top위치 1 감소시키기
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix()
{
	printf("Type the expression >>> ");
	scanf("%s", infixExp);
	/* 후처리검사 */
	for (int i = 0; i < strlen(infixExp); i++)
	{
		if (((int)infixExp[i] >= 0 && (int)infixExp[i] > 9) || infixExp[i] == '+' || infixExp[i] == '-' || infixExp[i] == '*' || infixExp[i] == '/' || infixExp[i] == '(' || infixExp[i] == ')')
			continue;
		else  //입력 불가능한 값 입력시 강제 종료
		{
            printf("Impossible Input\n");
            return 0;
        }	
	}
}

/* 입력된 원소의 우선순위 판단 가능한 함수 */
precedence getToken(char symbol) 
{
	switch (symbol) {
	case '(': return lparen;
	case ')': return rparen;
	case '+': return plus;
	case '-': return minus;
	case '/': return divide;
	case '*': return times;
	default: return operand;
	}
}

/* 우선순위 판단 가능한 함수 호출하는 함수 */
precedence getPriority(char x)  
{
	return getToken(x);
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c)
{
	if (postfixExp == NULL)  //postfixExp에 아무 값도 없다면(NULL이라면)    //제출하기 전, if (postfixExp == '\0')로 수정
		strncpy(postfixExp, c, 1);	//c의 첫번째 원소를 postfixExp에 복사한다
	else					//postfixExp에 값이 존재하다면,
		strncat(postfixExp, c, 1);	//c의 첫번째 위치부터 postfixExp에 연결시킨다
}

/*
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixEXP에 저장된다.
 */
void toPostfix()
{
	char* exp = infixExp;	/* infixExp의 문자 하나씩을 읽기 위한 포인터 */
	char x; /* 문자하나를 임시로 저장하기 위한 변수 */      //pop()을 통해 제거된 원소

	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	while (*exp != '\0')
	{
		if (getPriority(*exp) == 0)	//왼쪽 괄호일 경우!!!!!
			postfixPush(*exp);

		if (getPriority(*exp) == 9)	//오른쪽 괄호일 경우, 
		{
			x=postfixPop();
			while (x != '(') //pop한 값이 왼쪽괄호이기 전까지 반복
			{
				charCat(&x);  //postfixExp에 추가
				x=postfixPop();
			}
			//while문을 빠져나왔을 때는, 왼쪽 괄호를 pop했을 때임.
		}

		if ((getPriority(*exp)>=4)&& (getPriority(*exp) <= 7))  //'('와')'를 제외한 연산자일 경우
		{
			if (getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp))  //스택 top위치의 연산자 우선순위>=infixExp의 값 x
			{
				x=postfixPop();
				charCat(&x); //스택 top위치의 연산자는 postfixExp에 추가
				postfixPush(*exp); //x는 postfixStack에 push
			}
			else
				postfixPush(*exp); //postfixStack에 x를 push
		}

		if (getPriority(*exp) == 1)  //피연산자일 경우, 바로 postfixExp배열에 추가
			charCat(exp);  

		exp++;
	}

	while (postfixStackTop != -1)  //postfixStack에 있는 연산자들을 모두 pop해서 postfixExp에 추가
	{
		x = postfixPop();
		charCat(&x);
	}
}


/* 정보 출력(디버깅) 함수 */
void debug()
{
	//출력
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);
	printf("postExp =  %s\n", postfixExp);
	printf("eval result = %d\n", evalResult);

	printf("postfixStack : ");
	for (int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]);

	printf("\n");

}

/* reset(초기화)시키는 함수 */
void reset()
{
	//reset(초기화)을 위해 NULL 대입
	infixExp[0] = '\0';
	postfixExp[0] = '\0';

	for (int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0';

	postfixStackTop = -1;
	evalStackTop = -1;
	evalResult = 0;
}

/* postfixExp을 이용한 postfix계산 */
void evaluation()		
{
	int i = 0;
	char ev1, ev2;
	while (i < strlen(postfixExp))   
	{
		/* 연산자가 이항연산자로, 피연산자 2개가 필요함. -> pop을 두 번한 후 계산한 값을 다시 push */
		if (postfixExp[i] == '+')
		{
			ev2 = evalPop();
			ev1 = evalPop();
			evalPush(ev1 + ev2);
		}
		else if (postfixExp[i] == '-')
		{
			ev2 = evalPop();
			ev1 = evalPop();
			evalPush(ev1 - ev2);	//2번째로 pop한 피연산자가 원래의 식에서는 먼저 나타난 피연산자임 
		}
		else if (postfixExp[i] == '*')
		{
			ev2 = evalPop();
			ev1 = evalPop();
			evalPush(ev1 * ev2);
		}
		else if (postfixExp[i] == '/')
		{
			ev2 = evalPop();
			ev1 = evalPop();
			evalPush(ev1 / ev2);	//2번째로 pop한 피연산자가 원래의 식에서는 먼저 나타난 피연산자임 
		}
		else  //피연산자라면
		{
			evalPush(postfixExp[i]-'0');	/* postfixExp에 들어있는 원소는 문자이므로, 아스키코드를 이용하여 숫자로 변환시킴 */
		}
		i++;
	}
	//evalStack의 최종 답만 남은 상황
	evalResult = evalStack[evalStackTop];  
}