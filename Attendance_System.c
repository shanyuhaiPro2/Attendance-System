#include "Attendance_System.h"

// Student student[STUDENT_MAX];  // 学生基本信息结构体数组
// Attend attend[ATTEND_MAX];     //考勤信息结构体数组
int attend_num=0, student_num=0;   //全局变量，分别是但前考勤记录条数和学生人数

void menu() {
    printf("=== 菜单 ===\n");
    printf("1. 添加学生\n");
    printf("2. 删除学生或某条考勤记录\n");
    printf("3. 查询学生\n");
    printf("4. 显示所有学生\n");
    printf("5. 记录考勤\n");
    printf("6. 查看考勤\n");
    printf("7. 考勤统计\n");
    printf("8. 修改某条考勤记录\n");
    printf("0. 退出系统\n");
    printf("请选择操作：\n");
}

int main() {
    printf("欢迎使用学生考勤系统！\n\n");
    int n;
    loaddata();
    while (1) {
        menu();
        scanf("%d", &n);
        getchar();
        if (n == 0) {
            printf("感谢使用！\n");
            savedata();
            return 0;
        }
        switch(n){
            case 1:
                add_student();
                break;
            case 2:
                delect_student();
                break;
            case 3:
                search_student();
                break;
            case 4:
                list_student();
                break;
            case 5:
                record_attend();
                break;
            case 6:
                view_attend();
                break;
            case 7:
                count_attend();
                break;
            case 8:
                revise();
                break;
            default:
                printf("请输入正确选项！\n");
                break;
        }
    }
    return 0;
}

void addStudent() {
    if (student_num >= STUDENT_MAX) {
        printf("学生数量已达上限！\n");
        return;
    }

    char tempId[20];
    printf("请输入学号：");
    scanf("%s", tempId);
    for (int i = 0; i < student_num; i++) {
        if (strcmp(student[i].id, tempId) == 0) {
            printf("该学号已存在，添加失败！\n");
            return;
        }
    }

    strcpy(student[student_num].id, tempId);
    printf("请输入姓名：");
    scanf("%s", student[student_num].name);
    printf("请输入班级：");
    scanf("%s", student[student_num].className);
    student[student_num].score = 0; 
    student_num++;
    printf("学生添加成功！\n");
}


void deleteInfo() {
    int choice;
    printf("=== 删除选项 ===\n");
    printf("1. 删除学生（同步删除其所有考勤）\n");
    printf("2. 删除单条考勤记录\n");
    printf("请选择：");
    scanf("%d", &choice);

    if (choice == 1) {
        char delId[20];
        printf("请输入要删除的学生学号：");
        scanf("%s", delId);

        int index = -1;
        for (int i = 0; i < student_num; i++) {
            if (strcmp(student[i].id, delId) == 0) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            printf("未找到该学生！\n");
            return;
        }

        for (int i = index; i < student_num - 1; i++) {
            student[i] = student[i + 1];
        }
        student_num--;
		int j = 0;
        while (j < attend_num) {
            if (strcmp(attend[j].stuId, delId) == 0) {
                for (int k = j; k < attend_num - 1; k++) {
                    attend[k] = attend[k + 1];
                }
                attend_num--;
            } else {
                j++;
            }
        }
        printf("学生及关联考勤已删除！\n");

    } else if (choice == 2) {
        char delId[20], delDate[20];
        printf("请输入学生学号：");
        scanf("%s", delId);
        printf("请输入要删除的考勤日期（格式：2025-12-22）：");
        scanf("%s", delDate);

        int index = -1;
        for (int i = 0; i < attend_num; i++) {
            if (strcmp(attend[i].stuId, delId) == 0 && strcmp(attend[i].date, delDate) == 0) {
                index = i;
                break;
            }
        }

         if (index == -1) {
            printf("未找到该考勤记录！\n");
            return;
        }

	for (int i = index; i < attend_num - 1; i++) {
            attend[i] = attend[i + 1];
        }
        attend_num--;
        calculateScore(); // 删除后重新计算考核分
        printf("考勤记录已删除！\n");

    } else {
        printf("输入错误！\n");
    }
}

void loaddata(){
    int count_stu = 0;
    int count_att = 0;
    FILE* fp_stu = fopen("Studentdata.txt", "r");
    if(fp_stu==NULL){
        printf("文件不存在！\n");
        student_num = 0;
    }else{
    char line_stu[100];
    fgets(line_stu, sizeof(line_stu), fp_stu);
    for (int i = 0; student_num < STUDENT_MAX; i++) {
        if(fscanf(fp_stu, "学号：%d\n姓名：%s\n班级：%s\n性别：%s\n",
             &student[i].id, student[i].name,
             student[i].classes, student[i].sex)==4){
            count_stu++;
        }else{
            break;
        }
    }
    printf("已加载%d条学生数据,", count_stu);
    fclose(fp_stu);
}
    FILE* fp_att = fopen("Attenddata.txt", "r");
    if (fp_att == NULL) {
        printf("文件不存在！\n");
        attend_num = 0;
    }
    else {
        char line_att[100];
        fgets(line_att, sizeof(line_att), fp_att);
        for (int i = 0; attend_num < ATTEND_MAX; i++) {
            if (fscanf(fp_att, "%d\t\t%d\t%s\t\t%d\t%s", &attend[i].attend_id,
                       &attend[i].id, attend[i].subject, &attend[i].mode,
                       attend[i].date) == 5) {
                count_att++;
            } else {
                break;
            }
        }
        fclose(fp_att);
        printf("%d条考勤记录\n\n",count_att);
    }
    student_num = count_stu;
    attend_num = count_att;
}
void savedata(){
    FILE* fp_stu = fopen("Studentdata.txt", "w");
    if (fp_stu == NULL) {
        perror("文件打开错误！\n");
        return;
    }
    fprintf(fp_stu, "学生信息：\n");
    for (int i = 0; i < student_num; i++) {
        fprintf(fp_stu, "学号：%d\n", student[i].id);
        fprintf(fp_stu, "姓名：%s\n", student[i].name);
        fprintf(fp_stu, "班级：%s\n", student[i].classes);
        fprintf(fp_stu, "性别：%s\n\n", student[i].sex);

    }
    FILE* fp_att = fopen("Attenddata.txt", "w");
    if (fp_att == NULL) {
        perror("文件打开错误！\n");
        return;
    }
    fprintf(fp_att, "记录id\t学号\t课程\t\t考勤状态\t日期\n");
    for (int i = 0; i < attend_num; i++) {
        fprintf(fp_att, "%d\t\t", attend[i].attend_id);
        fprintf(fp_att, "%d\t", attend[i].id);
        fprintf(fp_att, "%s\t\t", attend[i].subject);
        fprintf(fp_att, "%d\t", attend[i].mode);
        fprintf(fp_att, "%s\n", attend[i].date);
    }
    fclose(fp_stu);
    fclose(fp_att);
}
