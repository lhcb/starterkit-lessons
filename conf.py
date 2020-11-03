# You should normally never do wildcard imports
# Here it is useful to allow the configuration to be maintained elsewhere
from starterkit_ci.sphinx_config import *  # NOQA

project = 'LHCb Starterkit Lessons'
copyright = '2015-2020, LHCb Starterkit'
author = 'LHCb Starterkit'
html_logo = 'starterkit.png'

exclude_patterns += [
    'first-analysis-steps/code/**',
    'README.md',
]

html_context = {
    'display_github': True,
    'github_user': 'lhcb',
    'github_repo': 'starterkit-lessons',
    'github_version': 'master',
    'conf_py_path': '/',
}

html_static_path += [
    f'_static',
]

linkcheck_ignore += [
]

starterkit_ci_redirects['first-analysis-steps/index.html'] = 'https://lhcb.github.io/starterkit-lessons/first-analysis-steps/README.html'
starterkit_ci_redirects['second-analysis-steps/index.html'] = 'https://lhcb.github.io/starterkit-lessons/second-analysis-steps/README.html'


def starterkit_ci_setup(app):
    app.add_css_file('starterkit.css')


setup.extra_setup_funcs += [starterkit_ci_setup]
