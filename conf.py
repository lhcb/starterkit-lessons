# You should normally never do wildcard imports
# Here it is useful to allow the configuration to be maintained elsewhere
from starterkit_ci.sphinx_config import *  # NOQA

project = 'LHCb Starterkit Lessons'
copyright = '2015-2019, LHCb Starterkit'
author = 'LHCb Starterkit'

exclude_patterns = [
    'first-analysis-steps/code/plots/README.md',
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
    # FIXME: The URLs have changed
    r'https://research\.cs\.wisc\.edu/htcondor/.*',
]


def starterkit_ci_setup(app):
    app.add_stylesheet('starterkit.css')


setup.extra_setup_funcs += [starterkit_ci_setup]
