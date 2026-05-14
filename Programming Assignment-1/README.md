# COP 3502C — Programming Assignment #1

## Dynamic Memory Allocation

A C program that reads data about courses, sections, students, and assignment scores from a file, then produces summarized statistics for each course using dynamic memory allocation.

## Problem Overview

An educational center offers a number of courses. Each course can have one or more sections, and each section can have one or more students. Every student completes a fixed number of assignments per section. A student passes the course if their average assignment score is `>= 70`.

For every course, the program reports:

- The course name
- The total number of students who passed
- The average score of each section (in file order)
- The id, last name, and average of the single student with the highest average score across the entire course (first student wins ties)

## Files

| File | Description |
|---|---|
| `assignment1.c` | Source code |
| `assignment1input.txt` | Input file (must be in the same directory as the executable; the filename is hardcoded per the spec) |
| `Assignment1.pdf` | Original assignment specification |

## Build & Run

The program reads from a file named `assignment1input.txt` in the current working directory.

```bash
gcc -o assignment1 assignment1.c
./assignment1
```

The code is intended to compile and run on the Eustis server.

## Input Format

```
t                              // number of test cases (t <= 25)
  c                            // number of courses for this test case (c <= 500)
    cn                         // course name (lowercase, single word, max 20 chars)
    s                          // number of sections (s <= 10)
      st m                     // students in section, assignments in section (st <= 500, m <= 20)
        id lname score1 ... scoreM   // one line per student
      ...                      // more sections
    ...                        // more courses
...                            // more test cases
```

Student ids are unique. All scores are positive floats `<= 100`.

## Output Format

One line per course, in the order the courses appear in the file:

```
course_name pass_count section_avg_1 section_avg_2 ... id lname avg_score
```

Section averages and the top student's average are printed to two decimal places. Each test case is preceded by a `test case N` header.

### Sample Output

```
test case 1
cs1 2 70.41 74.15 105 edward 83.07
math2 5 72.90 74.15 76.72 110 kyle 94.35
physics3 2 71.12 105 edward 79.35
test case 2
cs1 5 74.15 76.63 102 sahar 83.07
physics2 5 72.90 74.15 76.72 110 aisa 94.35
```

## Data Structures

The required structures (cannot be modified per the assignment spec):

```c
typedef struct student {
    int id;
    char *lname;     // last name
    float *scores;   // assignment scores; size = num_scores for the section
    float std_avg;   // computed average for this student
} student;

typedef struct course {
    char *course_name;
    int num_sections;
    student **sections;   // 2D array: sections[i] is the array of students in section i
    int *num_students;    // num_students[i] = students in section i
    int *num_scores;      // num_scores[i]   = assignments in section i
} course;
```

A partial memory map: `courses` is an array of `course`. For each course, `sections` is an array of student arrays (a 2D ragged array of `student`).

## Functions

| Function | Purpose |
|---|---|
| `read_courses(fp, *num_courses)` | Reads all course data for one test case; returns a fully populated `course *` array. |
| `read_sections(fp, num_students[], num_scores[], num_sections)` | Reads every section of one course; fills the `num_students` and `num_scores` arrays and returns the 2D `student **` table. Computes each student's `std_avg`. |
| `process_courses(courses, num_courses)` | Iterates the courses array and prints the per-course summary line by delegating to `display_sections`. |
| `display_sections(sections, num_students[], num_scores[], num_sections)` | Computes pass count, per-section averages, and the top student, then prints the formatted line. |
| `release_courses(courses, num_courses)` | Frees every allocation made for the test case. |
| `release_sections(sections, num_sections, num_students, num_scores)` | Frees the per-student `lname` and `scores`, then the section and the outer array. |

## Memory Management Notes

Every dynamically allocated block is released at the end of each test case in this order: each student's `lname` and `scores`, then the section's student array, then the outer `sections` array, then the course's `course_name`, `num_students`, and `num_scores`, and finally the `courses` array itself.

## Assignment Requirements Checklist

- Uses the required `student` and `course` structs without modification
- Uses dynamic memory allocation for every variable-sized array and string
- Frees all memory after each test case
- Implements `read_courses`, `read_sections`, `process_courses`, and `release_courses`
- Reads input from `assignment1input.txt` (filename is hardcoded as required)
- Output matches the format in the spec, with two decimal places for averages

