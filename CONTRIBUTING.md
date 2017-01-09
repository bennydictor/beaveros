# How to contribute?

## Official contributors
If you want to write something, branch off the `dev` branch.
Please choose a meaningful name (eg. `bootloader` is good and `asdf`
is bad). Also, if you are working on a github issue, please append
issue number to the end with a dash (eg. `ipc-sharedmem-#42`).
You can do it like so:
```
git checkout -b feature dev
```

Once you are on your branch, start working on whatever you should be working.
Please, name commits meaningfully (eg. `Add VGA support` is good and
`MY HANDS ARE TYPING WORDS` is bad). Remember that you can also include
a more detailed commit message, like so (you can use markdown):
```
The main commit message.

Details
* Detail 1
* Detail 2
```

Once you're done, you should merge your changes back into the `dev` branch.
```
git checkout dev
git pull # to ensure latest version
git merge --no-ff # --no-ff flag is used because we want every merge to be a commit
git push
```

## Others
The process is basically the same, but first you need to fork from the repository,
and after you done, you should create a pull request.
