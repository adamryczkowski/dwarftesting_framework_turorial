First clone the repository. For that you'd need a git, preferrably a modern one

```{bash}
sudo add-apt-repository ppa:git-core/ppa
sudo apt-get update
sudo apt-get install git
```

Then you can clone everything with 
```{bash}
git clone --recursive git@git.imgw.ad:aryczkowski/propoze.git all
```

"`all`" is the name of the _superproject_, i.e. the folder that encapsulates all project we build.

## Protip: optimize for smaller download with `--shallow-submodules`

If you have a git 2.90 or newer (not default in Ubuntu 16.04) then you can add `--shallow-submodules` like this: 

```{bash}
git clone --recursive --shallow-submodules git@git.imgw.ad:aryczkowski/propoze.git all
```

As of December 2016, it will save you around 15MB of disk space by not storing dead (unused) objects present in git history.


