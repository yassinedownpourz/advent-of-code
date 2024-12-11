import { readFile } from "../utils.ts";

type BoundsCheck = (i: number, j: number) => boolean;
const arrayBoundsCheck =
  (arr: readonly string[]): BoundsCheck =>
  (i, j) =>
    i >= 0 && j >= 0 && i < arr.length && j < (arr[0]?.length ?? 0);

type Direction =
  | "UP"
  | "DOWN"
  | "LEFT"
  | "RIGHT"
  | "UP_RIGHT"
  | "UP_LEFT"
  | "DOWN_RIGHT"
  | "DOWN_LEFT";
type DirectionHandlerFunction = (
  isWithinBounds: BoundsCheck,
) => (i: number, j: number) => string;
type DirectionHandler = Record<Direction, DirectionHandlerFunction>;

type CreateDirectionHandler = (words: readonly string[]) => DirectionHandler;
const createDirectionHandler: CreateDirectionHandler = (words) => {
  type GetWordInDirection = (
    positions: readonly number[][],
    isWithinBounds: BoundsCheck,
  ) => string;
  const getWordInDirection: GetWordInDirection = (positions, isWithinBounds) =>
    positions.every(([i, j]: number[]) => isWithinBounds(i, j))
      ? positions.map(([i, j]: number[]) => words[i][j]).join("")
      : "";

  return {
    UP: (isWithinBounds) => (i, j) =>
      getWordInDirection(
        [
          [i, j],
          [i - 1, j],
          [i - 2, j],
          [i - 3, j],
        ],
        isWithinBounds,
      ),
    DOWN: (isWithinBounds) => (i, j) =>
      getWordInDirection(
        [
          [i, j],
          [i + 1, j],
          [i + 2, j],
          [i + 3, j],
        ],
        isWithinBounds,
      ),
    LEFT: (isWithinBounds) => (i, j) =>
      getWordInDirection(
        [
          [i, j],
          [i, j - 1],
          [i, j - 2],
          [i, j - 3],
        ],
        isWithinBounds,
      ),
    RIGHT: (isWithinBounds) => (i, j) =>
      getWordInDirection(
        [
          [i, j],
          [i, j + 1],
          [i, j + 2],
          [i, j + 3],
        ],
        isWithinBounds,
      ),
    UP_RIGHT: (isWithinBounds) => (i, j) =>
      getWordInDirection(
        [
          [i, j],
          [i - 1, j + 1],
          [i - 2, j + 2],
          [i - 3, j + 3],
        ],
        isWithinBounds,
      ),
    UP_LEFT: (isWithinBounds) => (i, j) =>
      getWordInDirection(
        [
          [i, j],
          [i - 1, j - 1],
          [i - 2, j - 2],
          [i - 3, j - 3],
        ],
        isWithinBounds,
      ),
    DOWN_RIGHT: (isWithinBounds) => (i, j) =>
      getWordInDirection(
        [
          [i, j],
          [i + 1, j + 1],
          [i + 2, j + 2],
          [i + 3, j + 3],
        ],
        isWithinBounds,
      ),
    DOWN_LEFT: (isWithinBounds) => (i, j) =>
      getWordInDirection(
        [
          [i, j],
          [i + 1, j - 1],
          [i + 2, j - 2],
          [i + 3, j - 3],
        ],
        isWithinBounds,
      ),
  } as const;
};

type PartOne = (input: string) => number;
const partOne: PartOne = (input) => {
  const wordSearch = input.trim().split("\n");
  const isWithinBounds = arrayBoundsCheck(wordSearch);
  const directionHandler = createDirectionHandler(wordSearch);
  return wordSearch.reduce(
    (xmasCount: number, line: string, lineIndex: number) => {
      const xmasMatch = [...line].reduce(
        (xmasMatch: number, _, charIndex: number) => {
          Object.entries(directionHandler).forEach(([_, handler]) => {
            const directionChecker = handler(isWithinBounds);
            const word = directionChecker(lineIndex, charIndex);
            xmasMatch += +(word === "XMAS");
          });
          return xmasMatch;
        },
        0,
      );
      xmasCount += xmasMatch;
      return xmasCount;
    },
    0,
  );
};

type PartTwo = (input: string) => number;
const partTwo: PartTwo = (input) => {
  const wordSearch = input.trim().split("\n");
  const isWithinBounds = arrayBoundsCheck(wordSearch);
  return wordSearch.reduce(
    (xmasCount: number, line: string, lineIndex: number) => {
      const xmasMatch = [...line].reduce(
        (xmasMatch: number, char: string, charIndex: number) => {
          if (char === "A") {
            const [upLeft, upRight, downLeft, downRight] = [
              isWithinBounds(lineIndex - 1, charIndex - 1)
                ? wordSearch[lineIndex - 1][charIndex - 1]
                : "",
              isWithinBounds(lineIndex - 1, charIndex + 1)
                ? wordSearch[lineIndex - 1][charIndex + 1]
                : "",
              isWithinBounds(lineIndex + 1, charIndex - 1)
                ? wordSearch[lineIndex + 1][charIndex - 1]
                : "",
              isWithinBounds(lineIndex + 1, charIndex + 1)
                ? wordSearch[lineIndex + 1][charIndex + 1]
                : "",
            ];
            const upLeftToDownRight =
              (upLeft === "S" && downRight === "M") ||
              (upLeft === "M" && downRight === "S");
            const downLeftToUpRight =
              (downLeft === "S" && upRight === "M") ||
              (downLeft === "M" && upRight === "S");

            xmasMatch += +(upLeftToDownRight && downLeftToUpRight);
          }
          return xmasMatch;
        },
        0,
      );
      xmasCount += xmasMatch;
      return xmasCount;
    },
    0,
  );
};

const FILE_NAME = "input.txt";
const input = await readFile(FILE_NAME);

console.log("Part One: ", partOne(input));
console.log("Part Two: ", partTwo(input));
