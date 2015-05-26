first-analysis-steps is an open source project,
and we welcome contributions of all kinds:
new lessons,
fixes to existing material,
bug reports,
and reviews of proposed changes are all equally welcome.

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

To build the lessons locally install the following:

1. [Install Pandoc](http://www.pandoc.org/installing)

2. All Python packages required for lesson creation and validation can 
   be installed using:
   
    ~~~
    $ pip install -r requirements.txt
    ~~~
        
3. To convert Markdown files into HTML pages in the root directory, go
   into the root directory of your lesson and run:

   ~~~
   $ make preview
   ~~~

   You can run `make` on its own to get a list of other things it will
   do for you.

## Other Resources

1.  This lesson is based on the template found at
    [https://github.com/swcarpentry/lesson-template](https://github.com/swcarpentry/lesson-template).
    That repository has instructions on formatting and previewing lessons.

2.  For a list of helpful commands run `make` in this directory.

[conduct]: CONDUCT.md
[issues]: https://github.com/lhcb/first-analysis-steps/issues
[license]: LICENSE.md
[pro-git-chapter]: http://git-scm.com/book/en/v2/GitHub-Contributing-to-a-Project
[gh-fork]: https://help.github.com/articles/fork-a-repo/
[gh-pull]: https://help.github.com/articles/using-pull-requests/
[gh-fork-pull]: https://help.github.com/articles/using-pull-requests/#fork--pull
[swc-lessons]: http://software-carpentry.org/lessons.html
