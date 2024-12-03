#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include "../util.h"

#define GROWTH_FACTOR 2
#define ADJACENT_PART_NUMBERS 2
#define SEGMENT_TYPE_SIZE 3

typedef struct PartNumber
{
  uint16_t number;
  bool is_initialized;
} PartNumber;
typedef struct PartNumberList
{
  size_t size;
  size_t capacity;
  PartNumber part_numbers[];
} PartNumberList;
typedef enum SegmentType
{
  TOP,
  MIDDLE,
  BOTTOM,
} SegmentType;
typedef struct SegmentFlags
{
  bool has_left : 1;
  bool has_center : 1;
  bool has_right : 1;
} SegmentFlags;
typedef enum
{
  LEFT = -1,
  CENTER,
  RIGHT,
  NONE
} Direction;

PartNumberList *part_number_list_init(size_t initial_capacity)
{
  PartNumberList *part_number_list =
      malloc(sizeof(PartNumberList) + initial_capacity * sizeof(PartNumber));
  if (!part_number_list)
  {
    printf("\n\033[1;31m    Error: could not allocate memory    \n\033[0m");
    return NULL;
  }
  *part_number_list = (PartNumberList){
      .capacity = initial_capacity,
  };
  return part_number_list;
}

void part_number_list_push(PartNumberList *part_number_list, PartNumber part_number)
{
  if (part_number_list->size == part_number_list->capacity)
  {
    part_number_list->capacity *= GROWTH_FACTOR;
    PartNumberList *new_part_number_list =
        realloc(part_number_list,
                sizeof(PartNumberList) + part_number_list->capacity * sizeof(PartNumber));
    if (!new_part_number_list)
    {
      printf("\n\033[1;31m    Error: could not allocate memory    \n\033[0m");
      return;
    }
    part_number_list = new_part_number_list;
  }
  part_number_list->part_numbers[part_number_list->size++] = part_number;
}

void part_number_list_destroy(PartNumberList *part_number_list)
{
  free(part_number_list);
}

const char *find_start_of_number(const char string_segment[static 1])
{
  const char *ptr = string_segment + LEFT;
  while (isdigit(*ptr))
  {
    ptr -= 1;
  }
  return (ptr + 1);
}

PartNumber part_number_create(const char *string_segment, Direction direction)
{
  return (direction == LEFT)
             ? (PartNumber){
                   .number = atoi(find_start_of_number(string_segment)),
                   .is_initialized = true,
               }
         : (direction == CENTER) ? (PartNumber){
                                       .number = atoi(string_segment + CENTER),
                                       .is_initialized = true,
                                   }
         : (direction == RIGHT) ? (PartNumber){
                                      .number = atoi(string_segment + RIGHT),
                                      .is_initialized = true,
                                  }
                                : (PartNumber){0};
}
Direction direction_get(const int16_t segment_flags_bits)
{
  return ((segment_flags_bits == 0b001) ||
          (segment_flags_bits == 0b101))
             ? RIGHT
         : ((segment_flags_bits == 0b010) ||
            (segment_flags_bits == 0b011))
             ? CENTER
         : ((segment_flags_bits == 0b100) ||
            (segment_flags_bits == 0b110) ||
            (segment_flags_bits == 0b111))
             ? LEFT
             : NONE;
}
bool is_symbol(const char c)
{
  return c != '.' && ispunct(c);
}

bool is_gear(const char c)
{
  return c == '*';
}

void find_adjacent_part_numbers(
    const char string_segment[static 1], PartNumberList *part_number_list)
{
  if (string_segment == NULL || part_number_list == NULL)
  {
    return;
  }
  SegmentFlags segment_flags = {
      .has_left = *(string_segment + LEFT) != '\0' && isdigit(*(string_segment + LEFT)),
      .has_center = isdigit(*string_segment + CENTER),
      .has_right = *(string_segment + RIGHT) != '\0' && isdigit(*(string_segment + RIGHT)),
  };
  const int16_t segment_flags_bits = (segment_flags.has_left << 2) |
                                     (segment_flags.has_center << 1) |
                                     (segment_flags.has_right << 0);
  Direction primary_direction = direction_get(segment_flags_bits);
  PartNumber part_number = {0};
  if (primary_direction != NONE)
  {
    part_number = part_number_create(string_segment, primary_direction);
    part_number_list_push(part_number_list, part_number);
    if (segment_flags_bits == 0b101)
    {
      part_number = part_number_create(string_segment, LEFT);
      part_number_list_push(part_number_list, part_number);
    }
  }
}

