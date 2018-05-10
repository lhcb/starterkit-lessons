# Contribute to this lesson

{% objectives "Learning Objectives" %}
* Reporting a mistake
* How to look at the source of these lessons
* How to modify a lesson
{% endobjectives %} 

These lessons are not really about software, they are about people. If
you have followed along until this point you are more than qualified
to edit the lessons. There are probably several mistakes in these
lessons, or they will be outdated soon. Keeping the lessons working and
fixing all mistakes is a monumental task for one single person.

![The starterkit needs you! (Alfred Leete [Public domain], via
 Wikimedia Commons)](img/Kitchener-leete.jpg)

We need you! You now know everything you need to in order to
contribute. Take advantage of this.

The source of this lesson is hosted on GitHub:
[lhcb/starterkit-lessons](https://github.com/lhcb/starterkit-lessons).

{% callout "Submitting a bug report " %}
If you spot something that is wrong, create a bug report on the
[issue tracker](https://github.com/lhcb/starterkit-lessons/issues)
This is super simple and makes it easy for everyone to keep track of
what is broken and needs fixing. It also increases your chances of
someone posting a solution.
{% endcallout %} 

You do not need anyone's permission to start making changes. You can
start directly. If you want to edit something the first thing to do is
to create a fork of the repository. Visit
[lhcb/first-analysis-steps](https://github.com/lhcb/starterkit-lessons)
and click the "Fork" button at the top right.

![Click on the fork button to create a fork of `lhcb/first-analysis-steps`](img/fork-me.png)

A fork is simply a copy of the original repository. It works just as well
as the original. Clone the repository to your computer to start making changes:

```bash
$ git clone https://YOURUSERNAME@github.com/YOURUSERNAME/starterkit-lessons.git
```

As you can see each lesson has its own `.md` file. The source of this
lesson is in
[`CONTRIBUTING.md`](https://github.com/lhcb/starterkit-lessons/blob/master/CONTRIBUTING.md).
It is a simple text file with a few clever lines with special meaning.

The format the files are written in is called
[Markdown](http://daringfireball.net/projects/markdown/basics). It is
a very simple language, which adds some basic formatting to text
files. `**Bold text**` leads to **Bold text**, `_Italic_` is _italic_
and `[the search engine](http://google.com)` makes a link to [the
search engine](http://google.com).

To add a lesson create your lesson.md file then add the following line to the SUMMARY.md file:

```
* [My New Lesson](first-analysis-steps/lesson.md)
```

{% callout "Trying it out live " %}
Try out Markdown live in your browser with [Dillinger](http://dillinger.io/).
{% endcallout %} 

If you want to see what your changes look like, simply paste a lesson
to Dillinger.

If you found something to improve, create a new branch with a fitting
name (replace `fixing-typos`):

```bash
$ git checkout -b fixing-typos
```

Once you are done with your changes, commit them. To commit use `git
add 00-lesson-you-edited.md` and then `git commit`. After `git
push`'ing it, visit your copy of the repository on github:
`https://github.com/YOURUSERNAME/starterkit-lessons`.

In order to test your changes, you can run the starterkit website locally.
To do so, first install the required python packages.
This can be done by passing the `requirements.txt` that is present in the git repository to pip
```
pip install -r requirements.txt --user
```
Besides these packages you need to install pandoc on your system.
Once all requirements are satisfied, run
```
make preview
cd _site
python -m http.server
```
in the top level of the git repository.

This will start a webserver on your computer.
Then open you web browser and navigate to
```
localhost:8000
```
to see the website.

Next you want to [create a pull
request](https://help.github.com/articles/creating-a-pull-request/). The
github documentation is excellent, so we will not duplicate it here. Simply
follow the guide: [how to create a pull
request](https://help.github.com/articles/creating-a-pull-request/).

Now we can see your proposed changes and will probably leave you some
comments. Once everyone is happy, one of the main starterkit'ers will
merge your pull request. Congratulations, you have successfully
contributed!
