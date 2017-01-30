Here will be a simple reference of all commonly used flows with commands. Because not all git-flow commands are starting with the actual "`git flow`" and might be difficult to remember.

We will use **git flow** for code. Unless we use any form of proofreading, I am not really convinced to keep git flow for documentation, but I vote to keep it nonetheless, just for sake of uniformity. 

I think this part of the docs will be based on this Youtube tutorial: `SChuTnuBYMg`. 


# Q&A

What is the difference between `git flow feature` and `git flow hotfix`? 

> Hotfix is based on master, release is based on develop

How to compare all the changes between two arbitrary commits?

> `git difftool --tool=meld --dir-diff bef7 f2e18142aad`


