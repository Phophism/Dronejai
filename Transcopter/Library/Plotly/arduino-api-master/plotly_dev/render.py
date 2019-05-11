# -*- coding: utf-8 -*-

from mako.template import Template
from mako.runtime import Context
import os


destination_parent = '..'
libs = ["wifi", "ethernet", "gsm", "cc3000"]

meta = [
    {'src': './plotly_streaming.pre.cpp', 'destination': '../', 'folder_prefix': 'plotly_streaming_'},
    {'src': './plotly_streaming.pre.h', 'destination': '../', 'folder_prefix': 'plotly_streaming_'},
    {'src': './simple.pre.ino', 'destination': '../examples', 'folder_prefix': ''},
]

for m in meta:
    for lib in libs:
        src = m['src']
        # Write file to a destination folder
        directory = m['destination']+'/'+m['folder_prefix']+lib.lower()
        if not os.path.exists(directory):
            os.makedirs(directory)
        fout = open(directory+'/'+src.replace('.pre', '_'+lib.lower()), 'w')

        mytemplate = Template(filename=src)
        ctx = Context(fout, lib=lib)
        mytemplate.render_context(ctx)
        fout.close()