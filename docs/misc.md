# Installing nice font for coding

One of the best courier-like fonts to work is the `Consolas` from Windows 7. Its main point is that all glyphs are are well distinguished apart, like the family [`l`, `1`, `I`] and [`0`, `O`], so it is more difficult to do mistakes.  
All the fonts can be copied from Windows itself, or from e.g. the free PowerPointViewer. They are included in the `MS truetype fonts.zip`.

 
If instalation of the *older* Windows fonts from the package `ttf-mscorefonts-installer` fails due to 404 File not found error, try this script: 

```
TMP=`mktemp -d`   
cd $TMP
grep Url: /usr/share/package-data-downloads/ttf-mscorefonts-installer | awk '{print $2}' | xargs -n 1 wget  
sudo /usr/lib/msttcorefonts/update-ms-fonts $TMP/*  
cd -  
rm -rf $TMP
```

# The dark theme

If you prefer to use the dark theme in Ubuntu try the **Arc-Dark**. It can be installed as `arc-theme` package from ppa 
`sudo add-apt-repository ppa:noobslab/themes`. Homepage: http://www.noobslab.com/2016/06/arc-theme-light-dark-versions-and-icons.html


You activate it with 
```
gsettings set org.gnome.desktop.interface gtk-theme 'Arc-Dark'
```

and return to the default theme with
```
gsettings set org.gnome.desktop.interface gtk-theme 'Ambiance'
```

## meld
You would need to replace the theme with `cat /usr/share/meld/meld_black.css > /usr/share/meld/meld.css`. The meld_black.css is included. For this command to work change the permission on the meld.css itself.

# Treasuring your bash history
Many if not most of your programming tasks are trigerred by a specific shell command. Theese commands can be complicated and are easy to forget. That's why there is a special Bash functionality, the *command history*. Unfortunately, (maybe for performance reasons) in the default Ubuntu instalation the commands typed in the shell can easily evade being stored in history if you use more than one terminal simultaneously. This happens because each terminal window keeps its own copy of the command history read from the `~/.bash_history` *at the moment of time when it was spawn*. When you later close the terminal, it saves its own version of the history (with your new commands appended to it) blindly *overwriting the `~/.bash_history`*. So when you use two or more terminals at once, you will have few modified command history versions, one for each terminal. At the end of the day, only one command history version will prevail - the that came from the terminal which was closed the last.

I found a good remedy for this problem in forcing each terminal to synchronize its command history with the `~/.bash_history` *after each command execution*. You can do this, if you get the bash to execute the following code snippet at the beginning of each interactive session:


```{bash}
export HISTSIZE=100000
export HISTCONTROL=erasedups
shopt -s histappend
PROMPT_COMMAND="history -a; history -n"
HISTCONTROL=ignoreboth
```

## How to configure the bash to execute code?

Bash can be invoked in thee modes: *non-interactive*, *interactive login shell* and *interactive non-login shell*. (Non-interactive mode is used when you execute a bash script). Bash is in login shell mode if this is the first process spawned under your own UID (userid), just after login (e.g. in virtual terminal or by `ssh`). When you are already logged-in and invoke a Bash' command prompt you use a non-login shell. The difference is important, because in each mode a different set of startup files are being executed. 

The only place where you can put a code snipped and expect it will be run both when you login via ssh, or invoke it interactively by clicking the "Terminal" icon is `~/.bashrc` or `/etc/bash.bashrc`. In either file just append the code snipped or better yet: a code that executes it and possibly many other usefull bash code:

```{bash}
pushd "/etc/profile.d/nonlogin" >/dev/null && . "/etc/profile.d/nonlogin/.bash-profile-launcher.sh"; popd >/dev/null
```

and in `/etc/profile.d/nonlogin/.bash-profile-launcher.sh`:
```{bash}
#!/bin/bash

for i in *.sh; do
 if [ -r $i ] && [[ "$i" != ".bash-profile-launcher.sh" ]]; then
   . $i
 fi
done
unset i
```
(Yes, theoretically you can get a *non-interactive login shell*, but that is rarely used. )

sources: [http://unix.stackexchange.com/q/187369/17765] and [http://unix.stackexchange.com/a/50667/17765]  

# Easy preview of the command's exit code or the execution time in the shell prompt

You can this and a lot more if you tweak the Bash prompt. The most effective way of doing it is by re-using someone else's work. I suggest to take a look at the [liquidprompt](https://github.com/nojhan/liquidprompt). It is already available in Ubuntu's repository. After instalation, activate it for the current user by executing `liquidprompt_activate`. 


