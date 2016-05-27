#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define SCHEDULE_MAX_LENGTH 100

// 월별 일 수
int date[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// 스택 데이터 개수
int mCount;

// 스케줄 저장 배열
int mScheduleYear[SCHEDULE_MAX_LENGTH] = {0};
int mScheduleMonth[SCHEDULE_MAX_LENGTH] = {0};
int mScheduleDay[SCHEDULE_MAX_LENGTH] = {0};
char mSchedule[SCHEDULE_MAX_LENGTH][20] = {'\0'};

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
void scheduler();
void calculator();

// 계산기 함수
char getVariable(char*);
int getValue(char*);
int getLength(char*);
int isOperation(char*);
void transformation(char *str, char *variable, int *value);
int replaceVariableToInt(char*, int, char*, int*);
int replaceMathFunction(char*, int, int, char*, int*);
int getFunctionValue(char*, int, int*, char*, int*, int);
int checkFunction(char*);
int isStartWith(char*, char*);
int convertToString(char*, int);
void posifixNotaion();
int checkOperator(char);
int checkOperatorLevel(char, char);
char convertToInt(char);
void printDoubleArray(double[], int);

// 스케줄관리 함수
int checkDate(int, int, int);
int checkDay(int, int, int);
int checkMonth(int);
int schedulerMenu();
int getLeapYear(int, int);
void printCalendar(int, int, int);
int isLeapYear(int);
void insertSchedule(int);
void deleteSchedule();
void sort();

// String 함수
char* removeEnterInFgetsString(char*);
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
					// 스케쥴러
					subMenu = schedulerMenu();
					switch(subMenu) {
						case 1:
							insertSchedule(mScheduleCount);
							break;

						case 2:
							deleteSchedule();
							break;

						case 3:
							scheduler();
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


// fgets는 마지막에 \n도 저장되기에 \n을 제거.
char* removeEnterInFgetsString(char *str) {
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

// 스케줄 삭제
void deleteSchedule() {
	int year, month, day;
	char schedule[SCHEDULE_MAX_LENGTH];

	printf("입력 : ");
	scanf("%d %d %d", &year, &month, &day);
	getchar();
	fgets(schedule, sizeof(schedule), stdin);
	removeEnterInFgetsString(schedule);
	printf("출력 : %d년 %d월 %d일 %s\n",year,month,day,schedule);

	int i;
	// 년 월 일 스케줄에 매칭되는 스케줄 index 찾기
	for (i = 0; i < mScheduleCount; i++) {
		if (mScheduleYear[i] == year && mScheduleMonth[i] == month && mScheduleDay[i] == day && isEqual(mSchedule[i], schedule))
			break;
	}

	if (mScheduleYear[i] == year && mScheduleMonth[i] == month && mScheduleDay[i] == day && isEqual(mSchedule[i],schedule)) {
		for (int j = i + 1; j < mScheduleCount; j++) {
			mScheduleYear[j - 1] = mScheduleYear[j];
			mScheduleMonth[j - 1] = mScheduleMonth[j];
			mScheduleDay[j - 1] = mScheduleDay[j];
			copyStr(mSchedule[j], mSchedule[j -1]);
		}
		printf("%d년 %d월 %d일의 일정이 있습니다.\n",year,month,day);
		printf("일정을 삭제하였습니다.\n");
		mScheduleCount--;
		mScheduleYear[mScheduleCount] = 0;
	} else
	{
		printf("%d년 %d월 %d일의 일정이 없습니다.\n",year,month,day);
		printf("일치하는 일정이 없습니다.\n");
	}
	printf("아무키나 입력하세요.........");
	getchar();
}


// 스케줄 입력
void insertSchedule(int scheduleCount) {
	int year, month, day;
	char schedule[100];
	while (1) {
		printf("입력 : ");
		scanf("%d %d %d", &year, &month, &day);
		getchar();
		fgets(schedule, sizeof(schedule), stdin);
		removeEnterInFgetsString(schedule);

		// 날짜 제대로 입력했는지 확인
		if (checkDate(year, month, day)) {
			break;
		}
	}
	printf("출력 : %d년 %d월 %d일 %s\n", year, month, day, schedule);

	// 일정 겹치는지 확인
	int overwrite = 1;
	for (int i = 0; i < scheduleCount; i++) {
		// 겹침
		if (mScheduleYear[i] == year && mScheduleMonth[i] == month && mScheduleDay[i] == day) {
			char answer;
			printf("해당 날짜에 이미 일정이 있습니다. 덮어 씌우시겠습니까? (Y or N)");
			answer = getchar();
			getchar();
			// 덮어씌우기
			if (answer == 'Y' || answer == 'y') {
				scheduleCount = i;
				overwrite = 1;
			}
			// 되돌아가기
			else
				overwrite = 0;
			break;
		}
	}

	// 덮어쓰지 않겠다면 처음으로 돌아감
	if (!overwrite)
		return;

	mScheduleYear[scheduleCount] = year;
	mScheduleMonth[scheduleCount] = month;
	mScheduleDay[scheduleCount] = day;
	copyStr(schedule, mSchedule[scheduleCount]);

	if (mScheduleCount == scheduleCount)
		mScheduleCount++;

	printf("일정을 추가하였습니다.\n");
	printf("아무키나 입력하세요.......");
	getchar();
	system("clear");
	sort();
}

int checkDate(int year, int month, int day) {
	// 1 ~ 12월 입력했는지 확인
	if (!checkMonth(month)) {
		printf("1 ~ 12월로 입력해주세요.\n");
		return 0;
	} else if (!checkDay(year, month, day)) {
		printf("%d년 %d월 %d일은 존재하지 않습니다.\n", year, month, day);
		return 0;
	}
	return 1;
}


// 월 제대로 입력했는지 확인
int checkMonth(int month) {
	if (month >= 1 && month <= 12)
		return 1;
	return 0;
}


// 일 제대로 입력했는지 확인
int checkDay(int year, int month, int day) {
	int leap = isLeapYear(year);
	if (day > 0 && day <= date[month - 1])
		return 1;
	else if (month == 2 && leap)
		if (day > 0 && day <= 29)
			return 1;
	return 0;
<<<<<<< HEAD
=======
}

// 일정 정렬하기
void sort() {
	int i, j, temp;
	char swap[20];
	for(i=0; i < mScheduleCount; i++) {
		for(j=0; j < mScheduleCount - i - 1; j++) {
			if(mScheduleYear[j] > mScheduleYear[j+1])
			{
				temp = mScheduleYear[j];
				mScheduleYear[j] = mScheduleYear[j+1];
				mScheduleYear[j+1] = temp;

				temp = mScheduleMonth[j];
				mScheduleMonth[j] = mScheduleMonth[j+1];
				mScheduleMonth[j+1] = temp;

				temp = mScheduleDay[j];
				mScheduleDay[j] = mScheduleDay[j+1];
				mScheduleDay[j+1] = temp;

				copyStr(mSchedule[j], swap);
				copyStr(mSchedule[j+1], mSchedule[j]);
				copyStr(swap, mSchedule[j+1]);


			}

			else if(mScheduleYear[j] == mScheduleYear[j+1])
			{

				if(mScheduleMonth[j] > mScheduleMonth[j+1])
				{
					temp = mScheduleYear[j];
					mScheduleYear[j] = mScheduleYear[j+1];
					mScheduleYear[j+1] = temp;

					temp = mScheduleMonth[j];
					mScheduleMonth[j] = mScheduleMonth[j+1];
					mScheduleMonth[j+1] = temp;

					temp = mScheduleDay[j];
					mScheduleDay[j] = mScheduleDay[j+1];
					mScheduleDay[j+1] = temp;

					copyStr(mSchedule[j], swap);
					copyStr(mSchedule[j+1], mSchedule[j]);
					copyStr(swap, mSchedule[j+1]);

				}		
			}
		}
	}
>>>>>>> master
}

// 일정 정렬하기
void sort() {
	int i, j, temp;
	char swap[20];
	for(i=0; i < mScheduleCount; i++) {
		for(j=0; j < mScheduleCount - i - 1; j++) {
			if(mScheduleYear[j] > mScheduleYear[j+1])
			{
				temp = mScheduleYear[j];
				mScheduleYear[j] = mScheduleYear[j+1];
				mScheduleYear[j+1] = temp;

				temp = mScheduleMonth[j];
				mScheduleMonth[j] = mScheduleMonth[j+1];
				mScheduleMonth[j+1] = temp;

				temp = mScheduleDay[j];
				mScheduleDay[j] = mScheduleDay[j+1];
				mScheduleDay[j+1] = temp;

				copyStr(mSchedule[j], swap);
				copyStr(mSchedule[j+1], mSchedule[j]);
				copyStr(swap, mSchedule[j+1]);


			}

			else if(mScheduleYear[j] == mScheduleYear[j+1])
			{

				if(mScheduleMonth[j] > mScheduleMonth[j+1])
				{
					temp = mScheduleYear[j];
					mScheduleYear[j] = mScheduleYear[j+1];
					mScheduleYear[j+1] = temp;

					temp = mScheduleMonth[j];
					mScheduleMonth[j] = mScheduleMonth[j+1];
					mScheduleMonth[j+1] = temp;

					temp = mScheduleDay[j];
					mScheduleDay[j] = mScheduleDay[j+1];
					mScheduleDay[j+1] = temp;

					copyStr(mSchedule[j], swap);
					copyStr(mSchedule[j+1], mSchedule[j]);
					copyStr(swap, mSchedule[j+1]);

				}		
			}
		}
	}
}




<<<<<<< HEAD
=======

>>>>>>> master
// 스케줄 보기
void scheduler() {

	int i, year, day, month;
	getchar();
	for(i=0; i < mScheduleCount; i++)
	{

		if((mScheduleYear[i] == mScheduleYear[i+1]) && (mScheduleMonth[i] == mScheduleMonth[i+1]))
			continue;
		year = mScheduleYear[i];
		month = mScheduleMonth[i];
		day = mScheduleDay[i];

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
	int leapYear = getLeapYear(year, month);
	int allDay = year * 365 + leapYear;
	for (int i = 0; i < month - 1; i++)
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
	for (int i = 1; i <= date[month - 1] + isLeap + skipDay; i++, weekCount++) {
		// 시작 요일 맞추기 위해 건너뜀
		if (i <= skipDay)
			printf("\t\t");

		// 날짜 출력
		else
			printf("%d\t\t", i - skipDay);

		// 토요일까지 모두 출력 후 또는 day 모두 출력 후 개행
		if (i % 7 == 0 || i == date[month - 1] + isLeap + skipDay) {
			printf("\n");
			// 날짜 밑에 일정 출력
			for (int k = i - weekCount + 1; k <= i; k++) {
				weekCount = 0;
				// 해당 날짜에 일정 있는지 검사
				for (int j = 0; j < mScheduleCount; j++) {
					// 일정 있으면 출력
					if (mScheduleYear[j] == year && mScheduleMonth[j] == month && mScheduleDay[j] == k - skipDay) {
						printf("%s", mSchedule[j]);
						break;
					}
				}
				printf("\t\t");
			}
			printf("\n");
		}
	}
	printf("\n\n");
}


// 윤년인지 확인
int isLeapYear(int year) {
	// 100으로 나누어 떨어지면 윤년아님
	if (year % 100 == 0) {
		// 그중에서 400으로 나누어 떨어지면 윤년임
		if (year % 400 == 0)
			return 1;
	} else if (year % 4 == 0)
		return 1;
	return 0;
}


// 윤년 몇 번 지나왔는지 계산
int getLeapYear(int year, int month) {
	int leapYear = 0;
	int includeYear = 0;
	// 2월 지났으면 해당 년도도 포함해서 계산
	if (month > 2)
		includeYear = 1;
	for (int i = 1; i < year + includeYear; i++) {
		if (isLeapYear(i))
			leapYear++;
	}
	return leapYear;
}


// 계산기
void calculator() {
<<<<<<< HEAD
    // 변수 지정 or 수학 식 임시 저장
    char str[100];
    
    // 변수 명 저장
    char variable[10];
    
    // 변수 값 저장
    int value[10];
    
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
    
        // 변수식이면
        if (!isOperation(str)) {
            variable[variableCount] = getVariable(str);
            value[variableCount] = getValue(str);
            variableCount++;
        }
        
        // 연산식이면
        else {
            transformation(str, variable, value);           // 변수나 수학 함수를 수로 변환
            posifixNotaion(str, getLength(str));           // 후위 표기법으로 변환
            break;
        }
    }
=======
	// 변수 지정 or 수학 식 임시 저장
	char str[100];

	// 변수 명 저장
	char variable[10];

	// 변수 값 저장
	int value[10];

	int variableCount = 0;

	// 연산자 임시 저장
	char operator;

	getchar();

	// 변수 지정
	while(1) {
		printf("Input : ");
		fgets(str, sizeof(str), stdin);
		removeEnterInFgetsString(str);
		if (getLength(str) == 0)
			break;

		// 변수식이면
		if (!isOperation(str)) {
			variable[variableCount] = getVariable(str);
			value[variableCount] = getValue(str);
			//            printf("variable = %c\n", variable[variableCount]);
			//            printf("value = %d\n", value[variableCount]);
			variableCount++;
		}

		// 연산식이면
		else {
			transformation(str, variable, value);
			break;
		}
	}
>>>>>>> master
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
    int count = 0;
    char temp;
    for (int i = 0; i < length; i++) {
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
                printf("%s\n", result);
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
                printf("%s\n", result);
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
                printf("%s\n", result);
                break;
                
            default:
                while (*(str + i) >= '0' && *(str + i) <= '9') {
                    result[resultCount++] = *(str + i);
                    i++;
                }
                result[resultCount++] = ' ';
                printf("%s\n", result);
                break;
        }
    }
    while (!isEmpty(stack)) {
        char c = pop(stack, --count);
        result[resultCount++] = c;
        
        printf("%s\n", result);
    }
    
    double numStack[100] = {0};
    double numStackCount = 0;
    
    // 계산
    for (int i = 0; *(result + i) != '\0'; i++) {
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
            
            printf("%s\n", result);
            
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
    for (int i = 0; i < size; i++)
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
void transformation(char *str, char *variable, int *value) {
<<<<<<< HEAD
    int index = 0;
    while (*(str + index) != '\0') {
        if (*(str + index) >= 'A' && *(str + index) <= 'Z') {
            index += replaceVariableToInt(str, index, variable, value) - 1;
            printf("Transformation : %s\n", str);
        } else {
            int functionIndex = checkFunction((str + index));
            if (functionIndex != 0) {
                replaceMathFunction(str, functionIndex, index, variable, value);
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
int replaceMathFunction(char *str, int functionIndex, int index, char *variableArray, int *valueArray) {
    char valueStr[10] = {'\0'};         // 결과 값 String
    int functionLength, valueIndex, value, valueLength, strLength;
    
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
        value = getFunctionValue(str + valueIndex, functionIndex, &tempLength, variableArray, valueArray,  functionLength);
        functionLength += tempLength;
        valueIndex = index + 1;
        
        // ^ 이후의 값
        int temp = getFunctionValue(str + valueIndex, functionIndex + 1, &tempLength2, variableArray, valueArray, functionLength);
        functionLength += tempLength2;
        
        // 결과 값
        value = pow(value, temp);
        
        // 결과 값 String 변환 (value -> valueStr)
        convertToString(valueStr, value);
        valueLength = getLength(valueStr);
        strLength = getLength(str);
        
        // 수학함수 + 수학함수 인자 <-> 결과 값
        for (int i = 0; i < strLength - index + 1; i++) {
            if (i < valueLength)
                str[index + i - tempLength2] = valueStr[i];
            else
                str[index + i - tempLength2] = *(str + index + i + functionLength - valueLength - tempLength);
        }
    } else {                                    // not pow
        valueIndex = index + functionLength;
        value = getFunctionValue(str + valueIndex, functionIndex, &functionLength, variableArray, valueArray, functionLength);
        
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
                
            default:
                break;
        }
        
        // 결과 값 String 변환 (value -> valueStr)
        convertToString(valueStr, value);
        valueLength = getLength(valueStr);
        strLength = getLength(str);
        
        // 수학함수 + 수학함수 인자 <-> 결과 값
        for (int i = 0; i < strLength - index - valueLength; i++) {
            if (i < valueLength)
                str[index + i] = valueStr[i];
            else
                str[index + i] = *(str + index + i + functionLength - valueLength);
        }
    }
    
    return valueLength;
}

int getFunctionValue(char* str, int functionIndex, int *valueLength, char *variableArray, int *valueArray, int functionLength) {
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
            replaceVariableToInt(str, 0, variableArray, valueArray);
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
=======
	int index = 0;
	while (*str != '\0') {
		if (*str >= 'A' && *str <= 'Z') {
			replaceInt(str, index, value[index]);
			printf("Transformation : %s\n", str);
		}
		str++;
		index++;
	}
}


void replaceInt(char *str, int index, int value) {
	char valueStack[100];
	char valueStr[100];
	int a = 1;
	while (value / a) {
		int num = value % (a * 10);
		push(valueStack, num / a);
		printf("num = %d\n", num / a);

		a *= 10;
	}

	while (!isEmpty(valueStack)) {
		int index = 0;
		char c =  pop(valueStack);
		printf("%c\n", c);
		valueStr[index++] = pop(valueStack);
	}
	printf("valueStr = %s\n", valueStr);
>>>>>>> master
}


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
void reverse(char* stack, int size) {
    int i, j = size / 2;
    char temp;
    for (i = 0; i < j; i++) {
        temp = stack[i];
        stack[i] = stack[size - i];
        stack[size - i] = temp;
    }
}


/*
 * 숫자를 문자열로 바꾸는 함수
 */
int convertToString(char *str, int num) {
    if (num < 10) {
        *(str + 0) = convertToChar(num);
//        printf("num1 = %d\n", num);
        return 0;
    }
    int index = convertToString(str, num / 10) + 1;
//    printf("num2 = %d\n", num % 10);
    *(str + index) = convertToChar(num % 10);
    return index + 1;
}



/*
 * 변수를 숫자로 대체하는 함수
 */
int replaceVariableToInt(char *str, int index, char* variable, int* value) {
    char valueStr[100] = {'\0'};
    int valueStackIndex = 0;
    char var = *(str + index);
    int valueIndex;
    for (valueIndex = 0; *variable != var; valueIndex++)
        variable++;
    
    int curValue = *(value + valueIndex);
    while (curValue) {
        push(valueStr, convertToChar(curValue % 10), valueStackIndex++);
        curValue /= 10;
    }
    reverse(valueStr, valueStackIndex - 1);
    
    int length = getLength(valueStr);
    for (int i = getLength(str); i > index; i--) {
        *(str + i + length - 1) = *(str + i);
    }
    
    for (int i = 0; i < length; i++) {
        *(str + index++) = *(valueStr + i);
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


// 문자열 길이 얻는 함수
int getLength(char *str) {
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
<<<<<<< HEAD
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
=======
	char stack[100];

	int result = 0;
	int count = 1;
	while(*str < '0' || *str > '9')
		str++;
	while(*str >= '0' && *str <= '9') {
		push(stack, *str++);
	}
	while(!isEmpty(stack)) {
		char c = pop(stack);
		//        printf("%c\n", c);
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
>>>>>>> master
}


// 스택에 문자 하나 추가
<<<<<<< HEAD

void push(char* stack, char chr, int count) {
    stack[count] = chr;
}


// 스택에 수 하나 추가
void pushDouble(double* stack, double chr, int count) {
    stack[count] = chr;
=======
void push(char* stack, char chr) {
	stack[count++] = chr;
>>>>>>> master
}


// 스택에 문자열 추가
<<<<<<< HEAD
void pushStr(char* stack, char *str, int count) {
    while(*str != '\0') {
        stack[count++] = *str++;
    }
=======
void pushStr(char* stack, char *str) {
	while(*str != '\0') {
		stack[count++] = *str++;
	}
>>>>>>> master
}


// 스택에 있는 문자 하나 출력
<<<<<<< HEAD
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
=======
char pop(char* stack) {
	return stack[--count];
>>>>>>> master
}


// 스택이 비었는지 확인
int isEmpty(char *stack) {
<<<<<<< HEAD
    if (*stack == '\0')
        return 1;
    return 0;
=======
	if (count == 0)
		return 1;
	return 0;
>>>>>>> master
}
