# -*- coding: utf-8 -*-
"""
Created on Wed Mar 27 18:06:08 2013

@author: proto
"""

from pytagcloud import create_tag_image, make_tags
from pytagcloud.lang.counter import get_tag_counts

def cloudText(text,fileName):
    tags = make_tags(get_tag_counts(text), maxsize=80)
    create_tag_image(tags, fileName, size=(800, 600), fontname='Droid Sans')

