# How do I add a submodule?

Adding a submodule inserts as a folder with the submodule's contents. Make sure, that there is no folder with the exactly the same name as the submodule you want to insert.

Then type `git submodule add <uri with the submodule> [<folder-name>]`

If you want your users to push changes to this submodule, then make sure the `uri with the submodule` points to the read/write place. In case of github prefer links in the form of `git@github.com:adamryczkowski/gridtools.git` rather than `https://github.com/adamryczkowski/gridtools.git`. 

# How do I remove a submodule?

Via the page *[Git Submodule Tutorial][1]*:

**To remove a submodule you need to:**

   1. Delete the relevant section from the `.gitmodules` file.
   1. Stage the `.gitmodules` changes `git add .gitmodules`
   1. Delete the relevant section from `.git/config`.
   1. Run `git rm --cached path_to_submodule` (no trailing slash).
   1. Run `rm -rf .git/modules/path_to_submodule`
   1. Commit `git commit -m "Removed submodule <name>"`
   1. Delete the now untracked submodule files<br/>`rm -rf path_to_submodule`

  [1]: https://git.wiki.kernel.org/index.php/GitSubmoduleTutorial#Removal


**How to write a git commit message

![How not to write a git commit messages](xkcd1296.png)

https://chris.beams.io/posts/git-commit/



1.    Separate subject from body with a blank line
2.    Limit the subject line to 50 characters
3.    Capitalize the subject line
4.    Do not end the subject line with a period
5.    Use the imperative mood in the subject line
6.    Wrap the body at 72 characters
7.    Use the body to explain what and why vs. how


A properly formed Git commit subject line should always be able to complete the following sentence:

>    If applied, this commit will your subject line here

For example:

>    If applied, this commit will refactor subsystem X for readability
>    If applied, this commit will update getting started documentation
>    If applied, this commit will remove deprecated methods
>    If applied, this commit will release version 1.0.0
>    If applied, this commit will merge pull request #123 from user/branch

