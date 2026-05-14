#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct student {
    int id;
    char *lname;     // stores last name of student
    float *scores;   // stores scores of the student. Size is taken from num_scores array.
    float std_avg;   // average score of the student (to be calculated)
} student;

typedef struct course {
    char *course_name;    // stores course name
    int num_sections;     // number of sections
    student **sections;   // stores array of student arrays (2D array). Size is num_sections.
    int *num_students;    // array of number of students in each section. Size is num_sections.
    int *num_scores;      // array of number of assignments in each section. Size is num_sections.
} course;

student **read_sections(FILE *fp, int num_students[], int num_scores[], int num_sections);
course *read_courses(FILE *fp, int *num_courses);
void display_sections(student **sections, int num_students[], int num_scores[], int num_sections);
void process_courses(course *courses, int num_courses);
void release_sections(student **sections, int num_sections, int *num_students, int *num_scores);
void release_courses(course *courses, int num_courses);

int main(void)
{
    FILE *fp = fopen("assignment1input.txt", "r");

    int testCase;
    course *courses;   // to be used for courses array

    fscanf(fp, "%d", &testCase);   // scans in number of cases

    for (int caseCounter = 1; caseCounter < testCase + 1; caseCounter++)
    {
        int num_courses;                          // number of courses
        printf("test case %d\n", caseCounter);    // to match sample output

        courses = read_courses(fp, &num_courses); // reads in courses
        process_courses(courses, num_courses);
        release_courses(courses, num_courses);
    }

    fclose(fp);
    return 0;
}

course *read_courses(FILE *fp, int *num_courses)
{
    fscanf(fp, "%d", num_courses);   // reads in number of courses

    course *courses = (course *) malloc(sizeof(course) * *num_courses);   // allocates memory for courses

    char courseName[20];   // for reading course name

    for (int courseCounter = 0; courseCounter < *num_courses; courseCounter++)   // each course
    {
        fscanf(fp, "%s", courseName);   // reads in name of course

        int courseLength = strlen(courseName);   // length of name for cname

        courses[courseCounter].course_name = malloc(courseLength + 1);   // allocate memory to store name
        strcpy(courses[courseCounter].course_name, courseName);          // copy to the structure

        fscanf(fp, "%d", &courses[courseCounter].num_sections);   // reads in number of sections

        courses[courseCounter].num_students = (int *) malloc(sizeof(int) * courses[courseCounter].num_sections);   // allocates memory for students per section
        courses[courseCounter].num_scores   = (int *) malloc(sizeof(int) * courses[courseCounter].num_sections);   // allocates memory for scores per section

        courses[courseCounter].sections = read_sections(
            fp,
            courses[courseCounter].num_students,
            courses[courseCounter].num_scores,
            courses[courseCounter].num_sections
        );   // arrays filled in courses by calling this function
    }
    return courses;
}

student **read_sections(FILE *fp, int num_students[], int num_scores[], int num_sections)
{
    student **sections = (student **) malloc(sizeof(student *) * num_sections);   // allocate memory for number of sections

    char lname[20];

    for (int sectionCounter = 0; sectionCounter < num_sections; sectionCounter++)   // each section
    {
        fscanf(fp, "%d %d", &num_students[sectionCounter], &num_scores[sectionCounter]);   // scan in number of students and scores for each section

        sections[sectionCounter] = (student *) malloc(sizeof(student) * num_students[sectionCounter]);   // allocate memory for all students in the section

        for (int studentCounter = 0; studentCounter < num_students[sectionCounter]; studentCounter++)   // for each student of each section
        {
            fscanf(fp, "%d %s", &sections[sectionCounter][studentCounter].id, lname);   // scans in id and last name

            sections[sectionCounter][studentCounter].lname = malloc(strlen(lname) + 1);   // allocate memory for last name
            strcpy(sections[sectionCounter][studentCounter].lname, lname);                // copy to the structure

            sections[sectionCounter][studentCounter].scores = (float *) malloc(sizeof(float) * num_scores[sectionCounter]);   // allocate memory for scores

            float scoreSum = 0;

            for (int scoreCounter = 0; scoreCounter < num_scores[sectionCounter]; scoreCounter++)
            {
                fscanf(fp, "%f", &sections[sectionCounter][studentCounter].scores[scoreCounter]);   // scan in score for each student
                scoreSum += sections[sectionCounter][studentCounter].scores[scoreCounter];          // accumulate scores
            }

            sections[sectionCounter][studentCounter].std_avg = scoreSum / num_scores[sectionCounter];   // average score of the student
        }
    }
    return sections;
}

void process_courses(course *courses, int num_courses)
{
    for (int courseCounter = 0; courseCounter < num_courses; courseCounter++)
    {
        printf("%s ", courses[courseCounter].course_name);   // print course name

        display_sections(
            courses[courseCounter].sections,
            courses[courseCounter].num_students,
            courses[courseCounter].num_scores,
            courses[courseCounter].num_sections
        );
    }
}

void display_sections(student **sections, int num_students[], int num_scores[], int num_sections)
{
    float *section_avg = (float *) malloc(num_sections * sizeof(float));   // to store averages

    int totalStudentCounter = 0;
    int totalSectionCounter = 0;
    int passCounter = 0;
    float highestAverage = -1;

    for (int sectionCounter = 0; sectionCounter < num_sections; sectionCounter++)
    {
        float sum_section = 0;

        for (int studentCounter = 0; studentCounter < num_students[sectionCounter]; studentCounter++)
        {
            if (sections[sectionCounter][studentCounter].std_avg >= 70)   // counter for students who pass
            {
                passCounter++;
            }

            sum_section += sections[sectionCounter][studentCounter].std_avg;   // sum averages of all students

            if (sections[sectionCounter][studentCounter].std_avg > highestAverage)   // track student with highest average
            {
                highestAverage      = sections[sectionCounter][studentCounter].std_avg;
                totalStudentCounter = studentCounter;
                totalSectionCounter = sectionCounter;
            }
        }
        section_avg[sectionCounter] = sum_section / num_students[sectionCounter];   // average of the section
    }

    printf("%d ", passCounter);

    for (int i = 0; i < num_sections; i++)
    {
        printf("%.2f ", section_avg[i]);
    }
    printf("%d %s %.2f\n",
           sections[totalSectionCounter][totalStudentCounter].id,
           sections[totalSectionCounter][totalStudentCounter].lname,
           sections[totalSectionCounter][totalStudentCounter].std_avg);

    free(section_avg);   // free allocated section_avg
}

void release_courses(course *courses, int num_courses)
{
    for (int courseCounter = 0; courseCounter < num_courses; courseCounter++)   // for each course
    {
        free(courses[courseCounter].course_name);   // free course name
        release_sections(
            courses[courseCounter].sections,
            courses[courseCounter].num_sections,
            courses[courseCounter].num_students,
            courses[courseCounter].num_scores
        );   // free everything in sections
        free(courses[courseCounter].num_students);   // free number of students
        free(courses[courseCounter].num_scores);     // free number of scores
    }
    free(courses);
}

void release_sections(student **sections, int num_sections, int *num_students, int *num_scores)
{
    for (int sectionCounter = 0; sectionCounter < num_sections; sectionCounter++)   // for each section
    {
        for (int studentCounter = 0; studentCounter < num_students[sectionCounter]; studentCounter++)   // for each student
        {
            free(sections[sectionCounter][studentCounter].lname);    // free last name
            free(sections[sectionCounter][studentCounter].scores);   // free scores
        }

        free(sections[sectionCounter]);
    }

    free(sections);   // free sections
}
