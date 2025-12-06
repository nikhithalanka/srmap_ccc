#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 200

struct Student {
    char id[20];
    char name[100];
    char section[20];
    int grade;
    char remark[100];
};

void trim(char *str) {
    str[strcspn(str, "\n")] = '\0';
}

int login(char *role);
void loginMenu();
void adminMenu();
void staffMenu();
void studentMenu(char userid[]);
void addStudent();
void displayStudents();
void searchStudent();
void updateStudentAdmin();
void updateStudentStaff();
void deleteStudent();
void showStudentInfo(char userid[]);


// ---------------------------------------------
int main() {
    loginMenu();
    return 0;
}

// ---------------------------------------------
void loginMenu() {
    char role[20], userid[20];

    if (login(role)) {
        if (strcmp(role, "admin") == 0)
            adminMenu();
        else if (strcmp(role, "staff") == 0)
            staffMenu();
        else if (strcmp(role, "student") == 0) {
            printf("Enter your Student ID: ");
            scanf("%s", userid);
            studentMenu(userid);
        }
    } else {
        printf("\nInvalid Credentials! Try again.\n");
    }
}

// ---------------------------------------------
int login(char *role) {
    char user[50], pass[50], fuser[50], fpass[50], frole[50];

    printf("\n<~~~~~> LOGIN <~~~~~>\n");
    printf("Username: ");
    scanf("%s", user);
    printf("Password: ");
    scanf("%s", pass);

    FILE *fp = fopen("LoginDetails.txt", "r");
    if (!fp) {
        printf("Error: LoginDetails.txt not found!\n");
        return 0;
    }

    while (fscanf(fp, "%s %s %s", fuser, fpass, frole) != EOF) {
        if (strcmp(user, fuser) == 0 && strcmp(pass, fpass) == 0) {
            strcpy(role, frole);
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

// ---------------------------------------------
void adminMenu() {
    int choice;
    while (1) {
        printf("\n<~~~~~> ADMIN MENU <~~~~~>\n");
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudentAdmin(); break;
            case 5: deleteStudent(); break;
            case 6: return;
            default: printf("Invalid choice!\n");
        }
    }
}

// ---------------------------------------------
void staffMenu() {
    int choice;
    while (1) {
        printf("\n<~~~~~> STAFF MENU <~~~~~>\n");
        printf("1. Display All Students\n");
        printf("2. Search Student\n");
        printf("3. Update Student (Limited)\n");
        printf("4. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: updateStudentStaff(); break;
            case 4: return;
            default: printf("Invalid choice!\n");
        }
    }
}

// ---------------------------------------------
void studentMenu(char userid[]) {
    int choice;
    while (1) {
        printf("\n<~~~~~> STUDENT MENU <~~~~~>\n");
        printf("1. View My Info\n");
        printf("2. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1)
            showStudentInfo(userid);
        else if (choice == 2)
            return;
        else
            printf("Invalid choice!\n");
    }
}

// ---------------------------------------------
void addStudent() {
    struct Student s;

    getchar();  
    printf("\nEnter ID: ");
    fgets(s.id, 20, stdin); trim(s.id);

    printf("Enter Name: ");
    fgets(s.name, 100, stdin); trim(s.name);

    printf("Enter Section: ");
    fgets(s.section, 20, stdin); trim(s.section);

    printf("Enter Grade: ");
    scanf("%d", &s.grade);
    getchar();

    printf("Enter Remark: ");
    fgets(s.remark, 100, stdin); trim(s.remark);

    FILE *fp = fopen("StudentInfo.txt", "a");
    if (!fp) { printf("Error opening file!\n"); return; }

    fprintf(fp, "%s,%s,%s,%d,%s\n", s.id, s.name, s.section, s.grade, s.remark);
    fclose(fp);

    printf("Student Added Successfully!\n");
}

// ---------------------------------------------
void displayStudents() {
    struct Student s;
    char line[MAX];
    FILE *fp = fopen("StudentInfo.txt", "r");

    if (!fp) { printf("No student data found!\n"); return; }

    printf("\n<~~~~~> STUDENT LIST <~~~~~>\n");

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^,],%[^,],%[^,],%d,%[^\n]",
               s.id, s.name, s.section, &s.grade, s.remark);

        printf("ID      : %s\n", s.id);
        printf("NAME    : %s\n", s.name);
        printf("SECTION : %s\n", s.section);
        printf("GRADE   : %d\n", s.grade);
        printf("REMARK  : %s\n\n", s.remark);
    }
    fclose(fp);
}

