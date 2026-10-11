#!/usr/bin/env python3
"""Generate literal VS Code snippets from the canonical C++ source files."""

import argparse
import json
import sys
from pathlib import Path

from check_catalog import library_sources


OUTPUT = Path('snippets/competitive_programming.code-snippets')
TEMPLATE = Path('templates/AtCoder.cpp')
PREAMBLE = '#include <bits/stdc++.h>\nusing namespace std;\n'


def read_source(path):
    # Read bytes so CRLF and other formatting changes cannot go unnoticed.
    text = path.read_bytes().decode('utf-8')
    if '\r' in text:
        raise ValueError(f'Expected UTF-8/LF source: {path}')
    return text


def escape_literal(text):
    # Snippet expansion has its own escaping in addition to JSON escaping.
    return text.replace('\\', '\\\\').replace('$', '\\$')


def library_body(path):
    text = read_source(path)
    if not text.startswith(PREAMBLE):
        raise ValueError(f'Unexpected first two lines: {path}')
    # Keep every remaining character, including blank and final empty lines.
    return escape_literal(text[len(PREAMBLE):]).split('\n')


def template_body(path):
    lines = read_source(path).split('\n')
    positions = [i for i, line in enumerate(lines) if line == 'void solve() {']
    if len(positions) != 1:
        raise ValueError('Expected exactly one void solve() in the template')
    cursor_line = positions[0] + 1
    if cursor_line >= len(lines) or lines[cursor_line] != '    ':
        raise ValueError('Expected four spaces on the first line inside solve()')
    body = [escape_literal(line) for line in lines]
    body[cursor_line] += '$0'
    return body


def build_snippets(root):
    result = {'atcoder': {'prefix': '!atcoder',
                         'body': template_body(root / TEMPLATE)}}
    for relative, path in sorted(library_sources(root).items()):
        name = path.stem
        if name in result:
            raise ValueError(f'Duplicate snippet name: {name} ({relative})')
        result[name] = {'prefix': '!' + name, 'body': library_body(path)}
    return result


def generated_text(root):
    return json.dumps(build_snippets(root), ensure_ascii=False, indent=4) + '\n'


def check_snippets(root):
    try:
        expected = generated_text(root)
        actual = read_source(root / OUTPUT)
    except (OSError, UnicodeError, ValueError) as error:
        return [str(error)]
    if actual != expected:
        return ['Snippets differ from source; run python3 tools/generate_snippets.py']
    return []


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path,
                        default=Path(__file__).resolve().parents[1])
    parser.add_argument('--check', action='store_true')
    args = parser.parse_args()
    root = args.root.resolve()
    try:
        if args.check:
            errors = check_snippets(root)
            if errors:
                for error in errors:
                    print(f'ERROR: {error}', file=sys.stderr)
                return 1
        else:
            output = root / OUTPUT
            output.parent.mkdir(parents=True, exist_ok=True)
            output.write_bytes(generated_text(root).encode('utf-8'))
        print(f'OK: !atcoder first; {len(library_sources(root))} literal library snippets; '
              'only prefix/body; solve() cursor $0.')
        return 0
    except (OSError, UnicodeError, ValueError) as error:
        print(f'ERROR: {error}', file=sys.stderr)
        return 1


if __name__ == '__main__':
    sys.exit(main())
