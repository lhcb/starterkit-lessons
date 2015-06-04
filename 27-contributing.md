---
layout: page
title: First Steps in LHCb
subtitle: Contribute to this lesson
minutes: 10
---
> ## Learning Objectives {.objectives}
>
> * Reporting a mistake
> * How to look at the source of these lessons
> * How to modify a lesson

These lessons are not really about software, they are about people. If
you have followed along until this point you are more than qualified
to edit the lessons. There are probably several mistakes in these
lessons, or they will be outdated soon. Keeping them working and
fixing all mistakes is a monumental task for one single person.

![The starterkit needs you! (Alfred Leete [Public domain], via
 Wikimedia Commons)](img/Kitchener-leete.jpg)

We need you! You know everything you need to in order to
contribute. Take advantage of this.

The source of this lesson is hosted on GitHub:
[lhcb/first-analysis-steps](https://github.com/lhcb/first-analysis-steps).

> ## Submitting a bug report {.callout}
>
> If you spot something that is wrong, create a bug report on the
> [issue tracker](https://github.com/lhcb/first-analysis-steps/issues)
> This is super simple and makes it easy to keep track of what is broken
> and needs fixing.

You do not need anyone's permission to start making changes. You can
start directly. If you want to edit something the first thing to do is
to create a fork of the repository. Visit
[lhcb/first-analysis-steps](https://github.com/lhcb/first-analysis-steps)
and click the "Fork" button at the top right.

> ## GitHub {.callout}
>
> GitHub is an amazing service that makes it really easy for people
> to contribute to projects that they have never contributed to before.
> If you do not have an account we strongly suggest you
> [create an account](https://github.com/join).

Each lesson is in its own `.md` file. For example the source of this
lesson is in
[`27-contributing.md`](https://github.com/lhcb/first-analysis-steps/blob/master/27-contributing.md). It
is a simple text file with a few clever lines with special meaning.

The format the files are written in is called [Markdown](http://daringfireball.net/projects/markdown/basics). It is a very simple language, which adds some
basic formatting to text files. `**Bold text**` leads to **Bold text**,
`*Italic*` is *italic* and `[http://google.com](the search engine)`
makes a link to [the search engine](http://google.com).

> ## Trying it out live {.callout}
>
> Try out Markdown live in your browser with [Dillinger](http://dillinger.io/).

To get the source of this repository clone your fork of the lessons.

```bash
$ git clone https://YOURUSERNAME@github.com/YOURUSERNAME/first-analysis-steps.git
```

As you can see each lesson has its own `.md` file. You can modify one
and then commit it. After `git push`'ing it visit your copy of the
repository on github: `https://github.com/YOURUSERNAME/first-analysis-steps`.