// ---------------------------------------------
void searchStudent() {
    char id[20], line[MAX];
    struct Student s;
    int found = 0;

    printf("\nEnter ID to search: ");
    scanf("%s", id);

    FILE *fp = fopen("StudentInfo.txt", "r");
    if (!fp) { printf("No student data found!\n"); return; }

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^,],%[^,],%[^,],%d,%[^\n]",
               s.id, s.name, s.section, &s.grade, s.remark);

        if (strcmp(id, s.id) == 0) {
            printf("Found: %s | %s | %s | %d | %s\n",
                   s.id, s.name, s.section, s.grade, s.remark);
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (!found) printf("Student not found!\n");
}

// ---------------------------------------------
void updateStudentAdmin() {
    char id[20], line[MAX];
    struct Student s;
    int found = 0;

    printf("\nEnter ID to update: ");
    scanf("%s", id);
    getchar();

    FILE *fp = fopen("StudentInfo.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^,],%[^,],%[^,],%d,%[^\n]",
               s.id, s.name, s.section, &s.grade, s.remark);

        if (strcmp(id, s.id) == 0) {
            found = 1;
            printf("Enter New Name: ");
            fgets(s.name, 100, stdin); trim(s.name);

            printf("Enter New Section: ");
            fgets(s.section, 20, stdin); trim(s.section);

            printf("Enter New Grade: ");
            scanf("%d", &s.grade);
            getchar();

            printf("Enter New Remark: ");
            fgets(s.remark, 100, stdin); trim(s.remark);
        }

        fprintf(temp, "%s,%s,%s,%d,%s\n", s.id, s.name, s.section, s.grade, s.remark);
    }

    fclose(fp);
    fclose(temp);

    remove("StudentInfo.txt");
    rename("temp.txt", "StudentInfo.txt");

    if (found) printf("Student Updated Successfully!\n");
    else printf("Student Not Found!\n");
}

// ---------------------------------------------
void updateStudentStaff() {
    char id[20], line[MAX];
    struct Student s;
    int found = 0;

    printf("\nEnter ID to update: ");
    scanf("%s", id);
    getchar();

    FILE *fp = fopen("StudentInfo.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^,],%[^,],%[^,],%d,%[^\n]",
               s.id, s.name, s.section, &s.grade, s.remark);

        if (strcmp(id, s.id) == 0) {
            found = 1;

            printf("Enter New Section: ");
            fgets(s.section, 20, stdin); trim(s.section);

            printf("Enter New Grade: ");
            scanf("%d", &s.grade);
            getchar();

            printf("Enter New Remark: ");
            fgets(s.remark, 100, stdin); trim(s.remark);
        }

        fprintf(temp, "%s,%s,%s,%d,%s\n", s.id, s.name, s.section, s.grade, s.remark);
    }

    fclose(fp);
    fclose(temp);

    remove("StudentInfo.txt");
    rename("temp.txt", "StudentInfo.txt");

    if (found) printf("Student Updated Successfully!\n");
    else printf("Student Not Found!\n");
}

// ---------------------------------------------
void deleteStudent() {
    char id[20], line[MAX];
    struct Student s;
    int found = 0;

    printf("\nEnter ID to delete: ");
    scanf("%s", id);

    FILE *fp = fopen("StudentInfo.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^,],%[^,],%[^,],%d,%[^\n]",
               s.id, s.name, s.section, &s.grade, s.remark);

        if (strcmp(id, s.id) == 0) {
            found = 1;
            continue;
        }

        fprintf(temp, "%s,%s,%s,%d,%s\n", s.id, s.name, s.section, s.grade, s.remark);
    }

    fclose(fp);
    fclose(temp);

    remove("StudentInfo.txt");
    rename("temp.txt", "StudentInfo.txt");

    if (found) printf("Student Deleted!\n");
    else printf("Student Not Found!\n");
}

// ---------------------------------------------
void showStudentInfo(char userid[]) {
    struct Student s;
    char line[MAX];
    int found = 0;

    FILE *fp = fopen("StudentInfo.txt", "r");
    if (!fp) { printf("No student data found!\n"); return; }

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^,],%[^,],%[^,],%d,%[^\n]",
               s.id, s.name, s.section, &s.grade, s.remark);

        if (strcmp(userid, s.id) == 0) {
            printf("\nYour Info:\n");
            printf("%s | %s | %s | %d | %s\n",
                   s.id, s.name, s.section, s.grade, s.remark);
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (!found)
        printf("No record found for your ID!\n");
}
