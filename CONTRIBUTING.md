# How to contribute?

## Writing stuff
First of all, clone the [repository] (https://github.com/themikemikovi4/beaver).

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
`MY HANDS ARE TYPING WORDS` is bad). Don't add a period and use present simple
(`Add something` is good and `Added something.` is bad).

Everything before the first blank line is a short commit message. Keep it
less than 80 characters. If you including a long commit message, add a
`(see below)` notice to the short message. You can use markdown. Example:
```
The main commit message.

Details
* Detail 1
* Detail 2
```

Once you're done, you should rebase your changes on top os master:
```
git checkout dev
git pull # to ensure latest version
git rebase feature dev
# fix merge conflicts
git push
```
Now your feature branch should be fast-forwardable to dev. Tell @themikemikovi4
about it, and he will merge your changes. Do not do the merge youself.

## Other help
If you found a bug, or have an idea for a feature, go to the
[project page] (https://github.com/themikemikovi4/beaver), and submit an
issue.
