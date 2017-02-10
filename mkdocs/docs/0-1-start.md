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


# Instalation script on Ubuntu 16.04

This script was tested on fresh LXC container with Ubuntu 16.04 on 16.I.2017. It should work on all Ubuntu 16.04 systems.

First install the required packages:

## The minimal dependencies

```{bash}
sudo apt update
sudo apt install cmake git build-essential gfortran libboost-dev libboost-system1.58-dev libboost-timer1.58-dev libboost-chrono1.58-dev libboost-program-options1.58-dev mkdocs python-minimal libmpich-dev libopenmpi-dev

```

## The recommended dependencies

```{bash}

sudo add-apt-repository ppa:git-core/ppa
sudo apt update
sudo apt dist-upgrade #upgrades git
sudo apt install pandoc python3-pip cmake-curses-gui texlive-latex-base texlive-fonts-recommended
sudo dpkg-reconfigure locales
export LC_ALL=C; pip3 install mkdocs-pandoc
```

Recommended dependencies include newer version of git, 2.11 as of writing this manual. You may want it to pull recursively the submodules and for the `--shallow-submodules` switch. Other dependency are programs that generate the LaTeX documentation.

## Pulling the source first time and building it.

### Getting all the required certificates for various git repositories

Our project requires that you are registered, granted read permissions and preferrably register your public ssh key with the following sites:

* github.com for read-only access to private repository [github.com/GridTools/gridtools](https://github.com/GridTools/gridtools)
* bitbucket.org for read and write access to [bitbucket.org/havogt/eulag_dycore.git](https://bitbucket.org/havogt/eulag_dycore.git) with testing framework
* git.imgw.ad for all the rest. This is our home server, accessed via VPN.

#### which ssh key algorithm to use? 

The answer will change in time. As of writing the document (year 2016) the recommendation is to ditch DSA altogether; use RSA with at least 2048 bit key length or use ECDSA or ED25519 with a little preference on the latter, because it is developed independently from the NIST and is free from patent infringement threats and possible NSA backdoors (arguably).


Please remove the `--shallow-submodules` command line switch if your git is older than version 2.11.
```{bash}
git clone --shallow-submodules --recursive git@git.imgw.ad:aryczkowski/propoze.git all
mkdir all/build
cd all/build
cmake ..
ccmake .. #Optional. You can set PDF to 1, to make PDF documentation as well, if you've followed the recommended dependencies.
make -j 8
cmake .. #Yes, you need to run cmake & make twice...
make -j 8 
```

