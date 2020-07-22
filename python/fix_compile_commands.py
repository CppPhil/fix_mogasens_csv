#!/usr/bin/env python3
# -*- coding: utf8 -*-

import json

output_data = []

with open('build/compile_commands.json') as json_file:
  data = json.load(json_file)
  for o in data:
    if not "external" in o['file']:
      output_data.append(o)

with open('build/compile_commands.json', 'w') as outfile:
  json.dump(output_data, outfile)
