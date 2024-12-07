import { readFile } from "../utils.ts";

type Sum = (enabledMuls: number[]) => number;
const sum: Sum = (enabledMuls) =>
  enabledMuls.reduce((sum: number, product: number) => sum + product, 0);

type PartOne = (input: string) => number;
const partOne: PartOne = (input) => {
  const instructions = input.trim().matchAll(/mul\((\d{1,3}),(\d{1,3})\)/g);
  const enabledMuls = [...instructions].map(([_, ...args]: RegExpExecArray) => {
    const [X, Y] = args.map(Number);
    return X * Y;
  });

  return sum(enabledMuls);
};

type MemoryState = {
  enabledMuls: number[];
  isMulEnabled: boolean;
};
type InstructionHandlers = Record<
  string,
  (enabledMuls: number[]) => MemoryState
>;
const instructionHandlers: InstructionHandlers = {
  "don't()": (enabledMuls: number[]): MemoryState => ({
    enabledMuls,
    isMulEnabled: false,
  }),
  "do()": (enabledMuls: number[]): MemoryState => ({
    enabledMuls,
    isMulEnabled: true,
  }),
};

type PartTwo = (input: string) => number;
const partTwo: PartTwo = (input) => {
  const instructions = input
    .trim()
    .matchAll(/do\(\)|don't\(\)|mul\((\d{1,3}),(\d{1,3})\)/g);

  const { enabledMuls } = [...instructions].reduce<MemoryState>(
    (
      { enabledMuls, isMulEnabled }: MemoryState,
      [instructionMatch, ...args]: RegExpExecArray,
    ) => {
      const handler = instructionHandlers[instructionMatch];

      if (handler !== undefined) {
        return handler(enabledMuls);
      } else if (isMulEnabled) {
        const [X, Y] = args.map(Number);
        enabledMuls.push(X * Y);
      }
      return { enabledMuls, isMulEnabled };
    },
    { enabledMuls: [], isMulEnabled: true },
  );

  return sum(enabledMuls);
};

const FILE_NAME = "input.txt";
const input = await readFile(FILE_NAME);

console.log("Part One: ", partOne(input));
console.log("Part Two: ", partTwo(input));
