import { readFile } from "../utils.ts";

type ParseLine = (line: string) => number[];
const parseLine: ParseLine = (line) => line.split("   ").map(Number);

type ParseInput = (input: string) => number[][];
const parseInput: ParseInput = (input) =>
  input
    .trim()
    .split("\n")
    .reduce(
      ([leftList, rightList]: number[][], line: string) => {
        const [leftID, rightID] = parseLine(line);
        leftList.push(leftID);
        rightList.push(rightID);
        return [leftList, rightList];
      },
      [[], []],
    );

type Sum = (arr: number[]) => number;
const sum: Sum = (arr) =>
  arr.reduce((acc: number, curr: number) => acc + curr, 0);

type PartOne = (input: string) => number;
const partOne: PartOne = (input) => {
  const [leftList, rightList] = parseInput(input).map((list: number[]) =>
    list.sort((a: number, b: number) => a - b),
  );

  const pairListDistance = rightList.map((ID: number, index: number) =>
    Math.abs(ID - leftList[index]),
  );

  return sum(pairListDistance);
};

type PartTwo = (input: string) => number;
const partTwo: PartTwo = (input) => {
  const [leftList, rightList] = parseInput(input);

  const occurrenceCount = leftList.reduce(
    (acc: Map<number, number>, id: number) => {
      acc.set(id, (acc.get(id) ?? 0) + 1);
      return acc;
    },
    new Map(),
  );

  const similarityScore = rightList.map(
    (id: number) => id * (occurrenceCount.get(id) ?? 0),
  );

  return sum(similarityScore);
};

const FILE_NAME = "input.txt";
const input = await readFile(FILE_NAME);

console.log("Part One: ", partOne(input));
console.log("Part Two: ", partTwo(input));
