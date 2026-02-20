# Contributing

[starterkit-lessons][repo] is an open source project, and we welcome contributions of all kinds:

* New lessons;
* Fixes to existing material;
* Bug reports; and
* Reviews of proposed changes.

By contributing, you are agreeing that we may redistribute your work under [these licenses][license].
You also agree to abide by our [contributor code of conduct][conduct].

## Getting Started

1.  We use the [fork and pull][gh-fork-pull] model to manage changes.
    More information about [forking a repository][gh-fork] and [making a Pull Request][gh-pull].

2.  To build and check the lessons locally, follow [Dependencies and local checks](#dependencies-and-local-checks).

3.  For our lessons, you should branch from and submit pull requests against the `master` branch.

4.  When editing lesson pages, you need only commit changes to the Markdown source files.

5.  If you're looking for things to work on, please see [the list of issues for this repository][issues].
    Comments on issues and reviews of pull requests are equally welcome.

## Dependencies and local checks

The CI workflow builds and link-checks this repository using `starterkit-ci`.
Use the same commands locally before opening a PR.

### Option A: run without installing (recommended)

```shell
uvx starterkit-ci build --source-dir . --allow-warnings
uvx starterkit-ci check --source-dir . --allow-warnings
```

### Option B: install the tool first

```shell
pip install starterkit-ci
starterkit-ci build --source-dir . --allow-warnings
starterkit-ci check --source-dir . --allow-warnings
```

Note: `starterkit_ci` (underscore) is still accepted as a legacy alias, but `starterkit-ci` is the preferred command.

To preview the generated docs locally, start a web server:

```shell
cd build
uv run python -m http.server 8000
```

You can see your local version by using a web-browser to navigate to `http://localhost:8000` or wherever it says it's serving the book.

[conduct]: CONDUCT.md
[repo]: https://github.com/lhcb/starterkit-lessons
[issues]: https://github.com/lhcb/starterkit-lessons/issues
[license]: LICENSE.md
[pro-git-chapter]: http://git-scm.com/book/en/v2/GitHub-Contributing-to-a-Project
[gh-fork]: https://help.github.com/en/articles/fork-a-repo
[gh-pull]: https://help.github.com/en/articles/about-pull-requests
[gh-fork-pull]: https://reflectoring.io/github-fork-and-pull/


```{toctree}
:hidden:

CONDUCT.md
LICENSE.md
```