PartNumberList *find_adjacent_segment_part_numbers(
    const char *string_segment[static SEGMENT_TYPE_SIZE], const uint16_t max_adjacent_part_numbers)
{
  PartNumberList *part_number_list = part_number_list_init(ADJACENT_PART_NUMBERS);
  for (size_t i = TOP;
       i < SEGMENT_TYPE_SIZE &&
       part_number_list->size <= max_adjacent_part_numbers;
       i++)
  {
    find_adjacent_part_numbers(string_segment[i], part_number_list);
  }
  return part_number_list;
}

uint64_t part_one(const char file_content[static 1])
{
  SplitContent *splitted = split_file_content(file_content, '\n');
  uint64_t sum = {0};
  for (size_t i = 0; i < splitted->lines_count; i++)
  {
    for (const char *ptr = splitted->lines[i]; *ptr != '\0'; ptr++)
    {
      if (is_symbol(*ptr))
      {
        const ptrdiff_t offset = (ptr - splitted->lines[i]);
        const char *string_segment[SEGMENT_TYPE_SIZE] = {
            [TOP] = (i == 0)
                        ? NULL
                        : (splitted->lines[i - 1] + offset),
            [MIDDLE] = ptr,
            [BOTTOM] = (i == (splitted->lines_count - 1))
                           ? NULL
                           : (splitted->lines[i + 1] + offset),
        };
        PartNumberList *part_number_list =
            find_adjacent_segment_part_numbers(
                string_segment, ADJACENT_PART_NUMBERS * SEGMENT_TYPE_SIZE);
        for (size_t j = 0; j < part_number_list->size; j++)
        {
          sum += part_number_list->part_numbers[j].number;
        }
        part_number_list_destroy(part_number_list);
      }
    }
  }
  split_content_destroy(splitted);
  return sum;
}

uint64_t part_two(const char file_content[static 1])
{
  SplitContent *splitted = split_file_content(file_content, '\n');
  uint64_t sum = {0};
  for (size_t i = 0; i < splitted->lines_count; i++)
  {
    for (const char *ptr = splitted->lines[i]; *ptr != '\0'; ptr++)
    {
      if (is_gear(*ptr))
      {
        const ptrdiff_t offset = (ptr - splitted->lines[i]);
        const char *string_segment[SEGMENT_TYPE_SIZE] = {
            [TOP] = (i == 0)
                        ? NULL
                        : (splitted->lines[i - 1] + offset),
            [MIDDLE] = ptr,
            [BOTTOM] = (i == (splitted->lines_count - 1))
                           ? NULL
                           : (splitted->lines[i + 1] + offset),
        };
        PartNumberList *part_number_list =
            find_adjacent_segment_part_numbers(string_segment, ADJACENT_PART_NUMBERS);
        if (part_number_list->size == ADJACENT_PART_NUMBERS)
        {
          uint64_t gear_ratio = {1};
          for (size_t j = 0; j < ADJACENT_PART_NUMBERS; j++)
          {
            gear_ratio *= part_number_list->part_numbers[j].number;
          }
          sum += gear_ratio;
          gear_ratio = 1;
        }
        part_number_list_destroy(part_number_list);
      }
    }
  }
  split_content_destroy(splitted);
  return sum;
}

int main(int argc, char *argv[])
{
  if (argc > 1)
  {
    const char *file_name = argv[1];
    const char *file_content = file_content_read(file_name);
    printf("\nPart one: Sum <- %ld\n", part_one(file_content));
    printf("\nPart two: Sum <- %ld\n", part_two(file_content));

    free((void *)file_content);
    return EXIT_SUCCESS;
  }
  else
  {
    printf("\033[0;31m Error: no file specified\033[0m\n");
    return EXIT_FAILURE;
  }
}
