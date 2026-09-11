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






