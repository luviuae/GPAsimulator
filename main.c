#include <stdio.h>
#include <math.h>
#include <string.h>

// [유틸리티] 학교 성적 체계 (A+:4.5, A0:4.3, A-:4.0, B+:3.5, B0:3.3, B-:3.0 ...) 완벽 반영
double grade_to_point(const char* grade) {
    if (strcmp(grade, "A+") == 0 || strcmp(grade, "a+") == 0) return 4.5;
    if (strcmp(grade, "A0") == 0 || strcmp(grade, "a0") == 0) return 4.3;
    if (strcmp(grade, "A-") == 0 || strcmp(grade, "a-") == 0) return 4.0;
    if (strcmp(grade, "B+") == 0 || strcmp(grade, "b+") == 0) return 3.5;
    if (strcmp(grade, "B0") == 0 || strcmp(grade, "b0") == 0) return 3.3;
    if (strcmp(grade, "B-") == 0 || strcmp(grade, "b-") == 0) return 3.0;
    if (strcmp(grade, "C+") == 0 || strcmp(grade, "c+") == 0) return 2.5;
    if (strcmp(grade, "C0") == 0 || strcmp(grade, "c0") == 0) return 2.3;
    if (strcmp(grade, "C-") == 0 || strcmp(grade, "c-") == 0) return 2.0;
    if (strcmp(grade, "D+") == 0 || strcmp(grade, "d+") == 0) return 1.5;
    if (strcmp(grade, "D0") == 0 || strcmp(grade, "d0") == 0) return 1.3;
    if (strcmp(grade, "D-") == 0 || strcmp(grade, "d-") == 0) return 1.0;
    if (strcmp(grade, "F") == 0 || strcmp(grade, "f") == 0) return 0.0;
    return -1.0; // 에러 또는 P/F 과목 처리용
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

        printf("[%d번째 과목] 학점(숫자)과 등급(문자) 입력 (예: 3 A0): ", i + 1);
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

// [기능 2] 전체 학점 시뮬레이터
void simulate_overall_gpa() {
    double current_gpa, target_gpa;
    int current_credits, total_credits;
    int remaining_credits;
    double required_gpa;
    double current_grade_points, needed_grade_points;

    printf("\n--- [2] 전체 학점 시뮬레이터 ---\n");
    printf("P/F 과목을 제외한 평점 계산 대상 학점을 입력하세요.\n");
    printf("1. 현재까지 이수한 총 학점 수 (예: 18): ");
    if (scanf("%d", &current_credits) != 1) return;
    printf("2. 현재까지의 전체 평균 평점 (예: 3.5): ");
    if (scanf("%lf", &current_gpa) != 1) return;
    printf("3. 목표 졸업 전체 평점 (예: 4.0): ");
    if (scanf("%lf", &target_gpa) != 1) return;
    printf("4. 졸업 시점의 예상 총 학점\n   (참고)전자정보공학부의 졸업 최소 학점은 133학점입니다: ");
    if (scanf("%d", &total_credits) != 1) return;

    remaining_credits = total_credits - current_credits;
    current_grade_points = current_credits * current_gpa;

    if (remaining_credits <= 0) {
        if (current_gpa >= target_gpa) {
            printf("이미 목표 평점을 달성했습니다! 여유롭게 졸업 가능합니다!\n");
        } else {
            printf("남은 학점이 없습니다. 목표 평점 미달이며 변경이 불가능합니다.\n");
        }
        return;
    }

    needed_grade_points = total_credits * target_gpa - current_grade_points;
    required_gpa = needed_grade_points / (double)remaining_credits;

    if (required_gpa <= 0.0) {
        printf("현재 성적으로는 이미 목표 평점 달성 수준입니다! 여유롭게 졸업 가능합니다!\n");
        printf("남은 학점 %d에서 필요한 평균 평점: 0.00\n", remaining_credits);
    } else if (required_gpa > 4.5) {
        printf("남은 학점 %d에서 필요한 평균 평점: %.2f\n", remaining_credits, required_gpa);
        printf("현실적으로 불가능합니다 ㅠㅠ 목표 평점을 낮춰보세요.\n");
    } else {
        printf("남은 학점 %d에서 필요한 평균 평점: %.2f\n", remaining_credits, required_gpa);
        
        int total_courses = remaining_credits / 3;
        if (total_courses > 0) {
            printf("\n[추천 수강 로드맵 (3학점 과목 기준)]\n");
            printf("남은 학점은 대략 3학점 과목 %d개 분량입니다.\n", total_courses);

            if (required_gpa > 4.3 && required_gpa <= 4.5) {
                int num_aplus = (int)ceil(((required_gpa - 4.3) * total_courses) / 0.2);
                if (num_aplus > total_courses) num_aplus = total_courses;
                int num_azero = total_courses - num_aplus;
                printf("남은 과목 중 최소 [A+] %d개, [A0] %d개를 받으셔야 합니다!\n", num_aplus, num_azero);
            } 
            else if (required_gpa > 4.0 && required_gpa <= 4.3) {
                int num_azero = (int)ceil(((required_gpa - 4.0) * total_courses) / 0.3);
                if (num_azero > total_courses) num_azero = total_courses;
                int num_aminus = total_courses - num_azero;
                printf("남은 과목 중 최소 [A0] %d개, [A-] %d개를 받으셔야 합니다!\n", num_azero, num_aminus);
            } 
            else if (required_gpa > 3.5 && required_gpa <= 4.0) {
                int num_aminus = (int)ceil(((required_gpa - 3.5) * total_courses) / 0.5);
                if (num_aminus > total_courses) num_aminus = total_courses;
                int num_bplus = total_courses - num_aminus;
                printf("남은 과목 중 최소 [A-] %d개, [B+] %d개를 받으셔야 합니다!\n", num_aminus, num_bplus);
            }
            else if (required_gpa > 3.3 && required_gpa <= 3.5) {
                // B+(3.5)와 B0(3.3)의 조합 계산 (차이 0.2)
                int num_bplus = (int)ceil(((required_gpa - 3.3) * total_courses) / 0.2);
                if (num_bplus > total_courses) num_bplus = total_courses;
                int num_bzero = total_courses - num_bplus;
                printf("남은 과목 중 최소 [B+] %d개, [B0] %d개를 받으셔야 합니다!\n", num_bplus, num_bzero);
            }
            else if (required_gpa > 3.0 && required_gpa <= 3.3) {
                // B0(3.3)와 B-(3.0)의 조합 계산 (차이 0.3)
                int num_bzero = (int)ceil(((required_gpa - 3.0) * total_courses) / 0.3);
                if (num_bzero > total_courses) num_bzero = total_courses;
                int num_bminus = total_courses - num_bzero;
                printf("남은 과목 중 최소 [B0] %d개, [B-] %d개를 받으셔야 합니다!\n", num_bzero, num_bminus);
            }
            else {
                printf("모든 과목을 평균 [B-] (3.0) 로 유지하셔도 충분히 달성 가능합니다. 화이팅!\n");
            }
        }
    }
}

// [기능 3] 전공 학점 정밀 시뮬레이터
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
    printf("4. 졸업 시점의 예상 총 전공 학점\n   (참고)전자정보공학부의 졸업 최소 전공 학점은 66학점입니다: ");
    if (scanf("%d", &total_credits) != 1) return;
    printf("5. 앞으로 수강해야 할 '1학점 실험 과목' 수\n   (1학점 실험 과목은 2-1, 2-2, 3-1, 3-2 총 4개입니다.): ");
    if (scanf("%d", &lab_courses) != 1) return;

    remaining_credits = total_credits - current_credits;
    
    if (remaining_credits <= 0) {
        printf("남은 전공 학점이 없습니다.\n");
        return;
    }

    total_needed_points = total_credits * target_gpa;
    current_grade_points = current_credits * current_gpa;
    remaining_points = total_needed_points - current_grade_points;

    // 실험 과목은 무조건 Ao도 아니고 최고점인 A+(4.5)를 받는다고 가정하여 가중치 선반영
    double lab_points = lab_courses * 4.5;
    
    double major_3credit_points = remaining_points - lab_points;
    int major_3credit_credits = remaining_credits - (lab_courses * 1);
    int total_3credit_courses = major_3credit_credits / 3;

    printf("\n[분석 결과]\n");
    printf("- 남은 전공 학점: %d학점 (1학점 실험 %d개 + 3학점 전공 %d개)\n", 
           remaining_credits, lab_courses, total_3credit_courses);
    printf("- 1학점 실험 과목 %d개는 전부 [A+](4.5)를 받는다고 가정하고 계산합니다.\n", lab_courses);

    if (major_3credit_credits <= 0) {
        if (major_3credit_points <= 0) {
            printf("실험 과목만 잘 이수해도 전공 목표 평점을 달성합니다!\n");
        } else {
            printf("실험 과목을 전부 A+ 받아도 전공 목표 평점 달성이 어렵습니다. 목표를 조정하세요.\n");
        }
        return;
    }

    required_3credit_gpa = major_3credit_points / (double)major_3credit_credits;

    if (required_3credit_gpa <= 0.0) {
        printf("남은 3학점 전공 과목들에서 필요한 평균 평점: 0.00\n");
        printf("이미 전공 목표 학점을 달성한 수준입니다. 무난하게 졸업 가능합니다!\n");
    } else if (required_3credit_gpa > 4.5) {
        printf("남은 3학점 전공 과목들에서 필요한 평균 평점: %.2f\n", required_3credit_gpa);
        printf("현실적으로 불가능합니다 ㅠㅠ 실험 과목 외 3학점 전공에서 4.5를 넘어야 합니다.\n");
    } else {
        printf("-> 남은 3학점 전공 과목들에서 필요한 평균 평점: %.2f\n", required_3credit_gpa);
        printf("\n[추천 전공 수강 로드맵]\n");

        if (required_3credit_gpa > 4.3 && required_3credit_gpa <= 4.5) {
            int num_aplus = (int)ceil(((required_3credit_gpa - 4.3) * total_3credit_courses) / 0.2);
            if (num_aplus > total_3credit_courses) num_aplus = total_3credit_courses;
            int num_azero = total_3credit_courses - num_aplus;
            printf("남은 3학점 전공 %d개 중 최소 [A+] %d개, [A0] %d개를 받으셔야 합니다!\n", 
                   total_3credit_courses, num_aplus, num_azero);
        } 
        else if (required_3credit_gpa > 4.0 && required_3credit_gpa <= 4.3) {
            int num_azero = (int)ceil(((required_3credit_gpa - 4.0) * total_3credit_courses) / 0.3);
            if (num_azero > total_3credit_courses) num_azero = total_3credit_courses;
            int num_aminus = total_3credit_courses - num_azero;
            printf("남은 3학점 전공 %d개 중 최소 [A0] %d개, [A-] %d개를 받으셔야 합니다!\n", 
                   total_3credit_courses, num_azero, num_aminus);
        } 
        else if (required_3credit_gpa > 3.5 && required_3credit_gpa <= 4.0) {
            int num_aminus = (int)ceil(((required_3credit_gpa - 3.5) * total_3credit_courses) / 0.5);
            if (num_aminus > total_3credit_courses) num_aminus = total_3credit_courses;
            int num_bplus = total_3credit_courses - num_aminus;
            printf("남은 3학점 전공 %d개 중 최소 [A-] %d개, [B+] %d개를 받으셔야 합니다!\n", 
                   total_3credit_courses, num_aminus, num_bplus);
        }
        else if (required_3credit_gpa > 3.3 && required_3credit_gpa <= 3.5) {
            int num_bplus = (int)ceil(((required_3credit_gpa - 3.3) * total_3credit_courses) / 0.2);
            if (num_bplus > total_3credit_courses) num_bplus = total_3credit_courses;
            int num_bzero = total_3credit_courses - num_bplus;
            printf("남은 3학점 전공 %d개 중 최소 [B+] %d개, [B0] %d개를 받으셔야 합니다!\n", 
                   total_3credit_courses, num_bplus, num_bzero);
        }
        else if (required_3credit_gpa > 3.0 && required_3credit_gpa <= 3.3) {
            int num_bzero = (int)ceil(((required_3credit_gpa - 3.0) * total_3credit_courses) / 0.3);
            if (num_bzero > total_3credit_courses) num_bzero = total_3credit_courses;
            int num_bminus = total_3credit_courses - num_bzero;
            printf("남은 3학점 전공 %d개 중 최소 [B0] %d개, [B-] %d개를 받으셔야 합니다!\n", 
                   total_3credit_courses, num_bzero, num_bminus);
        }
        else {
            printf("남은 전공 과목들을 평균 [B-] (3.0) 이하로 유지하셔도 충분히 전공 목표 달성이 가능합니다. 화이팅!\n");
        }
    }
}

// 메인 함수
int main(void) {
    int choice;

    while (1) {
        printf("\n=========================================\n");
        printf("          학점 계산 시뮬레이터         \n");
        printf("=========================================\n");
        printf("1. 나의 평점 평균 계산기 (과목별 직접 입력)\n");
        printf("2. 전체 학점 시뮬레이션\n");
        printf("3. 전공 학점 시뮬레이션\n");
        printf("4. 프로그램 종료\n");
        printf("=========================================\n");
        printf("원하는 메뉴 번호를 선택하세요: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("잘못된 입력입니다. 숫자를 입력해주세요.\n");
            while (getchar() != '\n'); 
            continue;
        }

        if (choice == 1) {
            calculate_my_gpa();
        } else if (choice == 2) {
            simulate_overall_gpa();
        } else if (choice == 3) {
            simulate_major_gpa();
        } else if (choice == 4) {
            printf("프로그램을 종료합니다. 한 학기 동안 화이팅하세요!\n");
            break;
        } else {
            printf("1번부터 4번 사이의 번호를 입력해주세요.\n");
        }
    }

    return 0;
}