#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <ctype.h>
#include "../util.h"

#define MAX_RED_CUBES 12
#define MAX_GREEN_CUBES 13
#define MAX_BLUE_CUBES 14

typedef enum RGBValue
{
  RED = 'r',
  GREEN = 'g',
  BLUE = 'b',
} RGBValue;
typedef struct CubeCount
{
  uint8_t red;
  uint8_t green;
  uint8_t blue;
} CubeCount;

const char *game_id_skip(const char game_str[static 1])
{
  return strchr(game_str, ':') + 1;
}

uint8_t game_id_get(const char game_str[static 1])
{
  uint8_t game_id = {0};
  while (*game_str != ':')
  {
    game_id = isdigit(*game_str)
                  ? ((game_id * 10) + to_digit(*game_str))
                  : game_id;
    game_str++;
  }
  return game_id;
}

bool is_valid_cubes(CubeCount cube_count[static 1])
{
  return cube_count->red <= MAX_RED_CUBES &&
         cube_count->green <= MAX_GREEN_CUBES &&
         cube_count->blue <= MAX_BLUE_CUBES;
}
uint8_t *cube_count_color_get(CubeCount *cube_count, RGBValue color)
{
  return (color == RED)     ? &(cube_count->red)
         : (color == GREEN) ? &(cube_count->green)
         : (color == BLUE)  ? &(cube_count->blue)
                            : NULL;
}
void cube_count_update(const char c, uint8_t *count, CubeCount cube_count[static 1])
{
  *count = isdigit(c)
               ? ((*count * 10) + to_digit(c))
               : *count;
  uint8_t *cube_count_color = cube_count_color_get(cube_count, c);
  if (cube_count_color == NULL)
  {
    return;
  }
  *cube_count_color += *count;
  *count ^= *count;
}

uint32_t part_one(const char file_content[static 1])
{
  SplitContent *splitted = split_file_content(file_content, '\n');
  uint32_t sum = {0};
  for (size_t i = 0; i < splitted->lines_count; i++)
  {
    uint8_t count = {0};
    CubeCount cube_count = {0};
    bool is_valid_game = true;
    const char *ptr = game_id_skip(splitted->lines[i]);
    do
    {
      cube_count_update(*ptr, &count, &cube_count);
      if (*ptr == ';' || *ptr == '\0')
      {
        is_valid_game &= is_valid_cubes(&cube_count);
        cube_count = (CubeCount){0};
      }
      ptr += 1;
    } while (is_valid_game && *ptr != '\0');
    sum = is_valid_game
              ? (sum + game_id_get(splitted->lines[i]))
              : sum;
  }
  split_content_destroy(splitted);
  return sum;
}

uint32_t part_two(const char file_content[static 1])
{
  SplitContent *splitted = split_file_content(file_content, '\n');
  uint32_t sum = {0};
  for (size_t i = 0; i < splitted->lines_count; i++)
  {
    CubeCount cube_count, min_cube_count;
    cube_count = min_cube_count = (CubeCount){0};
    const char *ptr = game_id_skip(splitted->lines[i]);
    uint8_t count = {0};
    do
    {
      cube_count_update(*ptr, &count, &cube_count);
      if (*ptr == ';' || *ptr == '\0')
      {
        min_cube_count = (CubeCount){
            .red = max(min_cube_count.red, cube_count.red),
            .green = max(min_cube_count.green, cube_count.green),
            .blue = max(min_cube_count.blue, cube_count.blue)};
        cube_count = (CubeCount){0};
      }
      ptr += 1;
    } while (*ptr != '\0');
    uint32_t product = (min_cube_count.red * min_cube_count.green * min_cube_count.blue);
    sum += product;
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
    printf("\n\033[1;31m   Error: No file specified   \033[0m\n");
    return EXIT_FAILURE;
  }
}
