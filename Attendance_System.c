#include "Attendance_System.h"

 Student student[STUDENT_MAX];  // 学生基本信息结构体数组
 Attend attend[ATTEND_MAX];     //考勤信息结构体数组
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
                savedata();
                break;
            case 2:
                delete_student();
                savedata();
                break;
            case 3:
                search_student();
                savedata();
                break;
            case 4:
                list_student();
                savedata();
                break;
            case 5:
                record_attend();
                savedata();
                break;
            case 6:
                view_attend();
                savedata();
                break;
            case 7:
                count_attend();
                savedata();
                break;
            case 8:
                revise();
                savedata();
                break;
            default:
                printf("请输入正确选项！\n");
                break;
        }
    }
    return 0;
}


void add_student() {
    if (student_num >= STUDENT_MAX) {
        printf("学生数量已达上限，无法添加！\n");
        return;
    }

    int new_id;
    printf("请输入新学生学号：");
    scanf("%d", &new_id);
    getchar();

    for (int i = 0; i < student_num; i++) {
        if (student[i].id == new_id) {
            printf("该学号已存在，添加失败！\n");
            return;
        }
    }

    student[student_num].id = new_id;
    printf("请输入学生姓名：");
    fgets(student[student_num].name, sizeof(student[student_num].name), stdin);
    student[student_num].name[strcspn(student[student_num].name, "\n")] = '\0'; 

    printf("请输入学生班级：");
    fgets(student[student_num].classes, sizeof(student[student_num].classes), stdin);
    student[student_num].classes[strcspn(student[student_num].classes, "\n")] = '\0';

    printf("请输入学生性别（男/女）：");
    fgets(student[student_num].sex, sizeof(student[student_num].sex), stdin);
    student[student_num].sex[strcspn(student[student_num].sex, "\n")] = '\0';

    student_num++;
    printf("学生添加成功！当前总学生数：%d\n", student_num);
}
void delete_student() {  
    printf("=== 删除选项 ===\n");
    printf("1. 删除学生（连带其所有考勤记录）\n");
    printf("2. 删除单条考勤记录\n");
    printf("请选择：");
    int opt;
    scanf("%d", &opt);
    getchar();

    if (opt == 1) {

        if (student_num == 0) {
            printf("暂无学生数据，无需删除！\n");
            return;
        }

        int del_id;
        printf("请输入要删除的学生学号：");
        scanf("%d", &del_id);
        getchar();

        int index = -1;

        for (int i = 0; i < student_num; i++) {
            if (student[i].id == del_id) {
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
        printf("学生删除成功！\n");

       
        int new_attend_num = 0;
        for (int i = 0; i < attend_num; i++) {
            if (attend[i].id != del_id) {
                attend[new_attend_num++] = attend[i];
            }
        }
        attend_num = new_attend_num;
        printf("已同步删除该学生的所有考勤记录，剩余考勤记录数：%d\n", attend_num);

    } else if (opt == 2) {
       
        if (attend_num == 0) {
            printf("暂无考勤记录，无需删除！\n");
            return;
        }

        int del_attend_id;
        printf("请输入要删除的考勤记录ID：");
        scanf("%d", &del_attend_id);
        getchar();

        int index = -1;
        
        for (int i = 0; i < attend_num; i++) {
            if (attend[i].attend_id == del_attend_id) {
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
        printf("考勤记录删除成功！剩余考勤记录数：%d\n", attend_num);

    } else {
        printf("输入选项错误！\n");
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
