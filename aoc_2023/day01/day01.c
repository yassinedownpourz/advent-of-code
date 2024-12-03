#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "../util.h"

#define MAX_LEN 8
#define NUM_MAP_TO_DIGIT 13

typedef struct NumToDigit
{
  const char num[MAX_LEN];
  uint16_t digit;
  size_t len;
} NumToDigit;

NumToDigit map_num_to_digit[NUM_MAP_TO_DIGIT] = {
    {"oneight", 18, 6},
    {"twone", 21, 5},
    {"eightwo", 82, 7},
    {"zero", 0, 4},
    {"one", 1, 3},
    {"two", 2, 3},
    {"three", 3, 5},
    {"four", 4, 4},
    {"five", 5, 4},
    {"six", 6, 3},
    {"seven", 7, 5},
    {"eight", 8, 5},
    {"nine", 9, 4}};

uint16_t part_one(const char file_content[static 1])
{
  uint16_t sum = {0};
  int16_t first_digit, last_digit;
  first_digit = last_digit = -1;
  for (const char *ptr = file_content; *ptr != '\0'; ptr++)
  {
    if (*ptr == '\n' || *(ptr + 1) == '\0')
    {
      if (first_digit != -1 && last_digit != -1)
      {
        sum += (first_digit * 10) + last_digit;
      }
      first_digit = last_digit = -1;
    }
    else if (isdigit(*ptr))
    {
      uint16_t digit = to_digit(*ptr);
      if (first_digit == -1)
      {
        first_digit = digit;
      }
      last_digit = digit;
    }
  }
  return sum;
}

uint16_t part_two(const char file_content[static 1])
{
  uint16_t sum = {0};
  int16_t first_digit, last_digit;
  first_digit = last_digit = -1;
  for (const char *ptr = file_content; *ptr != '\0'; ptr++)
  {
    if (*ptr == '\n' || *(ptr + 1) == '\0')
    {
      if (first_digit != -1 && last_digit != -1)
      {
        sum += (first_digit * 10) + last_digit;
      }
      first_digit = last_digit = -1;
    }
    else if (isdigit(*ptr))
    {
      const uint16_t digit = to_digit(*ptr);
      if (first_digit == -1)
      {
        first_digit = digit;
      }
      last_digit = digit;
    }
    else
    {
      NumToDigit *map_end = map_num_to_digit + NUM_MAP_TO_DIGIT;
      for (NumToDigit *map_start = map_num_to_digit; map_start < map_end; map_start++)
      {
        if (!strncmp(map_start->num, ptr, map_start->len))
        {
          if (first_digit == -1)
          {
            first_digit = (map_start->digit > 10)
                              ? (map_start->digit / 10)
                              : map_start->digit;
          }
          last_digit = (map_start->digit % 10);
          ptr += strlen(map_start->num) - 1;
          break;
        }
      }
    }
  }
  return sum;
}

int main(int argc, char *argv[])
{
  if (argc > 1)
  {
    const char *file_name = argv[1];
    const char *file_content = file_content_read(file_name);
    if (!file_content)
    {
      printf("\n\033[1;31m    Error: could not read file    \n\033[0m");
      return EXIT_FAILURE;
    }
    printf("\nPart one: Sum <- %d\n", part_one(file_content));
    printf("\nPart two: Sum <- %d\n", part_two(file_content));

    free((void *)file_content);
    return EXIT_SUCCESS;
  }
  else
  {
    printf("\n\033[1;31m    Error: no file specified    \n\033[0m");
    return EXIT_FAILURE;
  }
}
