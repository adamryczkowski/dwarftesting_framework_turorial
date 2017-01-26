The directory is cloned. You can build it.

Now some time have passed and you've learned, that the repository's contents on you disk is no longer up to date. It is time to pull.

Normally you would simply type `git pull` in the directory. Unfortunately this repository consists of git submodules, and they make pulling way more complicated.

The best practice is to define a git command **pullall* that does `git pull && git submodule update --init --recursive` like this

```{bash}
git config --global alias.pullall '!git submodule update --init --recursive --remote'

git config --global alias.pullall '!git pull && git submodule update --init --recursive'
``` 



After that, to successfully pull the latest image of the repository as seen by last developer who pushed his commit is to do `git pullall` in the root superproject, i.e.

```{bash}
cd all
git pullall
```

