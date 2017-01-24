Pushing changes always requires that you have committed something new to the repository. This statement applies also to git with submodules but with a little twist: you must also inform the superproject `https://git.imgw.ad/aryczkowski/propoze` (usually named **all**) that a new commit is available to one of its submodules. 

Suppose you have did some coding in the `gridtools_verification` submodule of `all`. Your bash session would look like this:

```{bash}
git commit -a -m "This should be meaningful summary of your changes"
git push
cd ..
git commit -a -m "Updated submodule"
git push
```

If you work on something bigger than one commit, you should have used a separate branch, preferably with the `git flow`. In this case your bash session would look like this:

```{bash}
git flow feature start my_cool_feature
#Do some coding. Commit and push as much, as you want. All people using the superproject will not
#see any of your changes until you commit to the superproject your actual commit. 
git flow feature stop my_cool_feature

cd ..
git commit -a -m "Updated submodule"
git push  

```

Submodules don't track anything - they always point to a specific (static) commit. So the last three lines can be used whenever you feel you need to share your work with any other consumers of the whole superproject. Don't do this unless you are sure you are pushing a working and tested code, which often coincides with your decision to close the git flow feature (i.e. merge the git branch corresponding with the feature).

And make sure you have your VPN on, otherwise you wouldn't be able to talk with the git servers.
