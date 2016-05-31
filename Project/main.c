#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_LENGTH 100
//#define swap(a,b) {int t; t = a; a = b; b = t;}

// 월별 일 수
int date[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// 스택 데이터 개수
int mCount;

// 스케줄 구조체
struct mSchedule {
    int year;
    int month;
    int day;
    char schedule[20];
};

struct mVariable {
    char name;
    int value;
};

struct mSchedule mSchedule[MAX_LENGTH];

// 스케줄 저장 배열
//int mScheduleYear[MAX_LENGTH] = {0};
//int mScheduleMonth[MAX_LENGTH] = {0};
//int mScheduleDay[MAX_LENGTH] = {0};
//char mSchedule[MAX_LENGTH][20] = {'\0'};

// 스케줄 갯수
int mScheduleCount = 0;

// 스택 함수
void push(char*, char, int);
void pushStr(char*, char*, int);
char pop(char*, int);
double popDouble(double*, int);
void pushDouble(double*, double, int);
int isEmpty(char*);

// 메뉴 함수
void displaySchedule();
void calculator();

// 계산기 함수
char getVariable(char*);
int getValue(char*);
int getLength(char*);
int isOperation(char*);
void transformation(char *str, struct mVariable[]);
int replaceVariableToInt(char*, int, struct mVariable[]);
int replaceMathFunction(char*, int, int, struct mVariable[]);
int getFunctionValue(char*, int, int*, struct mVariable[], int);
int checkFunction(char*);
int isStartWith(char*, char*);
void convertToString(char*, int);
void posifixNotaion();
int checkOperator(char);
int checkOperatorLevel(char, char);
char convertToInt(char);
void printDoubleArray(double[], int);

// 스케줄관리 함수
int checkDateValidation(int, int, int);
int checkDayValidation(int, int, int);
int checkMonthValidationValidation(int);
int schedulerMenu();
int getLeapYear(int, int);
void printCalendar(int, int, int);
int isLeapYear(int);
void insertSchedule(int);
void deleteSchedule();
void sort();

// String 함수
char* removeEnterInFgetsString(char[]);
void copyStr(char*, char*);
int isEqual(char*, char*);


int main(void) {
	int menuChoice;
	int subMenu;

	while(1){
		printf("\tMenu\n");
		printf("1) 공학용 계산기\n");
		printf("2) 스케줄 관리\n");
		printf("3) 프로그램 종료\n");


		scanf("%d", &menuChoice);
		switch(menuChoice) {
			case 1:
				// 계산기
				calculator();
				break;

			case 2:
				while (1) {
					// 스케줄러
					subMenu = schedulerMenu();
					switch(subMenu) {
						case 1:
							insertSchedule(mScheduleCount);
							break;

						case 2:
							deleteSchedule();
							break;

						case 3:
							displaySchedule();
							break;

						case 4:
							system("clear");
							break;
					}
					if(subMenu == 4)
						break;
				}
				break;

			case 3:
				// 종료
				printf("종료됩니다.\n");
				break;
		}
		if(menuChoice == 3)
			break;
	}
	return 0;
}

//************************ 스케줄러 **************************//

// 스케쥴 관리 메뉴 출력
int schedulerMenu() {
	int menu;
	printf("1. 스케줄 입력\n");
	printf("2. 스케줄 삭제\n");
	printf("3. 스케줄 보기\n");
	printf("4. Main menu\n");
	printf("메뉴 : ");
	scanf("%d", &menu);
	return menu;
}


// 스케줄 삭제
void deleteSchedule() {
	int i, j;
	int year, month, day;
	char schedule[MAX_LENGTH];

	printf("입력 : ");
	scanf("%d %d %d", &year, &month, &day);
	getchar();
	fgets(schedule, sizeof(schedule), stdin);
	removeEnterInFgetsString(schedule);
	printf("출력 : %d년 %d월 %d일 %s\n",year,month,day,schedule);

	// 년 월 일 스케줄에 매칭되는 스케줄 index 찾기
	for (i = 0; i < mScheduleCount; i++) {
		if (mSchedule[i].year == year && mSchedule[i].month == month && mSchedule[i].day == day && isEqual(mSchedule[i].schedule, schedule))
			break;
	}

	if (mSchedule[i].year == year && mSchedule[i].month == month && mSchedule[i].day == day && isEqual(mSchedule[i].schedule,schedule)) {
		for (j = i + 1; j < mScheduleCount; j++) {
            mSchedule[j - 1] = mSchedule[j];
//            mSchedule[j - 1].year = mSchedule[j].year;
//			mSchedule[j - 1].month = mSchedule[j].month;
//			mSchedule[j - 1].day = mSchedule[j].day;
//			copyStr(mSchedule[j].schedule, mSchedule[j -1].schedule);
		}
		printf("%d년 %d월 %d일의 일정이 있습니다.\n",year,month,day);
		printf("일정을 삭제하였습니다.\n");
		mScheduleCount--;
        
        // ??????
		mSchedule[mScheduleCount].year = 0;
	} else
	{
		printf("%d년 %d월 %d일의 일정이 없습니다.\n", year, month, day);
		printf("일치하는 일정이 없습니다.\n");
	}
	printf("아무키나 입력하세요.........");
	getchar();
}


// 스케줄 입력
void insertSchedule(int scheduleCount) {
	int i;
	int year, month, day;
	char schedule[100];
	while (1) {
		printf("입력 : ");
		scanf("%d %d %d", &year, &month, &day);
		getchar();
		fgets(schedule, sizeof(schedule), stdin);
		removeEnterInFgetsString(schedule);

		// 날짜 제대로 입력했는지 확인
		if (checkDateValidation(year, month, day)) {
			break;
		}
	}
	printf("출력 : %d년 %d월 %d일 %s\n", year, month, day, schedule);

	// 일정 겹치는지 확인
	int overwrite = 0;
	for (i = 0; i < scheduleCount; i++) {
		// 겹침
		if (mSchedule[i].year == year && mSchedule[i].month == month && mSchedule[i].day == day) {
			char answer;
			printf("해당 날짜에 이미 일정이 있습니다. 일정을 추가하려면Yes, 덮어쓰려면 No를 입력하세요(Y or N)");
			answer = getchar();
			getchar();
			// 추가
			if (answer == 'Y' || answer == 'y') {
				break ;
			}
			// x되돌아가기 덮어쓰기
			else
			{
				overwrite = 1;
				scheduleCount = i;
				break;
			}
			//overwrite = 0 ;
			//break;
		}
	}

	// 덮어쓰지 않겠다면 처음으로 돌아감
	//if (!overwrite)
	//	scheduleCount++;	

	mSchedule[scheduleCount].year = year;
	mSchedule[scheduleCount].month = month;
	mSchedule[scheduleCount].day = day;
	copyStr(schedule, mSchedule[scheduleCount].schedule);
	if (overwrite == 0) { // Yes를 입력했을때 or 일정이 없어서 추가
		scheduleCount++;
		mScheduleCount++;
		printf("일정을 추가하였습니다.\n");
		printf("아무키나 입력하세요.......");
	}

	//	if (!overwrite)
	//		mScheduleCount++;
	//if (mScheduleCount == scheduleCount)
	//	mScheduleCount++;
	else { // No를 입력했을때
		printf("일정을 덮어썼습니다.\n");
		printf("아무키나 입력하세요.......");
	}
	getchar();
	system("clear");
	sort();
}


int checkDateValidation(int year, int month, int day) {
	// 1 ~ 12월 입력했는지 확인
	if (!checkMonthValidation(month)) {
		printf("1 ~ 12월로 입력해주세요.\n");
		return 0;
	} else if (!checkDayValidation(year, month, day)) {
		printf("%d년 %d월 %d일은 존재하지 않습니다.\n", year, month, day);
		return 0;
	}
	return 1;
}


/*
 * TODO : 월 제대로 입력했는지 확인
 * @param :
 *      month = 월
 */
int checkMonthValidation(int month) {
	if (month >= 1 && month <= 12)
		return 1;
	return 0;
}


/*
 * TODO : 일 제대로 입력했는지 확인
 * @param :
 *      year = 년
 *      month = 월
 *      day = 일
 */
int checkDayValidation(int year, int month, int day) {
	int leap = isLeapYear(year);
	if (day > 0 && day <= date[month - 1])
		return 1;
	else if (month == 2 && leap)
		if (day > 0 && day <= 29)
			return 1;
	return 0;
}


/*
 * TODO : first와 second를 바꾸는 함수
 */
void swap(int* first, int* second) {
    int temp = *first;
    *first = *second;
    *second = temp;
}


/*
 * TODO : 정렬
 */
void sort() {
	int i, j;
	char scheduleTemp[100];
    struct mSchedule tempSchedule;
	for(i = 0; i < mScheduleCount; i++) {
		for(j = 0; j < mScheduleCount - i - 1; j++) {
			if(mSchedule[j].year > mSchedule[j+1].year) {
                tempSchedule = mSchedule[j];
                mSchedule[j] = mSchedule[j + 1];
                mSchedule[j + 1] = tempSchedule;
                
//				swap(&mSchedule[j].year,&mSchedule[j+1].year);
//				swap(&mSchedule[j].month,&mSchedule[j+1].month);
//				swap(&mSchedule[j].day,&mSchedule[j+1].day);
				/*temp = mScheduleYear[j];
				  mScheduleYear[j] = mScheduleYear[j+1];
				  mScheduleYear[j+1] = temp;

				  temp = mScheduleMonth[j];
				  mScheduleMonth[j] = mScheduleMonth[j+1];
				  mScheduleMonth[j+1] = temp;

				  temp = mScheduleDay[j];
				  mScheduleDay[j] = mScheduleDay[j+1];
				  mScheduleDay[j+1] = temp;*/

//				copyStr(mSchedule[j].schedule, scheduleTemp);
//				copyStr(mSchedule[j+1].schedule, mSchedule[j].schedule);
//				copyStr(scheduleTemp, mSchedule[j+1].schedule);


			}

			else if(mSchedule[j].year == mSchedule[j+1].year)
			{

				if(mSchedule[j].month > mSchedule[j+1].month) {
                    tempSchedule = mSchedule[j];
                    mSchedule[j] = mSchedule[j + 1];
                    mSchedule[j + 1] = tempSchedule;
                    
//					swap(&mSchedule[j].year, &mSchedule[j+1].year);
//					swap(&mSchedule[j].month, &mScheduleMonth[j+1]);
//					swap(&mScheduleDay[j], &mScheduleDay[j+1]);
					/*temp = mScheduleYear[j];
					  mScheduleYear[j] = mScheduleYear[j+1];
					  mScheduleYear[j+1] = temp;

					  temp = mScheduleMonth[j];
					  mScheduleMonth[j] = mScheduleMonth[j+1];
					  mScheduleMonth[j+1] = temp;

					  temp = mScheduleDay[j];
					  mScheduleDay[j] = mScheduleDay[j+1];
					  mScheduleDay[j+1] = temp;*/

//					copyStr(mSchedule[j], scheduleTemp);
//					copyStr(mSchedule[j+1], mSchedule[j]);
//					copyStr(scheduleTemp, mSchedule[j+1]);

				}		
			}
		}
	}
}




/*
 * TODO : 스케줄 보기
 */
void displaySchedule() {
	//printf("%d",mScheduleCount);
	int i, year, day, month;
	getchar();
	printf("%d\n",mScheduleCount);
	for(i=0; i < mScheduleCount; i++)
	{
		if((mSchedule[i].year == mSchedule[i+1].year) && (mSchedule[i].month == mSchedule[i+1].month))
			continue;
		year = mSchedule[i].year;
		month = mSchedule[i].month;
		day = mSchedule[i].day;

		printf("\t\t\t\t\t\t %d년 %d월\n\n", year, month);

		// 캘린더 출력
		printCalendar(year, month, day);
		while(1){
			char c;
			c = getchar();
			if(c == '\n')
				break;
		}
	}

}


/*
 * 캘린더 출력
 * @Params : year = 년도
 *			leapYear = 윤년 횟수
 * 			month = 월
 *			day = 일
 */
void printCalendar(int year, int month, int day) {
	// 1년1월1일로부터 해당 년월일까지 총 일 수 계산
	int i, j = 0, k;
	int leapYear = getLeapYear(year, month);
	int checkSchedule[100] = {0}, breakpoint;
	int allDay = year * 365 + leapYear;
	for (i = 0; i < month - 1; i++)
		allDay += date[i];

	// 캘린더 출력 시 시작 요일 계산
	int skipDay = allDay % 7;

	// 2월이면 윤년인지 확인해서 윤년이면 29일까지 출력
	int isLeap = 0;
	if (month == 2 && isLeapYear(year))
		isLeap = 1;

	// 캘린더 출력 (윤년의 2월이면 29일까지, 시작 요일 포함)
	int weekCount = 1;
	printf("Sun.\t\tMon.\t\tTues.\t\tWed\t\tThurs.\t\tFri.\t\tSat.\n");
	for (i = 1; i <= date[month - 1] + isLeap + skipDay; i++, weekCount++) {
		if (i <= skipDay)
			printf("\t\t");

		// 날짜 출력
		else
			printf("%d\t\t", i - skipDay);

        // 일정 체크
		if (i % 7 == 0 || i == date[month - 1] + isLeap + skipDay) {
			printf("\n");
			while(1) {
				breakpoint = 0;
				// 날짜 밑에 일정 출력
				for (k = i - weekCount + 1; k <= i; k++) {
					for (j = 0; j < mScheduleCount; j++) {
						// 일정 출력
						if (checkSchedule[j] == 0 && mSchedule[j].year == year &&
								mSchedule[j].month == month && mSchedule[j].day == k - skipDay) {
							int len = getLength(mSchedule[j].schedule);
							char str[100];
							checkSchedule[j] = 1;
							breakpoint = 1;
							copyStr(mSchedule[j].schedule,str);
                            
                            // 12글자 넘으면 ...으로 자른다
							if(len >= 12) {
								str[12] = '\0';
								printf("%s...", str);
							}
							else {
								printf("%s", mSchedule[j].schedule);
							}
							break;
						}
					}
					printf("\t\t");
				}
				printf("\n");
				if(breakpoint == 0) break;
			}
			weekCount = 0;
		}
	}
	printf("\n\n");
}


/*
 * TODO : 윤년인지 확인
 * @params : year = 연도
 * @return : 윤년이면 1 아니면 0
 */
int isLeapYear(int year) {
	if (year % 100 == 0) {
		if (year % 400 == 0)
			return 1;
	} else if (year % 4 == 0)
		return 1;
	return 0;
}


/*
 * TODO : 윤년 몇 번 지나왔는지 계산
 * @params :
 *      year = 연도
 *      month = 월
 * @return : 윤년 횟수
 */
int getLeapYear(int year, int month) {
	int i;
	int leapYear = 0;
	int includeYear = 0;
	// 2월 지났으면 해당 년도도 포함해서 계산
	if (month > 2)
		includeYear = 1;
	for (i = 1; i < year + includeYear; i++) {
		if (isLeapYear(i))
			leapYear++;
	}
	return leapYear;
}


/*
 * TODO : 계산기 main
 */
void calculator() {
    // 수식 저장
    char str[100];
    
    // 변수
    struct mVariable regVariable[10];
    
    int variableCount = 0;
    getchar();
    
    // 변수 지정
    while(1) {
        // 변수나 수식 입력
        printf("Input : ");
        fgets(str, sizeof(str), stdin);
        removeEnterInFgetsString(str);      // fgets의 마지막 \n 제거
        if (getLength(str) == 0)            // 아무것도 입력안하면 다시 입력
            break;
    
        // 변수 저장
        if (!isOperation(str)) {
            regVariable[variableCount].name = getVariable(str);
            regVariable[variableCount].value = getValue(str);
            variableCount++;
        }
        
        // 연산식이면
        else {
            transformation(str, regVariable);           // 변수나 수학 함수를 수로 변환
            posifixNotaion(str, getLength(str));           // 후위 표기법으로 변환
            break;
        }
    }
}


/*
 * 후위 표기법으로 변환
 * @prarms
 *      str = 수식
 *      length = 길이
 */
void posifixNotaion(char* str, int length) {
    char result[100] = {'\0'};
    int resultCount = 0;
    
    char stack[100] = {'\0'};
    int count = 0, i;
    char temp;
    for (i = 0; i < length; i++) {
        if (*(str + i) == ' ') {
            continue;
        }
        switch (*(str + i)) {
            case '(':
                push(stack, *(str + i), count++);
                break;
                
            case ')':
                // (가 나올 때까지 팝하고 ( 는 팝하여 버린다.
                while (!isEmpty(stack) && ((temp = pop(stack, --count)) != '(')) {
                    result[resultCount++] = temp;
                }
                printf("Posifix notation : %s\n", result);
                break;
                
            case '+':
            case '-':
                while (!isEmpty(stack)) {
                    temp = pop(stack, --count);
                    if (checkOperator(temp)) {
                        result[resultCount++] = temp;
                    } else {
                        push(stack, temp, count++);
                        break;
                    }
                }
                push(stack, *(str + i), count++);
                printf("Posifix notation : %s\n", result);
                break;
                
            case '*':
            case '/':
                while (!isEmpty(stack)) {
                    temp = pop(stack, --count);
                    // 우선순위가 같다면
                    if (checkOperatorLevel(temp, *(str + i)) == 1) {
                        result[resultCount++] = temp;
                    } else {
                        push(stack, temp, count++);
                        break;
                    }
                }
                push(stack, *(str + i), count++);
                printf("Posifix notation : %s\n", result);
                break;
                
            default:
                while (*(str + i) >= '0' && *(str + i) <= '9') {
                    result[resultCount++] = *(str + i);
                    i++;
                }
                i--;
                result[resultCount++] = ' ';
                printf("Posifix notation : %s\n", result);
                break;
        }
    }
    while (!isEmpty(stack)) {
        char c = pop(stack, --count);
        result[resultCount++] = c;
        
//        printf("Posifix notation : ");
        printf("Posifix notation : %s\n", result);
    }
    
    double numStack[100] = {0};
    double numStackCount = 0;
    
    // 계산
    for (i = 0; *(result + i) != '\0'; i++) {
        if (*(result + i) == ' ') {
            continue;
        }
        int operator;
        if ((operator = checkOperator(*(result + i)))) {
            double num1 = popDouble(numStack, --numStackCount);
            double num2 = popDouble(numStack, --numStackCount);
            
            switch (operator) {
                case 1:
                    // 덧셈
                    pushDouble(numStack, num2 + num1, numStackCount++);
                    break;
                    
                case 2:
                    // 뺄셈
                    pushDouble(numStack, num2 - num1, numStackCount++);
                    break;
                    
                case 3:
                    // 곱셈
                    pushDouble(numStack, num2 * num1, numStackCount++);
                    break;
                    
                case 4:
                    // 나눗셈
                    pushDouble(numStack, num2 / num1, numStackCount++);
                    break;
            }
            printf("Posifix notation : ");
            printDoubleArray(numStack, numStackCount);
            printf("%s\n", result + i + 1);
        } else {
            char curStack[10] = {'\0'};
            int curStackCount = 0;
            
            while (*(result + i) >= '0' && *(result + i) <= '9') {
                push(curStack, *(result + i), curStackCount++);
                i++;
            }
            i--;
//            result[resultCount++] = ' ';
            
            printf("Posifix notation : %s\n", result);
            
            int ab = 0, c = 1;
            while (!isEmpty(curStack)) {
                ab += convertToInt(pop(curStack, --curStackCount)) * c;
                c *= 10;
            }
//            giprintf("ab = %d\n", ab);
            
            pushDouble(numStack, ab, numStackCount++);
        }
    }
    printf("\n");
}


void printDoubleArray(double array[], int size) {
    int i;
    for (i = 0; i < size; i++)
        printf("%f ", array[i]);
}


/*
 * 연산자인지 판별
 * @param : c - 비교할 문자
 * @return : 
 *      1 - 덧셈
 *      2 - 뺄셈
 *      3 - 곱셈
 *      4 - 나눗셈
 *      0 - 연산자 아닌 것
 */
int checkOperator(char c) {
    switch (c) {
        case '+':
            return 1;
            break;
            
        case '-':
            return 2;
            break;
            
        case '*':
            return 3;
            break;
            
        case '/':
            return 4;
            break;
            
        default:
            return 0;
            break;
    }
}


/*
 * 연산자 우선순위 비교
 * @param :
        operator1, operator2 = 비교할 연산자들
 * @return :
        0 = operator1이 더 낮음
        1 = 같음
        2 = operator1이 더 높음
 */
int checkOperatorLevel(char operator1, char operator2) {
    switch (operator1) {
        case '+':
        case '-':
            switch (operator2) {
                case '+':
                case '-':
                    return 1;
                    
                case '*':
                case '/':
                    return 0;
            }
            break;
            
        case '*':
        case '/':
            switch (operator2) {
                case '+':
                case '-':
                    return 2;
                    
                case '*':
                case '/':
                    return 1;
            }
            break;
    }
    return 0;
}


/*
 * 변수들 숫자로 변환
 * @params : *str = 연산식
 *           *variable = 변수 배열
 *           *value = 변수 값 배열
 */
void transformation(char str[], struct mVariable regVariable[]) {
    int index = 0;
    while (str[index] != '\0') {
        if (str[index] >= 'A' && str[index] <= 'Z') {
            index += replaceVariableToInt(str, index, regVariable) - 1;
            printf("Transformation : %s\n", str);
        } else {
            int functionIndex = checkFunction((str + index));
            if (functionIndex != 0) {
                replaceMathFunction(str, functionIndex, index, regVariable);
                printf("Transformation : %s\n", str);
            }
        }
        index++;
    }
}


/*
 * TODO : 수학함수 계산
 * @params :
 *      *str = 수식
 *      functionIndex = 함수 구분 인덱스 1:log, 2:pow, 3:sqrt, 4:sin, 5:cos, 6:tan
 *      index = 전체 문자열 중 수학 함수의 첫글자 인덱스 값
 * @return : 결과값 길이
 */
int replaceMathFunction(char str[], int functionIndex, int index, struct mVariable regVariable[]) {
    char valueStr[10] = {'\0'};         // 결과 값 String
    int functionLength, valueIndex, value, valueLength, strLength, i;
    
    if (functionIndex == 3) {       // sqrt
        functionLength = 4;
    } else if (functionIndex == 2) {    // pow(^)
        functionLength = 1;
    } else {
        functionLength = 3;
    }
    
    if (functionIndex == 2) {                   // pow
        int tempLength = 0, tempLength2 = 0;
        valueIndex = index - 1;
        
        // ^ 이전의 값
        value = getFunctionValue(str + valueIndex, functionIndex, &tempLength, regVariable,  functionLength);
        functionLength += tempLength;
        valueIndex = index + 1;
        
        // ^ 이후의 값
        int temp = getFunctionValue(str + valueIndex, functionIndex + 1, &tempLength2, regVariable, functionLength);
        functionLength += tempLength2;
        
        // 결과 값
        value = pow(value, temp);
        
        // 결과 값 String 변환 (value -> valueStr)
        convertToString(valueStr, value);
        valueLength = getLength(valueStr);
        strLength = getLength(str);
        
        // 수학함수 + 수학함수 인자 <-> 결과 값
        for (i = 0; i < strLength - index + 1; i++) {
            if (i < valueLength)
                str[index + i - tempLength2] = valueStr[i];
            else
                str[index + i - tempLength2] = str[index + i + functionLength - valueLength - tempLength];
        }
    } else {                                    // not pow
        valueIndex = index + functionLength;
        value = getFunctionValue(str + valueIndex, functionIndex, &functionLength, regVariable, functionLength);
        
        // 함수 별 계산
        switch (functionIndex) {
            case 1:
                // 로그
                value = log10(value);
                break;
                
            case 3:
                // 제곱근
                value = sqrt(value);
                break;
                
            case 4:
                // 사인
                value = sin(value);
                break;
                
            case 5:
                // 코사인
                value = cos(value);
                break;
                
            case 6:
                // 탄젠트
                value = tan(value);
                break;
        }
        
        // 결과 값 String 변환 (value -> valueStr)
        convertToString(valueStr, value);
        valueLength = getLength(valueStr);
        strLength = getLength(str);
        
        // 수학함수 + 수학함수 인자 <-> 결과 값
        for (i = 0; i < strLength - index - valueLength; i++) {
            if (i < valueLength)
                str[index + i] = valueStr[i];
            else
                str[index + i] = str[index + i + functionLength - valueLength];
        }
    }
    
    return valueLength;
}


/*
 * TODO : 수학함수 인자값 추출
 * @params
        str[] = 수식
        functionIndex = 수학함수 위치(인덱스)
        *valueLength = 결괏값 길이 리턴용
        regVariable[] = 변수 리스트
        functionLength = 수학 함수 길이
 * @return : 수학함수 (앞)뒤의 인자값
 */
int getFunctionValue(char str[], int functionIndex, int *valueLength, struct mVariable regVariable[], int functionLength) {
    char stack[10] = {'\0'};
    int stackCount = 0;
    int count = 1;
    int result = 0;
    if (functionIndex == 2) {
        while (*str == ' ') {
            (*valueLength)++;
            str--;
        }
        while (*str >= '0' && *str <= '9') {
            (*valueLength)++;
            push(stack, *str--, stackCount++);
        }
        char tempStack[10] = {'\0'};
        int tempCount = 0;
        
        // tempStack = reverse of stack
        while (!isEmpty(stack)) {
            push(tempStack, pop(stack, --stackCount), tempCount++);
        }
        
        while (!isEmpty(tempStack)) {
            char c = pop(tempStack, --tempCount);
            switch(c) {
                case '1':
                    result += 1 * count;
                    break;
                    
                case '2':
                    result += 2 * count;
                    break;
                    
                case '3':
                    result += 3 * count;
                    break;
                    
                case '4':
                    result += 4 * count;
                    break;
                    
                case '5':
                    result += 5 * count;
                    break;
                    
                case '6':
                    result += 6 * count;
                    break;
                    
                case '7':
                    result += 7 * count;
                    break;
                    
                case '8':
                    result += 8 * count;
                    break;
                    
                case '9':
                    result += 9 * count;
                    break;
                    
            }
            count *= 10;
        }
    } else {
        while (*str == ' ') {
            (*valueLength)++;
            str++;
        }
        
        // 수학 함수 뒤에 변수 있으면 변환
        if (*str >= 'A' && *str <= 'B') {
            replaceVariableToInt(str, 0, regVariable);
        }
        
        while (*str >= '0' && *str <= '9') {
            (*valueLength)++;
            push(stack, *str++, stackCount++);
        }
        while(!isEmpty(stack)) {
            char c = pop(stack, --stackCount);
            switch(c) {
                case '1':
                    result += 1 * count;
                    break;
                    
                case '2':
                    result += 2 * count;
                    break;
                    
                case '3':
                    result += 3 * count;
                    break;
                    
                case '4':
                    result += 4 * count;
                    break;
                    
                case '5':
                    result += 5 * count;
                    break;
                    
                case '6':
                    result += 6 * count;
                    break;
                    
                case '7':
                    result += 7 * count;
                    break;
                    
                case '8':
                    result += 8 * count;
                    break;
                    
                case '9':
                    result += 9 * count;
                    break;
                    
            }
            count *= 10;
        }
    }
    return result;
}


/*
 * TODO: 수학함수 판별
 * @params : str[] = 변수 or 수학함수로 시작하는 수식
 * @return
        0 = 함수 아님
        1 = log
        2 = pow
        3 = sqrt
        4 = sin
        5 = cos
        6 = tan
 */
int checkFunction(char *str) {
    if (isStartWith(str, "log"))
        return 1;
    else if (isStartWith(str, "^"))
        return 2;
    else if (isStartWith(str, "root"))
        return 3;
    else if (isStartWith(str, "sin"))
        return 4;
    else if (isStartWith(str, "cos"))
        return 5;
    else if (isStartWith(str, "tan"))
        return 6;
    return 0;
}


int isStartWith(char* allStr, char* startStr) {
    while (*startStr != '\0') {
        if (*allStr++ != *startStr++)
            return 0;
    }
    return 1;
}


char convertToChar(int num) {
    char c;
    switch (num) {
        case 0:
            c = '0';
            break;
            
        case 1:
            c = '1';
            break;
            
        case 2:
            c = '2';
            break;
            
        case 3:
            c = '3';
            break;
            
        case 4:
            c = '4';
            break;
            
        case 5:
            c = '5';
            break;
            
        case 6:
            c = '6';
            break;
            
        case 7:
            c = '7';
            break;
            
        case 8:
            c = '8';
            break;
            
        default:
            c = '9';
            break;
    }
    return c;
}


char convertToInt(char num) {
    int result;
    switch (num) {
        case '0':
            result = 0;
            break;
            
        case '1':
            result = 1;
            break;
            
        case '2':
            result = 2;
            break;
            
        case '3':
            result = 3;
            break;
            
        case '4':
            result = 4;
            break;
            
        case '5':
            result = 5;
            break;
            
        case '6':
            result = 6;
            break;
            
        case '7':
            result = 7;
            break;
            
        case '8':
            result = 8;
            break;
            
        default:
            result = 9;
            break;
    }
    return result;
}


/*
 * 스택 순서 뒤집기
 */
void reverse(char stack[], int size) {
    int i, j = size / 2;
    char temp;
    for (i = 0; i < j; i++) {
        temp = stack[i];
        stack[i] = stack[size - i - 1];
        stack[size - i - 1] = temp;
    }
}


/*
 * 수를 문자열로 바꾸는 함수
 * @param
        str[] = 저장할 문자열 배열
        num = 바꿀 수
 */
void convertToString(char str[], int num) {
    int index = 0;
    while (num) {
        push(str, convertToChar(num % 10), index++);
        num /= 10;
    }
    reverse(str, index);
}



/*
 * 변수를 숫자로 대체하는 함수
 */
int replaceVariableToInt(char str[], int index, struct mVariable regVariable[]) {
    char valueStr[100] = {'\0'};
    int valueStackIndex = 0;
    char var = str[index];
    int valueIndex = 0, i;
    while (regVariable[valueIndex].name != var) {
        valueIndex++;
    }
    
    int curValue = regVariable[valueIndex].value;
    
    // int -> string
    convertToString(valueStr, curValue);
    
    int length = getLength(valueStr);
    for (i = getLength(str); i > index; i--) {
        str[i + length - 1] = str[i];
    }
    
    for (i = 0; i < length; i++) {
        str[index++] = valueStr[i];
    }
    return length;
}


/* 계산식인지 변수식인지 구분
 * @Param : *str = 식
 * @Return Value : 0 -> 변수식
 *				   1 -> 계산식 
 */
int isOperation(char *str) {
	while (*str++ != '\0')
		if (*str == '=')
			return 0;
	return 1;
}


// 중위표기법 -> 후위 표기법
void convertToLast() {

}


/*
 * TODO : 문자열 길이 얻기
 * @param : str[] = 문자열
 * @return : 문자열 길이
 */
int getLength(char str[]) {
	int count = 0;
	while(*str++ != '\0')
		count++;
	return count;
}


/*
 * 문자열 중 변수 추출
 * Input : A = 10
 * Output : A
 */
char getVariable(char *str) {
	while(*str == ' ')
		str++;
	return *str;
}


/*
 * 문자열 중 변수 값 추출
 * Input : A = 10
 * Output : 10
 */
int getValue(char *str) {

    char stack[100] = {'\0'};
    int stackCount = 0;
    int result = 0;
    int count = 1;
    while(*str < '0' || *str > '9')
        str++;
    while(*str >= '0' && *str <= '9') {
        push(stack, *str++, stackCount++);
    }
    while(!isEmpty(stack)) {
        char c = pop(stack, --stackCount);
        switch(c) {
            case '1':
                result += 1 * count;
                break;
                
            case '2':
                result += 2 * count;
                break;
                
            case '3':
                result += 3 * count;
                break;
                
            case '4':
                result += 4 * count;
                break;
                
            case '5':
                result += 5 * count;
                break;
                
            case '6':
                result += 6 * count;
                break;
                
            case '7':
                result += 7 * count;
                break;
                
            case '8':
                result += 8 * count;
                break;
                
            case '9':
                result += 9 * count;
                break;
                
        }
        count *= 10;
    }
    return result;

}


// 스택에 문자 하나 추가

void push(char* stack, char chr, int count) {
    stack[count] = chr;
}


// 스택에 수 하나 추가
void pushDouble(double* stack, double chr, int count) {
    stack[count] = chr;
}


// 스택에 문자열 추가

void pushStr(char* stack, char *str, int count) {
    while(*str != '\0') {
        stack[count++] = *str++;
    }
}


// 스택에 있는 문자 하나 출력

char pop(char* stack, int length) {
    char c = stack[length];
    stack[length] = '\0';
    return c;
}


// 스택에 있는 숫자 하나 출력
double popDouble(double* stack, int length) {
    double c = stack[length];
    stack[length] = 0;
    return c;
}


// 스택이 비었는지 확인
int isEmpty(char *stack) {

    if (*stack == '\0')
        return 1;
    return 0;
}



//************************** String ***************************//

// fgets는 마지막에 \n도 저장되기에 \n을 제거.
char* removeEnterInFgetsString(char str[]) {
    int length = getLength(str);
    str[length - 1] = '\0';
    return str;
}


// 문자열 같은지 비교
int isEqual(char* str1, char* str2) {
    while (*str1 != '\0' && *str2 != '\0') {
        if (*str1++ != *str2++)
            return 0;
    }
    if (*str1 != *str2)
        return 0;
    return 1;
}


// 문자열 복사
void copyStr(char* fromStr, char* toStr) {
    while(*fromStr != '\0')
        *toStr++ = *fromStr++;
    *toStr = 0;
}