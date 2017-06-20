# Contributing

[starterkit-lessons][repo] is an open source project,
and we welcome contributions of all kinds:

* New lessons;
* Fixes to existing material;
* Bug reports; and
* Reviews of proposed changes.

By contributing,
you are agreeing that we may redistribute your work under
[these licenses][license].
You also agree to abide by our
[contributor code of conduct][conduct].

## Getting Started

1.  We use the [fork and pull][gh-fork-pull] model to manage changes. More information
    about [forking a repository][gh-fork] and [making a Pull Request][gh-pull].

2.  To build the lessons please install the [dependencies](#DEPENDENCIES).

2.  For our lessons,
    you should branch from and submit pull requests against the `master` branch.

3.  When editing lesson pages, you need only commit changes to the Markdown source files.

4.  If you're looking for things to work on,
    please see [the list of issues for this repository][issues].
    Comments on issues and reviews of pull requests are equally welcome.

## Dependencies

To build the lessons locally, install the following:

1. [Gitbook](https://github.com/GitbookIO/gitbook/blob/master/docs/setup.md)

Install the Gitbook plugins:

```shell
$ gitbook install
```

Then build the pages and start a web server to host them:

```shell
$ gitbook serve
```

[conduct]: CONDUCT.md
[repo]: https://github.com/lhcb/starterkit-lessons/issues
[issues]: https://github.com/lhcb/starterkit-lessons/issues
[license]: LICENSE.md
[pro-git-chapter]: http://git-scm.com/book/en/v2/GitHub-Contributing-to-a-Project
[gh-fork]: https://help.github.com/articles/fork-a-repo/
[gh-pull]: https://help.github.com/articles/using-pull-requests/
[gh-fork-pull]: https://help.github.com/articles/using-pull-requests/#fork--pull
