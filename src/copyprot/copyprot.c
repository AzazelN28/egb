#include "copyprot.h"

copyprot_student_t students[COPYPROT_MAX_STUDENTS] = {
    {"Iván", {{1, 3, 4}, {2, 3, 4}, {4, 3, 4}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}}},
    {"Alba", {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}}},
    {"Macarena", {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}}},
    {"Aitor", {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}}},
    {"Julia", {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}}},
    {"Juan", {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}}},
    {"Javier", {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}}},
    {"Joaquín", {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}}}};

copyprot_subject_t subjects[COPYPROT_MAX_SUBJECTS] = {
    {"Lengua"},
    {"Matemáticas"},
    {"Ciencias sociales"},
    {"Ciencias naturales"},
    {"Dibujo"},
    {"Música"},
    {"Religión"},
    {"Educación física"}};

int8_t copyprot_get_grade_from_string(const char *grade)
{
  if (strcmp(grade, "INS") == 0)
  {
    return 0;
  }
  else if (strcmp(grade, "SUF") == 0)
  {
    return 1;
  }
  else if (strcmp(grade, "BIE") == 0)
  {
    return 2;
  }
  else if (strcmp(grade, "NOT") == 0)
  {
    return 3;
  }
  else if (strcmp(grade, "SOB") == 0)
  {
    return 4;
  }
  else
  {
    return -1;
  }
}

int8_t copyprot_prompt()
{
  char grade_str[3];

  srand(0);
  uint8_t student = rand() % COPYPROT_MAX_STUDENTS;
  uint8_t subject = rand() % COPYPROT_MAX_SUBJECTS;
  uint8_t eval = rand() % COPYPROT_MAX_EVALS;
  printf("¿Cuál es la nota de %s de %s en la %dª evaluación?\n", students[student].name, subjects[subject].name, (eval + 1));
  scanf("%3s%[^\n]", grade_str);
  int8_t grade = copyprot_get_grade_from_string(grade_str);
  if (grade == -1)
  {
    return COPYPROT_ERR_INVALID;
  }
  else if (students[student].grades[subject][eval] != grade)
  {
    return COPYPROT_FAILURE;
  }
  return COPYPROT_SUCCESS;
}

int copyprot_main(int argc, char **argv)
{
  int8_t tries = 3;
  int8_t result = COPYPROT_ERR_INVALID;
  do
  {
    result = copyprot_prompt();
    if (result == COPYPROT_ERR_INVALID)
    {
      printf("¡Has introducido una nota inválida! Vuelve a intentarlo.\n");
    }
    else if (result == COPYPROT_FAILURE)
    {
      printf("¡Has fallado! Vuelve a intentarlo. Te quedan %d intentos\n", tries);
      tries--;
    }
  } while (result != COPYPROT_SUCCESS && tries > 0);

  if (result == COPYPROT_FAILURE)
  {
    printf("¡Lo siento! Has fallado el reto.\n");
    return 1;
  }
  printf("¡Enhorabuena! Has superado el reto.\n");
  return 0;
}
