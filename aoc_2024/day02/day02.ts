import { readFile } from "../utils.ts";

type ParseReports = (input: string) => number[][];
const parseReports: ParseReports = (input) =>
  input
    .trim()
    .split("\n")
    .map((report: string) => report.split(" ").map(Number));

type LevelDifference = (a: number, b: number) => number;
const levelDifference: LevelDifference = (a, b) => a - b;

type PartOne = (input: string) => number;
const partOne: PartOne = (input) => {
  const reports = parseReports(input);

  const safeReports = reports
    .map((report: number[]) =>
      report.slice(1).map((level: number, index: number) => {
        const [currLevel, adjLevel] = [level, report[index]];
        return levelDifference(currLevel, adjLevel);
      }),
    )
    .filter((report: number[]) => {
      const allLevelsIncreasing = report.every((levelDifference: number) =>
        new Set([1, 2, 3]).has(levelDifference),
      );
      const allLevelsDecreasing = report.every((levelDifference: number) =>
        new Set([-1, -2, -3]).has(levelDifference),
      );
      return allLevelsIncreasing || allLevelsDecreasing;
    });

  return safeReports.length;
};

type PartTwo = (input: string) => number;
const partTwo: PartTwo = (input) => {
  const reports = parseReports(input);

  const safeReports = reports.filter((report: number[]) =>
    report.some((_, problemDampener: number) => {
      const dampenedReport = report.filter(
        (_, levelIndex) => problemDampener !== levelIndex,
      );

      const tolerableLevels = dampenedReport
        .slice(1)
        .map((level: number, index: number) => {
          const [currLevel, adjLevel] = [level, dampenedReport[index]];
          return levelDifference(currLevel, adjLevel);
        });
      const allLevelsIncreasing = tolerableLevels.every(
        (levelDifference: number) => new Set([1, 2, 3]).has(levelDifference),
      );
      const allLevelsDecreasing = tolerableLevels.every(
        (levelDifference: number) => new Set([-1, -2, -3]).has(levelDifference),
      );
      return allLevelsIncreasing || allLevelsDecreasing;
    }),
  );

  return safeReports.length;
};

const FILE_NAME = "input.txt";
const input = await readFile(FILE_NAME);

console.log("Part One: ", partOne(input));
console.log("Part Two: ", partTwo(input));
