"""Verify snippet text according to the supported literal/escape/$0 grammar."""

import json
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools'))
from generate_snippets import PREAMBLE, OUTPUT, TEMPLATE, build_snippets, library_body
from check_catalog import library_sources


def expand(body):
    # An independent scanner: recognize escaped literals and the final cursor.
    text = '\n'.join(body)
    result = []
    cursors = []
    index = 0
    while index < len(text):
        if text[index] == '\\':
            if index + 1 >= len(text) or text[index + 1] not in '\\$}':
                raise AssertionError('Invalid snippet escape')
            result.append(text[index + 1])
            index += 2
        elif text[index:index + 2] == '$0':
            cursors.append(len(result))
            index += 2
        else:
            if text[index] == '$':
                raise AssertionError('Unexpected variable or tabstop')
            result.append(text[index])
            index += 1
    return ''.join(result), cursors


class SnippetChecks(unittest.TestCase):
    def test_current_sources_and_cursor(self):
        snippets = json.loads((ROOT / OUTPUT).read_text(encoding='utf-8'))
        self.assertEqual(list(snippets)[0], 'atcoder')
        self.assertEqual(len(snippets), len(library_sources(ROOT)) + 1)
        for name, item in snippets.items():
            self.assertEqual(set(item), {'prefix', 'body'})
            self.assertEqual(item['prefix'], '!' + name)
        actual, cursors = expand(snippets['atcoder']['body'])
        expected = (ROOT / TEMPLATE).read_bytes().decode('utf-8')
        self.assertEqual(actual, expected)
        self.assertEqual(cursors, [expected.index('void solve() {\n') + len('void solve() {\n') + 4])
        for path in library_sources(ROOT).values():
            actual, cursors = expand(snippets[path.stem]['body'])
            self.assertEqual(actual, path.read_bytes().decode('utf-8')[len(PREAMBLE):])
            self.assertEqual(cursors, [])

    def test_literal_dollars_backslashes_and_blank_lines(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / 'Literal.cpp'
            suffix = '\n// $0 ${NAME} $100 \\ ending\nconst char* s = "\\n\\t";\n}\n\n'
            path.write_text(PREAMBLE + suffix, encoding='utf-8')
            actual, cursors = expand(library_body(path))
            self.assertEqual(actual, suffix)
            self.assertEqual(cursors, [])

    def test_wrong_preamble_is_rejected(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / 'Wrong.cpp'
            path.write_text('#include <vector>\nint preserved;\n', encoding='utf-8')
            with self.assertRaises(ValueError):
                library_body(path)

    def test_duplicate_names_are_rejected(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root / 'templates').mkdir()
            (root / TEMPLATE).write_bytes((ROOT / TEMPLATE).read_bytes())
            for name in ('graph', 'math'):
                (root / name).mkdir()
                (root / name / 'Same.cpp').write_text(PREAMBLE + '\n', encoding='utf-8')
            with self.assertRaises(ValueError):
                build_snippets(root)


if __name__ == '__main__':
    unittest.main()
