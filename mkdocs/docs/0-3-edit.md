At the moment we support the following IDEs:

For the Fortran code we support Eclipse Mars (*not the current one*) with the [Photran plugin](http://www.eclipse.org/photran/).

For the C++ we haven't decided yet, but it may be either Eclipse with the CDT or QT Designer.

# Fortran code with Eclipse

## Installing the Eclipse Mars with Photran 

### Eclipse installation.

Thankfully there is a cool [Eclipse Oomph Installer](https://wiki.eclipse.org/Eclipse_Installer). It requires Java, so make sure you have any version of Java installed (the OpenJDK will do: `sudo apt install openjdk-jre`)

Download the installer somewhere, extract it. 

1. Run the only executable file `eclipse-installer/eclipse-inst`: ![Installer](oomph-install-default.png "Oomph installer")


2. Change the installer into an advanced mode by clicking the hamburger menu on the top right
![Oomph hambuerger menu](oomph-hamburger-menu.png "Oomph hambuerger menu")


3. Select the **eclipse platform** in the installer and **Mars** Product Version with matching 32bit/64bit to your OS. Bundle pool is a place on you hard drive that caches instalation files, so they will not get downloaded again if you re-run your installer again with the same or simmilar product.   
![Installer, advanced mode](oomph-install-advanced.png)

4. Click next
![Installer, step 2](oomph-install-step2.png)

5. Verify, that the `Root install folder` point somewhere where you want the software to be installed and where you have the `write` permission. I suggest not to use the home directory if you use Home directory encryption (using ecryptfs, usually set up by the Ubuntu installer), because you probably don't want the Eclipse binaries to be encrypted. Encrypted files will not get deduplicated and there might be some issues with very long filenames. I suggest designating  the `/opt/eclipse` directory for this:
```{bash}
sudo mkdir -p /opt/eclipse
sudo chown $USER:$USER +x /opt/eclipse
```  
![Installer, step 3](oomph-install-step3.png)
6. Click Finish and wait a moment.
![Installer, the last step](oomph-install-step4.png)


### (Optional) Installing the nice shortcut for the Eclipse.

Because we have installed the Eclipse more-or-less manually we need to handle nice icon by ourselves. 

Edit a file `~/.local/share/applications/opt_eclipse.desktop` or `/usr/share/applications/opt_eclipse.desktop` (the latter one will require super user privileges) and add the following contents:

```
[Desktop Entry]
Type=Application
Name=Eclipse
Comment=Eclipse Mars Integrated Development Environment
Icon=/opt/eclipse/ide-mars/eclipse/icon.xpm
Exec=SWT_GTK3=0 /opt/eclipse/ide-mars/eclipse/eclipse
Terminal=false
Categories=Development;IDE;Java;
StartupWMClass=Eclipse
```


### Running the eclipse

This Eclipse is rather old and it does not know how to deal with a new GTK+ 3.0 that is default on Ubuntu. We need to make sure it will fall back to the GTK+ 2.0 by setting the environment variable `SWT_GTK3` to `0`. So, you either run Eclipse by 

```
export SWT_GTK3=0; /opt/eclipse/ide-mars/eclipse/eclipse
```

or by the desktop icon created in the previous step.

### Photran installation

We have installed the Eclipse Platform, which is a bare-bones distribution of Eclipse. All functionality is to be added via a system of plugins. We install the plugins in a quite non-intuitive place, under the `Help` menu. So select the Help menu, then the `Install New Software...`

![Help menu in Eclipse](eclipse-help-menu.png)

Choose the `Mars - http://download.eclipse.org/releases/mars` in the `Work with` edit box and wait a little until a list with all the available software for the Mars will get downloaded.

![Install dialog in Eclipse 1](eclipse-install1.png)

After a while type in `Photran` in the edit box and check the *Fortran Development Tools (Photran)* item in the tree below. Click **Next**.

![Install dialog in Eclipse 2](eclipse-install2.png)

Click **Next** again.

![Install dialog in Eclipse 3](eclipse-install3.png)

Accept the licence and click **Finish**

![Install dialog in Eclipse 4](eclipse-install3.png)

When the instalation finishes the Eclipse will ask you to restart. Do it.

![Restart dialog in Eclipse](eclipse-restart.png)

When the Eclipse is back on, it will present you the Photran's welcome screen:

![Photran's welcome screen](photran-welcome.png)

Now it is a time to tell the Eclipse to configure all views as recommended by the Photran team. Of course everything can be customized later on. Click on **Window** menu, then **Perspective**, then **Open Perspective** and **Other...**. The following window will appear:

![Select perspective](photran-new-perspective.png)

Now Eclipse will change its face a little bit:

![Photran's default perspective](photran-default1.png)

### Importing the Fortran code into Eclipse

Select the **File** menu, then **Import...** and in the window select **General->Existing Projects into Workspace...** and click **Next**:

![Eclipse import project window 1](eclipse-import-project1.png)

Select the **Browse...** button by the *Select root directory* text box and navigate into the folder `all/mpdata-eclipse` relative to the pulled repository with our code. After that click the **Finish**

![Eclipse import project window - browse](eclipse-import-project2-browse.png)

![Eclipse import project window 3](eclipse-import-project3.png)

After that you will have contents of our project in the Eclipse:

![Photran's default perspective with our project](photran-default2.png)

Next think to do is to teach Eclipse how to debug our code and where is the executable to run. But before proceeding any further make sure you have actually built the project, and there is an executable to run and debug under `all/build/mpdata-fortran` directory.

Click **Run** and then **Run Configurations...**. 

![Run Configurations dialog](photran-run-configurations.png)

First double-click **Fortran Local Application** item on the left hand side list to create a child item. Name this item accordingly to your needs, something like `Launch dwarf1`. You will create a separate run configuration for each executable file you will work on, so you need a way to distinguish between them.

Next choose a Fortran application to run - click the **Browse** button immidiately below the first edit box under the tab **Main** and navigate to the location of your executable:

![Run Configurations dialog](photran-run-configurations-browse.png)

Then you also need to specify the Fortran project with the code, otherwise Photran will not know how to build the file. Click on the **Browse** button to the left of the **Project:** edit box and select the imported Photran project:

![Run Configurations dialog](photran-run-configurations-project.png)

Then click **Run** to test if it runs. The Eclipse will create the build configuration for you and build the project in its own project directory. 

Now come back to the same the dialog and change the location of the executable to the one inside the Photran's project, `all/mpdata-eclipse/dwarf` in case of our example. Otherwise you will build one file, but debug another. 

That's it! Now you can run, debug, and edit the Fortran code inside the Eclispe IDE.

## Alternative path

[http://stackoverflow.com/questions/9453851/how-to-configure-eclipse-cdt-for-cmake]

# Develop with command-line tools

## Meaningful command prompt

I recommend using `liquidprompt` To install do `sudo apt install liquidprompt` and then `liquidprompt_activate` as a current user.  

Liquidprompt alters command line bash prompt to give various visual cues about the current directory, and state of the machine:

* It shows you whether you are logged directly, or remotely
* It shows whether the current directory is writable by you
* It shows current branch and number of modified lines if you are inside a git repository (very helpful)
* It shows the temperature of the CPU if it is over a safe limit.

and many more. 

![example liquidprompt prompt](liquidprompt1.png)

In the picture above you can see that

* the temperature of the CPU (60°C, slightly warm)
* green `@` means that I have write permission in the current directory
* the `±` sign means that I am inside the git repository, after this sign...
* ...there is a name of the current branch. Red means that the directory is dirty (there are some changes not commited), yellow means that my repository is ahead of the remote and I probably need to do `git push`, and green means that the working tree is clean.
* Number in the brackets tells number of lines added/removed from all the files in the repository relative to the last commit in the whole current repository. 

## Terminal multiplexer (tmux)

Install the package `byobu`. The byoubu in itself is very helpful, but it is even better if you would customize it a little. 

### Bare byobu

At first glance Byobu is a "better screen", but it is powered by the `tmux` to give the best of both programs. 

It allows splitting the terminal window into panes and, like screen, allows switching the windows. It persists between logins, so if you work remotely, you can simply disconnect, return home, and reconnect to find all the opened panes and windows preserved.

**Warning**: If byobu is run with `screen` keyboard bindings, it binds all function keys. To regain the them (for instance to operate Midnight Commander) press Shift+F12.

![example session in tmux](byobu.png)

In some directories in our repository you may find `set-tmux.sh` files. They define my own customization of windows and panes for a typical work in a given directory. The picture above illustrates this with the layout for the `propoze-docs` directory.

Like `vim`, `tmux` operates using a bunch of keyboard shortcuts. All the keys can be customized. Here are the most important ones with default bindings:

* `Shift-F12` disables all function key bindings specific to byobu
* `C-a space` or `C-a n` switches to the next window (if there are any)
* `C-a c` creates a new window
* `C-a d` detaches from the current tmux/byobu session. The session will keep running in the background.
* `C-a [` enters the text copying mode. In this mode use arrows to navigate to the beginning of the text you want to copy, press `space` and navigate to the end of the text, and press `enter` to copy it to the buffer. 
* `C-a ]` pastes previously copied text.
* `C-a arrows` navigate between panes.

There are many customizations the make `tmux` seamlessly blend with the `vim`, but since I am not a user of the `vim` so I cannot test them.



### Customized tmux

Since byobu runs on top of `tmux`, it can benefit from its plugin system - most of the plugins for the `tmux` work equally well under `byobu`. To set this up, see `install-byobu-tmux.sh` file in the main repository.

It will re-map keys for pane navigation: `C-a + |` will split vertically current window, `C-a + -` - horizontally. Keys `H`, `J`, `K` and `L` will work in similar fassion as in vim to move focus around panes and resize them. 

Additionaly it will install four plug-ins:


* `sensible` is a package with default customizations of tmux. Most of them are already present in byobu, but it never hurt to load it anyway.

* `resurrect` adds `C-a + C-s` ('s' as in 'save') commands that capture current window layout *and* running applications and save them to the session file. Later on (e.g. after system restart) you can restore the session with `C-a + C-r` ('r' as in 'restore').

* continuum continuously saves the current session, so you don't have to remember to save them with keys above.

* yank allows for `C-a + C-y` to copy current command line to the real clipboard; `C-a C-Y` to copy current pwd to the real clipboard, `y` to copy selection to the real clipboard in the copy mode.

