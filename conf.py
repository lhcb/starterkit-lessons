# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
from os.path import isfile, join, dirname
import re

from docutils import nodes
from recommonmark.transform import AutoStructify
from sphinx.addnodes import download_reference, pending_xref
from sphinx.transforms import SphinxTransform

# -- Project information -----------------------------------------------------

project = 'LHCb Starterkit Lessons'
copyright = '2015-2019, LHCb Starterkit'
author = 'LHCb Starterkit'

# master_doc = 'README'

# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'sphinx_rtd_theme',
    'recommonmark',
    'sphinx.ext.mathjax',
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = [
    'first-analysis-steps/code/plots/README.md',
    'README.md',
]

html_theme = 'sphinx_rtd_theme'
html_logo = 'starterkit.png'
html_show_sourcelink = True
html_theme_options = {
    'collapse_navigation': False,
}

html_context = {
    'display_github': True,
    'github_user': 'lhcb',
    'github_repo': 'starterkit-lessons',
    'github_version': 'master',
    'conf_py_path': '/source/',
}

html_static_path = [
    '_static',
]

linkcheck_ignore = [
    r'https://lhcb-portal-dirac\.cern\.ch/DIRAC/',
    r'https://lhcb-nightlies\.cern\.ch.*',
    r'https://gitlab\.cern\.ch/.*/merge_requests/new',
    r'https://gitlab\.cern\.ch/lhcb/Stripping/blob/.*',
    r'https://research\.cs\.wisc\.edu/htcondor/.*',
]
linkcheck_workers = 128


class FixMardownDownloads(SphinxTransform):
    default_priority = 5

    def apply(self, **kwargs):
        for old_node in self.document.traverse(pending_xref):
            if not isfile(join(dirname(old_node.source), old_node['reftarget'])):
                continue
            node1 = nodes.literal('', '', *old_node.children[0].children, classes=['xref', 'download'])
            old_node.replace(old_node.children[0], node1)
            new_node = download_reference(old_node.rawsource, '', *old_node.children, **old_node.attributes)
            old_node.parent.replace(old_node, new_node)


class AddPanels(SphinxTransform):
    default_priority = 10

    # Mapping of name -> (default_visibile, icon)
    panel_defaults = {
        'prereq': (True, 'rocket'),
        'callout': (True, 'info-circle'),
        'challenge': (True, 'square-o'),
        'hiddenchallenge': (False, 'square-o'),
        'solution': (False, 'check-square-o'),
        'objectives': (True, 'line-chart'),
        'keypoints': (True, 'key'),
        'discussion': (False, 'bell'),
    }

    def apply(self, **kwargs):
        for node in self.document.traverse(nodes.Element):
            match = re.match(r'^ *{%\s*(\w+)\s*"([^"]+)"\s*%} *$', node.rawsource)
            if match:
                panel_type, title = match.groups()
                try:
                    visibile, icon = self.panel_defaults[panel_type]
                except KeyError:
                    raise ValueError(f'Unrecognised panel type {panel_type}',
                                     self.panel_defaults.keys())

                # Find the body of the panel
                inner_node = node
                current_nodes = []
                while True:
                    inner_node = inner_node.next_node(descend=False, siblings=True, ascend=False)
                    if inner_node is None:
                        raise ValueError(f'Failed to find end block for {node.rawsource} in {node.source}')
                    match = re.match(r'^ *{%\s*(\w+)\s*"([^"]+)"\s*%} *$', node.rawsource)
                    # Check if we're at the end of the panel block
                    if re.match(r'^\s*{%\s*end' + panel_type + r'\s*%}\s*$', inner_node.rawsource):
                        inner_node.parent.remove(inner_node)
                        break
                    current_nodes.append(inner_node)

                # Create a div
                panel_body = nodes.container()
                panel_body.attributes['classes'].append('panel-body')
                for inner_node in current_nodes:
                    inner_node.parent.remove(inner_node)
                    panel_body.append(inner_node)

                # Create the title text
                header_text = nodes.paragraph()
                header_text.append(nodes.raw('', f'<i class="fa fa-{icon}"></i> ', format='html'))
                header_text.append(nodes.Text(title))

                # Create the title bar
                header = nodes.container()
                header.attributes['classes'].append('panel-header')
                if visibile:
                    header.attributes['classes'].append('open')
                header.append(header_text)

                # Move the inner nodes to the new container node and replace the parent
                new_node = nodes.container()
                new_node.attributes['classes'].append('panel')
                new_node.attributes['classes'].append('panel-' + panel_type)
                new_node.append(panel_body)
                new_node.insert(0, header)
                node.parent.replace(node, new_node)


def setup(app):
    app.add_config_value('recommonmark_config', {
        # 'url_resolver': lambda url: 'http://0.0.0.0/' + url,
        'auto_toc_tree_section': 'Contents',
        'enable_math': True,
        'enable_inline_math': True,
        'enable_eval_rst': True,
    }, True)

    app.add_transform(AutoStructify)
    app.add_transform(FixMardownDownloads)
    app.add_transform(AddPanels)

    app.add_stylesheet('panels.css')
    app.add_stylesheet('starterkit.css')
    app.add_javascript('panels.js')
