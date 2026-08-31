# Mandatory read-only policy

This project is strictly read-only for Codex.

Never create, edit, overwrite, rename, move, delete, generate, format, or run any command that can modify project files.

This includes, without exception:
- no code edits;
- no new files;
- no deletion of files;
- no STM32CubeMX code generation;
- no build, clean, flash, debug, formatter, or migration;
- no changing `.ioc`, linker scripts, CubeMX files, IDE configuration, or generated code.

Your only permitted actions are read-only inspection and explanation.

When I ask for an implementation or a fix:
1. Inspect the relevant existing files read-only.
2. Reply only in chat.
3. State the exact file path and current line number/range.
4. Show the exact code to insert, replace, or remove.
5. State precisely whether the code goes before or after a specific existing line.
6. Explain briefly why it is needed.
7. Never apply the change yourself, even if I say “implement it”.
8. If line numbers are uncertain, inspect the file first; never guess.

Preserve STM32CubeMX user-code sections. Do not propose edits outside `/* USER CODE BEGIN */` / `/* USER CODE END */` unless explicitly explaining why this is unavoidable.

Before proposing code, inspect the whole relevant project context, including `.ioc`, both core folders, generated configuration, and existing application code. Report configuration mismatches, but do not fix them.




# Project task

Target board: STM32H747I-DISCO with STM32H747XI dual-core MCU.

This is an interview task. The firmware must, every 5 seconds:

1. Generate exactly five random integer numbers.
2. Each number must be in the inclusive range 256 to 511.
3. Create one text line in this exact format:

   `MM:SS N1, N2, N3, N4, N5`

   Example:
   `00:00 290, 401, 263, 510, 337`

4. Append the line followed by a newline to the microSD-card file:
   `file01.txt`
5. Display the same line on the board’s built-in LCD.
6. Send the same line through UART8.

The timer starts at `00:00` when the firmware begins running. A new line is produced every 5 seconds: `00:00`, `00:05`, `00:10`, and so on.

