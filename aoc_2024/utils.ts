export const readFile = async (filename: string): Promise<string> =>
  await Deno.readTextFile(filename);
