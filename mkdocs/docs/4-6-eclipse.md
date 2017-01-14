# Troubleshooting

## Stability problems on Ubuntu 16.04

See the [http://askubuntu.com/questions/761604/eclipse-not-working-in-16-04]. 

### Solution 1.

> I had something the same with 16.04 and Eclipse Mars. I thought it had frozen but in fact it was running very, very slowly. The problem is the version of GTK+ 3, shipped with 16.04. Fortunately the solution is very easy. Open a terminal and then type `export SWT_GTK3=0`, then start Eclipse from the terminal. If that works, then a more persistent fix is to put the *2 lines below, as they appear* in your `eclipse.ini`:

```
--launcher.GTK_version
2
```

before the line:

```
--launcher.appendVmargs
```

The bug is filed with Ubuntu at [bug 1552764][1]


  [1]: https://bugs.launchpad.net/ubuntu/+source/java-common/+bug/1552764

### Solution 2.


Change required version of java from :

```
-Dosgi.requiredJavaVersion=1.7
```

into

```
-Dosgi.requiredJavaVersion=1.8
```

and remove this line as well :

```
-XX:MaxPermSize=256m
```


