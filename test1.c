#include <stdio.h>
#include <math.h>
#include <string.h>

// [유틸리티] 등급별 점수 매핑
double grade_to_point(const char* grade) {
    if (strcmp(grade, "A+") == 0) return 4.5;
    if (strcmp(grade, "A0") == 0 || strcmp(grade, "Ao") == 0 || strcmp(grade, "AO") == 0) return 4.3;
    if (strcmp(grade, "A-") == 0) return 4.0;
    if (strcmp(grade, "B+") == 0) return 3.5;
    if (strcmp(grade, "B0") == 0 || strcmp(grade, "Bo") == 0 || strcmp(grade, "BO") == 0) return 3.3;
    if (strcmp(grade, "B-") == 0) return 3.0;
    if (strcmp(grade, "F") == 0 || strcmp(grade, "f") == 0) return 0.0;
    return -1.0;
}

// [핵심 엔진] B- 구간까지 확장하여 모든 학점 조합을 탐색하는 함수
void print_grade_combinations(int total_courses, double required_gpa) {
    if (total_courses <= 0) return;

    printf("\n? [추천 수강 로드맵 - 가능한 성적 조합 (최대 5개 추출)]\n");
    printf("-----------------------------------------------------------------------\n\n");
    int count = 0;

    // B-(3.0)부터 시작해서 등급별 개수를 조합하는 5중 루프 (나머지는 자동으로 A+의 몫)
    for (int b_minus = total_courses; b_minus >= 0; b_minus--) {
        for (int b_zero = total_courses - b_minus; b_zero >= 0; b_zero--) {
            for (int b_plus = total_courses - b_minus - b_zero; b_plus >= 0; b_plus--) {
                for (int a_minus = total_courses - b_minus - b_zero - b_plus; a_minus >= 0; a_minus--) {
                    for (int a_zero = total_courses - b_minus - b_zero - b_plus - a_minus; a_zero >= 0; a_zero--) {
                        
                        // 남은 과목은 최고점인 A+가 가져감
                        int a_plus = total_courses - b_minus - b_zero - b_plus - a_minus - a_zero;

                        // 현재 조합의 평균 평점 계산 (학교 성적 체계 기준 반영)
                        double total_points = (a_plus * 4.5) + (a_zero * 4.3) + (a_minus * 4.0) + 
                                              (b_plus * 3.5) + (b_zero * 3.3) + (b_minus * 3.0);
                        double avg = total_points / total_courses;

                        // 목표 평점을 만족하면 옵션으로 출력
                        if (avg >= required_gpa) {
                            count++;
                            printf(" 옵션 %d) [A+] %d개, [Ao] %d개, [A-] %d개, [B+] %d개, [Bo] %d개, [B-] %d개  (예상 평균: %.2f)\n\n", 
                                   count, a_plus, a_zero, a_minus, b_plus, b_zero, b_minus, avg);
                            
                            if (count >= 5) { // 최적 옵션 5개만 보여주고 과부하 방지 겸 종료
                                printf("-----------------------------------------------------------------------\n");
                                return;
                            }
                        }
                    }
                }
            }
        }
    }

    if (count == 0) {
        printf(" ?? B- 이상으로 달성 가능한 조합이 없습니다. 올 A+에 가까운 성적이 필요합니다.\n");
    }
    printf("-----------------------------------------------------------------------\n");
}

// [기능 1] 나의 평점 평균 계산기
void calculate_my_gpa() {
    int num_subjects;
    int total_credits = 0;
    double total_points = 0.0;

    printf("\n--- [1] 나의 평점 평균 계산기 ---\n");
    printf("계산할 과목의 총 개수를 입력하세요: ");
    if (scanf("%d", &num_subjects) != 1 || num_subjects <= 0) {
        printf("올바른 과목 수를 입력해주세요.\n");
        while (getchar() != '\n');
        return;
    }

    for (int i = 0; i < num_subjects; i++) {
        int credits;
        char grade[10];

        printf("[%d번째 과목] 학점(숫자)과 등급(문자) 입력 (예: 3 Ao): ", i + 1);
        if (scanf("%d %s", &credits, grade) != 2) {
            printf("입력 오류가 발생했습니다. 이 과목을 다시 입력합니다.\n");
            while (getchar() != '\n');
            i--;
            continue;
        }

        if (strcmp(grade, "P") == 0 || strcmp(grade, "p") == 0) {
            printf("-> [안내] Pass 과목은 평점 계산 대상에서 제외됩니다.\n");
            continue;
        }

        double point = grade_to_point(grade);
        if (point < 0.0) {
            printf("-> [오류] 잘못된 등급 기호입니다. (A+, A0, A-, B+, B0, B- 등 입력)\n");
            i--;
            continue;
        }

        total_credits += credits;
        total_points += (credits * point);
    }

    printf("\n=========================================\n");
    if (total_credits == 0) {
        printf("평점 계산 대상 학점이 0학점입니다.\n");
    } else {
        printf("[계산 결과]\n");
        printf("- 평점 반영 학점 총합: %d 학점\n", total_credits);
        printf("- 당신의 평점 평균: %.2f\n", total_points / total_credits);
    }
    printf("=========================================\n");
}

