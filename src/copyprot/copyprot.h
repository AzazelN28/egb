#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COPYPROT_MAX_STUDENTS 8
#define COPYPROT_MAX_SUBJECTS 8
#define COPYPROT_MAX_EVALS 3
#define COPYPROT_ERR_INVALID -1
#define COPYPROT_FAILURE 1
#define COPYPROT_SUCCESS 0

typedef struct copyprot_student_
{
  char *name;
  uint8_t grades[COPYPROT_MAX_SUBJECTS][COPYPROT_MAX_EVALS];
} copyprot_student_t;

typedef struct copyprot_subject_
{
  char *name;
} copyprot_subject_t;
