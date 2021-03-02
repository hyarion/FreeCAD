#!/usr/bin/env python3

import xml.etree.ElementTree as ET
from pathlib import Path
import os


def _find_path(path, filename):
    if filename.endswith('.cpp'):
        return os.path.normpath(Path(path).parent / filename)
    elif filename.endswith('.py'):
        return os.path.normpath(Path(path).parent.parent.parent / filename)
    return None

def add_github_links(path):
    github_tool_path = 'https://github.com/FreeCAD/FreeCAD/tree/master'

    tree = ET.parse(path)
    for parent in tree.iterfind('.//location/..'):
        if parent.find('extracomment'):
            continue
        location = parent.find('location')

        filename = location.attrib['filename']
        line = location.attrib['line']

        file_path = _find_path(f'src/Tools/{path}', filename)
        if file_path:
            comment = ET.SubElement(parent, 'extracomment')
            comment.text = f'{github_tool_path}/{file_path}#L{line}'

    return ET.tostring(tree.getroot(), encoding='utf8')


if __name__ == "__main__":
    test_path = "../Mod/PartDesign/Gui/Resources/translations/PartDesign.ts"
    print(add_github_links(test_path).decode('utf8'))