// [기능 2] 전체 학점 시뮬레이터 (엔진 연동)
void simulate_overall_gpa() {
    double current_gpa, target_gpa;
    int current_credits, total_credits;
    int remaining_credits;
    double required_gpa;
    double current_grade_points, needed_grade_points;

    printf("\n--- [2] 전체 학점 시뮬레이터 ---\n");
    printf("P/F 과목을 제외한 평점 계산 대상 학점을 입력하세요.\n");
    printf("1. 현재까지 이수한 총 학점 수: ");
    if (scanf("%d", &current_credits) != 1) return;
    printf("2. 현재까지의 전체 평균 평점: ");
    if (scanf("%lf", &current_gpa) != 1) return;
    printf("3. 목표 졸업 전체 평점: ");
    if (scanf("%lf", &target_gpa) != 1) return;
    printf("4. 졸업 시점의 예상 총 학점: ");
    if (scanf("%d", &total_credits) != 1) return;

    remaining_credits = total_credits - current_credits;
    current_grade_points = current_credits * current_gpa;

    if (remaining_credits <= 0) {
        if (current_gpa >= target_gpa) printf("이미 목표 평점을 달성했습니다!\n");
        else printf("남은 학점이 없습니다. 목표 평점 미달입니다.\n");
        return;
    }

    needed_grade_points = total_credits * target_gpa - current_grade_points;
    required_gpa = needed_grade_points / (double)remaining_credits;

    if (required_gpa <= 0.0) {
        printf("이미 목표 평점 달성 수준입니다! 남은 학점 필요한 평균 평점: 0.00\n");
    } else if (required_gpa > 4.5) {
        printf("남은 학점 %d에서 필요한 평균 평점: %.2f (현실적으로 불가능)\n", remaining_credits, required_gpa);
    } else {
        printf("남은 학점 %d에서 필요한 평균 평점: %.2f\n", remaining_credits, required_gpa);
        int total_courses = remaining_credits / 3;
        // 새로 만든 다양한 경우의 수 출력 함수 호출!
        print_grade_combinations(total_courses, required_gpa);
    }
}

// [기능 3] 전공 학점 정밀 시뮬레이터 (엔진 연동)
void simulate_major_gpa() {
    double current_gpa, target_gpa;
    int current_credits, total_credits;
    int remaining_credits;
    int lab_courses;
    double current_grade_points, total_needed_points, remaining_points;
    double required_3credit_gpa;

    printf("\n--- [3] 전공 학점 정밀 시뮬레이터 ---\n");
    printf("1. 현재까지 이수한 전공 학점 수: ");
    if (scanf("%d", &current_credits) != 1) return;
    printf("2. 현재까지의 전공 평균 평점: ");
    if (scanf("%lf", &current_gpa) != 1) return;
    printf("3. 목표 졸업 전공 평점: ");
    if (scanf("%lf", &target_gpa) != 1) return;
    printf("4. 졸업 시점의 예상 총 전공 학점: ");
    if (scanf("%d", &total_credits) != 1) return;
    printf("5. 앞으로 수강해야 할 '1학점 실험 과목' 수: ");
    if (scanf("%d", &lab_courses) != 1) return;

    remaining_credits = total_credits - current_credits;
    
    if (remaining_credits <= 0) {
        printf("남은 전공 학점이 없습니다.\n");
        return;
    }

    total_needed_points = total_credits * target_gpa;
    current_grade_points = current_credits * current_gpa;
    remaining_points = total_needed_points - current_grade_points;

    double lab_points = lab_courses * 4.5; // 실험 과목 A+ 선반영
    double major_3credit_points = remaining_points - lab_points;
    int major_3credit_credits = remaining_credits - (lab_courses * 1);
    int total_3credit_courses = major_3credit_credits / 3;

    printf("\n[분석 결과]\n");
    printf("- 남은 전공 학점: %d학점 (1학점 실험 %d개 + 3학점 전공 %d개)\n", remaining_credits, lab_courses, total_3credit_courses);
    printf("- 1학점 실험 과목은 전부 [A+](4.5)를 받는다고 가정하고 계산합니다.\n");

    if (major_3credit_credits <= 0) {
        if (major_3credit_points <= 0) printf("실험 과목만 잘 이수해도 목표 달성!\n");
        else printf("실험 과목을 다 A+ 받아도 목표 달성이 어렵습니다.\n");
        return;
    }

    required_3credit_gpa = major_3credit_points / (double)major_3credit_credits;

    if (required_3credit_gpa <= 0.0) {
        printf("남은 3학점 전공 필요한 평균 평점: 0.00 (이미 달성 수준)\n");
    } else if (required_3credit_gpa > 4.5) {
        printf("남은 3학점 전공 필요한 평균 평점: %.2f (현실적으로 불가능)\n", required_3credit_gpa);
    } else {
        printf("-> 남은 3학점 전공 과목들에서 필요한 평균 평점: %.2f\n", required_3credit_gpa);
        // 새로 만든 다양한 경우의 수 출력 함수 호출!
        print_grade_combinations(total_3credit_courses, required_3credit_gpa);
    }
}

// 메인 함수
int main(void) {
    int choice;
    while (1) {
        printf("\n=========================================\n");
        printf("    학점 계산 및 다중 로드맵 시뮬레이터   \n");
        printf("=========================================\n");
        printf("1. 나의 평점 평균 계산기 (과목별 직접 입력)\n");
        printf("2. 전체 학점 시뮬레이션 (다중 옵션 제안)\n");
        printf("3. 전공 학점 시뮬레이션 (실험 반영 및 다중 옵션)\n");
        printf("4. 프로그램 종료\n");
        printf("=========================================\n");
        printf("원하는 메뉴 번호를 선택하세요: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("잘못된 입력입니다. 숫자를 입력해주세요.\n");
            while (getchar() != '\n'); 
            continue;
        }

        if (choice == 1) calculate_my_gpa();
        else if (choice == 2) simulate_overall_gpa();
        else if (choice == 3) simulate_major_gpa();
        else if (choice == 4) {
            printf("프로그램을 종료합니다. 화이팅!\n");
            break;
        } else printf("1~4번 사이의 번호를 입력해주세요.\n");
    }
    return 0;
}