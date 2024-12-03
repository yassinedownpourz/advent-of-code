#include <stddef.h>
#ifndef __UTIL_H__
#define __UTIL_H__

#define MAX_LENGTH 200

const char *file_content_read(const char file_name[static 1])
{
  FILE *file = fopen(file_name, "r");
  if (!file)
  {
    return NULL;
  }
  fseek(file, 0, SEEK_END);
  const long length = ftell(file);
  fseek(file, 0, SEEK_SET);
  char *buffer = (char *)malloc(length + 1);
  if (!buffer)
  {
    return NULL;
  }
  fread(buffer, 1, length, file);
  fclose(file);
  buffer[length] = '\0';
  return buffer;
}

typedef struct SplitContent
{
  char *lines[MAX_LENGTH];
  size_t lines_count;
} SplitContent;

SplitContent *split_file_content(const char file_content[static 1], char delimiter)
{
  SplitContent *splitted = malloc(sizeof *splitted);
  if (!splitted)
  {
    printf("\n\033[1;31m    Error: could not allocate memory    \n\033[0m");
    return NULL;
  }
  const char *lead, *lag;
  lead = lag = file_content;
  splitted->lines_count = 0;
  while (*lead != '\0')
  {
    if (*lead == delimiter)
    {
      const ptrdiff_t length = lead - lag;
      char *new_line = malloc(length + 1);
      if (!new_line)
      {
        printf("\n\033[1;31m    Error: could not allocate memory    \n\033[0m");
        return NULL;
      }
      strncpy(new_line, lag, length);
      new_line[length] = '\0';
      splitted->lines[splitted->lines_count++] = new_line;
      lag = lead + 1;
    }
    lead++;
  }
  if (lead != lag)
  {
    const size_t length = lead - lag;
    char *new_line = malloc(length + 1);
    if (!new_line)
    {
      printf("\n\033[1;31m    Error: could not allocate memory    \n\033[0m");
      return NULL;
    }
    strncpy(new_line, lag, length);
    new_line[length] = '\0';
    splitted->lines[splitted->lines_count++] = new_line;
  }
  return splitted;
}
void split_content_print(SplitContent splitted[static 1])
{

  printf("Lines count: %ld\n", splitted->lines_count);
  for (size_t i = 0; i < splitted->lines_count; i++)
  {
    printf("%s\n", splitted->lines[i]);
  }
}
void split_content_destroy(SplitContent splitted[static 1])
{
  for (size_t i = 0; i < splitted->lines_count; i++)
  {
    free(splitted->lines[i]);
  }
}
const uint16_t max(const uint16_t a, const uint16_t b)
{
  return a * (a >= b) + b * (a < b);
}
const uint16_t to_digit(const char c)
{
  return c - '0';
}

#endif
