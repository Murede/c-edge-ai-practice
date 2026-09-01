# Git and GitHub Command Reference

Use these commands from the project folder:

```powershell
Set-Location "C:\Users\Murede\OpenClaw-System\workspaces\embedded\projects\c-edge-ai-practice"
```

Before running a Git command, `git rev-parse --show-toplevel` should point to `c-edge-ai-practice`, not the broader Embedded workspace.

## Daily commands

### 1. Start the session

```powershell
git status
git branch --show-current
git pull --ff-only
```

- `git status` shows modified, staged, and untracked files.
- `git branch --show-current` confirms which branch you are using.
- `git pull --ff-only` downloads remote work without creating an unexpected merge commit.

### 2. Inspect your changes

```powershell
git status
git diff
```

Read the diff before staging anything. Confirm that it contains only intentional work and no passwords, tokens, generated binaries, or unrelated files.

### 3. Stage changes deliberately

Recommended:

```powershell
git add -p
```

`git add -p` presents each change and lets you decide whether to stage it.

To stage one complete file:

```powershell
git add src/motor_samples.c
```

To stage several known files:

```powershell
git add include/motor_samples.h src/motor_samples.c tests/test_motor_samples.c
```

Avoid using `git add .` until you have checked `git status` and understand every file it will include.

### 4. Review the staged commit

```powershell
git status
git diff --cached
```

`git diff --cached` shows exactly what the next commit will contain.

### 5. Commit one logical change

```powershell
git commit -m "Add bounded RPM averaging function"
```

Good commit messages state what changed:

```text
Add fixed-capacity RPM sample buffer
Test invalid motor sample counts
Fix off-by-one error in sample loop
Document Week 1 build commands
```

Avoid vague messages such as `updates`, `stuff`, or `final`.

### 6. Push your commit

```powershell
git push
```

### 7. Confirm a clean stopping point

```powershell
git status
git log --oneline --decorate -5
```

Record the latest passing commit in the daily log:

```powershell
git rev-parse --short HEAD
```

## First-time repository setup

Run these commands only when turning this folder into its own repository.

```powershell
git init -b main
git rev-parse --show-toplevel
git config --global user.name "Murede Adetiba"
git config --global user.email "YOUR_GITHUB_EMAIL"
```

Check the configuration:

```powershell
git config --global --list
```

After adding and reviewing the initial project files:

```powershell
git add README.md ROADMAP.md WEEK_01.md PROGRESS.md GIT_COMMANDS.md .gitignore
git add daily-logs include notes src tests
git diff --cached
git commit -m "Initialize C and Edge AI practice workspace"
```

## Connect to GitHub

Create an empty GitHub repository without a generated README, `.gitignore`, or license. Then connect it using the URL GitHub provides:

```powershell
git remote add origin https://github.com/Murede/c-edge-ai-practice.git
git remote -v
git push -u origin main
```

Check `git remote -v` carefully before the first push. After `-u` establishes the upstream connection, use `git push` for normal pushes.

## Commands to know

### Repository and file state

```powershell
git status
git rev-parse --show-toplevel
git branch --show-current
git diff
git diff --cached
git log --oneline --graph --decorate --all
```

### Remote information

```powershell
git remote -v
git fetch origin
git branch -vv
```

- `git fetch origin` downloads remote information without changing your working files.
- `git branch -vv` shows the upstream branch and whether the local branch is ahead or behind.

### View previous versions

```powershell
git show HEAD
git show HEAD:src/motor_samples.c
git log --oneline -- src/motor_samples.c
```

These commands inspect history without changing any files.

## Branch workflow

Create a branch for a focused feature:

```powershell
git switch main
git pull --ff-only
git switch -c feature/rpm-sample-buffer
```

Work, test, commit, and publish the branch:

```powershell
git add -p
git diff --cached
git commit -m "Add fixed-capacity RPM sample buffer"
git push -u origin feature/rpm-sample-buffer
```

Merge completed work into `main`:

```powershell
git switch main
git pull --ff-only
git merge --no-ff feature/rpm-sample-buffer
git push
```

After confirming the merge, delete the completed local branch:

```powershell
git branch -d feature/rpm-sample-buffer
```

If you merged through a GitHub pull request, update local `main` with:

```powershell
git switch main
git pull --ff-only
```

## Safe recovery commands

### Unstage a file but keep its edits

```powershell
git restore --staged PATH_TO_FILE
```

Example:

```powershell
git restore --staged src/motor_samples.c
```

### Discard unstaged edits to one file

```powershell
git diff -- PATH_TO_FILE
git restore PATH_TO_FILE
```

Inspect the diff first. `git restore` discards the unstaged edits in that file.

### Temporarily store unfinished tracked work

```powershell
git stash push -m "unfinished RPM test work"
git stash list
git stash show -p stash@{0}
git stash pop
```

In PowerShell, quote the stash name if parsing causes a problem:

```powershell
git stash show -p "stash@{0}"
```

### Correct the latest local commit message

Use this only before pushing:

```powershell
git commit --amend -m "Corrected commit message"
```

### Revert a pushed commit safely

```powershell
git log --oneline
git revert COMMIT_ID
git push
```

`git revert` creates a new commit that reverses the selected commit. It preserves shared history.

## Merge conflicts

If a pull or merge reports a conflict:

```powershell
git status
```

Open each conflicted file and resolve the marked sections. Then test the result and run:

```powershell
git add PATH_TO_RESOLVED_FILE
git status
git commit
```

If you are uncertain, stop before committing and ask for help. Do not randomly delete conflict markers or choose a side without understanding the code.

## Commands to treat cautiously

Do not use these casually while learning:

```text
git reset --hard
git clean -fd
git push --force
git branch -D
```

They can discard work, delete untracked files, or rewrite shared history. Prefer `git restore --staged`, `git stash`, `git revert`, and ordinary non-force pushes.

## Daily quick reference

```powershell
# Start
git status
git branch --show-current
git pull --ff-only

# After editing and testing
git diff
git add -p
git diff --cached
git commit -m "Describe one completed change"
git push

# Finish
git status
git rev-parse --short HEAD
```
